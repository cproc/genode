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
	using Genode::Ram_session_client;
	using Genode::Ram_session_capability;
	using Genode::Arg_string;
	using Genode::Object_pool;
	using Genode::Attached_dataspace;
	using Genode::Attached_ram_dataspace;
	using Genode::Signal_rpc_member;
}


namespace Desktop {

	struct Decorator_nitpicker_session;
	struct Decorator_nitpicker_service;
	struct Decorator_anchor_callback;
	struct Decorator_anchor_registry;
}


struct Desktop::Decorator_anchor_callback
{
	virtual void window_anchor_view(Window_registry::Id win_id,
	                                Nitpicker::View_capability anchor_view) = 0;

	virtual void window_anchor_position(Window_registry::Id win_id,
	                                    Point position) = 0;
};


class Desktop::Decorator_anchor_registry
{
	public:

		/**
		 * Exception type
		 */
		struct Lookup_failed { };

	private:

		struct Entry : List<Entry>::Element
		{
			Nitpicker::Session::View_handle const decorator_view_handle;
			Window_registry::Id             const win_id;

			Entry(Nitpicker::Session::View_handle decorator_view_handle,
			      Window_registry::Id             win_id)
			:
				decorator_view_handle(decorator_view_handle),
				win_id(win_id)
			{ }
		};

		List<Entry>  _list;
		Allocator   &_entry_alloc;

		Entry const &_lookup(Nitpicker::Session::View_handle view_handle) const
		{
			for (Entry const *e = _list.first(); e; e = e->next()) {
				if (e->decorator_view_handle == view_handle)
					return *e;
			}

			throw Lookup_failed();
		}

		void _remove(Entry const &e)
		{
			_list.remove(&e);
			destroy(_entry_alloc, const_cast<Entry *>(&e));
		}

	public:

		Decorator_anchor_registry(Allocator &entry_alloc)
		:
			_entry_alloc(entry_alloc)
		{ }

		~Decorator_anchor_registry()
		{
			while (Entry *e = _list.first())
				_remove(*e);
		}

		void insert(Nitpicker::Session::View_handle decorator_view_handle,
		            Window_registry::Id             win_id)
		{
			Entry *e = new (_entry_alloc) Entry(decorator_view_handle, win_id);
			_list.insert(e);
		}

		/**
		 * Lookup window ID for a given decorator anchor view
		 *
		 * \throw Lookup_failed
		 */
		Window_registry::Id lookup(Nitpicker::Session::View_handle view_handle) const
		{
			return _lookup(view_handle).win_id;
		}

		/**
		 * Remove entry
		 *
		 * \throw Lookup_failed
		 */
		void remove(Nitpicker::Session::View_handle view_handle)
		{
			_remove(_lookup(view_handle));
		}
};


struct Desktop::Decorator_nitpicker_session : Genode::Rpc_object<Nitpicker::Session>
{
	typedef Nitpicker::View_capability      View_capability;
	typedef Nitpicker::Session::View_handle View_handle;

	Nitpicker_slave &_nitpicker;

	Ram_session_client _ram;

	size_t _ram_quota = 0;  /* session quota */

	Nitpicker::Session_client _nitpicker_session;

	typedef Nitpicker::Session::Command_buffer Command_buffer;

	Attached_ram_dataspace _command_ds { &_ram, sizeof(Command_buffer) };

	Command_buffer &_command_buffer = *_command_ds.local_addr<Command_buffer>();

	Input::Session_client _nitpicker_input { _nitpicker_session.input_session() };

	Attached_dataspace _nitpicker_input_ds { _nitpicker_input.dataspace() };

	Input::Session_component &_window_layouter_input;

	Decorator_anchor_callback &_anchor_callback;

	/* XXX don't allocate anchor-registry entries from heap */
	Decorator_anchor_registry _anchor_registry { *Genode::env()->heap() };

	Entrypoint &_ep;

	Allocator &_md_alloc;

	Signal_rpc_member<Decorator_nitpicker_session>
		_input_dispatcher { _ep, *this, &Decorator_nitpicker_session::_input_handler };

	/**
	 * Constructor
	 *
	 * \param ep  entrypoint used for dispatching signals
	 */
	Decorator_nitpicker_session(Nitpicker_slave &nitpicker_slave,
	                            Ram_session_capability ram,
	                            Entrypoint &ep, Allocator &md_alloc,
	                            Input::Session_component &window_layouter_input,
	                            Decorator_anchor_callback &anchor_callback)
	:
		_nitpicker(nitpicker_slave),
		_ram(ram),
		_nitpicker_session(_nitpicker.session("decorator")),
		_window_layouter_input(window_layouter_input),
		_anchor_callback(anchor_callback),
		_ep(ep), _md_alloc(md_alloc)
	{
		_nitpicker_input.sigh(_input_dispatcher);
	}

