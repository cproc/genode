/*
 * \brief   Titlebar interface
 * \date    2005-10-24
 * \author  Norman Feske <norman.feske@genode-labs.com>
 */

/*
 * Copyright (C) 2005-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _TITLEBAR_H_
#define _TITLEBAR_H_

#include "widgets.h"

#define TITLE_TFF _binary_vera18_tff_start
extern char TITLE_TFF[];


/***************
 ** Title bar **
 ***************/

static Font title_font(TITLE_TFF);

template <typename PT>
class Titlebar : public Parent_element
{
	private:

		Icon<PT, 32, 32> _fg;
		const char *_txt;
		int _txt_w, _txt_h, _txt_len;

	public:

		/**
		 * Define text displayed within titlebar
		 */
		void text(const char *txt)
		{
			_txt     = txt ? txt : "Scout";
			_txt_w   = title_font.str_w(_txt, strlen(_txt));
			_txt_h   = title_font.str_h(_txt, strlen(_txt));
			_txt_len = strlen(_txt);
		}

		/**
		 * Constructor
		 */
		Titlebar()
		{
			_fg.alpha(255);
			_fg.findable(0);
			text(0);

			append(&_fg);
		}

		/**
		 * Define foreground of titlebar
		 */
		void rgba(unsigned char *rgba) { _fg.rgba(rgba, 0, 0); };

		/**
		 * Element interface
		 */

		void format_fixed_width(int w)
		{
			_min_w = w;
			_min_h = 32;
			_fg.geometry(0, 0, _min_w, _min_h);
		}

		void draw(Canvas *c, int x, int y)
		{
			const int b = 180, a = 200;
			c->draw_box(x + _x, y + _y, _w, _h, Color(b, b, b, a));

			int _txt_x = x + _x + max((_w - _txt_w)/2, 8);
			int _txt_y = y + _y + max((_h - _txt_h)/2, 0) - 1;
			c->draw_string(_txt_x , _txt_y, &title_font, Color(0,0,0,200), _txt, strlen(_txt));
			::Parent_element::draw(c, x, y);
		}
};

#endif
