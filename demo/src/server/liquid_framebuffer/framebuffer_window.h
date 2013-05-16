/*
 * \brief  Window with holding a fixed-size content element
 * \author Norman Feske
 * \date   2006-09-21
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _FRAMEBUFFER_WINDOW_H_
#define _FRAMEBUFFER_WINDOW_H_

#include "window.h"
#include "titlebar.h"
#include "sky_texture.h"
#include "fade_icon.h"

#define TITLEBAR_RGBA _binary_titlebar_rgba_start
#define SIZER_RGBA    _binary_sizer_rgba_start

extern unsigned char TITLEBAR_RGBA[];
extern unsigned char SIZER_RGBA[];


template <typename PT>
class Framebuffer_window : public Window
{
	private:

		/**
		 * Constants
		 */
		enum { _TH   = 32  };  /* height of title bar */

		/**
		 * Widgets
		 */
		Titlebar<PT>              _titlebar;
		Sky_texture<PT, 512, 512> _bg_texture;
		int                       _bg_offset;
		Fade_icon<PT, 32, 32>     _sizer;
		Element                  *_content;
		bool                      _config_alpha;

	public:

		/**
		 * Constructor
		 */
		Framebuffer_window(Platform       *pf,
		                   Redraw_manager *redraw,
		                   Element        *content,
		                   const char     *name,
		                   bool            config_alpha,
		                   bool            config_resize_handle)
		:
			Window(pf, redraw, content->min_w() + 2, content->min_h() + 1 + _TH),
			_bg_offset(0), _content(content), _config_alpha(config_alpha)
		{
			/* titlebar */
			_titlebar.rgba(TITLEBAR_RGBA);
			_titlebar.text(name);
			_titlebar.event_handler(new Mover_event_handler(this));

			/* resize handle */
			_sizer.rgba(SIZER_RGBA);
			_sizer.event_handler(new Sizer_event_handler(this));
			_sizer.alpha(100);

			append(&_titlebar);
			append(_content);

			if (config_resize_handle)
				append(&_sizer);

			_min_w = max_w();
			_min_h = max_h();
		}

		/**
		 * Resize the window according to the new content size
		 */
		void content_geometry(int x, int y, int w, int h)
		{
			vpos(x, y);
			format(w + 2, h + 1 + _TH);
		}

		/**
		 * Window interface
		 */
		void format(int w, int h)
		{
			w = (w > max_w()) ? max_w() : w;
			h = (h > max_h()) ? max_h() : h;
			_w = w;
			_h = h;

			int y = 0;

			_titlebar.format_fixed_width(w);
			_titlebar.geometry(1, y, _titlebar.min_w(), _titlebar.min_h());
			y += _titlebar.min_h();

			int const content_h = (h > y + 1) ? (h - y - 1) : 0;
			int const content_w = w - 2;
			_content->format_fixed_size(content_w, content_h);
			_content->geometry(1, y, content_w, content_h);

			_sizer.geometry(_w - 32, _h - 32, 32, 32);

			pf()->view_geometry(pf()->vx(), pf()->vy(), _w, _h);
			redraw()->size(_w, _h);
			refresh();
		}

		/**
		 * Configure background texture offset (for background animation)
		 */
		void bg_offset(int bg_offset) { _bg_offset = bg_offset; }

		/**
		 * Element interface
		 */
		void draw(Canvas *c, int x, int y)
		{
			if (_config_alpha)
				_bg_texture.draw(c, 0, - _bg_offset);

			::Parent_element::draw(c, x, y);

			/* border */
			Color col(0, 0, 0);
			c->draw_box(0, 0, _w, 1, col);
			c->draw_box(0, _TH, _w, 1, col);
			c->draw_box(0, _h - 1, _w, 1, col);
			c->draw_box(0, 1, 1, _h - 2, col);
			c->draw_box(_w - 1, 1, 1, _h - 2, col);
		};
};

#endif
