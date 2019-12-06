/*
 * \brief  Intel framebuffer driver session component
 * \author Stefan Kalkowski
 * \date   2015-10-16
 */

/*
 * Copyright (C) 2015-2017 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

#ifndef __COMPONENT_H__
#define __COMPONENT_H__

/* Genode includes */
#include <base/component.h>
#include <base/rpc_server.h>
#include <root/component.h>
#include <dataspace/capability.h>
#include <framebuffer_session/framebuffer_session.h>
#include <timer_session/connection.h>
#include <util/reconstructible.h>
#include <base/attached_dataspace.h>
#include <base/attached_ram_dataspace.h>
#include <base/attached_rom_dataspace.h>
#include <os/reporter.h>
#include <os/pixel_rgb565.h>
#include <os/pixel_rgb888.h>
#include <os/dither_painter.h>
#include <blit/blit.h>

#include <lx_emul_c.h>

namespace Framebuffer {
	class Driver;
	class Session_component;
	class Root;
}


class Framebuffer::Driver
{
	private:

		struct Configuration
		{
			struct lx_c_fb_config _lx = { 16, 64, 64, 4,
			                              nullptr, 0, nullptr };
		} _config;

		Session_component             &_session;
		Timer::Connection              _timer;
		Genode::Reporter               _reporter;
#if 0
		Genode::Signal_handler<Driver> _poll_handler;
		Genode::uint64_t               _poll_ms = 0;
#endif
		Genode::Signal_context_capability _config_sigh;

		drm_display_mode * _preferred_mode(drm_connector *connector,
		                                   unsigned &brightness);
#if 0
		void _poll();
#endif
	public:

		Driver(Genode::Env & env, Session_component &session)
		: _session(session), _timer(env),
		  _reporter(env, "connectors")/*,
		  _poll_handler(env.ep(), *this, &Driver::_poll)*/ {}

		int      width()   const { return _config._lx.width;  }
		int      height()  const { return _config._lx.height; }
		int      bpp()     const { return _config._lx.bpp;    }

		void *   fb_addr() const { return _config._lx.addr;   }
		unsigned pitch()   const { return _config._lx.pitch;  }

		void finish_initialization();
#if 0
		void set_polling(Genode::uint64_t poll);
#endif
		void update_mode();

		void generate_report();

		/**
		 * Register signal handler used for config updates
		 *
		 * The signal handler is artificially triggered as a side effect
		 * of connector changes.
		 */
		void config_sigh(Genode::Signal_context_capability sigh)
		{
			_config_sigh = sigh;
		}

		void trigger_reconfiguration()
		{
			/*
			 * Trigger the reprocessing of the configuration following the
			 * same ontrol flow as used for external config changes.
			 */
			if (_config_sigh.valid())
				Genode::Signal_transmitter(_config_sigh).submit();
			else
				Genode::warning("config signal handler unexpectedly undefined");
		}
};


class Framebuffer::Session_component : public Genode::Rpc_object<Session>
{
	private:

		template <typename T> using Lazy = Genode::Constructible<T>;

		Driver                               _driver;
		Genode::Attached_rom_dataspace      &_config;
		Genode::Signal_context_capability    _mode_sigh;
		Timer::Connection                    _timer;
		Genode::Ram_allocator               &_ram;
		Genode::Attached_ram_dataspace       _ds;
		bool                                 _in_mode_change = true;

#if 0
		Genode::uint64_t _polling_from_config() {
			return _config.xml().attribute_value<Genode::uint64_t>("poll", 0); }
#endif
	public:

		Session_component(Genode::Env &env,
		                  Genode::Attached_rom_dataspace &config)
		: _driver(env, *this), _config(config), _timer(env),
		  _ram(env.ram()), _ds(env.ram(), env.rm(), 0) {}

		Driver & driver() { return _driver; }

		void config_changed()
		{
			_config.update();
			if (!_config.valid()) return;
#if 0
			_driver.set_polling(_polling_from_config());
#endif
			_in_mode_change = true;

			_driver.update_mode();

			if (_mode_sigh.valid())
				Genode::Signal_transmitter(_mode_sigh).submit();
		}

