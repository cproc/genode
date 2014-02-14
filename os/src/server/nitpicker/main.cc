/*
 * \brief  Nitpicker main program for Genode
 * \author Norman Feske
 * \date   2006-08-04
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/env.h>
#include <base/sleep.h>
#include <base/printf.h>
#include <base/allocator_guard.h>
#include <os/attached_ram_dataspace.h>
#include <input/event.h>
#include <input/keycodes.h>
#include <root/component.h>
#include <dataspace/client.h>
#include <timer_session/connection.h>
#include <input_session/connection.h>
#include <input_session/input_session.h>
#include <nitpicker_view/nitpicker_view.h>
#include <nitpicker_session/nitpicker_session.h>
#include <framebuffer_session/connection.h>
#include <util/color.h>
#include <os/pixel_rgb565.h>
#include <os/session_policy.h>
#include <os/server.h>
#include <os/reporter.h>

/* local includes */
#include "input.h"
#include "big_mouse.h"
#include "background.h"
#include "clip_guard.h"
#include "mouse_cursor.h"
#include "chunky_menubar.h"

namespace Input       { class Session_component; }
namespace Framebuffer { class Session_component; }
namespace Nitpicker {
	class Session_component;
	template <typename> class Root;
	struct Main;
}

using Genode::size_t;
using Genode::Allocator;
using Genode::Rpc_entrypoint;
using Genode::List;
using Genode::Pixel_rgb565;
using Genode::strcmp;
using Genode::config;
using Genode::env;
using Genode::Arg_string;
using Genode::Object_pool;
using Genode::Dataspace_capability;
using Genode::Session_label;
using Genode::Signal_transmitter;
using Genode::Signal_context_capability;
using Genode::Signal_rpc_member;
using Genode::Attached_ram_dataspace;
using Genode::Attached_dataspace;


/***************
 ** Utilities **
 ***************/

/*
 * Font initialization
 */
extern char _binary_default_tff_start;

Text_painter::Font default_font(&_binary_default_tff_start);


class Flush_merger
{
	private:

		Rect _to_be_flushed = { Point(), Area() };

	public:

		bool defer = false;

		Rect to_be_flushed() const { return _to_be_flushed; }

		void merge(Rect rect)
		{
			if (_to_be_flushed.valid())
				_to_be_flushed = Rect::compound(_to_be_flushed, rect);
			else
				_to_be_flushed = rect;
		}

		void reset() { _to_be_flushed = Rect(Point(), Area()); }
};


template <typename PT>
class Screen : public Canvas<PT>, public Flush_merger
{
	protected:

		/**
		 * Surface_base::Flusher interface
		 */
		void flush_pixels(Rect rect) { merge(rect); }

	public:

		/**
		 * Constructor
		 */
		Screen(PT *base, Area size) : Canvas<PT>(base, size) { }
};


class Buffer
{
	private:

		Area                           _size;
		Framebuffer::Mode::Format      _format;
		Genode::Attached_ram_dataspace _ram_ds;

	public:

		/**
		 * Constructor - allocate and map dataspace for virtual frame buffer
		 *
		 * \throw Ram_session::Alloc_failed
		 * \throw Rm_session::Attach_failed
		 */
		Buffer(Area size, Framebuffer::Mode::Format format, Genode::size_t bytes)
		:
			_size(size), _format(format),
			_ram_ds(env()->ram_session(), bytes)
		{ }

		/**
		 * Accessors
		 */
		Genode::Ram_dataspace_capability ds_cap() const { return _ram_ds.cap(); }
		Area                               size() const { return _size; }
		Framebuffer::Mode::Format        format() const { return _format; }
		void                        *local_addr() const { return _ram_ds.local_addr<void>(); }
};


/**
 * Interface for triggering the re-allocation of a virtual framebuffer
 *
 * Used by 'Framebuffer::Session_component',
 * implemented by 'Nitpicker::Session_component'
 */
struct Buffer_provider
{
	virtual Buffer *realloc_buffer(Framebuffer::Mode mode, bool use_alpha) = 0;
};


struct Canvas_accessor
{
	virtual Canvas_base &canvas() = 0;
};


