/*
 * \brief   Framebuffer filter
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
#include <base/sleep.h>
#include <cap_session/connection.h>

#include "framebuffer_root.h"


using namespace Genode;

int main(int argc, char *argv[])
{
	enum { STACK_SIZE = 8*1024*sizeof(addr_t) };
	static Cap_connection cap;
	static Rpc_entrypoint ep(&cap, STACK_SIZE, "vnc_fb_ep");

	static Framebuffer::Root framebuffer_root(&ep, env()->heap());
	env()->parent()->announce(ep.manage(&framebuffer_root));

	Genode::sleep_forever();

	return 0;
}
