/*
 * \brief  Example window decorator that mimics the Motif look
 * \author Norman Feske
 * \date   2013-01-04
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
#include <nitpicker_session/connection.h>
#include <os/pixel_rgb565.h>
#include <os/attached_rom_dataspace.h>

/* Nitpicker graphics backend */
#include <nitpicker_gfx/text_painter.h>
#include <nitpicker_gfx/box_painter.h>

/* decorator includes */
#include <decorator/window_stack.h>
#include <decorator/xml_utils.h>

/* local includes */
#include "canvas.h"
#include "window.h"


namespace Decorator {
	using namespace Genode;
	struct Main;
}


struct Decorator::Main : Window_factory_base
{
	Signal_receiver &sig_rec;

	Nitpicker::Connection nitpicker;

	Framebuffer::Mode mode = { nitpicker.mode() };

	Attached_dataspace fb_ds = { (nitpicker.buffer(mode, false),
	                              nitpicker.framebuffer()->dataspace()) };

	Canvas<Pixel_rgb565> canvas = { fb_ds.local_addr<Pixel_rgb565>(),
	                                Area(mode.width(), mode.height()) };

	Window_stack window_stack = { *this };

	/**
	 * Install handler for responding to window-layout changes
	 */
	void handle_window_layout_update(unsigned);

	Signal_dispatcher<Main> window_layout_dispatcher = {
		sig_rec, *this, &Main::handle_window_layout_update };

	Attached_rom_dataspace window_layout { "window_layout" };

	/**
	 * Install handler for responding to pointer-position updates
	 */
	void handle_pointer_update(unsigned);

	Signal_dispatcher<Main> pointer_dispatcher = {
		sig_rec, *this, &Main::handle_pointer_update };

	Attached_rom_dataspace pointer { "pointer" };

	/**
	 * Constructor
	 */
	Main(Signal_receiver &sig_rec) : sig_rec(sig_rec)
	{
		window_layout.sigh(window_layout_dispatcher);
		pointer.sigh(pointer_dispatcher);
	}

	/**
	 * Window_factory_base interface
	 */
	Window_base *create(Xml_node window_node) override
	{
		return new (env()->heap())
			Window(attribute(window_node, "id", 0UL), nitpicker);
	}

	/**
	 * Window_factory_base interface
	 */
	void destroy(Window_base *window) override
	{
		Genode::destroy(env()->heap(), static_cast<Window *>(window));
	}
};


void Decorator::Main::handle_window_layout_update(unsigned)
{
	window_layout.update();

	bool flush = true;

	if (window_layout.is_valid()) {
		try {
			Xml_node xml(window_layout.local_addr<char>(),
			             window_layout.size());
			window_stack.update_model(xml);

			/* everything went fine, let us keep the cached state */
			flush = false;
		} catch (Xml_node::Invalid_syntax) { }
	}

	/* XXX refresh only the changed portions */
	Rect rect(Point(0, 0), Area(mode.width(), mode.height()));

	window_stack.draw(canvas, rect);

	nitpicker.framebuffer()->refresh(rect.x1(), rect.y1(), rect.w(), rect.h());

	/*
	 * An error occured with processing the XML model. Flush the internal
	 * representation.
	 */
	if (flush)
		window_stack.flush();
}


void Decorator::Main::handle_pointer_update(unsigned)
{
	pointer.update();

	if (pointer.is_valid())
		PLOG("pointer: %s", pointer.local_addr<char>());
}


int main(int argc, char **argv)
{
	static Genode::Signal_receiver sig_rec;

	static Decorator::Main application(sig_rec);

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
