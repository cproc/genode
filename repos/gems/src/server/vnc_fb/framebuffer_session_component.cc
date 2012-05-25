/*
 * \brief   Framebuffer session component
 * \author  Christian Prochaska
 * \date    2014-09-21
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/env.h>
#include <dataspace/client.h>
#include <framebuffer_session/framebuffer_session.h>

#include "framebuffer_session_component.h"

static rfbScreenInfoPtr _rfb_screen;

static void ptr_event(int button_mask, int x, int y, rfbClientPtr cl)
{
	PDBG("pointer event");
}

namespace Framebuffer {

	using namespace Genode;

	Session_component::Session_component(const char *args)
	{
		_framebuffer_ds_cap = env()->ram_session()->alloc(MODE_W*MODE_H*2);
		_framebuffer_ds_addr = env()->rm_session()->attach(_framebuffer_ds_cap);

		int argc=1;
		_rfb_screen = rfbGetScreen(&argc, 0, MODE_W, MODE_H, 5, 3, 2);
		_rfb_screen->frameBuffer = _framebuffer_ds_addr;
		_rfb_screen->serverFormat.greenMax = 63;
		_rfb_screen->serverFormat.redShift = 11;
		_rfb_screen->serverFormat.blueShift = 0;
		_rfb_screen->ptrAddEvent = ptr_event;
		rfbInitServer(_rfb_screen);
		rfbRunEventLoop(_rfb_screen, -1, 1);
	}


	Session_component::~Session_component()
	{
		env()->rm_session()->detach(_framebuffer_ds_addr);
		env()->ram_session()->free(_framebuffer_ds_cap);
		/* TODO: close RFB server? */
	}


	Genode::Dataspace_capability Session_component::dataspace()
	{
		return _framebuffer_ds_cap;
	}


	Mode Session_component::mode() const
	{
		return Mode(MODE_W, MODE_H, Mode::RGB565);
	}


	void Session_component::mode_sigh(Genode::Signal_context_capability sigh_cap)
	{
	}


	void Session_component::refresh(int x, int y, int w, int h)
	{
		x = min(max(x, 0), MODE_W - 1);
		y = min(max(y, 0), MODE_H - 1);
		w = min(max(w, 0), MODE_W - x);
		h = min(max(h, 0), MODE_H - y);

		/*
		 * Apparently, the 'x2' and 'y2' arguments are supposed to be outside
		 * the rectangle (or there is an off-by-one bug in the function itself),
		 * so we don't subtract 1 pixel here.
		 */
		rfbMarkRectAsModified(_rfb_screen, x, y, x + w, y + h);
	}

	void Session_component::sync_sigh(Genode::Signal_context_capability sigh_cap)
	{
	}
}
