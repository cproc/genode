/*
 * \brief  Floating window layouter
 * \author Norman Feske
 * \date   2013-02-14
 */

/*
 * Copyright (C) 2013-2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/printf.h>
#include <base/signal.h>
#include <os/attached_rom_dataspace.h>
#include <os/reporter.h>
#include <input_session/connection.h>
#include <input/event.h>
#include <input/keycodes.h>
#include <rom_session/connection.h>
#include <decorator/xml_utils.h>


namespace Floating_window_layouter {

	using namespace Genode;
	struct Main;
	class Window;

	typedef Decorator::Point Point;
	typedef Decorator::Area  Area;
	typedef Decorator::Rect  Rect;

	using Decorator::attribute;
	using Decorator::string_attribute;
	using Decorator::area_attribute;


	static Xml_node xml_lookup_window_by_id(Xml_node node, unsigned const id)
	{
		char const *tag     = "window";
		char const *id_attr = "id";

		for (node = node.sub_node(tag); ; node = node.next(tag))
			if (attribute(node, id_attr, 0UL) == id)
				return node;

		throw Xml_node::Nonexistent_sub_node();
	}


	/**
	 * Return true if compound XML node contains a sub node with ID
	 */
	static bool xml_contains_window_node_with_id(Xml_node node,
	                                             unsigned const id)
	{
		try { xml_lookup_window_by_id(node, id); return true; }
		catch (Xml_node::Nonexistent_sub_node) { return false; }
	}
}


class Floating_window_layouter::Window : public List<Window>::Element
{
	public:

		typedef String<256> Title;

	private:

		unsigned const _id = 0;
		Title          _title;
		Rect           _geometry;

	public:

		Window(unsigned id) : _id(id) { }

		bool has_id(unsigned id) const { return id == _id; }

		unsigned id() const { return _id; }

		void title(Title const &title) { _title = title; }

		void geometry(Rect geometry) { _geometry = geometry; }

		Point position() const { return _geometry.p1(); }

		void position(Point pos) { _geometry = Rect(pos, _geometry.area()); }

		void serialize(Xml_generator &xml, bool focused)
		{
			xml.node("window", [&]() {
				xml.attribute("id",      _id);
				xml.attribute("title",   _title.string());
				xml.attribute("xpos",    _geometry.x1());
				xml.attribute("ypos",    _geometry.y1());
				xml.attribute("width",   _geometry.w());
				xml.attribute("height",  _geometry.h());

				if (focused)
					xml.attribute("focused", "yes");
			});
		}
};


struct Floating_window_layouter::Main
{
	Signal_receiver &sig_rec;

	List<Window> windows;

	unsigned focused_window_id = 0;
	bool     drag_state = false;

	Window *lookup_window_by_id(unsigned id)
	{
		for (Window *w = windows.first(); w; w = w->next())
			if (w->has_id(id))
				return w;

		return nullptr;
	}


	/**
	 * Install handler for responding to window-list changes
	 */
	void handle_window_list_update(unsigned);

	Signal_dispatcher<Main> window_list_dispatcher = {
		sig_rec, *this, &Main::handle_window_list_update };

	Attached_rom_dataspace window_list { "window_list" };


	/**
	 * Install handler for responding to hover changes
	 */
	void handle_hover_update(unsigned);

	Signal_dispatcher<Main> hover_dispatcher = {
		sig_rec, *this, &Main::handle_hover_update };

	Attached_rom_dataspace hover { "hover" };


	/**
	 * Install handler for responding to user input
	 */
	void handle_input(unsigned);

	Signal_dispatcher<Main> input_dispatcher = {
		sig_rec, *this, &Main::handle_input };

	Input::Connection input;

	Attached_dataspace input_ds { input.dataspace() };

	Reporter window_layout_reporter = { "window_layout" };
	Reporter focus_reporter         = { "focus" };

	void import_window_list(Xml_node);
	void generate_window_list_model();
	void generate_focus_model();

	/**
	 * Constructor
	 */
	Main(Signal_receiver &sig_rec) : sig_rec(sig_rec)
	{
		window_list.sigh(window_list_dispatcher);

		hover.sigh(hover_dispatcher);

		input.sigh(input_dispatcher);

		window_layout_reporter.enabled(true);
		focus_reporter.enabled(true);
	}
};


