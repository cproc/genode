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
#include <util/xml_generator.h>
#include <nitpicker_session/client.h>
#include <base/snprintf.h>

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

		struct Hover
		{
			bool left_sizer  = false,
				 right_sizer = false,
				 upper_sizer = false,
				 lower_sizer = false,
				 title       = false;

			unsigned window_id = 0;

			bool operator != (Hover const &other) const
			{
				return other.left_sizer  != left_sizer
				    || other.right_sizer != right_sizer
				    || other.upper_sizer != upper_sizer
				    || other.lower_sizer != lower_sizer
				    || other.title       != title
				    || other.window_id   != window_id;
			}
		};

	private:

		Nitpicker::Session_client &_nitpicker;

		/*
		 * Geometry of content
		 */
		Rect _geometry;

		/*
		 * Unique window ID
		 */
		unsigned const _id;

		/*
		 * Flag indicating that the current window position has been propagated
		 * to the window's corresponding nitpicker views.
		 */
		bool _nitpicker_views_up_to_date = false;

		/*
		 * Flag indicating that the stacking position of the window within the
		 * window stack has changed. The new stacking position must be
		 * propagated to nitpicker.
		 */
		bool _nitpicker_stacking_up_to_date = false;

		Nitpicker::Session::View_handle _neighbor;

		Border const _border;

		struct Nitpicker_view
		{
			Nitpicker::Session_client      &_nitpicker;
			Nitpicker::Session::View_handle _handle { _nitpicker.create_view() };

			typedef Nitpicker::Session::Command Command;

			Nitpicker_view(Nitpicker::Session_client &nitpicker, unsigned id = 0)
			:
				_nitpicker(nitpicker)
			{
				/*
				 * We supply the window ID as label for the anchor view.
				 */
				if (id) {
					char buf[128];
					Genode::snprintf(buf, sizeof(buf), "%d", id);

					_nitpicker.enqueue<Command::Title>(_handle, buf);
				}
			}

			~Nitpicker_view()
			{
				_nitpicker.destroy_view(_handle);
			}

			Nitpicker::Session::View_handle handle() const { return _handle; }

			void stack(Nitpicker::Session::View_handle neighbor)
			{
				_nitpicker.enqueue<Command::To_front>(_handle, neighbor);
			}

			void place(Rect rect)
			{
				_nitpicker.enqueue<Command::Geometry>(_handle, rect);
				Point offset = Point(0, 0) - rect.p1();
				_nitpicker.enqueue<Command::Offset>(_handle, offset);
			}
		};

		Nitpicker_view _top_view, _left_view, _right_view, _bottom_view;
		Nitpicker_view _content_view;

	public:

		Window_base(unsigned id, Nitpicker::Session_client &nitpicker,
		            Border border)
		:
			_nitpicker(nitpicker), _id(id), _border(border),
			_top_view(nitpicker),   _left_view(nitpicker),
			_right_view(nitpicker), _bottom_view(nitpicker),
			_content_view(nitpicker, _id)
		{ }

		void stack(Nitpicker::Session::View_handle neighbor)
		{
			_neighbor = neighbor;
			_nitpicker_stacking_up_to_date = false;
		}

		Nitpicker::Session::View_handle anchor_view()
		{
			return _content_view.handle();
		}

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

		/**
		 * Update internal window representation from XML model
		 *
		 * \return true if window changed
		 */
		virtual bool update(Xml_node window_node)
		{
			Rect new_geometry = rect_attribute(window_node);

			if (new_geometry.p1() == _geometry.p1()
			 && new_geometry.p2() == _geometry.p2())
				return false;

			_geometry = new_geometry;

			/*
			 * Trigger an update of the nitpicker views.
			 *
			 * We do not immediately update the views as part of the update
			 * function because at the time when updating the model, the
			 * decorations haven't been redrawn already. If we updated the
			 * nitpicker views at this point, we would reveal not-yet-drawn
			 * pixels.
			 */
			_nitpicker_views_up_to_date = false;

			return true;
		}

		virtual void update_nitpicker_views()
		{
			bool const need_nitpicker_execute = !_nitpicker_views_up_to_date
			                                 || !_nitpicker_stacking_up_to_date;

			if (!_nitpicker_views_up_to_date) {

				/* update view positions */
				Rect top, left, right, bottom;
				border_rects(&top, &left, &right, &bottom);

				_content_view.place(Rect(_geometry.p1(), Area(0, 0)));
				_top_view    .place(top);
				_left_view   .place(left);
				_right_view  .place(right);
				_bottom_view .place(bottom);

				_nitpicker_views_up_to_date = true;
			}

			if (!_nitpicker_stacking_up_to_date) {

				_top_view.stack(_neighbor);
				_left_view.stack(_top_view.handle());
				_right_view.stack(_left_view.handle());
				_bottom_view.stack(_right_view.handle());
				_content_view.stack(_bottom_view.handle());

				_nitpicker_stacking_up_to_date = true;
			}

			if (need_nitpicker_execute)
				_nitpicker.execute();
		}

		/**
		 * Report information about element at specified position
		 *
		 * \param position  screen position
		 */
		virtual Hover hover(Point position) const = 0;
};

#endif /* _INCLUDE__DECORATOR__WINDOW_H_ */
