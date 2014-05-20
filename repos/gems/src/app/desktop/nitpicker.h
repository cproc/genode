/*
 * \brief  Virtualized nitpicker service announced to the outside world
 * \author Norman Feske
 * \date   2014-02-14
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _NITPICKER_H_
#define _NITPICKER_H_

/* Genode includes */
#include <util/list.h>
#include <os/server.h>
#include <os/surface.h>
#include <cap_session/connection.h>
#include <root/component.h>
#include <nitpicker_session/nitpicker_session.h>
#include <nitpicker_view/nitpicker_view.h>
#include <nitpicker_view/capability.h>
#include <input_session/capability.h>

/* local includes */
#include <window_registry.h>
#include <input_session.h>
#include <nitpicker_slave.h>


namespace Desktop {

	using Genode::Rpc_object;
	using Genode::List;
	using Genode::Allocator;
	using Genode::Affinity;
	using Genode::static_cap_cast;
	using Genode::Signal_rpc_member;
}

namespace Desktop { namespace Nitpicker {

	using namespace ::Nitpicker;

	class View_component;
	class Session_component;
	class Root;

	typedef Genode::Surface_base::Rect  Rect;
	typedef Genode::Surface_base::Point Point;
	typedef Genode::String<200> Session_label;
} }


class Desktop::Nitpicker::View_component : public Rpc_object<View>,
                                           public List<View_component>::Element
{
	private:

		typedef Genode::String<100> Title;

		Session_label       _session_label;
		Title               _title;
		Nitpicker::Session &_real_nitpicker;
		Window_registry    &_window_registry;
		View_component     *_parent;
		View_capability     _neighbor;
		Rect                _geometry;
		Point               _anchor_position;
		Point               _buffer_offset;
		bool                _neighbor_behind = false;
		View_capability     _real_view;
		Window_registry::Id _win_id;

	public:

		/**
		 * Constructor
		 *
		 * \oaram real_view  view capability of the back-end nitpicker session
		 * \param id         window ID, or an invalid ID for creating a child
		 *                   view
		 */
		View_component(Nitpicker::Session &real_nitpicker,
		               Window_registry &window_registry,
		               View_component *parent,
		               Session_label const &session_label)
		:
			_session_label(session_label),
			_real_nitpicker(real_nitpicker),
			_window_registry(window_registry),
			_parent(parent)
		{ }

		~View_component()
		{
			if (_win_id.valid())
				_window_registry.destroy(_win_id);
		}

		bool is_top_level() const { return _parent == 0; }

		bool has_win_id(Window_registry::Id id) const { return id == _win_id; }

		View_capability real_view() { return _real_view; }

		bool has_parent(View_component &parent) const { return _parent == &parent; }

		/**
		 * Return position as set by the client
		 */
		Point virtual_position() const { return _geometry.p1(); }

		/**
		 * Return actual position of view on screen
		 */
		Point anchor_position() const { return _anchor_position; }


		/*****************************************
		 ** Decorator_anchor_callback interface **
		 *****************************************/

		void window_anchor_view(View_capability window_anchor_view)
		{
			/*
			 * If the real view already exists, the call of this function
			 * indicates a changed stacking order.
			 */
			if (_real_view.valid()) {
				stack(window_anchor_view, false, false);
				return;
			}

			_real_view = _real_nitpicker.create_view(window_anchor_view);

			viewport(_geometry.x1(), _geometry.y1(), _geometry.w(), _geometry.h(),
			         _buffer_offset.x(), _buffer_offset.y(), true);

			stack(_neighbor, _neighbor_behind, true);

			title(_title.string());
		}

		void window_anchor_position(Point pos) { _anchor_position = pos; }

		void update()
		{
			if (!_real_view.valid() && _parent && _parent->real_view().valid())
				window_anchor_view(_parent->real_view());
		}


		/*******************************
		 ** Nitpicker::View interface **
		 *******************************/

		int viewport(int x, int y, int w, int h,
		             int buf_x, int buf_y, bool redraw) override
		{
			_geometry = Rect(Point(x, y), Area(w, h));
			_buffer_offset = Point(buf_x, buf_y);

			if (is_top_level()) {

				/* add window to the window-list model on the first call */
				if (!_win_id.valid())
					_win_id = _window_registry.create();

				_window_registry.size(_win_id, Area(w, h));

				/* XXX constrain view geometry to window size? */
				if (_real_view.valid())
					View_client(_real_view).viewport(0, 0, w, h, buf_x, buf_y, redraw);

			} else {

				if (_real_view.valid())
					View_client(_real_view).viewport(x, y, w, h, buf_x, buf_y, redraw);
			}
			return 0;
		}

		int stack(View_capability neighbor,
		          bool behind, bool redraw) override
		{
			_neighbor = neighbor;
			_neighbor_behind = behind;

			/* XXX lookup neighbor */
			if (_real_view.valid())
				View_client(_real_view).stack(View_capability(), true, true);
			return 0;
		}

		int title(Nitpicker::View::Title const &title) override
		{
			bool const has_title = Genode::strlen(title.string()) > 0;

			/*
			 * The window title is the concatenation of the session label with
			 * view title.
			 */
			char window_title[256];
			Genode::snprintf(window_title, sizeof(window_title), "%s%s%s",
			                 _session_label.string(),
			                 has_title ? " " : "", title.string());

			_title = title.string();

			if (is_top_level())
				_window_registry.title(_win_id, window_title);

			if (_real_view.valid())
				View_client(_real_view).title(title);

			return 0;
		}
};