	~Decorator_nitpicker_session()
	{
		/* withdraw session quota from nitpicker slave */
		if (_nitpicker.ram().transfer_quota(_ram, _ram_quota) != 0)
			PWRN("misbehaving nitpicker slave, could not regain %zd bytes",
				 _ram_quota);
	}

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

	void _execute_command(Command const &cmd)
	{
		switch (cmd.opcode) {

		case Command::OP_TITLE:
			{
				unsigned long id = 0;
				Genode::ascii_to(cmd.title.title.string(), &id);

				if (id > 0)
					_anchor_registry.insert(cmd.title.view,
					                        Window_registry::Id(id));
				return;
			}

		case Command::OP_TO_FRONT:

			try {

				/* if the anchor view is re-stacked, propagate the event */
				Window_registry::Id win_id = _anchor_registry.lookup(cmd.to_front.view);

				Nitpicker::View_capability view_cap =
					_nitpicker_session.view_capability(cmd.to_front.view);

				_anchor_callback.window_anchor_view(win_id, view_cap);
			}
			catch (Decorator_anchor_registry::Lookup_failed) { }

			_nitpicker_session.enqueue(cmd);
			return;

		case Command::OP_GEOMETRY:

			try {

				/* if the anchor view changes position, propagate the event */
				Window_registry::Id win_id = _anchor_registry.lookup(cmd.geometry.view);
				_anchor_callback.window_anchor_position(win_id,
				                                        cmd.geometry.rect.p1());
			}
			catch (Decorator_anchor_registry::Lookup_failed) { }

			_nitpicker_session.enqueue(cmd);
			return;

		case Command::OP_OFFSET:
		case Command::OP_TO_BACK:
		case Command::OP_BACKGROUND:
		case Command::OP_NOP:

			_nitpicker_session.enqueue(cmd);
			return;
		}
	}

	void upgrade(size_t const amount)
	{
		_ram_quota += amount;
		_nitpicker.upgrade(_nitpicker_session, _ram, amount);
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

	View_handle create_view(View_handle parent) override
	{
		View_handle view = _nitpicker_session.create_view(parent);

		/* XXX create local view handle representation */

		return view;
	}

	void destroy_view(View_handle view) override
	{
		_nitpicker_session.destroy_view(view);
	}

	View_handle view_handle(View_capability view_cap) override
	{
		return _nitpicker_session.view_handle(view_cap);
	}

	View_capability view_capability(View_handle view) override
	{
		return _nitpicker_session.view_capability(view);
	}

	void release_view_handle(View_handle view) override
	{
		/* XXX dealloc View_ptr */
		_nitpicker_session.release_view_handle(view);
	}

	Genode::Dataspace_capability command_dataspace() override
	{
		return _command_ds.cap();
	}

	void execute() override
	{
		for (unsigned i = 0; i < _command_buffer.num(); i++) {
			try {
				_execute_command(_command_buffer.get(i));
			}
			catch (...) {
				PWRN("unhandled exception while processing command from decorator");
			}
		}
		_nitpicker_session.execute();
	}

	Framebuffer::Mode mode() override
	{
		return _nitpicker_session.mode();
	}

	void buffer(Framebuffer::Mode mode, bool use_alpha) override
	{
		_nitpicker_session.buffer(mode, use_alpha);
	}

	void focus(Genode::Capability<Nitpicker::Session>) { }
};


struct Desktop::Decorator_nitpicker_service : Genode::Service, Genode::Noncopyable
{
	private:

		Nitpicker_slave           &_nitpicker_slave;
		Entrypoint                &_ep;
		Allocator                 &_md_alloc;
		Ram_session_capability     _ram;
		Input::Session_component  &_window_layouter_input;
		Decorator_anchor_callback &_anchor_callback;


	public:

		Decorator_nitpicker_service(Nitpicker_slave &nitpicker_slave,
		                            Entrypoint &ep, Allocator &md_alloc,
		                            Ram_session_capability ram,
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
			Decorator_nitpicker_session *s = new (_md_alloc)
				Decorator_nitpicker_session(_nitpicker_slave, _ram, _ep, _md_alloc,
				        _window_layouter_input, _anchor_callback);

			return _ep.manage(*s);
		}

		void upgrade(Genode::Session_capability session, const char *args) override
		{
			size_t const amount = Arg_string::find_arg(args, "ram_quota").ulong_value(0);

			typedef typename Object_pool<Decorator_nitpicker_session>::Guard Object_guard;
			Object_guard np_session(_ep.rpc_ep().lookup_and_lock(session));

			if (np_session)
				np_session->upgrade(amount);
		}

		void close(Genode::Session_capability) { }
};

#endif /* _DECORATOR_NITPICKER_H_ */
