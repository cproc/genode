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
#include <base/tslab.h>
#include <os/server.h>
#include <os/surface.h>
#include <os/attached_ram_dataspace.h>
#include <os/session_policy.h>
#include <cap_session/connection.h>
#include <root/component.h>
#include <nitpicker_session/nitpicker_session.h>
#include <input_session/capability.h>
#include <input/component.h>

/* local includes */
#include <window_registry.h>
#include <nitpicker_slave.h>


namespace Desktop {

	using Genode::Rpc_object;
	using Genode::List;
	using Genode::Allocator;
	using Genode::Affinity;
	using Genode::static_cap_cast;
	using Genode::Signal_rpc_member;
	using Genode::Ram_session_capability;
	using Genode::Weak_ptr;
	using Genode::Locked_ptr;
	using Genode::Tslab;
	using Genode::Attached_ram_dataspace;
}

namespace Desktop { namespace Nitpicker {

	using namespace ::Nitpicker;

	class View_handle_ctx;
	class View;
	class Top_level_view;
	class Child_view;
	class Direct_view;
	class Session_component;
	class Root;

	typedef Genode::Surface_base::Rect  Rect;
	typedef Genode::Surface_base::Point Point;
	typedef Genode::Session_label       Session_label;
} }


struct Nitpicker::View { GENODE_RPC_INTERFACE(); };


class Desktop::Nitpicker::View : public Genode::Weak_object<View>,
                                 public Genode::Rpc_object< ::Nitpicker::View>
{
	protected:

		typedef Genode::String<100>             Title;
		typedef Nitpicker::Session::Command     Command;
		typedef Nitpicker::Session::View_handle View_handle;

		Session_label              _session_label;
		Nitpicker::Session_client &_real_nitpicker;
		View_handle                _real_handle;
		Title                      _title;
		Rect                       _geometry;
		Point                      _buffer_offset;
		Weak_ptr<View>             _neighbor_ptr;
		bool                       _neighbor_behind;

		View(Nitpicker::Session_client &real_nitpicker,
		     Session_label       const &session_label)
		:
			_session_label(session_label), _real_nitpicker(real_nitpicker)
		{ }

		virtual void _propagate_view_geometry() = 0;

		void _apply_view_config()
		{
			if (!_real_handle.valid())
				return;

			_propagate_view_geometry();
			_real_nitpicker.enqueue<Command::Offset>(_real_handle, _buffer_offset);
			_real_nitpicker.enqueue<Command::Title> (_real_handle, _title.string());

			View_handle real_neighbor_handle;

			Locked_ptr<View> neighbor(_neighbor_ptr);
			if (neighbor.is_valid())
				real_neighbor_handle = _real_nitpicker.view_handle(neighbor->real_view_cap());

			if (_neighbor_behind)
				_real_nitpicker.enqueue<Command::To_front>(_real_handle, real_neighbor_handle);
			else
				_real_nitpicker.enqueue<Command::To_back>(_real_handle, real_neighbor_handle);

			_real_nitpicker.execute();

			if (real_neighbor_handle.valid())
				_real_nitpicker.release_view_handle(real_neighbor_handle);
		}

	public:

		~View()
		{
			if (_real_handle.valid())
				_real_nitpicker.destroy_view(_real_handle);
		}

		Point virtual_position() const { return _geometry.p1(); }

		virtual void geometry(Rect geometry)
		{
			_geometry = geometry;

			/*
			 * Propagate new size to real nitpicker view but
			 */
			if (_real_handle.valid()) {
				_propagate_view_geometry();
				_real_nitpicker.execute();
			}
		}

		virtual void title(char const *title)
		{
			_title = Title(title);

			if (_real_handle.valid()) {
				_real_nitpicker.enqueue<Command::Title>(_real_handle, title);
				_real_nitpicker.execute();
			}
		}

		virtual Point input_anchor_position() const = 0;

		void stack(Weak_ptr<View> neighbor_ptr, bool behind)
		{
			_neighbor_ptr    = neighbor_ptr;
			_neighbor_behind = behind;

			_apply_view_config();
		}

		View_handle real_handle() const { return _real_handle; }

		View_capability real_view_cap()
		{
			return _real_nitpicker.view_capability(_real_handle);
		}

		void buffer_offset(Point buffer_offset)
		{
			_buffer_offset = buffer_offset;

			if (_real_handle.valid()) {
				_real_nitpicker.enqueue<Command::Offset>(_real_handle, _buffer_offset);
				_real_nitpicker.execute();
			}
		}
};


