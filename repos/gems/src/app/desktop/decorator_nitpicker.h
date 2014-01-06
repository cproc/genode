/*
 * \brief  Local nitpicker service provided to decorator
 * \author Norman Feske
 * \date   2014-02-14
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _DECORATOR_NITPICKER_H_
#define _DECORATOR_NITPICKER_H_

/* Genode includes */
#include <util/string.h>
#include <os/server.h>
#include <os/attached_dataspace.h>
#include <nitpicker_session/client.h>
#include <nitpicker_view/client.h>
#include <nitpicker_view/capability.h>
#include <input_session/client.h>
#include <input/event.h>
#include <input/component.h>

/* local includes */
#include <nitpicker_slave.h>
#include <window_registry.h>

namespace Desktop { class Main;
	using Genode::size_t;
	using Genode::Allocator;
	using Server::Entrypoint;
	using Genode::Ram_session;
	using Genode::Arg_string;
	using Genode::Object_pool;
	using Genode::Attached_dataspace;
	using Genode::Signal_rpc_member;
}


namespace Desktop { struct Decorator_nitpicker_service; struct Decorator_anchor_callback; }


struct Desktop::Decorator_anchor_callback
{
	virtual void window_anchor_view(Window_registry::Id win_id,
	                                Nitpicker::View_capability anchor_view) = 0;

	virtual void window_anchor_position(Window_registry::Id win_id,
	                                    Point position) = 0;
};


struct Desktop::Decorator_nitpicker_service : Genode::Service, Genode::Noncopyable
{
	private:

		Nitpicker_slave           &_nitpicker_slave;
		Entrypoint                &_ep;
		Allocator                 &_md_alloc;
		Ram_session               &_ram;
		Input::Session_component  &_window_layouter_input;
		Decorator_anchor_callback &_anchor_callback;

		class View_component : public Genode::Rpc_object<Nitpicker::View>,
		                       public List<View_component>::Element
		{
			private:

				Entrypoint                &_ep;
				Decorator_anchor_callback &_anchor_callback;
				Nitpicker::View_client     _real_view;
				Window_registry::Id        _id;

				typedef Nitpicker::View_capability View_capability;

			public:

				View_component(Entrypoint &ep,
				               Decorator_anchor_callback &anchor_callback,
				               View_capability real_view)
				:
					_ep(ep),
					_anchor_callback(anchor_callback),
					_real_view(real_view)
				{ }

				View_capability real_view() { return _real_view; }

				int viewport(int x, int y, int w, int h,
				             int buf_x, int buf_y, bool redraw) override
				{
					/* if the anchor view changes position, propagate the event */
					if (_id.valid())
						_anchor_callback.window_anchor_position(_id, Point(x, y));

					return _real_view.viewport(x, y, w, h, buf_x, buf_y, redraw);
				}

				int stack(View_capability neighbor_cap,
				          bool behind, bool redraw) override
				{
					Object_pool<View_component>::Guard
						neighbor(_ep.rpc_ep().lookup_and_lock(neighbor_cap));

					/* if the anchor view is re-stacked, propagate the event */
					if (_id.valid())
						_anchor_callback.window_anchor_view(_id, _real_view);

					if (neighbor)
						return _real_view.stack(neighbor->real_view(), behind, redraw);
					else
						return _real_view.stack(View_capability(), behind, redraw);
				}

				int title(Nitpicker::View::Title const &title) override
				{
					unsigned long id = 0;
					Genode::ascii_to(title.string(), &id);

					if (id > 0)
						_id = Window_registry::Id(id);

					return 0;
				}
		};

		struct Session : Genode::Rpc_object<Nitpicker::Session>
		{
			typedef Nitpicker::View_capability View_capability;

			Nitpicker::Session_client _nitpicker_session;

			Input::Session_client _nitpicker_input { _nitpicker_session.input_session() };

			Attached_dataspace _nitpicker_input_ds { _nitpicker_input.dataspace() };

			Input::Session_component &_window_layouter_input;

			Decorator_anchor_callback &_anchor_callback;

			List<View_component> _views;

			Entrypoint &_ep;

