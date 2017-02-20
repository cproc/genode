/*
 * \brief  QPA signal handler thread
 * \author Christian Prochaska
 * \date   2015-09-18
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include "qsignalhandlerthread.h"
#include <base/log.h>
void QSignalHandlerThread::run()
{
	for (;;) {
Genode::log("calling wait_for_signal()");
		Genode::Signal s = _signal_receiver.wait_for_signal();
Genode::log("wait_for_signal() returned, calling dispatch()");
		static_cast<Genode::Signal_dispatcher_base*>(s.context())->dispatch(s.num());
Genode::log("dispatch() returned");
	}
}