class Desktop::Nitpicker::Top_level_view : public View,
                                           public List<Top_level_view>::Element
{
	private:

		Window_registry::Id _win_id;

		Window_registry &_window_registry;

		/* position of window-anchor view, used for input-event transformation */
		Point _anchor_position;

		/*
		 * The window title is the concatenation of the session label with
		 * view title.
		 */
		struct Window_title : Title
		{
			Window_title(Session_label const session_label, Title const &title)
			{
				bool const has_title = Genode::strlen(title.string()) > 0;
				char buf[256];
				Genode::snprintf(buf, sizeof(buf), "%s%s%s",
				                 session_label.string(),
				                 has_title ? " " : "", title.string());

				*static_cast<Title *>(this) = Title(buf);
			}
		} _window_title;

		typedef Nitpicker::Session::Command Command;

	public:

		Top_level_view(Nitpicker::Session_client &real_nitpicker,
		               Session_label       const &session_label,
		               Window_registry           &window_registry)
		:
			View(real_nitpicker, session_label),
			_window_registry(window_registry),
			_window_title(session_label, "")
		{ }

		~Top_level_view()
		{
			if (_win_id.valid())
				_window_registry.destroy(_win_id);
		}

		void _propagate_view_geometry() override
		{
			/*
			 * Pin the position of the real view to (0, 0). This is done
			 * because the real view is actually a child view of the anchor,
			 * appearing on screen at the anchor position.
			 */
			Rect real_geometry = Rect(Point(0, 0), _geometry.area());

			_real_nitpicker.enqueue<Command::Geometry>(_real_handle, real_geometry);
		}

		void geometry(Rect geometry) override
		{
			/*
			 * Add window to the window-list model on the first call. We
			 * defer the creation of the window ID until the time when the
			 * initial geometry is known.
			 */
			if (!_win_id.valid()) {
				_win_id = _window_registry.create();
				_window_registry.title(_win_id, _window_title.string());
			}

			_window_registry.size(_win_id, geometry.area());

			View::geometry(geometry);
		}

		void title(char const *title) override
		{
			View::title(title);

			_window_title = Window_title(_session_label, title);

			if (_win_id.valid())
				_window_registry.title(_win_id, _window_title.string());
		}

		bool has_win_id(Window_registry::Id id) const { return id == _win_id; }

		Point input_anchor_position() const override
		{
			return _anchor_position;
		}

		void window_anchor_position(Point pos) { _anchor_position = pos; }

		void window_anchor_view(View_capability anchor_view)
		{
			/*
			 * If the real view already exists, the call of this function
			 * indicates a changed stacking order.
			 */
			if (_real_handle.valid()) {
				_real_nitpicker.enqueue<Command::To_front>(_real_handle);
				_real_nitpicker.execute();
				return;
			}

			/*
			 * Create and configure physical nitpicker view.
			 */
			View_handle anchor_handle = _real_nitpicker.view_handle(anchor_view);
			_real_handle = _real_nitpicker.create_view(anchor_handle);
			_real_nitpicker.release_view_handle(anchor_handle);
			_apply_view_config();
		}
};


class Desktop::Nitpicker::Child_view : public View,
                                       public List<Child_view>::Element
{
	private:

		Weak_ptr<View> mutable _parent;

	public:

		Child_view(Nitpicker::Session_client &real_nitpicker,
		           Session_label       const &session_label,
		           Weak_ptr<View>             parent)
		:
			View(real_nitpicker, session_label), _parent(parent)
		{
			try_to_init_real_view();
		}

		void _propagate_view_geometry() override
		{
			_real_nitpicker.enqueue<Command::Geometry>(_real_handle, _geometry);
		}

		Point input_anchor_position() const override
		{
			Locked_ptr<View> parent(_parent);
			if (parent.is_valid())
				return parent->input_anchor_position();

			return Point();
		}

		void try_to_init_real_view()
		{
			if (_real_handle.valid())
				return;

			Locked_ptr<View> parent(_parent);
			if (!parent.is_valid())
				return;

			View_handle parent_handle = _real_nitpicker.view_handle(parent->real_view_cap());
			if (!parent_handle.valid())
				return;

			_real_handle = _real_nitpicker.create_view(parent_handle);

			_real_nitpicker.release_view_handle(parent_handle);

			_apply_view_config();
		}
};