		Genode::Xml_node config() { return _config.xml(); }

		int force_width_from_config()
		{
			return _config.xml().attribute_value<unsigned>("force_width", 0);
		}

		int force_height_from_config()
		{
			return _config.xml().attribute_value<unsigned>("force_height", 0);
		}

		/***********************************
		 ** Framebuffer session interface **
		 ***********************************/

		Genode::Dataspace_capability dataspace() override
		{
			_ds.realloc(&_ram, _driver.width() * _driver.height() *
			                   Mode::bytes_per_pixel(Mode::RGB565));
			_in_mode_change = false;
			return _ds.cap();
		}

		Mode mode() const override {
			Genode::log("mode(): ", _driver.width(), "x", _driver.height());
			return Mode(_driver.width(), _driver.height(), Mode::RGB565); }

		void mode_sigh(Genode::Signal_context_capability sigh) override {
			_mode_sigh = sigh; }

		void sync_sigh(Genode::Signal_context_capability sigh) override
		{
			_timer.sigh(sigh);
			_timer.trigger_periodic(10*1000);
		}

		void refresh(int x, int y, int w, int h) override
		{
			using namespace Genode;

			if (!_driver.fb_addr()         ||
				!_ds.local_addr<void>() ||
				_in_mode_change) return;

			int width      = _driver.width();
			int height     = _driver.height();
#if 0
			unsigned bpp   = _driver.bpp();
			unsigned pitch = _driver.pitch();
#endif
			/* clip specified coordinates against screen boundaries */
			int x2 = min(x + w - 1, width  - 1),
			    y2 = min(y + h - 1, height - 1);
			int x1 = max(x, 0),
			    y1 = max(y, 0);
			if (x1 > x2 || y1 > y2) return;

//Genode::log("refresh(): x1: ", x1, ", x2: ", x2, ", y1: ", y1, ", y2: ", y2, ", bpp: ", bpp, ", pitch: ", pitch);

			/* copy pixels from back buffer to physical frame buffer */

#if 0
			char *src = _ds.local_addr<char>()  + bpp*(width*y1 + x1),
			     *dst = (char*)_driver.fb_addr() + pitch*y1 + bpp*x1;

			blit(src, bpp*width, dst, pitch,
			     bpp*(x2 - x1 + 1), y2 - y1 + 1);
#endif

#if 0
			for (int y = y1; y <= y2; y++) {
				for (x = x1; x <= x2; x++) {
					uint16_t *src = (uint16_t*)((Genode::addr_t)_ds.local_addr<char>() + 2 * (width * y + x));
					uint32_t *dst = (uint32_t*)((Genode::addr_t)_driver.fb_addr() + pitch*y + 4*x);
					uint32_t r = (*src >> 11) * 256 / 32;
					uint32_t g = ((*src >> 5) & 0x3f) * 256 / 32;
					uint32_t b = (*src & 0x1f) * 256 / 32;
					*dst = (r << 24) | (g << 16) | (b << 8) | 0xff;
				}
			}
#endif

			Surface_base::Area const area = Surface_base::Area(width, height);

			char *src = _ds.local_addr<char>(),
			     *dst = (char*)_driver.fb_addr();

			typedef Genode::Pixel_rgb565 Pixel_src;
			typedef Genode::Pixel_rgb888 Pixel_dst;

			Genode::Texture<Pixel_src> texture((Pixel_src *)src, nullptr, area);
			Genode::Surface<Pixel_dst> surface((Pixel_dst *)dst, area);

			Dither_painter::paint(surface, texture, Genode::Surface_base::Point(x1, y1));
		}
};


struct Framebuffer::Root
: Genode::Root_component<Framebuffer::Session_component, Genode::Single_client>
{
	Session_component session; /* single session */

	Session_component *_create_session(const char *) override {
		return &session; }

	void _destroy_session(Session_component *) override { }

	Root(Genode::Env &env, Genode::Allocator &alloc,
	     Genode::Attached_rom_dataspace &config)
	: Genode::Root_component<Session_component,
	                         Genode::Single_client>(env.ep(), alloc),
	  session(env, config) { }
};

#endif /* __COMPONENT_H__ */