template <typename PT>
class Chunky_dataspace_texture : public Buffer,
                                 public Texture<PT>
{
	private:

		Framebuffer::Mode::Format _format() {
			return Framebuffer::Mode::RGB565; }

		/**
		 * Return base address of alpha channel or 0 if no alpha channel exists
		 */
		unsigned char *_alpha_base(Area size, bool use_alpha)
		{
			if (!use_alpha) return 0;

			/* alpha values come right after the pixel values */
			return (unsigned char *)local_addr() + calc_num_bytes(size, false);
		}

	public:

		/**
		 * Constructor
		 */
		Chunky_dataspace_texture(Area size, bool use_alpha)
		:
			Buffer(size, _format(), calc_num_bytes(size, use_alpha)),
			Texture<PT>((PT *)local_addr(),
			            _alpha_base(size, use_alpha), size) { }

		static Genode::size_t calc_num_bytes(Area size, bool use_alpha)
		{
			/*
			 * If using an alpha channel, the alpha buffer follows the
			 * pixel buffer. The alpha buffer is followed by an input
			 * mask buffer. Hence, we have to account one byte per
			 * alpha value and one byte for the input mask value.
			 */
			Genode::size_t bytes_per_pixel = sizeof(PT) + (use_alpha ? 2 : 0);
			return bytes_per_pixel*size.w()*size.h();
		}

		unsigned char *input_mask_buffer()
		{
			if (!Texture<PT>::alpha()) return 0;

			Area const size = Texture<PT>::size();

			/* input-mask values come right after the alpha values */
			return (unsigned char *)local_addr() + calc_num_bytes(size, false)
			                                     + size.count();
		}
};


/***********************
 ** Input sub session **
 ***********************/

class Input::Session_component : public Genode::Rpc_object<Session>
{
	public:

		enum { MAX_EVENTS = 200 };

		static size_t ev_ds_size() {
			return Genode::align_addr(MAX_EVENTS*sizeof(Event), 12); }

	private:

		/*
		 * Exported event buffer dataspace
		 */
		Attached_ram_dataspace _ev_ram_ds = { env()->ram_session(), ev_ds_size() };

		/*
		 * Local event buffer that is copied
		 * to the exported event buffer when
		 * flush() gets called.
		 */
		Event      _ev_buf[MAX_EVENTS];
		unsigned   _num_ev = 0;

	public:

		/**
		 * Enqueue event into local event buffer of the input session
		 */
		void submit(const Event *ev)
		{
			/* drop event when event buffer is full */
			if (_num_ev >= MAX_EVENTS) return;

			/* insert event into local event buffer */
			_ev_buf[_num_ev++] = *ev;
		}


		/*****************************
		 ** Input session interface **
		 *****************************/

		Dataspace_capability dataspace() { return _ev_ram_ds.cap(); }

		bool is_pending() const { return _num_ev > 0; }

		int flush()
		{
			unsigned ev_cnt;

			/* copy events from local event buffer to exported buffer */
			Event *ev_ds_buf = _ev_ram_ds.local_addr<Event>();
			for (ev_cnt = 0; ev_cnt < _num_ev; ev_cnt++)
				ev_ds_buf[ev_cnt] = _ev_buf[ev_cnt];

			_num_ev = 0;
			return ev_cnt;
		}
};


/*****************************
 ** Framebuffer sub session **
 *****************************/

class Framebuffer::Session_component : public Genode::Rpc_object<Session>
{
	private:

		::Buffer                 *_buffer = 0;
		View_stack               &_view_stack;
		::Session                &_session;
		Flush_merger             &_flush_merger;
		Framebuffer::Session     &_framebuffer;
		Canvas_accessor          &_canvas_accessor;
		Buffer_provider          &_buffer_provider;
		Signal_context_capability _mode_sigh;
		Framebuffer::Mode         _mode;
		bool                      _alpha = false;

		Canvas_base &_canvas() { return _canvas_accessor.canvas(); }

	public:

		/**
		 * Constructor
		 */
		Session_component(View_stack           &view_stack,
		                  ::Session            &session,
		                  Flush_merger         &flush_merger,
		                  Framebuffer::Session &framebuffer,
		                  Canvas_accessor      &canvas_accessor,
		                  Buffer_provider      &buffer_provider)
		:
			_view_stack(view_stack),
			_session(session),
			_flush_merger(flush_merger),
			_framebuffer(framebuffer),
			_canvas_accessor(canvas_accessor),
			_buffer_provider(buffer_provider)
		{ }

