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

		void entry() override {

			for (;;) {
				_activation_sem.down();
				_ts = Genode::Trace::timestamp();
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

		Genode::uint64_t us()
		{
			/* PINE64: 816 MHz */
			return _ts / 816;
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

		void entry() override {
			for (volatile unsigned long i = 0; i < 10000000000UL; i++) { }
		}
};


void Component::construct(Genode::Env &env)
{
	Genode::Heap alloc(env.ram(), env.rm());
	Timer::Connection timer(env);

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

		/* compare with idle cpu */
		int compare_cpu = 3;
		for (int cpu = 0; cpu < NUM_CPUS; cpu++) {
			Genode::log("cpu ", cpu, ": ",
			            t[cpu]->ts(), " / ", t[cpu]->us(), " us (diff to cpu ", compare_cpu, ": ",
			            (int)(t[cpu]->ts() - t[compare_cpu]->ts()), " / ",
			            (int)(t[cpu]->us() - t[compare_cpu]->us()), " us)");
		}

		Genode::Trace::Timestamp ts1 = Genode::Trace::timestamp();
		timer.msleep(10000);
		Genode::Trace::Timestamp ts2 = Genode::Trace::timestamp();
		Genode::log("freq: ", (ts2 - ts1) / 10, " Hz");
	}
}