class Desktop::Nitpicker::Direct_view : public View
{
	public:

		Direct_view(Nitpicker::Session_client &real_nitpicker,
		            Session_label       const &session_label,
		            bool                const  direct)
		:
			View(real_nitpicker, session_label)
		{
			if (!direct)
				return;

			typedef Nitpicker::Session::Command Command;
			_real_handle = _real_nitpicker.create_view();
			_real_nitpicker.enqueue<Command::Geometry>(_real_handle,
			                                    Rect(Point(0, 0), Area(0, 0)));
			_real_nitpicker.enqueue<Command::To_back>(_real_handle);
			_real_nitpicker.execute();
		}

		void _propagate_view_geometry() override { }

		Point input_anchor_position() const override { return Point(); }
};


class Desktop::Nitpicker::Session_component : public Genode::Rpc_object<Session>,
                                              public List<Session_component>::Element
{
	private:

		typedef Nitpicker::Session::View_handle View_handle;

		Session_label                _session_label;
		Nitpicker_slave             &_nitpicker;
		Ram_session_client           _ram;
		Session_client               _session;
		Direct_view                  _direct_view;
		Window_registry             &_window_registry;
		Entrypoint                  &_ep;
		Tslab<Top_level_view, 4000>  _top_level_view_alloc;
		Tslab<Child_view, 4000>      _child_view_alloc;
		List<Top_level_view>         _top_level_views;
		List<Child_view>             _child_views;
		Input::Session_component     _input_session;
		Input::Session_capability    _input_session_cap;
		size_t                       _ram_quota = 0;

		/*
		 * Command buffer
		 */
		typedef Nitpicker::Session::Command_buffer Command_buffer;

		Attached_ram_dataspace _command_ds { &_ram, sizeof(Command_buffer) };

		Command_buffer &_command_buffer = *_command_ds.local_addr<Command_buffer>();

		/*
		 * View handle registry
		 */
		typedef Genode::Handle_registry<View_handle, View>
			View_handle_registry;

		View_handle_registry _view_handle_registry;

		/*
		 * Input
		 */
		Input::Session_client _nitpicker_input    { _session.input_session() };
		Attached_dataspace    _nitpicker_input_ds { _nitpicker_input.dataspace() };

		Signal_rpc_member<Session_component> _input_dispatcher {
			_ep, *this, &Session_component::_input_handler };

		Point _input_origin() const
		{
			if (Top_level_view const *v = _top_level_views.first())
				return v->virtual_position() - v->input_anchor_position();

			if (Child_view const *v = _child_views.first())
				return Point(0, 0) - v->input_anchor_position();

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

			try {
				while (_nitpicker_input.is_pending()) {

					size_t const num_events = _nitpicker_input.flush();

					/* we trust nitpicker to return a valid number of events */

					for (size_t i = 0; i < num_events; i++)
						_input_session.submit(_translate_event(events[i], input_origin));
				}
			}
			catch (Input::Event_queue::Overflow) {
				PWRN("client \"%s\" does not respond to user input",
				     _session_label.string()); }
		}

		View &_create_view_object(View_handle parent_handle)
		{
			/*
			 * If the session operates in direct mode, we subordinate all
			 * top-level views of the session to the 'direct_parent' pseudo
			 * view, which is located at the screen origin.
			 */
			if (!parent_handle.valid() && _direct_view.real_handle().valid()) {

				Child_view *view = new (_child_view_alloc)
					Child_view(_session, _session_label, _direct_view.weak_ptr());

				_child_views.insert(view);
				return *view;
			}

			/*
			 * Create child view
			 */
			if (parent_handle.valid()) {

				Weak_ptr<View> parent_ptr = _view_handle_registry.lookup(parent_handle);

				Child_view *view = new (_child_view_alloc)
					Child_view(_session, _session_label, parent_ptr);

				_child_views.insert(view);
				return *view;
			}

			/*
			 * Create top-level view
			 */
			else {
				Top_level_view *view = new (_top_level_view_alloc)
					Top_level_view(_session, _session_label, _window_registry);

				_top_level_views.insert(view);
				return *view;
			}
		}

		void _destroy_top_level_view(Top_level_view &view)
		{
			_top_level_views.remove(&view);
			_ep.dissolve(view);
			Genode::destroy(&_top_level_view_alloc, &view);
		}

		void _destroy_child_view(Child_view &view)
		{
			_child_views.remove(&view);
			_ep.dissolve(view);
			Genode::destroy(&_child_view_alloc, &view);
		}

		void _destroy_view_object(View &view)
		{
			if (Top_level_view *v = dynamic_cast<Top_level_view *>(&view))
				_destroy_top_level_view(*v);

			if (Child_view *v = dynamic_cast<Child_view *>(&view))
				_destroy_child_view(*v);
		}

		void _execute_command(Command const &command)
		{
			switch (command.opcode) {

			case Command::OP_GEOMETRY:
				{
					Locked_ptr<View> view(_view_handle_registry.lookup(command.geometry.view));
					if (view.is_valid())
						view->geometry(command.geometry.rect);
					return;
				}

			case Command::OP_OFFSET:
				{
					Locked_ptr<View> view(_view_handle_registry.lookup(command.geometry.view));
					if (view.is_valid())
						view->buffer_offset(command.offset.offset);

					return;
				}

			case Command::OP_TO_FRONT:
				{
					Locked_ptr<View> view(_view_handle_registry.lookup(command.to_front.view));
					if (!view.is_valid())
						return;

					/* bring to front if no neighbor is specified */
					if (!command.to_front.neighbor.valid()) {
						view->stack(Weak_ptr<View>(), true);
						return;
					}

					/* stack view relative to neighbor */
					view->stack(_view_handle_registry.lookup(command.to_front.neighbor),
					            true);
					return;
				}

			case Command::OP_TO_BACK:
				{
					PDBG("OP_TO_BACK not implemented");
					return;
				}

			case Command::OP_BACKGROUND:
				{
					PDBG("OP_BACKGROUND not implemented");
					return;
				}

			case Command::OP_TITLE:
				{
					Locked_ptr<View> view(_view_handle_registry.lookup(command.title.view));
					if (view.is_valid())
						view->title(command.title.title.string());

					return;
				}

			case Command::OP_NOP:
				return;
			}
		}

	public:

		/**
		 * Constructor
		 *
		 * \param nitpicker  real nitpicker service
		 * \param ep         entrypoint used for managing the views
		 */
		Session_component(Nitpicker_slave       &nitpicker,
		                  Ram_session_capability ram,
		                  Window_registry       &window_registry,
		                  Entrypoint            &ep,
		                  Allocator             &session_alloc,
		                  Session_label   const &session_label,
		                  bool            const  direct)
		:
			_session_label(session_label),
			_nitpicker(nitpicker),
			_ram(ram),
			_session(_nitpicker.session(_session_label.string())),
			_direct_view(_session, session_label, direct),
			_window_registry(window_registry),
			_ep(ep),
			_top_level_view_alloc(&session_alloc),
			_child_view_alloc(&session_alloc),
			_input_session_cap(_ep.manage(_input_session)),
			_view_handle_registry(session_alloc)
		{
			_nitpicker_input.sigh(_input_dispatcher);
			_input_session.event_queue().enabled(true);
		}

		~Session_component()
		{
			while (Top_level_view *view = _top_level_views.first())
				_destroy_view_object(*view);

			while (Child_view *view = _child_views.first())
				_destroy_view_object(*view);

			_nitpicker.close(_session);

			/* withdraw session quota from nitpicker slave */
			if (_nitpicker.ram().transfer_quota(_ram, _ram_quota) != 0)
				PWRN("misbehaving nitpicker slave, could not regain %zd bytes",
				     _ram_quota);

			_ep.dissolve(_input_session);
		}

		void upgrade_ram_quota(size_t amount)
		{
			_ram_quota += amount;
			_nitpicker.upgrade(_session, _ram, amount);
		}

		void window_anchor_view(Window_registry::Id id, View_capability anchor)
		{
			for (Top_level_view *v = _top_level_views.first(); v; v = v->next()) {
				if (!v->has_win_id(id))
					continue;

				v->window_anchor_view(anchor);
			}
		}

		void try_to_init_real_child_views()
		{
			for (Child_view *v = _child_views.first(); v; v = v->next())
				v->try_to_init_real_view();
		}

		void window_anchor_position(Window_registry::Id id, Point position)
		{
			for (Top_level_view *v = _top_level_views.first(); v; v = v->next()) {
				if (!v->has_win_id(id))
					continue;

				v->window_anchor_position(position);
				break;
			}
		}

		bool has_win_id(unsigned id) const
		{
			for (Top_level_view const *v = _top_level_views.first(); v; v = v->next())
				if (v->has_win_id(id))
					return true;

			return false;
		}

		/**
		 * Return session capability to real nitpicker session
		 */
		Capability<Session> session() { return _session; }


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

		View_handle create_view(View_handle parent) override
		{
			try {
				View &view = _create_view_object(parent);
				_ep.manage(view);
				return _view_handle_registry.alloc(view);
			}
			catch (View_handle_registry::Lookup_failed) {
				return View_handle(); }
		}

		void destroy_view(View_handle handle) override
		{
			try {
				Locked_ptr<View> view(_view_handle_registry.lookup(handle));
				if (view.is_valid())
					_destroy_view_object(*view);
			}
			catch (View_handle_registry::Lookup_failed) { }

			_view_handle_registry.free(handle);
		}

		View_handle view_handle(View_capability view_cap) override
		{
			View *view = dynamic_cast<View *>(_ep.rpc_ep().lookup_and_lock(view_cap));
			if (!view) return View_handle();

			Object_pool<Rpc_object_base>::Guard guard(view);

			return _view_handle_registry.alloc(*view);
		}

		View_capability view_capability(View_handle handle) override
		{
			Locked_ptr<View> view(_view_handle_registry.lookup(handle));

			return view.is_valid() ? view->cap() : View_capability();
		}

		void release_view_handle(View_handle handle) override
		{
			try {
				_view_handle_registry.free(handle); }

			catch (View_handle_registry::Lookup_failed) {
				PWRN("view lookup failed while releasing view handle");
				return;
			}
		}

		Genode::Dataspace_capability command_dataspace() override
		{
			return _command_ds.cap();
		}

		void execute() override
		{
			for (unsigned i = 0; i < _command_buffer.num(); i++) {
				try {
					_execute_command(_command_buffer.get(i)); }
				catch (View_handle_registry::Lookup_failed) {
					PWRN("view lookup failed during command execution"); }
			}
		}

		Framebuffer::Mode mode() override
		{
			return _session.mode();
		}

		void buffer(Framebuffer::Mode mode, bool use_alpha) override
		{
			_session.buffer(mode, use_alpha);
		}

		void focus(Genode::Capability<Nitpicker::Session>) { }
};