			Allocator &_md_alloc;

			Signal_rpc_member<Session> _input_dispatcher { _ep, *this, &Session::_input_handler };


			/**
			 * Constructor
			 *
			 * \param ep  entrypoint used for dispatching signals
			 */
			Session(Nitpicker_slave &nitpicker_slave, Ram_session &ram,
			        Entrypoint &ep, Allocator &md_alloc,
			        Input::Session_component &window_layouter_input,
			        Decorator_anchor_callback &anchor_callback)
			:
				_nitpicker_session(nitpicker_slave.session("decorator")),
				_window_layouter_input(window_layouter_input),
				_anchor_callback(anchor_callback),
				_ep(ep), _md_alloc(md_alloc)
			{
				_nitpicker_input.sigh(_input_dispatcher);
			}

			~Session()
			{
				while (View_component *view = _views.first())
					destroy_view(view->cap());
			}

			Nitpicker::Session_capability cap() { return _nitpicker_session; }

			void _input_handler(unsigned)
			{
				Input::Event const * const events =
					_nitpicker_input_ds.local_addr<Input::Event>();

				while (_nitpicker_input.is_pending()) {

					size_t const num_events = _nitpicker_input.flush();

					/* we trust nitpicker to return a valid number of events */

					for (size_t i = 0; i < num_events; i++)
						_window_layouter_input.submit(events[i]);
				}
			}


			/*********************************
			 ** Nitpicker session interface **
			 *********************************/
			
			Framebuffer::Session_capability framebuffer_session() override
			{
				return _nitpicker_session.framebuffer_session();
			}

			Input::Session_capability input_session() override
			{
				/*
				 * Deny input to the decorator. User input referring to the
				 * window decorations is routed to the window manager.
				 */
				return Input::Session_capability();
			}

			View_capability create_view(View_capability parent) override
			{
				View_component * const view = new (_md_alloc)
					View_component(_ep, _anchor_callback,
					               _nitpicker_session.create_view(parent));

				_views.insert(view);

				return _ep.manage(*view);
			}

			void destroy_view(View_capability view_cap) override
			{
				View_component * const view =
					dynamic_cast<View_component *>(_ep.rpc_ep().lookup_and_lock(view_cap));

				_nitpicker_session.destroy_view(view->real_view());

				_views.remove(view);

				_ep.dissolve(*view);

				Genode::destroy(&_md_alloc, view);
			}

			int background(View_capability view) override
			{
				return 0;
			}

			Framebuffer::Mode mode() override
			{
				return _nitpicker_session.mode();
			}

			void buffer(Framebuffer::Mode mode, bool use_alpha) override
			{
				_nitpicker_session.buffer(mode, use_alpha);
			}
		};

	public:

		Decorator_nitpicker_service(Nitpicker_slave &nitpicker_slave,
		                            Entrypoint &ep, Allocator &md_alloc,
		                            Ram_session &ram,
		                            Input::Session_component &window_layouter_input,
		                            Decorator_anchor_callback &anchor_callback)
		:
			Service("Nitpicker"),
			_nitpicker_slave(nitpicker_slave), _ep(ep), _md_alloc(md_alloc),
			_ram(ram), _window_layouter_input(window_layouter_input),
			_anchor_callback(anchor_callback)
		{ }

		Genode::Session_capability
		session(const char *, Genode::Affinity const &) override
		{
			Session *s = new (_md_alloc)
				Session(_nitpicker_slave, _ram, _ep, _md_alloc,
				        _window_layouter_input, _anchor_callback);

			return _ep.manage(*s);
		}

		void upgrade(Genode::Session_capability session, const char *args) override
		{
			size_t ram_quota = Arg_string::find_arg(args, "ram_quota").ulong_value(0);

			typedef typename Object_pool<Session>::Guard Object_guard;
			Object_guard np_session(_ep.rpc_ep().lookup_and_lock(session));

			if (np_session)
				_nitpicker_slave.upgrade(np_session->cap(), _ram, ram_quota);
		}

		void close(Genode::Session_capability) { }
};

#endif /* _DECORATOR_NITPICKER_H_ */