		/**
		 * Change virtual framebuffer mode
		 *
		 * Called by Nitpicker::Session_component when re-dimensioning the
		 * buffer.
		 *
		 * The new mode does not immediately become active. The client can
		 * keep using an already obtained framebuffer dataspace. However,
		 * we inform the client about the mode change via a signal. If the
		 * client calls 'dataspace' the next time, the new mode becomes
		 * effective.
		 */
		void notify_mode_change(Framebuffer::Mode mode, bool alpha)
		{
			_mode  = mode;
			_alpha = alpha;

			if (_mode_sigh.valid())
				Signal_transmitter(_mode_sigh).submit();
		}


		/************************************
		 ** Framebuffer::Session interface **
		 ************************************/

		Dataspace_capability dataspace()
		{
			_buffer = _buffer_provider.realloc_buffer(_mode, _alpha);

			return _buffer ? _buffer->ds_cap() : Genode::Ram_dataspace_capability();
		}

		Mode mode() const
		{
			return _mode;
		}

		void mode_sigh(Signal_context_capability mode_sigh)
		{
			_mode_sigh = mode_sigh;
		}

		void refresh(int x, int y, int w, int h)
		{
			_view_stack.update_session_views(_canvas(), _session,
			                                 Rect(Point(x, y), Area(w, h)));

			/* flush dirty pixels to physical frame buffer */
			if (_flush_merger.defer == false) {
				Rect r = _flush_merger.to_be_flushed();
				_framebuffer.refresh(r.x1(), r.y1(), r.w(), r.h());
				_flush_merger.reset();
			}
			_flush_merger.defer = true;
		}
};


class View_component : public Genode::List<View_component>::Element,
                       public Genode::Rpc_object<Nitpicker::View>
{
	private:

		typedef Genode::Rpc_entrypoint Rpc_entrypoint;

		View_stack      &_view_stack;
		::View           _view;
		Canvas_accessor &_canvas_accessor;
		Rpc_entrypoint  &_ep;

		Canvas_base &_canvas() { return _canvas_accessor.canvas(); }

	public:

		/**
		 * Constructor
		 */
		View_component(::Session &session, View_stack &view_stack,
		               Canvas_accessor &canvas_accessor,
		               Rpc_entrypoint &ep, ::View *parent_view):
			_view_stack(view_stack),
			_view(session,
			      session.stay_top() ? ::View::STAY_TOP : ::View::NOT_STAY_TOP,
			      ::View::NOT_TRANSPARENT, ::View::NOT_BACKGROUND, parent_view),
			_canvas_accessor(canvas_accessor),
			_ep(ep)
		{ }

		::View &view() { return _view; }


		/******************************
		 ** Nitpicker view interface **
		 ******************************/

		int viewport(int x, int y, int w, int h,
		             int buf_x, int buf_y, bool redraw)
		{
			/* transpose y position of top-level views by vertical session offset */
			if (_view.top_level())
				y += _view.session().v_offset();

			_view_stack.viewport(_canvas(), _view,
			                     Rect(Point(x, y), Area(w, h)),
			                     Point(buf_x, buf_y), redraw);
			return 0;
		}

		int stack(Nitpicker::View_capability neighbor_cap, bool behind, bool redraw)
		{
			Object_pool<View_component>::Guard nvc(_ep.lookup_and_lock(neighbor_cap));

			::View *neighbor_view = nvc ? &nvc->view() : 0;

			_view_stack.stack(_canvas(), _view, neighbor_view, behind, redraw);
			return 0;
		}

		int title(Title const &title)
		{
			_view_stack.title(_canvas(), _view, title.string());
			return 0;
		}
};


/*****************************************
 ** Implementation of Nitpicker service **
 *****************************************/

