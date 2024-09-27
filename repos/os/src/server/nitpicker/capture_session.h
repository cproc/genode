/*
 * \brief  Capture session component
 * \author Norman Feske
 * \date   2020-06-27
 */

/*
 * Copyright (C) 2020 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CAPTURE_SESSION_H_
#define _CAPTURE_SESSION_H_

/* Genode includes */
#include <base/session_object.h>
#include <capture_session/capture_session.h>

namespace Nitpicker { class Capture_session; }


class Nitpicker::Capture_session : public Session_object<Capture::Session>
{
	public:

		struct Handler : Interface
		{
			/**
			 * Prompt nitpicker to adjust the screen size depending on all
			 * present capture buffers.
			 */
			virtual void capture_buffer_size_changed() = 0;

			virtual void capture_requested(Label const &) = 0;
		};

		struct Policy
		{
			template <typename T>
			struct Attr
			{
				bool _defined { };
				T    _value   { };

				Attr() { }

				Attr(T value) : _defined(true), _value(value) { }

				Attr(Xml_node const node, auto const &attr)
				{
					if (node.has_attribute(attr)) {
						_value   = node.attribute_value(attr, T { });
						_defined = true;
					}
				}

				/**
				 * Return defined attribute value, or default value
				 */
				T or_default(T def) const { return _defined ? _value : def; }
			};

			Attr<int>      x, y;       /* placement within panorama */
			Attr<unsigned> w, h;       /* capture contraints */
			Attr<unsigned> w_mm, h_mm; /* physical size overrides */

			static Policy from_xml(Xml_node const &policy)
			{
				return { .x    = { policy, "xpos"      },
				         .y    = { policy, "ypos"      },
				         .w    = { policy, "width"     },
				         .h    = { policy, "height"    },
				         .w_mm = { policy, "width_mm"  },
				         .h_mm = { policy, "height_mm" } };
			}

			static Policy unconstrained() { return { }; }

			static Policy blocked()
			{
				Policy result { };
				result.w = 0, result.h = 0;
				return result;
			}
		};

		static void gen_attr(Xml_generator &xml, Rect const rect)
		{
			if (rect.x1()) xml.attribute("xpos", rect.x1());
			if (rect.y1()) xml.attribute("ypos", rect.y1());

			xml.attribute("width",  rect.w());
			xml.attribute("height", rect.h());
		}

	private:

		Env &_env;

		Constrained_ram_allocator _ram;

		Handler &_handler;

		View_stack const &_view_stack;

		Policy _policy = Policy::blocked();

		Buffer_attr _buffer_attr { };

		Constructible<Attached_ram_dataspace> _buffer { };

		Signal_context_capability _screen_size_sigh { };

		Signal_context_capability _wakeup_sigh { };

		bool _stopped = false;

		using Dirty_rect = Genode::Dirty_rect<Rect, Affected_rects::NUM_RECTS>;

		Dirty_rect _dirty_rect { };

		void _wakeup_if_needed()
		{
			if (_stopped && !_dirty_rect.empty() && _wakeup_sigh.valid()) {
				Signal_transmitter(_wakeup_sigh).submit();
				_stopped = false;
			}
		}

		Point _anchor_point() const
		{
			return { .x = _policy.x.or_default(0),
			         .y = _policy.y.or_default(0) };
		}

		Area _area_bounds() const
		{
			return { .w = _policy.w.or_default(_buffer_attr.px.w),
			         .h = _policy.h.or_default(_buffer_attr.px.h) };
		}

	public:

		Capture_session(Env              &env,
		                Resources  const &resources,
		                Label      const &label,
		                Diag       const &diag,
		                Handler          &handler,
		                View_stack const &view_stack)
		:
			Session_object(env.ep(), resources, label, diag),
			_env(env),
			_ram(env.ram(), _ram_quota_guard(), _cap_quota_guard()),
			_handler(handler),
			_view_stack(view_stack)
		{
			_dirty_rect.mark_as_dirty(view_stack.bounding_box());
		}

		~Capture_session() { }


		/*****************************************
		 ** Interface used by 'Nitpicker::Main' **
		 *****************************************/

		/**
		 * Geometry within the panorama, depending on policy and client buffer
		 */
		Rect bounding_box() const { return { _anchor_point(), _area_bounds() }; }

		void mark_as_damaged(Rect rect)
		{
			_dirty_rect.mark_as_dirty(rect);
			_wakeup_if_needed();
		}

		void screen_size_changed()
		{
			if (_screen_size_sigh.valid())
				Signal_transmitter(_screen_size_sigh).submit();
		}

		void apply_policy(Policy const &policy) { _policy = policy; }


		/*******************************
		 ** Capture session interface **
		 *******************************/

		Area screen_size() const override
		{
			return Rect::intersect(_view_stack.bounding_box(), bounding_box()).area;
		}

		void screen_size_sigh(Signal_context_capability sigh) override
		{
			_screen_size_sigh = sigh;
		}

		void wakeup_sigh(Signal_context_capability sigh) override
		{
			_wakeup_sigh = sigh;
			_wakeup_if_needed();
		}

		Buffer_result buffer(Buffer_attr const attr) override
		{
			Buffer_result result = Buffer_result::OK;

			_buffer_attr = { };

			if (!attr.px.valid()) {
				_buffer.destruct();
				return result;
			}

			try {
				_buffer.construct(_ram, _env.rm(), buffer_bytes(attr.px));
				_buffer_attr = attr;
			}
			catch (Out_of_ram)  { result = Buffer_result::OUT_OF_RAM; }
			catch (Out_of_caps) { result = Buffer_result::OUT_OF_CAPS; }

			_handler.capture_buffer_size_changed();

			/* report complete buffer as dirty on next call of 'capture_at' */
			mark_as_damaged({ _anchor_point(), attr.px });

			return result;
		}

		Dataspace_capability dataspace() override
		{
			if (_buffer.constructed())
				return _buffer->cap();

			return Dataspace_capability();
		}

		Affected_rects capture_at(Point const pos) override
		{
			_handler.capture_requested(label());

			if (!_buffer.constructed())
				return Affected_rects { };

			Canvas<Pixel_rgb888> canvas { _buffer->local_addr<Pixel_rgb888>(),
			                              _anchor_point() + pos, _buffer_attr.px };

			canvas.clip(Rect::intersect(bounding_box(), _view_stack.bounding_box()));

			Rect const buffer_rect { { }, _buffer_attr.px };

			Affected_rects affected { };
			unsigned i = 0;
			_dirty_rect.flush([&] (Rect const &rect) {

				_view_stack.draw(canvas, rect);

				if (i < Affected_rects::NUM_RECTS) {
					Rect const translated(rect.p1() - _anchor_point() - pos, rect.area);
					Rect const clipped = Rect::intersect(translated, buffer_rect);
					affected.rects[i++] = clipped;
				}
			});

			return affected;
		}

		void capture_stopped() override
		{
			_stopped = true;

			/* dirty pixels may be pending */
			_wakeup_if_needed();
		}
};

#endif /* _CAPTURE_SESSION_H_ */
