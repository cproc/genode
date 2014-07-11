/*
 * \brief  Signal handler thread implementation
 * \author Christian Prochaska
 * \date   2011-08-15
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* libc includes */
#include <signal.h>

/* GDB monitor includes */
extern "C" {
#include "genode-low.h"
}
#include "thread_info.h"
#include "signal_handler_thread.h"

using namespace Genode;
using namespace Gdb_monitor;

Signal_handler_thread::Signal_handler_thread(Signal_receiver *receiver)
:
	Thread<SIGNAL_HANDLER_THREAD_STACK_SIZE>("sig_handler"),
	_signal_receiver(receiver) { }


void Signal_handler_thread::entry()
{
	while(1) {
		Signal s = _signal_receiver->wait_for_signal();

		if (Thread_info *thread_info = dynamic_cast<Thread_info*>(s.context())) {
			genode_send_signal_to_thread(thread_info->lwpid(), SIGTRAP);
		} else {
			genode_send_signal_to_thread(genode_find_segfault_lwpid(), SIGSEGV);
		}
	}
};