class Nitpicker::Session_component : public Genode::Rpc_object<Session>,
                                     public ::Session,
                                     public Buffer_provider
{
	private:

		Genode::Allocator_guard _buffer_alloc;

		Framebuffer::Session &_framebuffer;

		/* Framebuffer_session_component */
		Framebuffer::Session_component _framebuffer_session_component;

		Canvas_accessor &_canvas_accessor;

		/* Input_session_component */
		Input::Session_component _input_session_component;

		/*
		 * Entrypoint that is used for the views, input session,
		 * and framebuffer session.
		 */
		Rpc_entrypoint &_ep;

		View_stack &_view_stack;

		List<View_component> _view_list;

		/* capabilities for sub sessions */
		Framebuffer::Session_capability _framebuffer_session_cap;
		Input::Session_capability       _input_session_cap;

		bool const _provides_default_bg;

		/* size of currently allocated virtual framebuffer, in bytes */
		size_t _buffer_size = 0;

		void _release_buffer()
		{
			if (!::Session::texture())
				return;

			typedef Pixel_rgb565 PT;

			/* retrieve pointer to texture from session */
			Chunky_dataspace_texture<PT> const *cdt =
				static_cast<Chunky_dataspace_texture<PT> const *>(::Session::texture());

			::Session::texture(0, false);
			::Session::input_mask(0);

			destroy(&_buffer_alloc, const_cast<Chunky_dataspace_texture<PT> *>(cdt));

			_buffer_alloc.upgrade(_buffer_size);
			_buffer_size = 0;
		}

		Canvas_base &_canvas() { return _canvas_accessor.canvas(); }

	public:

		/**
		 * Constructor
		 */
		Session_component(Session_label  const &label,
		                  View_stack           &view_stack,
		                  Rpc_entrypoint       &ep,
		                  Flush_merger         &flush_merger,
		                  Framebuffer::Session &framebuffer,
		                  Canvas_accessor      &canvas_accessor,
		                  int                   v_offset,
		                  bool                  provides_default_bg,
		                  bool                  stay_top,
		                  Allocator            &buffer_alloc,
		                  size_t                ram_quota)
		:
			::Session(label, v_offset, stay_top),
			_buffer_alloc(&buffer_alloc, ram_quota),
			_framebuffer(framebuffer),
			_framebuffer_session_component(view_stack, *this, flush_merger,
			                               framebuffer, canvas_accessor, *this),
			_canvas_accessor(canvas_accessor),
			_ep(ep), _view_stack(view_stack),
			_framebuffer_session_cap(_ep.manage(&_framebuffer_session_component)),
			_input_session_cap(_ep.manage(&_input_session_component)),
			_provides_default_bg(provides_default_bg)
		{
			_buffer_alloc.upgrade(ram_quota);
		}

		/**
		 * Destructor
		 */
		~Session_component()
		{
			_ep.dissolve(&_framebuffer_session_component);
			_ep.dissolve(&_input_session_component);

			while (View_component *vc = _view_list.first())
				destroy_view(vc->cap());

			_release_buffer();
		}

		void upgrade_ram_quota(size_t ram_quota) { _buffer_alloc.upgrade(ram_quota); }


		/******************************************
		 ** Nitpicker-internal session interface **
		 ******************************************/

		void submit_input_event(Input::Event e)
		{
			using namespace Input;

			/*
			 * Transpose absolute coordinates by session-specific vertical
			 * offset.
			 */
			if (e.ax() || e.ay())
				e = Event(e.type(), e.code(), e.ax(),
				          Genode::max(0, e.ay() - v_offset()), e.rx(), e.ry());

			_input_session_component.submit(&e);
		}


		/*********************************
		 ** Nitpicker session interface **
		 *********************************/

		Framebuffer::Session_capability framebuffer_session() {
			return _framebuffer_session_cap; }

		Input::Session_capability input_session() {
			return _input_session_cap; }

		View_capability create_view(View_capability parent_cap)
		{
			/* lookup parent view */
			View_component *parent_view =
				dynamic_cast<View_component *>(_ep.lookup_and_lock(parent_cap));

			/*
			 * FIXME: Do not allocate View meta data from Heap!
			 *        Use a heap partition!
			 */
			View_component *view = new (env()->heap())
				View_component(*this, _view_stack, _canvas_accessor, _ep,
				               parent_view ? &parent_view->view() : 0);

			if (parent_view)
				parent_view->view().add_child(&view->view());

			if (parent_view)
				parent_view->release();

			_view_list.insert(view);
			return _ep.manage(view);
		}

		void destroy_view(View_capability view_cap)
		{
			View_component *vc = dynamic_cast<View_component *>(_ep.lookup_and_lock(view_cap));
			if (!vc) return;

			_view_stack.remove_view(_canvas(), vc->view());
			_ep.dissolve(vc);
			_view_list.remove(vc);
			destroy(env()->heap(), vc);
		}

		int background(View_capability view_cap)
		{
			if (_provides_default_bg) {
				Object_pool<View_component>::Guard vc(_ep.lookup_and_lock(view_cap));
				vc->view().background(true);
				_view_stack.default_background(vc->view());
				return 0;
			}

			/* revert old background view to normal mode */
			if (::Session::background()) ::Session::background()->background(false);

			/* assign session background */
			Object_pool<View_component>::Guard vc(_ep.lookup_and_lock(view_cap));
			::Session::background(&vc->view());

			/* switch background view to background mode */
			if (::Session::background()) vc->view().background(true);

			return 0;
		}

		Framebuffer::Mode mode()
		{
			unsigned const width  = _framebuffer.mode().width();
			unsigned const height = _framebuffer.mode().height()
			                      - ::Session::v_offset();

			return Framebuffer::Mode(width, height,
			                         _framebuffer.mode().format());
		}

		void buffer(Framebuffer::Mode mode, bool use_alpha)
		{
			/* check if the session quota suffices for the specified mode */
			if (_buffer_alloc.quota() < ram_quota(mode, use_alpha))
				throw Nitpicker::Session::Out_of_metadata();

			_framebuffer_session_component.notify_mode_change(mode, use_alpha);
		}


		/*******************************
		 ** Buffer_provider interface **
		 *******************************/

		Buffer *realloc_buffer(Framebuffer::Mode mode, bool use_alpha)
		{
			_release_buffer();

			Area const size(mode.width(), mode.height());

			typedef Pixel_rgb565 PT;

			_buffer_size =
				Chunky_dataspace_texture<PT>::calc_num_bytes(size, use_alpha);

			Chunky_dataspace_texture<PT> * const texture =
				new (&_buffer_alloc) Chunky_dataspace_texture<PT>(size, use_alpha);

			if (!_buffer_alloc.withdraw(_buffer_size)) {
				destroy(&_buffer_alloc, texture);
				_buffer_size = 0;
				return 0;
			}

			::Session::texture(texture, use_alpha);
			::Session::input_mask(texture->input_mask_buffer());

			return texture;
		}
};