void Floating_window_layouter::Main::import_window_list(Xml_node window_list_xml)
{
	char const *tag = "window";

	/*
	 * Remove windows from layout that are no longer in the window list
	 */
	for (Window *win = windows.first(), *next = 0; win; win = next) {
		next = win->next();
		if (!xml_contains_window_node_with_id(window_list_xml, win->id())) {
			windows.remove(win);
			destroy(env()->heap(), win);
		}
	}

	/*
	 * Update window attributes, add new windows to the layout
	 */
	try {
		for (Xml_node node = window_list_xml.sub_node(tag); ; node = node.next(tag)) {

			unsigned long id = 0;
			node.attribute("id").value(&id);

			Window *win = lookup_window_by_id(id);
			if (!win) {
				win = new (env()->heap()) Window(id);
				windows.insert(win);

				/*
				 * Define initial window position
				 */
				win->position(Point(150*id % 800, 30 + (100*id % 500)));
			}

			win->geometry(Rect(win->position(), area_attribute(node)));
			win->title(string_attribute(node, "title", Window::Title("untitled")));
		}
	} catch (...) { }
}


void Floating_window_layouter::Main::generate_window_list_model()
{
	Reporter::Xml_generator xml(window_layout_reporter, [&] ()
	{
		for (Window *w = windows.first(); w; w = w->next()) {
			w->serialize(xml, w->has_id(focused_window_id));
		}
	});
}


void Floating_window_layouter::Main::generate_focus_model()
{
	Reporter::Xml_generator xml(focus_reporter, [&] ()
	{
		xml.node("window", [&] () {
			xml.attribute("id", focused_window_id);
		});
	});
}


void Floating_window_layouter::Main::handle_window_list_update(unsigned)
{
	window_list.update();

	try {
		import_window_list(Xml_node(window_list.local_addr<char>())); }
	catch (...) {
		PERR("Error while importing window list"); }

	generate_window_list_model();
}


void Floating_window_layouter::Main::handle_hover_update(unsigned)
{
	hover.update();

	try {
		Xml_node hover_xml(hover.local_addr<char>());

		unsigned const id = attribute(hover_xml.sub_node("window"), "id", 0UL);

		if (!drag_state && id != focused_window_id) {
			focused_window_id = id;
			generate_window_list_model();
			generate_focus_model();
		}
	} catch (...) { }
}


void Floating_window_layouter::Main::handle_input(unsigned)
{
	static Point    orig_window_pos;
	static Point    pointer_clicked;
	static Point    pointer_last;
	static Point    pointer_curr;
	static unsigned dragged_window_id = 0;

	bool need_regenerate_window_list_model = false;

	Window *focused_window = lookup_window_by_id(focused_window_id);

	while (input.is_pending()) {

		size_t const num_events = input.flush();

		Input::Event const * const ev = input_ds.local_addr<Input::Event>();

		for (size_t i = 0; i < num_events; i++) {

			Input::Event e = ev[i];

			if (e.type() == Input::Event::MOTION
			 || e.type() == Input::Event::PRESS)
				pointer_curr = Point(e.ax(), e.ay());

			if (e.type() == Input::Event::PRESS) {
				if (e.keycode() == Input::BTN_LEFT) {

					drag_state        = true;
					dragged_window_id = focused_window_id;
					pointer_clicked   = pointer_curr;
					pointer_last      = pointer_clicked;
					orig_window_pos   = focused_window->position();

					/* bring focused window to front */
					if (focused_window && focused_window != windows.first()) {
						windows.remove(focused_window);
						windows.insert(focused_window);

						need_regenerate_window_list_model = true;
					}
				}
			}

			if (e.type() == Input::Event::RELEASE) {
				if (e.keycode() == Input::BTN_LEFT) {
					drag_state = false;
					generate_focus_model();
				}
			}
		}
	}

	if (drag_state && (pointer_curr != pointer_last)) {

		pointer_last = pointer_curr;

		Window *dragged_window = lookup_window_by_id(dragged_window_id);
		if (dragged_window) {

			Point d = pointer_curr - pointer_clicked;
			dragged_window->position(orig_window_pos + d);

			need_regenerate_window_list_model = true;
		}
	}

	if (need_regenerate_window_list_model)
		generate_window_list_model();
}


int main(int argc, char **argv)
{
	static Genode::Signal_receiver sig_rec;

	static Floating_window_layouter::Main application(sig_rec);

	/* import initial state */
	application.handle_window_list_update(0);

	/* process incoming signals */
	for (;;) {
		using namespace Genode;

		Signal sig = sig_rec.wait_for_signal();
		Signal_dispatcher_base *dispatcher =
			dynamic_cast<Signal_dispatcher_base *>(sig.context());

		if (dispatcher)
			dispatcher->dispatch(sig.num());
	}
}