class Desktop::Nitpicker::Root : public Genode::Root_component<Session_component>,
                                 public Decorator_anchor_callback
{
	private:

		Entrypoint &_ep;

		Nitpicker_slave &_nitpicker_slave;

		Ram_session_capability _ram;

		enum { STACK_SIZE = 1024*sizeof(long) };

		Window_registry &_window_registry;

		List<Session_component> _sessions;

	protected:

		Session_component *_create_session(const char *args) override
		{
			bool direct = false;

			Session_label session_label(args);

			/*
			 * Determine session policy
			 */
			try {
				Genode::Xml_node policy = Genode::Session_policy(session_label);
				direct = policy.attribute("direct").has_value("yes");
			}
			catch (...) { }

			Session_component *session = new (md_alloc())
				Session_component(_nitpicker_slave, _ram, _window_registry,
				                  _ep, *md_alloc(), session_label, direct);

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
		     Ram_session_capability ram)
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
				s->try_to_init_real_child_views();
		}

		void window_anchor_position(Window_registry::Id id, Point position) override
		{
			for (Session_component *s = _sessions.first(); s; s = s->next())
				s->window_anchor_position(id, position);
		}

		Capability<Session> lookup_nitpicker_session(unsigned win_id)
		{
			for (Session_component *s = _sessions.first(); s; s = s->next())
				if (s->has_win_id(win_id))
					return s->session();

			return Capability<Session>();
		}
};

#endif /* _NITPICKER_H_ */