class Desktop::Nitpicker::Session_component : public Genode::Rpc_object<Session>,
                                              public List<Session_component>::Element
{
	private:

		Session_label             _session_label;
		Nitpicker_slave          &_nitpicker;
		Genode::Ram_session      &_ram;
		Session_client            _session;
		Window_registry          &_window_registry;
		Entrypoint               &_ep;
		Allocator                &_view_alloc;
		List<View_component>      _views;
		Input::Session_component  _input_session;
		Input::Session_capability _input_session_cap;

		Input::Session_client _nitpicker_input    { _session.input_session() };
		Attached_dataspace    _nitpicker_input_ds { _nitpicker_input.dataspace() };

		/*
		 * Timer session used for polling input from the nitpicker session.
		 */
		Timer::Connection _timer;
		Signal_rpc_member<Session_component> _input_dispatcher {
			_ep, *this, &Session_component::_input_handler };

		Point _input_origin() const
		{
			for (View_component const *v = _views.first(); v; v = v->next())
				if (v->is_top_level())
					return v->virtual_position() - v->anchor_position();

			return Point();
		}

		/**
		 * Translate input event to the client's coordinate system
		 */
		Input::Event _translate_event(Input::Event const ev, Point const input_origin)
		{
			switch (ev.type()) {

			case Input::Event::MOTION:
			case Input::Event::PRESS:
			case Input::Event::RELEASE:
			case Input::Event::FOCUS:
			case Input::Event::LEAVE:
				{
					Point abs_pos = Point(ev.ax(), ev.ay()) + input_origin;
					return Input::Event(ev.type(), ev.code(),
					                    abs_pos.x(), abs_pos.y(), 0, 0);
				}

			case Input::Event::INVALID:
			case Input::Event::WHEEL:
				return ev;
			}
			return Input::Event();
		}

		void _input_handler(unsigned)
		{
			Point const input_origin = _input_origin();

			Input::Event const * const events =
				_nitpicker_input_ds.local_addr<Input::Event>();

			while (_nitpicker_input.is_pending()) {

				size_t const num_events = _nitpicker_input.flush();

				/* we trust nitpicker to return a valid number of events */

				for (size_t i = 0; i < num_events; i++)
					_input_session.submit(_translate_event(events[i], input_origin));
			}
		}

	public:

		/**
		 * Constructor
		 *
		 * \param nitpicker  real nitpicker service
		 * \param ep         entrypoint used for managing the views
		 */
		Session_component(Nitpicker_slave     &nitpicker,
		                  Genode::Ram_session &ram,
		                  Window_registry     &window_registry,
		                  Entrypoint          &ep,
		                  Allocator           &view_alloc,
		                  Session_label const &session_label)
		:
			_session_label(session_label),
			_nitpicker(nitpicker),
			_ram(ram),
			_session(_nitpicker.session(_session_label.string())),
			_window_registry(window_registry),
			_ep(ep), _view_alloc(view_alloc),
			_input_session_cap(_ep.manage(_input_session))
		{
			_timer.sigh(_input_dispatcher);
			_timer.trigger_periodic(10*1000);
		}

		~Session_component()
		{
			while (View_component *view = _views.first())
				destroy_view(view->cap());

			_nitpicker.close(_session);
			_ep.dissolve(_input_session);
		}

		void upgrade_ram_quota(size_t amount)
		{
			_nitpicker.upgrade(_session, _ram, amount);
		}

		void window_anchor_view(Window_registry::Id id, View_capability anchor)
		{
			for (View_component *v = _views.first(); v; v = v->next()) {
				if (!v->has_win_id(id))
					continue;

				v->window_anchor_view(anchor);

				/*
				 * Now that we have view for the top-level window, let's go
				 * through the views, searching for possible child views of the
				 * top-level views.
				 */
				for (View_component *cv = _views.first(); cv; cv = cv->next()) {
					if (!cv->has_parent(*v))
						continue;

					/* use top-level view as anchor for the child view */
					cv->window_anchor_view(v->real_view());
				}
			}
		}

		void update_views()
		{
			for (View_component *v = _views.first(); v; v = v->next()) {
				v->update();
			}
		}

		void window_anchor_position(Window_registry::Id id, Point position)
		{
			for (View_component *v = _views.first(); v; v = v->next()) {
				if (!v->has_win_id(id))
					continue;

				v->window_anchor_position(position);
			}
		}


		/*********************************
		 ** Nitpicker session interface **
		 *********************************/
		
		Framebuffer::Session_capability framebuffer_session() override
		{
			return _session.framebuffer_session();
		}

		Input::Session_capability input_session() override
		{
			return _input_session_cap;
		}

		View_capability create_view(View_capability parent_cap) override
		{
			/*
			 * If no parent was specified, we create a top-level window for
			 * the view.
			 */
			View_component * const parent =
				parent_cap.valid() ? dynamic_cast<View_component *>(_ep.rpc_ep().lookup_and_lock(parent_cap)) : 0;

			View_component * const view =
				new (_view_alloc) View_component(_session, _window_registry,
				                                 parent, _session_label);

			Nitpicker::View_capability view_cap = _ep.manage(*view);

			_views.insert(view);

			if (parent)
				parent->release();

			return view_cap;
		}

		void destroy_view(View_capability view_cap) override
		{
			View_component * const view =
				dynamic_cast<View_component *>(_ep.rpc_ep().lookup_and_lock(view_cap));

			if (!view) {
				PWRN("view lookup failed during view destruction");
				return;
			}

			_views.remove(view);

			_ep.dissolve(*view);

			Genode::destroy(&_view_alloc, view);
		}

		int background(View_capability) override
		{
			return false;
		}

		Framebuffer::Mode mode() override
		{
			return _session.mode();
		}

		void buffer(Framebuffer::Mode mode, bool use_alpha) override
		{
			_session.buffer(mode, use_alpha);
		}
};


