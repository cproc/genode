/*
 * \brief  Synchronize trace timestamps across arm64 CPUs
 * \author Christian Prochaska
 * \date   2022-10-17
 *
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <base/component.h>
#include <base/heap.h>
#include <base/log.h>
#include <trace/timestamp.h>
#include <timer_session/connection.h>

class Timestamp_thread : public Genode::Thread
{
	private:

		int _cpu_index;
		Genode::Trace::Timestamp _ts { 0 };
		Genode::Semaphore _activation_sem { 0 };
		Genode::Semaphore _completion_sem { 0 };
		Timer::Connection &_timer;

		Genode::uint64_t _elapsed_us { 0 };
		Genode::uint64_t _curr_time_us { 0 };

	public:

		Timestamp_thread(Genode::Env &env, int cpu_index, Timer::Connection &timer)
		: Genode::Thread(env,
		                 "timestamp_thread",
		                 8192,
		                 env.cpu().affinity_space().location_of_index(cpu_index),
		                 Weight(),
		                 env.cpu()),
		  _cpu_index(cpu_index),
		  _timer(timer) { }

		void entry() override
		{
			for (;;) {
				_activation_sem.down();
				_ts = Genode::Trace::timestamp();

				_curr_time_us = _timer.curr_time().trunc_to_plain_us().value;
				_elapsed_us = _timer.elapsed_us();

#if 0
				/* synchronize cycle counter with timer */
				Genode::Trace::Timestamp new_ts =
					_timer.elapsed_us() * 816;
				asm("msr pmccntr_el0, %0" :: "r" (new_ts));
#endif
				_completion_sem.up();
			}
		}

		Genode::uint64_t ts()
		{
			return _ts;
		}

		Genode::uint64_t ts_us()
		{
			/* PINE64: 816 MHz */
			return _ts / 816;
		}

		Genode::uint64_t elapsed_us()
		{
			return _elapsed_us;
		}

		Genode::uint64_t curr_time_us()
		{
			return _curr_time_us;
		}

		void activate()
		{
			_activation_sem.up();
		}

		void wait_for_completion()
		{
			_completion_sem.down();
		}
};


class Burner_thread : public Genode::Thread
{
	public:

		Burner_thread(Genode::Env &env, int cpu_index)
		: Genode::Thread(env,
		                 "burner_thread",
		                 8192,
		                 env.cpu().affinity_space().location_of_index(cpu_index),
		                 Weight(),
		                 env.cpu()) { }

		void entry() override
		{
			for (volatile unsigned long i = 0; i < 10000000000UL; i++) { }
		}
};


void Component::construct(Genode::Env &env)
{
	Genode::Heap alloc(env.ram(), env.rm());
	Timer::Connection timer(env);
	Timer::Connection sleep_timer(env);

	enum { NUM_CPUS = 4 };

#if 0
	Burner_thread burner_thread(env, 1);
	burner_thread.start();
#endif

	Timestamp_thread *t[NUM_CPUS];

	for (int cpu = 0; cpu < NUM_CPUS; cpu++) {
		t[cpu] = new (alloc) Timestamp_thread(env, cpu, timer);
		t[cpu]->start();
	}

	for (;;) {

		for (int cpu = 0; cpu < NUM_CPUS; cpu++)
			t[cpu]->activate();

		for (int cpu = 0; cpu < NUM_CPUS; cpu++)
			t[cpu]->wait_for_completion();

#if 1
		/* compare with idle cpu */
		int compare_cpu = 3;
		for (int cpu = 0; cpu < NUM_CPUS; cpu++) {
			Genode::log("cpu ", cpu, ": ts:         ",
			            t[cpu]->ts(), " (diff to cpu ", compare_cpu, ": ",
			            (int)(t[cpu]->ts() - t[compare_cpu]->ts()), ")");
			Genode::log("cpu ", cpu, ": ts time:    ",
			            t[cpu]->ts_us(), " us (diff to cpu ", compare_cpu, ": ",
			            (int)(t[cpu]->ts_us() - t[compare_cpu]->ts_us()), " us)");
			Genode::log("cpu ", cpu, ": curr_time:  ",
			            t[cpu]->curr_time_us(), " us (diff to cpu ", compare_cpu, ": ",
			            (int)(t[cpu]->curr_time_us() - t[compare_cpu]->curr_time_us()), " us)");
			Genode::log("cpu ", cpu, ": elapsed_us: ",
			            t[cpu]->elapsed_us(), " us (diff to cpu ", compare_cpu, ": ",
			            (int)(t[cpu]->elapsed_us() - t[compare_cpu]->elapsed_us()), " us)");
		}
#endif

		Genode::Trace::Timestamp ts1 = Genode::Trace::timestamp();
		sleep_timer.msleep(10000);
		Genode::Trace::Timestamp ts2 = Genode::Trace::timestamp();
		Genode::log("freq: ", (ts2 - ts1) / 10, " Hz");
	}
}
