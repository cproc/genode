/*
 * \brief  Test for timer service
 * \author Norman Feske
 * \date   2009-06-22
 */

/*
 * Copyright (C) 2009-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/log.h>
#include <timer_session/connection.h>

using namespace Genode;

int main(int argc, char **argv)
{
	static Timer::Connection main_timer;
	static Timer::Connection dummy_timer;

	Signal_receiver sig_rcv;
	Signal_context  sig_cxt;

	for (;;) {
		//Genode::log("1");
		Signal_context_capability sig = sig_rcv.manage(&sig_cxt);
		//Genode::log("2");
		main_timer.sigh(sig);
		//Genode::log("3");
		sig_rcv.dissolve(&sig_cxt);
	}

	return 0;
}
