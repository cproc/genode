/*
 * \brief  Example window decorator that mimics the Motif look
 * \author Norman Feske
 * \date   2014-01-10
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <decorator/window.h>

namespace Decorator { class Window; }


class Decorator::Window : public Window_base
{
	public:

		typedef Genode::String<200> Title;

	private:

		Title _title;

		bool _focused = false;

		static unsigned const _corner_size = 16;
		static unsigned const _border_size = 4;
		static unsigned const _title_height = 16;

		static Border _border() {
			return Border(_border_size + _title_height,
			              _border_size, _border_size, _border_size); }

		Color _color;
		Color _bright = { 255, 255, 255, 64 };
		Color _dark   = { 0, 0, 0, 127 };

		void _draw_hline(Canvas_base &canvas, Point pos, unsigned w,
		                 bool at_left, bool at_right,
		                 unsigned border, Color color) const
		{
			int const x1 = at_left  ? (pos.x())         : (pos.x() + w - border);
			int const x2 = at_right ? (pos.x() + w - 1) : (pos.x() + border - 1);

			canvas.draw_box(Rect(Point(x1, pos.y()),
			                     Point(x2, pos.y())), color);
		}

		void _draw_vline(Canvas_base &canvas, Point pos, unsigned h,
		                 bool at_top, bool at_bottom,
		                 unsigned border, Color color) const
		{
			int const y1 = at_top    ? (pos.y())         : (pos.y() + h - border);
			int const y2 = at_bottom ? (pos.y() + h - 1) : (pos.y() + border - 1);

			canvas.draw_box(Rect(Point(pos.x(), y1),
			                     Point(pos.x(), y2)), color);
		}

		void _draw_raised_frame(Canvas_base &canvas, Rect rect) const
		{
			_draw_hline(canvas, rect.p1(), rect.w(), true, true, 0, _bright);
			_draw_vline(canvas, rect.p1(), rect.h(), true, true, 0, _bright);
			_draw_hline(canvas, Point(rect.p1().x(), rect.p2().y()), rect.w(),
			            true, true, 0, _dark);
			_draw_vline(canvas, Point(rect.p2().x(), rect.p1().y()), rect.h(),
			            true, true, 0, _dark);
		}

		void _draw_raised_box(Canvas_base &canvas, Rect rect) const
		{
			canvas.draw_box(rect, _color);
			_draw_raised_frame(canvas, rect);
		}

		void _draw_title_box(Canvas_base &canvas, Rect rect) const
		{
			canvas.draw_box(rect, _color);
			for (unsigned i = 0; i < rect.h(); i++)
				canvas.draw_box(Rect(rect.p1() + Point(0, i),
				                     Area(rect.w(), 1)),
				                     Color(255,255,255, 30 + (rect.h() - i)*4));

			_draw_raised_frame(canvas, rect);
		}

		void _draw_corner(Canvas_base &canvas, Rect const rect,
		                  unsigned const border,
		                  bool const left, bool const top) const
		{
			bool const bottom = !top;
			bool const right  = !left;

			int const x1 = rect.p1().x();
			int const y1 = rect.p1().y();
			int const x2 = rect.p2().x();
			int const y2 = rect.p2().y();
			int const w  = rect.w();
			int const h  = rect.h();

			canvas.draw_box(Rect(Point(x1, top ? y1 : y2 - border + 1),
			                     Area(w, border)), _color);

			canvas.draw_box(Rect(Point(left ? x1 : x2 - border + 1,
			                           top  ? y1 + border : y1),
			                     Area(border, h - border)), _color);

			/* top bright line */
			_draw_hline(canvas, rect.p1(), w,
			            top || left, top || right, border, _bright);

			/* inner horizontal line */
			int y = top ? y1 + border - 1 : y2 - border + 1;
			_draw_hline(canvas, Point(x1, y), w, right, left, w - border,
			            top ? _dark : _bright);

			/* bottom line */
			_draw_hline(canvas, Point(x1, y2), w,
			            bottom || left, bottom || right, border, _dark);

			/* left bright line */
			_draw_vline(canvas, rect.p1(), h,
			            left || top, left || bottom, border, _bright);

			/* inner vertical line */
			int x = left ? x1 + border - 1 : x2 - border + 1;
			_draw_vline(canvas, Point(x, y1), h, bottom, top, h - border + 1,
			            left ? _dark : _bright);

			/* right line */
			_draw_vline(canvas, Point(x2, y1), h,
			            right || top, right || bottom, border, _dark);
		}

	public:

		Window(unsigned id, Nitpicker::Session &nitpicker)
		:
			Window_base(id, nitpicker, _border())
		{ }

		void draw(Canvas_base &canvas, Rect clip) const override;

		bool update(Xml_node window_node) override;

		Hover hover(Point) const override;
};


