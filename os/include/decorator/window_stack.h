/*
 * \brief  Window-stack handling for decorator
 * \author Norman Feske
 * \date   2014-01-09
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__DECORATOR__WINDOW_STACK_H_
#define _INCLUDE__DECORATOR__WINDOW_STACK_H_

/* Genode includes */
#include <nitpicker_session/nitpicker_session.h>
#include <base/printf.h>

/* local includes */
#include <decorator/types.h>
#include <decorator/window.h>
#include <decorator/window_factory.h>
#include <decorator/xml_utils.h>

namespace Decorator { class Window_stack; }


class Decorator::Window_stack
{
	private:

		Window_list          _windows;
		Window_factory_base &_window_factory;

		inline void _draw_rec(Canvas_base &canvas, Window_base const *win,
		                      Rect rect) const;

		Window_base *_lookup_by_id(unsigned const id)
		{
			for (Window_base *win = _windows.first(); win; win = win->next())
				if (win->id() == id)
					return win;

			return 0;
		}

		static inline
		Xml_node _xml_node_by_window_id(Genode::Xml_node node, unsigned id)
		{
			for (node = node.sub_node("window"); ; node = node.next()) {

				if (node.has_type("window") && attribute(node, "id", 0UL) == id)
					return node;

				if (node.is_last()) break;
			}

			throw Xml_node::Nonexistent_sub_node();
		}

		void _destroy(Window_base &window)
		{
			_windows.remove(&window);
			_window_factory.destroy(&window);
		}

	public:

		Window_stack(Window_factory_base &window_factory)
		:
			_window_factory(window_factory)
		{ }

		void draw(Canvas_base &canvas, Rect rect) const
		{
			_draw_rec(canvas, _windows.first(), rect);
		}

		inline void update_model(Xml_node root_node);

		void flush()
		{
			while (Window_base *window = _windows.first())
				_destroy(*window);
		}
};


void Decorator::Window_stack::_draw_rec(Decorator::Canvas_base       &canvas,
                                        Decorator::Window_base const *win,
                                        Decorator::Rect               rect) const
{
	Rect clipped;

	/* find next window that intersects with the rectangle */
	for ( ; win && !(clipped = Rect::intersect(win->outer_geometry(), rect)).valid(); )
		win = win->next();;

	/* check if we hit the bottom of the window stack */
	if (!win) return;

	/* draw areas around the current window */
	if (Window_base const * const next = win->next()) {
		Rect top, left, right, bottom;
		rect.cut(clipped, &top, &left, &right, &bottom);

		if (top.valid())    _draw_rec(canvas, next, top);
		if (left.valid())   _draw_rec(canvas, next, left);
		if (right.valid())  _draw_rec(canvas, next, right);
		if (bottom.valid()) _draw_rec(canvas, next, bottom);
	}

	/* draw current window */
	win->draw(canvas, clipped);
}


void Decorator::Window_stack::update_model(Genode::Xml_node root_node)
{
	/*
	 * Step 1: Remove windows that are no longer present.
	 */
	for (Window_base *window = _windows.first(), *next = 0; window; window = next) {
		next = window->next();
		try {
			_xml_node_by_window_id(root_node, window->id()); }

		catch (Xml_node::Nonexistent_sub_node) {
			_destroy(*window); };
	}

	/*
	 * Step 2: Update window properties of already present windows.
	 */
	for (Window_base *window = _windows.first(); window; window = window->next()) {

		/*
		 * After step 1, a Xml_node::Nonexistent_sub_node exception can no
		 * longer occur. All windows remaining in the window stack are present
		 * in the XML model.
		 */
		try {
			window->update(_xml_node_by_window_id(root_node, window->id())); }
		catch (Xml_node::Nonexistent_sub_node) {
			PERR("could not look up window %ld in XML model", window->id()); }
	}

	/*
	 * Step 3: Add new appearing windows to the window stack.
	 */
	for_each_sub_node(root_node, "window", [&] (Xml_node window_node) {

		unsigned long const id = attribute(window_node, "id", 0UL);

		if (!_lookup_by_id(id)) {
			Window_base *new_window = _window_factory.create(window_node);
			if (new_window) {
				new_window->update(window_node);
				_windows.insert(new_window);
			}
		}
	});

	/*
	 * Step 4: Adjust window order.
	 */
	Window_base *previous_window = 0;
	Window_base *window          = _windows.first();

	for_each_sub_node(root_node, "window", [&] (Xml_node window_node) {

		if (!window) {
			PERR("unexpected end of window list during re-ordering");
			return;
		}

		unsigned long const id = attribute(window_node, "id", 0UL);

		if (window->id() != id) {
			window = _lookup_by_id(id);
			if (!window) {
				PERR("window lookup unexpectedly failed during re-ordering");
				return;
			}

			_windows.remove(window);
			_windows.insert(window, previous_window);
		}

		previous_window = window;
		window          = window->next();
	});
}

#endif /* _INCLUDE__DECORATOR__WINDOW_STACK_H_ */
