/*
 * \brief  C-library back end
 * \author Christian Prochaska
 * \author Norman Feske
 * \date   2010-05-19
 */

/*
 * Copyright (C) 2010-2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Libc includes */
#include <sys/time.h>

#include "task.h"
#include "libc_errno.h"

/* Genode includes */
#include <base/log.h>

namespace Libc { time_t read_rtc(); }


extern "C" __attribute__((weak))
int clock_gettime(clockid_t clk_id, struct timespec *ts)
{
	if (!ts) return Libc::Errno(EFAULT);

	if (clk_id == CLOCK_REALTIME || clk_id == CLOCK_SECOND) {
		static bool   initial_rtc_requested = false;
		static time_t initial_rtc = 0;
		static unsigned long t0_ms = 0;

		/* try to read rtc once */
		if (!initial_rtc_requested) {
			initial_rtc_requested = true;
			initial_rtc = Libc::read_rtc();
			if (initial_rtc) {
				t0_ms = Libc::current_time().trunc_to_plain_ms().value;
			}
		}

		/* return success if this is RTC time */
		if (initial_rtc) {
			unsigned long time = Libc::current_time().trunc_to_plain_ms().value - t0_ms;

			ts->tv_sec  = initial_rtc + time/1000;
			ts->tv_nsec = (time % 1000) * (1000*1000);

			return 0;
		}
	}

	/* otherwise return arbitrary monotonic time */

	unsigned long us = Libc::current_time().trunc_to_plain_us().value;
	ts->tv_sec  = us / (1000*1000);
	ts->tv_nsec = (us % (1000*1000)) * 1000;

	/*
	 * return an error if the semantics of the
	 * requested clock are not implemented
	 */
	if (clk_id == CLOCK_MONOTONIC) {
		return 0;
	} else {
		return Libc::Errno(EINVAL);
	}
}


extern "C" __attribute__((weak))
int gettimeofday(struct timeval *tv, struct timezone *)
{
	if (!tv) return 0;

	struct timespec ts;

	if (int ret = clock_gettime(CLOCK_REALTIME, &ts))
		return ret;

	tv->tv_sec  = ts.tv_sec;
	tv->tv_usec = ts.tv_nsec / 1000;
	return 0;
}


extern "C"
clock_t clock()
{
	Genode::error(__func__, " not implemented, use 'clock_gettime' instead");
	return -1;
}
