/*
 * \brief  Window representation for decorator
 * \author Norman Feske
 * \date   2014-01-09
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__DECORATOR__WINDOW_H_
#define _INCLUDE__DECORATOR__WINDOW_H_

/* Genode includes */
#include <util/list.h>
#include <util/string.h>
#include <nitpicker_view/client.h>

/* decorator includes */
#include <decorator/types.h>
#include <decorator/xml_utils.h>


namespace Decorator {
	class Canvas_base;
	class Window_base;
	typedef Genode::List<Window_base> Window_list;
}


class Decorator::Window_base : public Window_list::Element
{
	public:

		struct Border
		{
			unsigned top, left, right, bottom;

			Border(unsigned top, unsigned left, unsigned right, unsigned bottom)
			: top(top), left(left), right(right), bottom(bottom) { }
		};

	private:

		Rect           _geometry;     /* geometry of content */
		unsigned const _id;

		Border const _border;

		struct Nitpicker_view : Nitpicker::View_client
		{
			Nitpicker::Session &_nitpicker;

			/*
			 * XXX supply window ID as label
			 */
			Nitpicker_view(Nitpicker::Session &nitpicker)
			:
				Nitpicker::View_client(nitpicker.create_view()),
				_nitpicker(nitpicker)
			{
				stack(Nitpicker::View_capability(), true, false);
			}

			~Nitpicker_view()
			{
				_nitpicker.destroy_view(*this);
			}

			void place(Rect rect)
			{
				viewport(rect.x1(), rect.y1(), rect.w(), rect.h(),
				         -rect.x1(), -rect.y1(), false);
			}
		};

		Nitpicker_view _top_view, _left_view, _right_view, _bottom_view;

	public:

		Window_base(unsigned id, Nitpicker::Session &nitpicker,
		            Border border)
		:
			_id(id), _border(border),
			_top_view(nitpicker),   _left_view(nitpicker),
			_right_view(nitpicker), _bottom_view(nitpicker)
		{ }

		Rect outer_geometry() const
		{
			return Rect(_geometry.p1() - Point(_border.left,  _border.top),
			            _geometry.p2() + Point(_border.right, _border.bottom));
		}

		void border_rects(Rect *top, Rect *left, Rect *right, Rect *bottom) const
		{
			outer_geometry().cut(_geometry, top, left, right, bottom);
		}

		unsigned long id()       const { return _id; }
		Rect          geometry() const { return _geometry; }

		/**
		 * Draw window elements
		 *
		 * \param canvas  graphics back end
		 * \param clip    clipping area to apply
		 */
		virtual void draw(Canvas_base &canvas, Rect clip) const = 0;

		virtual void update(Xml_node window_node)
		{
			_geometry = rect_attribute(window_node);

			/* update view positions */
			Rect top, left, right, bottom;
			border_rects(&top, &left, &right, &bottom);

			_top_view   .place(top);
			_left_view  .place(left);
			_right_view .place(right);
			_bottom_view.place(bottom);
		}
};

#endif /* _INCLUDE__DECORATOR__WINDOW_H_ */
