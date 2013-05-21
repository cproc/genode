/*
 * \brief  Nitpicker-based virtual framebuffer
 * \author Norman Feske
 * \date   2006-09-21
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/signal.h>
#include <os/config.h>

#include "framebuffer_window.h"
#include "canvas_rgb565.h"
#include "user_state.h"
#include "services.h"

using namespace Genode;

/**
 * Runtime configuration
 */
namespace Config
{
	int iconbar_detail    = 1;
	int background_detail = 1;
	int mouse_cursor      = 1;
	int browser_attr      = 0;
}


void Launcher::launch() { }

extern int native_startup(int, char **);


class Background_animator : public Tick
{
	private:

		Framebuffer_window<Pixel_rgb565> *_fb_win;
		int                               _bg_offset;

	public:

		/**
		 * Constructor
		 */
		Background_animator(Framebuffer_window<Pixel_rgb565> *fb_win):
			_fb_win(fb_win), _bg_offset(0) {
			schedule(20); }

		/**
		 * Tick interface
		 */
		int on_tick()
		{
			_fb_win->bg_offset(_bg_offset);
			_bg_offset += 2;
			_fb_win->refresh();

			/* schedule next tick */
			return 1;
		}
};


/**
 * Animated background
 */
static bool config_animate = true;
static bool config_alpha   = true;

/**
 * Size and position of virtual frame buffer
 */
static long config_fb_width  = 500;
static long config_fb_height = 400;
static long config_fb_x      = 400;
static long config_fb_y      = 260;

/**
 * Window title
 */
static const char *config_title = "Liquid Framebuffer";

/**
 * Resize handle
 */
static bool config_resize_handle = false;

/**
 * Window decoration
 */
static bool config_decoration = true;

/**
 * Parse configuration
 */
static void read_config()
{
	using namespace Genode;

	Xml_node config_node = config()->xml_node();

	try {
		char buf[16];
		config_node.attribute("animate").value(buf, sizeof(buf));

		if      (!strcmp("off", buf)) config_animate = false;
		else if (!strcmp("on",  buf)) config_animate = true;
		else
			Genode::printf("Warning: invalid value for animate declaration,\n"
			               "         valid values are 'on', 'off.\n'");
	} catch (Xml_node::Nonexistent_attribute) { }

	config_alpha = config_animate;

	try { config_node.attribute("xpos").value(&config_fb_x); }
	catch (Xml_node::Nonexistent_attribute) { }

	try { config_node.attribute("ypos").value(&config_fb_y); }
	catch (Xml_node::Nonexistent_attribute) { }

	try { config_node.attribute("width").value(&config_fb_width); }
	catch (Xml_node::Nonexistent_attribute) { }

	try { config_node.attribute("height").value(&config_fb_height); }
	catch (Xml_node::Nonexistent_attribute) { }

	try {
		static char buf[64];
		config_node.attribute("title").value(buf, sizeof(buf));
		config_title = buf;
	} catch (Xml_node::Nonexistent_attribute) { }

	try {
		char buf[16];
		config_node.attribute("decoration").value(buf, sizeof(buf));

		if      (!strcmp("off", buf)) config_decoration = false;
		else if (!strcmp("on",  buf)) config_decoration = true;
		else
			Genode::printf("Warning: invalid value for decoration declaration,\n"
			               "         valid values are 'on', 'off.\n'");
	} catch (Xml_node::Nonexistent_attribute) { }

	try {
		char buf[16];
		config_node.attribute("resize_handle").value(buf, sizeof(buf));

		if      (!strcmp("off", buf)) config_resize_handle = false;
		else if (!strcmp("on",  buf)) config_resize_handle = true;
		else
			Genode::printf("Warning: invalid value for resize_handle declaration,\n"
			               "         valid values are 'on', 'off.\n'");
	} catch (Xml_node::Nonexistent_attribute) { }

}


/**
 * Main program
 */
