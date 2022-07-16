/*
 * \brief  Timestamp implementation for the Genode Timer
 * \author Christian Prochaska
 * \date   2022-07-14
 *
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Linux includes */
#include <linux_syscalls.h>
#include <sys/time.h>

/* Genode includes */
#include <timer_session/connection.h>

using namespace Genode;


static inline int lx_gettimeofday(struct timeval *tv, struct timeval *tz) {
	return (int)lx_syscall(SYS_gettimeofday, tv, tz); }


Trace::Timestamp Timer::Connection::_timestamp()
{
//Genode::log("_timestamp()");
	struct timeval tv;
	lx_gettimeofday(&tv, 0);
	return (uint64_t)tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}
