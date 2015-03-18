/*
 * \brief  Console implementation of VirtualBox for Genode
 * \author Alexander Boettcher
 * \author Norman Feske
 * \date   2013-10-16
 */

/*
 * Copyright (C) 2013-2014 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

/* Genode includes */
#include <input/event.h>
#include <input/keycodes.h>
#include <input_session/connection.h>
#include <timer_session/connection.h>

/* included from os/src/drivers/input/ps2 */
#include <scan_code_set_1.h>

/* VirtualBox includes */
#include "ConsoleImpl.h"
#include <base/printf.h>
#include <os/attached_dataspace.h>
#include <report_session/connection.h>
#include <trace/timestamp.h>

/* XXX */
enum { KMOD_RCTRL = 0, SDLK_RCTRL = 0 };


class Scan_code
{
	private:

		class Converter
		{
			public:

				unsigned char scan_code     [Input::KEY_UNKNOWN];
				unsigned char scan_code_ext [Input::KEY_UNKNOWN];

			private:

				unsigned char _search_scan_code(Input::Keycode keycode)
				{
					for (unsigned i = 0; i < SCAN_CODE_SET_1_NUM_KEYS; i++)
						if (scan_code_set_1[i] == keycode)
							return i;
					return 0;
				}

				unsigned char _search_scan_code_ext(Input::Keycode keycode)
				{
					for (unsigned i = 0; i < SCAN_CODE_SET_1_NUM_KEYS; i++)
						if (scan_code_set_1_0xe0[i] == keycode)
							return i;
					return 0;
				}

			public:

				Converter()
				{
					init_scan_code_set_1_0xe0();

					for (unsigned i = 0; i < Input::KEY_UNKNOWN; i++) {
						scan_code     [i] = _search_scan_code    ((Input::Keycode)i);
						scan_code_ext [i] = _search_scan_code_ext((Input::Keycode)i);
					}
				}
		};

		static Converter &converter()
		{
			static Converter inst;
			return inst;
		}

		Input::Keycode _keycode;

	public:

		Scan_code(Input::Keycode keycode) : _keycode(keycode) { }

		bool is_normal() const { return converter().scan_code[_keycode]; }
		bool is_ext()    const { return converter().scan_code_ext[_keycode]; }

		bool valid() const
		{
			return is_normal() || is_ext();
		}

		unsigned char code() const
		{
			return converter().scan_code[_keycode];
		}

		unsigned char ext() const
		{
			return converter().scan_code_ext[_keycode];
		}
};

uint64_t tsc_start = 0;
uint64_t tsc_diff_sum = 0;
unsigned int tsc_count = 0;
uint64_t tsc_diff_min = (uint64_t)-1;
uint64_t tsc_diff_max = 0;

class GenodeConsole : public Console {

	private:

		Input::Connection          _input;
		Genode::Signal_receiver    _receiver;
		Genode::Signal_context     _context;
		Input::Event              *_ev_buf;
		unsigned                   _ax, _ay;
		Report::Connection         _shape_report_connection;
		Genode::Attached_dataspace _shape_report_ds;

		bool _key_status[Input::KEY_MAX + 1];

		static bool _is_mouse_button(Input::Keycode keycode)
		{
			return keycode == Input::BTN_LEFT
			    || keycode == Input::BTN_RIGHT
			    || keycode == Input::BTN_MIDDLE;
		}

	public:

		GenodeConsole()
		:
			Console(),
			_ev_buf(static_cast<Input::Event *>(Genode::env()->rm_session()->attach(_input.dataspace()))),
			_ax(0), _ay(0),
			_shape_report_connection("shape", 4096),
			_shape_report_ds(_shape_report_connection.dataspace())
		{
			for (unsigned i = 0; i <= Input::KEY_MAX; i++)
				_key_status[i] = 0;

			_input.sigh(_receiver.manage(&_context));
		}

