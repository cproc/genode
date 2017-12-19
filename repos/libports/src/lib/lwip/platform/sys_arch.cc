/*
 * \brief  lwIP platform support
 * \author Stefan Kalkowski
 * \author Emery Hemingway
 * \date   2016-12-01
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <timer_session/connection.h>
#include <util/reconstructible.h>
#include <base/sleep.h>

#include <lwip/genode_init.h>

namespace Lwip {

/* LwIP includes */
#include <lwip/timeouts.h>
#include <lwip/init.h>
#include <lwip/sys.h>
#include <arch/cc.h>

	static Genode::Allocator *_heap;

	/**
	 * XXX: can this be replaced with an alarm?
	 */
	struct Sys_timer
	{
		void check_timeouts(Genode::Duration) {
			Lwip::sys_check_timeouts(); }

		Genode::Timeout_scheduler &timer;

		Timer::Periodic_timeout<Sys_timer> timeout {
			timer, *this, &Sys_timer::check_timeouts,
			Genode::Microseconds{250*1000} };

		Sys_timer(Genode::Timeout_scheduler &timer) : timer(timer) { }
	};

	static Genode::Constructible<Sys_timer> _sys_timer;

	void genode_init(Genode::Allocator &heap, Genode::Timeout_scheduler &timer)
	{
		LWIP_ASSERT("heap does not track allocation sizes", heap.need_size_for_free());

		_heap = &heap;
		_sys_timer.construct(timer);
		lwip_init();
	}
}


extern "C" {

	void lwip_platform_assert(char const* msg, char const *file, int line)
	{
		Genode::error("Assertion \"", msg, "\" ", file, ":", line);
		Genode::sleep_forever();
	}

	void genode_free(void *ptr)
	{
		Lwip::_heap->free(ptr, 0);
	}

	void *genode_malloc(unsigned long size)
	{
		void *ptr = nullptr;
		return Lwip::_heap->alloc(size, &ptr) ? ptr : 0;
	}

	void *genode_calloc(unsigned long number, unsigned long size)
	{
		void *ptr = nullptr;
		size *= number;
		if (Lwip::_heap->alloc(size, &ptr)) {
			Genode::memset(ptr, 0x00, size);
			return ptr;
		}
		return nullptr;
	}

	Lwip::u32_t sys_now(void) {
		return Lwip::_sys_timer->timer.curr_time().trunc_to_plain_ms().value; }

	void genode_memcpy(void * dst, const void *src, unsigned long size) {
		Genode::memcpy(dst, src, size); }

}