template <typename PT>
class Nitpicker::Root : public Genode::Root_component<Session_component>
{
	private:

		Session_list         &_session_list;
		Global_keys          &_global_keys;
		Framebuffer::Mode     _scr_mode;
		View_stack           &_view_stack;
		Flush_merger         &_flush_merger;
		Framebuffer::Session &_framebuffer;
		Canvas_accessor      &_canvas_accessor;
		int                   _default_v_offset;

	protected:

		Session_component *_create_session(const char *args)
		{
			PINF("create session with args: %s\n", args);
			size_t const ram_quota = Arg_string::find_arg(args, "ram_quota").ulong_value(0);

			int const v_offset = _default_v_offset;

			bool const stay_top  = Arg_string::find_arg(args, "stay_top").bool_value(false);

			size_t const required_quota = Input::Session_component::ev_ds_size();

			if (ram_quota < required_quota) {
				PWRN("Insufficient dontated ram_quota (%zd bytes), require %zd bytes",
				     ram_quota, required_quota);
				throw Root::Quota_exceeded();
			}

			size_t const unused_quota = ram_quota - required_quota;

			Session_label const label(args);
			bool const provides_default_bg = (strcmp(label.string(), "backdrop") == 0);

			Session_component *session = new (md_alloc())
				Session_component(Session_label(args), _view_stack, *ep(),
				                  _flush_merger, _framebuffer, _canvas_accessor,
				                  v_offset, provides_default_bg, stay_top,
				                  *md_alloc(), unused_quota);

			session->apply_session_color();
			_session_list.insert(session);
			_global_keys.apply_config(_session_list);

			return session;
		}

		void _upgrade_session(Session_component *s, const char *args)
		{
			size_t ram_quota = Arg_string::find_arg(args, "ram_quota").long_value(0);
			s->upgrade_ram_quota(ram_quota);
		}

		void _destroy_session(Session_component *session)
		{
			_session_list.remove(session);
			_global_keys.apply_config(_session_list);

			destroy(md_alloc(), session);
		}

	public:

		/**
		 * Constructor
		 */
		Root(Session_list &session_list, Global_keys &global_keys,
		     Rpc_entrypoint &session_ep, View_stack &view_stack,
		     Allocator &md_alloc, Flush_merger &flush_merger,
		     Framebuffer::Session &framebuffer, Canvas_accessor &canvas_accessor,
		     int default_v_offset)
		:
			Root_component<Session_component>(&session_ep, &md_alloc),
			_session_list(session_list), _global_keys(global_keys),
			_view_stack(view_stack), _flush_merger(flush_merger),
			_framebuffer(framebuffer), _canvas_accessor(canvas_accessor),
			_default_v_offset(default_v_offset)
		{ }
};


