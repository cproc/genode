/*
 * \brief   Framebuffer session component
 * \author  Christian Prochaska
 * \date    2012-04-10
 */

/*
 * Copyright (C) 2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/env.h>
#include <dataspace/client.h>
#include <framebuffer_session/framebuffer_session.h>

#include "framebuffer_session_component.h"


namespace Framebuffer {

	using namespace Genode;

	Session_component::Session_component(const char *args)
	: _framebuffer(Genode::env()->parent()->session<Framebuffer::Session>(args))
	{
		Dataspace_capability ds = dataspace();
		_framebuffer_ds_addr = env()->rm_session()->attach(ds);
		_framebuffer_ds_size = Dataspace_client(ds).size();

	}


	Session_component::~Session_component()
	{
		env()->rm_session()->detach(_framebuffer_ds_addr);
	}


	Genode::Dataspace_capability Session_component::dataspace()
	{
		return _framebuffer.dataspace();
	}


	void Session_component::release()
	{
		_framebuffer.release();
	}


	Mode Session_component::mode() const
	{
		return _framebuffer.mode();
	}


	void Session_component::mode_sigh(Genode::Signal_context_capability sigh_cap)
	{
		_framebuffer.mode_sigh(sigh_cap);
	}


	void Session_component::refresh(int x, int y, int w, int h)
	{
		for (unsigned int i = 0; i < _framebuffer_ds_size / sizeof(uint16_t); i++)
			_framebuffer_ds_addr[i] = ~_framebuffer_ds_addr[i];

		_framebuffer.refresh(x, y, w, h);
	}
}