		void eventWait(IKeyboard * gKeyboard, IMouse * gMouse)
		{
			static Timer::Connection timer;
#if 0
			for (int d = 8; ; d = -d) {

				for (int x = 0; x < 128; x++) {
					tsc_start = Genode::Trace::timestamp();
					//RTLogPrintf("input, %lu", Genode::Trace::timestamp());
					gMouse->PutMouseEvent(d, 0, 0, 0, 0);
					timer.msleep(250);
				}

			}


			return;
#endif
			/* ----- */

			_receiver.wait_for_signal();

			for (int i = 0, num_ev = _input.flush(); i < num_ev; ++i) {
				Input::Event &ev = _ev_buf[i];

				bool const is_press   = ev.type() == Input::Event::PRESS;
				bool const is_release = ev.type() == Input::Event::RELEASE;
				bool const is_key     = is_press || is_release;
				bool const is_motion  = ev.type() == Input::Event::MOTION;
				bool const is_wheel   = ev.type() == Input::Event::WHEEL;

				if (is_key) {
					Scan_code scan_code(ev.keycode());

					unsigned char const release_bit =
						(ev.type() == Input::Event::RELEASE) ? 0x80 : 0;

					if (scan_code.is_normal())
						gKeyboard->PutScancode(scan_code.code() | release_bit);

					if (scan_code.is_ext()) {
						gKeyboard->PutScancode(0xe0);
						gKeyboard->PutScancode(scan_code.ext() | release_bit);
					}
				}

				/*
				 * Track press/release status of keys and buttons. Currently,
				 * only the mouse-button states are actually used.
				 */
				if (is_press)
					_key_status[ev.keycode()] = true;

				if (is_release)
					_key_status[ev.keycode()] = false;

				bool const is_mouse_button_event =
					is_key && _is_mouse_button(ev.keycode());

				bool const is_mouse_event = is_mouse_button_event || is_motion;

				if (is_mouse_event) {
					unsigned const buttons = (_key_status[Input::BTN_LEFT]   ? MouseButtonState_LeftButton : 0)
					                       | (_key_status[Input::BTN_RIGHT]  ? MouseButtonState_RightButton : 0)
					                       | (_key_status[Input::BTN_MIDDLE] ? MouseButtonState_MiddleButton : 0);
					if (ev.is_absolute_motion()) {
						int const rx = ev.ax() - _ax; _ax = ev.ax();
						int const ry = ev.ay() - _ay; _ay = ev.ay();
						//gMouse->PutMouseEvent(rx, ry, 0, 0, buttons);
						gMouse->PutMouseEventAbsolute(ev.ax(), ev.ay(), 0, 0, buttons);
					} else if (ev.is_relative_motion())
						gMouse->PutMouseEvent(ev.rx(), ev.ry(), 0, 0, buttons);

					/* only the buttons changed */
					else
						gMouse->PutMouseEventAbsolute(_ax, _ay, 0, 0, buttons);
				}

				if (is_wheel)
					gMouse->PutMouseEvent(0, 0, ev.rx(), ev.ry(), 0);
			}
		}

		void onMousePointerShapeChange(bool fVisible, bool fAlpha,
		                               uint32_t xHot, uint32_t yHot,
		                               uint32_t width, uint32_t height,
		                               ComSafeArrayIn(BYTE,pShape))
		{
			com::SafeArray<BYTE> shape_array(ComSafeArrayInArg(pShape));

			PDBG("visible: %d, alpha: %d, hot: %u,%u, width: %u, height: %u", fVisible, fAlpha, xHot, yHot, width, height);

			enum { MAX_SHAPE_SIZE = 3*1024 };

			struct Shape_data
			{
				bool          visible;
				uint32_t      x_hot;
				uint32_t      y_hot;
				uint32_t      width;
				uint32_t      height;
				unsigned char shape[MAX_SHAPE_SIZE];
			};

			Shape_data shape_data;
			shape_data.visible = fVisible;
			shape_data.x_hot = xHot;
			shape_data.y_hot = yHot;
			shape_data.width = width;
			shape_data.height = height;

			if (shape_array.size() > MAX_SHAPE_SIZE) {
				PERR("%s: shape data buffer is too small for %zu bytes", __func__, shape_array.size());
				return;
			}

			Genode::memcpy(&shape_data.shape, shape_array.raw(), shape_array.size());

			Genode::memcpy(_shape_report_ds.local_addr<unsigned char>(), &shape_data, sizeof(Shape_data));
		}
};