struct Nitpicker::Main
{
	Server::Entrypoint &ep;

	/*
	 * Sessions to the required external services
	 */
	Framebuffer::Connection framebuffer;
	Input::Connection       input;

	Input::Event * const ev_buf =
		env()->rm_session()->attach(input.dataspace());

	typedef Pixel_rgb565 PT;  /* physical pixel type */

	/*
	 * Initialize framebuffer and menu bar
	 *
	 * The framebuffer and menubar are encapsulated in a volatile object to
	 * allow their reconstruction at runtime as a response to resolution
	 * changes.
	 */
	struct Framebuffer_screen
	{
		Framebuffer::Session &framebuffer;

		Framebuffer::Mode const mode = framebuffer.mode();

		Attached_dataspace fb_ds = { framebuffer.dataspace() };

		Screen<PT> screen = { fb_ds.local_addr<PT>(), Area(mode.width(), mode.height()) };

		enum { MENUBAR_HEIGHT = 16 };

		/**
		 * Size of menubar pixel buffer in bytes
		 */
		size_t const menubar_size = sizeof(PT)*mode.width()*MENUBAR_HEIGHT;

		PT *menubar_pixels =
			(PT *)env()->heap()->alloc(menubar_size);

		Chunky_menubar<PT> menubar =
			{ menubar_pixels, Area(mode.width(), MENUBAR_HEIGHT) };

		/**
		 * Constructor
		 */
		Framebuffer_screen(Framebuffer::Session &fb) : framebuffer(fb) { }

		/**
		 * Destructor
		 */
		~Framebuffer_screen() { env()->heap()->free(menubar_pixels, menubar_size); }
	};

	Genode::Volatile_object<Framebuffer_screen> fb_screen = { framebuffer };

	struct Canvas_accessor : ::Canvas_accessor
	{
		Genode::Volatile_object<Framebuffer_screen> &fb_screen;

		Canvas_accessor(Genode::Volatile_object<Framebuffer_screen> &fb_screen)
		: fb_screen(fb_screen) { }

		Canvas_base &canvas() override { return fb_screen->screen; }

	} canvas_accessor = { fb_screen };

	void handle_fb_mode(unsigned);

	Signal_rpc_member<Main> fb_mode_dispatcher = { ep, *this, &Main::handle_fb_mode };

	/*
	 * User-input policy
	 */
	Global_keys global_keys;

	Session_list session_list;

	User_state user_state = { global_keys, fb_screen->screen.size(), fb_screen->menubar };

	/*
	 * Create view stack with default elements
	 */
	Area                   mouse_size { big_mouse.w, big_mouse.h };
	Mouse_cursor<PT const> mouse_cursor { (PT const *)&big_mouse.pixels[0][0],
	                                      mouse_size, user_state };

	Background background = { Area(99999, 99999) };

	/*
	 * Initialize Nitpicker root interface
	 */
	Genode::Sliced_heap sliced_heap = { env()->ram_session(), env()->rm_session() };

	Root<PT> np_root = { session_list, global_keys, ep.rpc_ep(),
	                     user_state, sliced_heap, fb_screen->screen,
	                     framebuffer, canvas_accessor,
	                     Framebuffer_screen::MENUBAR_HEIGHT };

	Genode::Reporter pointer_reporter = { "pointer" };

	/*
	 * Configuration-update dispatcher, executed in the context of the RPC
	 * entrypoint.
	 *
	 * In addition to installing the signal dispatcher, we trigger first signal
	 * manually to turn the initial configuration into effect.
	 */
	void handle_config(unsigned);

	Signal_rpc_member<Main> config_dispatcher = { ep, *this, &Main::handle_config};

	/**
	 * Signal handler invoked on the reception of user input
	 */
	void handle_input(unsigned);

	Signal_rpc_member<Main> input_dispatcher = { ep, *this, &Main::handle_input };

	/*
	 * Dispatch input on periodic timer signals every 10 milliseconds
	 */
	Timer::Connection timer;