int main(int argc, char **argv)
{
	if (native_startup(argc, argv)) return -1;

	try { read_config(); } catch (...) { }

	/*
	 * Register signal handler for config changes
	 */
	static Signal_receiver sig_rec;
	static Signal_context sig_ctx;

	try { config()->sigh(sig_rec.manage(&sig_ctx)); } catch (...) { }

	/* heuristic for allocating the double-buffer backing store */
	enum { WINBORDER_WIDTH = 10, WINBORDER_HEIGHT = 40 };

	/* init platform */
	static Platform pf(config_fb_x, config_fb_y,
	                   config_fb_width  + WINBORDER_WIDTH,
	                   config_fb_height + WINBORDER_HEIGHT,
	                   config_fb_width  + WINBORDER_WIDTH,
	                   config_fb_height + WINBORDER_HEIGHT);

	/* initialize our window content */
	init_window_content(config_fb_width, config_fb_height, config_alpha);

	/* init canvas */
	static Chunky_canvas<Pixel_rgb565> canvas;
	canvas.init(static_cast<Pixel_rgb565 *>(pf.buf_adr()),
	            pf.scr_w()*pf.scr_h());
	canvas.set_size(pf.scr_w(), pf.scr_h());
	canvas.clip(0, 0, pf.scr_w(), pf.scr_h());

	/* init redraw manager */
	static Redraw_manager redraw(&canvas, &pf, pf.vw(), pf.vh());

	/* create instance of browser window */
	static Framebuffer_window<Pixel_rgb565>
		fb_win(&pf, &redraw, window_content(), config_title, config_alpha,
		       config_resize_handle, config_decoration);

	if (config_animate) {
		static Background_animator fb_win_bg_anim(&fb_win);
	}

	/* create user state manager */
	static User_state user_state(&fb_win, &fb_win, pf.vx(), pf.vy());

	/* assign framebuffer window as root element to redraw manager */
	redraw.root(&fb_win);

	fb_win.parent(&user_state);
	fb_win.format(fb_win.min_w(), fb_win.min_h());

	/* initialize our services */
	init_services();

	/* enter main loop */
	Event ev;
	unsigned long curr_time, old_time;
	curr_time = old_time = pf.timer_ticks();
	lock_window_content();
	do {
		unlock_window_content();
		pf.get_event(&ev);
		lock_window_content();

		if (ev.type != Event::WHEEL) {
			ev.mx -= user_state.vx();
			ev.my -= user_state.vy();
		}

		/* direct all keyboard events to the window content */
		if ((ev.type == Event::PRESS || ev.type == Event::RELEASE)
		 && (ev.code != Event::BTN_LEFT))
			window_content()->handle_event(ev);
		else
			user_state.handle_event(ev);

		if (ev.type == Event::REFRESH)
			pf.scr_update(0, 0, pf.scr_w(), pf.scr_h());

		if (ev.type == Event::TIMER) {
			Tick::handle(pf.timer_ticks());
			/* check for configuration changes */
			if (sig_rec.pending()) {
				sig_rec.wait_for_signal();
				config()->reload();
				/* keep the current values by default */
				config_fb_x = fb_win.view_x();
				config_fb_y = fb_win.view_y();
				config_fb_width = fb_win.view_w();
				config_fb_height = fb_win.view_h();
				try { read_config(); } catch (...) { }
				fb_win.name(config_title);
				fb_win.config_alpha(config_alpha);
				fb_win.config_resize_handle(config_resize_handle);
				fb_win.config_decoration(config_decoration);
				/* must get called after 'config_decoration()' */
				fb_win.content_geometry(config_fb_x, config_fb_y,
				                        config_fb_width, config_fb_height);
				user_state.update_view_offset();
			}
		}

		/* perform periodic redraw */
		curr_time = pf.timer_ticks();
		if (!pf.event_pending() && ((curr_time - old_time > 20) || (curr_time < old_time))) {
			old_time = curr_time;
			redraw.process();
		}
	} while (ev.type != Event::QUIT);

	return 0;
}