void Decorator::Window::draw(Decorator::Canvas_base &canvas,
                             Decorator::Rect clip) const
{
	Clip_guard clip_guard(canvas, clip);

	Rect rect = outer_geometry();
	Area corner(_corner_size, _corner_size);

	Point p1 = rect.p1();
	Point p2 = rect.p2();

	canvas.draw_box(geometry(), Color(10, 20, 40));

	_draw_corner(canvas, Rect(p1, corner), _border_size, true, true);
	_draw_corner(canvas, Rect(Point(p1.x(), p2.y() - _corner_size + 1), corner), _border_size, true, false);
	_draw_corner(canvas, Rect(Point(p2.x() - _corner_size + 1, p1.y()), corner), _border_size, false, true);
	_draw_corner(canvas, Rect(Point(p2.x() - _corner_size + 1, p2.y() - _corner_size + 1), corner), _border_size, false, false);

	_draw_raised_box(canvas, Rect(Point(p1.x() + _corner_size, p1.y()),
	                              Area(rect.w() - 2*_corner_size, _border_size)));

	_draw_raised_box(canvas, Rect(Point(p1.x() + _corner_size, p2.y() - _border_size + 1),
	                              Area(rect.w() - 2*_corner_size, _border_size)));

	_draw_raised_box(canvas, Rect(Point(p1.x(), p1.y() + _corner_size),
	                              Area(_border_size, rect.h() - 2*_corner_size)));

	_draw_raised_box(canvas, Rect(Point(p2.x() - _border_size + 1, p1.y() + _corner_size),
	                              Area(_border_size, rect.h() - 2*_corner_size)));

	Rect title_rect(Point(p1.x() + _border_size, p1.y() + _border_size),
	                Area(rect.w() - 2*_border_size, _title_height));

	_draw_title_box(canvas, title_rect);

	char const * const text = _title.string();;

	Area const label_area(default_font().str_w(text),
	                      default_font().str_h(text));

	Point text_pos = title_rect.center(label_area) - Point(0, 1);

	{
		Clip_guard clip_guard(canvas, title_rect);

		canvas.draw_text(text_pos + Point(1, 1), default_font(),
		                 Color(0, 0, 0, 128), text);

		canvas.draw_text(text_pos, default_font(),
		                 Color(255, 255, 255, _focused ? 235 : 96 + 16), text);
	}
}


bool Decorator::Window::update(Genode::Xml_node window_node)
{
	bool updated = Window_base::update(window_node);

	bool const focused = window_node.has_attribute("focused")
	                  && window_node.attribute("focused").has_value("yes");

	updated |= (focused != _focused);
	_focused = focused;

	_color = Color(45, 49, 65);
	if (_focused)
		_color = Color(_color.r + 70, _color.g + 70, _color.b + 70);

	Title title = Decorator::string_attribute(window_node, "title", Title("<untitled>"));
	updated |= !(title == _title);
	_title = title;

	return updated;
}


Decorator::Window_base::Hover Decorator::Window::hover(Point abs_pos) const
{
	Hover hover;

	if (!outer_geometry().contains(abs_pos))
		return hover;

	hover.window_id = id();

	unsigned const x = abs_pos.x() - outer_geometry().x1(),
	               y = abs_pos.y() - outer_geometry().y1();

	Area const area = outer_geometry().area();

	bool const at_border = x <  _border_size
	                    || x >= area.w() - _border_size
	                    || y <  _border_size
	                    || y >= area.h() - _border_size;

	if (at_border) {

		hover.left_sizer  = (x < _corner_size);
		hover.upper_sizer = (y < _corner_size);
		hover.right_sizer = (x >= area.w() - _corner_size);
		hover.lower_sizer = (y >= area.h() - _corner_size);

	} else {

		hover.title = (y < _border_size + _title_height);
	}

	return hover;
}


#endif /* _WINDOW_H_ */