	Main(Server::Entrypoint &ep) : ep(ep)
	{
		fb_screen->menubar.state(Menubar_state(user_state, "", "", BLACK));

		user_state.default_background(background);
		user_state.stack(canvas_accessor.canvas(), mouse_cursor);
		user_state.stack(canvas_accessor.canvas(), fb_screen->menubar);
		user_state.stack(canvas_accessor.canvas(), background);

		config()->sigh(config_dispatcher);
		Signal_transmitter(config_dispatcher).submit();

		timer.sigh(input_dispatcher);
		timer.trigger_periodic(10*1000);

		framebuffer.mode_sigh(fb_mode_dispatcher);

		env()->parent()->announce(ep.manage(np_root));
	}
};


void Nitpicker::Main::handle_input(unsigned)
{
	/*
	 * If kill mode is already active, we got recursively called from
	 * within this 'input_func' (via 'wait_and_dispatch_one_signal').
	 * In this case, return immediately. New input events will be
	 * processed in the local 'do' loop.
	 */
	if (user_state.kill())
		return;

	do {
		Point const old_mouse_pos = user_state.mouse_pos();

		/* handle batch of pending events */
		if (input.is_pending())
			import_input_events(ev_buf, input.flush(), user_state,
			                    canvas_accessor.canvas());

		Point const new_mouse_pos = user_state.mouse_pos();

		/* report mouse-position updates */
		if (pointer_reporter.is_enabled() && old_mouse_pos != new_mouse_pos)
			Genode::Reporter::Xml_generator xml(pointer_reporter, [&] ()
			{
				xml.attribute("xpos", new_mouse_pos.x());
				xml.attribute("ypos", new_mouse_pos.y());
			});

		/* update mouse cursor */
		if (old_mouse_pos != new_mouse_pos)
			user_state.viewport(canvas_accessor.canvas(), mouse_cursor,
			                    Rect(new_mouse_pos, mouse_size),
			                    Point(), true);

		/* flush dirty pixels to physical frame buffer */
		if (fb_screen->screen.defer == false) {
			Rect const r = fb_screen->screen.to_be_flushed();
			if (r.valid())
				framebuffer.refresh(r.x1(), r.y1(), r.w(), r.h());
			fb_screen->screen.reset();
		}
		fb_screen->screen.defer = false;

		/*
		 * In kill mode, we do not leave the dispatch function in order to
		 * block RPC calls from Nitpicker clients. We block for signals
		 * here to stay responsive to user input and configuration changes.
		 * Nested calls of 'input_func' are prevented by the condition
		 * check for 'user_state.kill()' at the beginning of the handler.
		 */
		if (user_state.kill())
			Server::wait_and_dispatch_one_signal();

	} while (user_state.kill());
}


void Nitpicker::Main::handle_config(unsigned)
{
	config()->reload();

	/* update global keys policy */
	global_keys.apply_config(session_list);

	/* update background color */
	try {
		config()->xml_node().sub_node("background")
		.attribute("color").value(&background.color);
	} catch (...) { }

	/* enable or disable reporting */
	try {
		pointer_reporter.enabled(config()->xml_node().sub_node("report")
		                                             .attribute("pointer")
		                                             .has_value("yes"));
	} catch (...) { }

	/* update session policies */
	for (::Session *s = session_list.first(); s; s = s->next())
		s->apply_session_color();

	/* redraw */
	user_state.update_all_views(canvas_accessor.canvas());
}


void Nitpicker::Main::handle_fb_mode(unsigned)
{
	/* save state of menu bar */
	Menubar_state menubar_state = fb_screen->menubar.state();

	/* remove old version of menu bar from view stack */
	user_state.remove_view(canvas_accessor.canvas(), fb_screen->menubar, false);

	/* reconstruct framebuffer screen and menu bar */
	fb_screen.construct(framebuffer);

	/* let the view stack use the new size */
	user_state.size(Area(fb_screen->mode.width(), fb_screen->mode.height()));

	/* load original state into new menu bar */
	fb_screen->menubar.state(menubar_state);

	/* re-insert menu bar behind mouse cursor */
	user_state.stack(canvas_accessor.canvas(), fb_screen->menubar, &mouse_cursor);

	/* redraw */
	user_state.update_all_views(canvas_accessor.canvas());
}


/************
 ** Server **
 ************/

namespace Server {

	char const *name() { return "nitpicker_ep"; }

	size_t stack_size() { return 4*1024*sizeof(long); }

	void construct(Entrypoint &ep)
	{
		static Nitpicker::Main nitpicker(ep);
	}
}