class Desktop::Nitpicker::Root : public Genode::Root_component<Session_component>,
                                 public Decorator_anchor_callback
{
	private:

		Entrypoint &_ep;

		Nitpicker_slave &_nitpicker_slave;

		Genode::Ram_session &_ram;

		enum { STACK_SIZE = 1024*sizeof(long) };

		Window_registry &_window_registry;

		List<Session_component> _sessions;

	protected:

		Session_component *_create_session(const char *args) override
		{
			char buf[200];
			Genode::Arg_string::find_arg(args, "label").string(buf, sizeof(buf), "");

			Session_component *session = new (md_alloc())
				Session_component(_nitpicker_slave, _ram, _window_registry,
				                  _ep, *md_alloc(), Session_label(buf));

			_sessions.insert(session);

			return session;
		}

		void _destroy_session(Session_component *session)
		{
			_sessions.remove(session);

			Root_component<Session_component>::_destroy_session(session);
		}

		void _upgrade_session(Session_component *s, const char *args)
		{
			size_t amount = Arg_string::find_arg(args, "ram_quota").ulong_value(0);
			PDBG("upgrade_ram_quota %zd", amount);
			s->upgrade_ram_quota(amount);
		}

	public:

		/**
		 * Constructor
		 *
		 * \parent nitpicker_slave  real nitpicker server
		 */
		Root(Entrypoint &ep, Nitpicker_slave &nitpicker_slave,
		     Window_registry &window_registry, Allocator &md_alloc,
		     Genode::Ram_session &ram)
		:
			Root_component<Session_component>(&ep.rpc_ep(), &md_alloc),
			_ep(ep), _nitpicker_slave(nitpicker_slave), _ram(ram),
			_window_registry(window_registry)
		{
			Genode::env()->parent()->announce(_ep.manage(*this));
		}


		/*****************************************
		 ** Decorator_anchor_callback interface **
		 *****************************************/

		void window_anchor_view(Window_registry::Id id, View_capability anchor) override
		{
			for (Session_component *s = _sessions.first(); s; s = s->next())
				s->window_anchor_view(id, anchor);

			for (Session_component *s = _sessions.first(); s; s = s->next())
				s->update_views();
		}

		void window_anchor_position(Window_registry::Id id, Point position) override
		{
			for (Session_component *s = _sessions.first(); s; s = s->next())
				s->window_anchor_position(id, position);
		}
};

#endif /* _NITPICKER_H_ */
