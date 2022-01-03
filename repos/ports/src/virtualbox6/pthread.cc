/*
 * \brief  VirtualBox libc runtime: pthread adaptions
 * \author Christian Helmuth
 * \date   2021-03-03
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is distributed under the terms of the GNU General Public License
 * version 2.
 */

/* libc includes */
#include <errno.h>
#include <sched.h>
#include <pthread.h>

/* libc internal */
#include <internal/thread_create.h> /* Libc::pthread_create() */
#include <internal/call_func.h>     /* call_func() */

/* VirtualBox includes */
#include <VBox/vmm/uvm.h>
#include <internal/thread.h> /* RTTHREADINT etc. */

/* Genode includes */
#include <base/attached_rom_dataspace.h>
#include <base/env.h>
#include <base/entrypoint.h>
#include <base/heap.h>
#include <base/registry.h>
#include <cpu_session/connection.h>
#include <util/string.h>
#include <util/reconstructible.h>

/* local includes */
#include <init.h>
#include <pthread_emt.h>
#include <sup.h>
#include <stub_macros.h>

static bool const debug = true; /* required by stub_macros.h */


using namespace Genode;


extern "C" int sched_yield()
{
	static unsigned long counter = 0;

	if (++counter % 100'000 == 0)
		warning(__func__, " called ", counter, " times");

	return 0;
}

extern "C" int sched_get_priority_max(int policy)                                                   { return 0; }
extern "C" int sched_get_priority_min(int policy)                                                   { return 0; }
extern "C" int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param) { return 0; }
extern "C" int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param)      { return 0; }


namespace Pthread {

	struct Entrypoint;
	struct Factory;

} /* namespace Pthread */


class Pthread::Entrypoint : public Pthread::Emt
{
	private:

		/* members initialized by constructing thread */

		Sup::Cpu_index const _cpu;
		size_t         const _stack_size; /* stack size for EMT mode */

		Genode::Entrypoint _ep;
		Blockade           _construction_finalized { };

		void *(*_emt_start_routine) (void *);
		void   *_emt_arg;

		enum class Mode { VCPU, EMT } _mode { Mode::VCPU };

		jmp_buf _vcpu_jmp_buf;
		jmp_buf _emt_jmp_buf;

		/* members finally initialized by the entrypoint itself */

		void      *_emt_stack   { nullptr };
		pthread_t  _emt_pthread { };

		void _finalize_construction()
		{
			Genode::Thread &myself = *Genode::Thread::myself();

			_emt_stack = myself.alloc_secondary_stack(myself.name().string(),
			                                          _stack_size);

			Libc::pthread_create_from_thread(&_emt_pthread, myself, _emt_stack);

			_construction_finalized.wakeup();

			/* switch to EMT mode and call pthread start_routine */
			if (setjmp(_vcpu_jmp_buf) == 0) {
				_mode = Mode::EMT;
				call_func(_emt_stack, (void *)_emt_start_routine, _emt_arg);
			}
		}

		Genode::Signal_handler<Entrypoint> _finalize_construction_sigh {
			_ep, *this, &Entrypoint::_finalize_construction };

	public:

		Entrypoint(Env &env, Sup::Cpu_index cpu, size_t stack_size,
		           char const *name, Affinity::Location location,
		           void *(*start_routine) (void *), void *arg)
		:
			_cpu(cpu), _stack_size(stack_size),
			_ep(env, 64*1024, name, location),
			_emt_start_routine(start_routine), _emt_arg(arg)
		{
			Signal_transmitter(_finalize_construction_sigh).submit();

			_construction_finalized.block();
		}

		/* registered object must have virtual destructor */
		virtual ~Entrypoint() { }

		Sup::Cpu_index cpu() const { return _cpu; }

		pthread_t pthread() const { return _emt_pthread; }

		/* Pthread::Emt interface */

		void switch_to_emt() override
		{
			Assert(_mode == Mode::VCPU);

			if (setjmp(_vcpu_jmp_buf) == 0) {
				_mode = Mode::EMT;
				longjmp(_emt_jmp_buf, 1);
			}
		}

		void switch_to_vcpu() override
		{
			Assert(pthread_self() == _emt_pthread);
			Assert(_mode == Mode::EMT);

			if (setjmp(_emt_jmp_buf) == 0) {
				_mode = Mode::VCPU;
				longjmp(_vcpu_jmp_buf, 1);
			}
		}

		Genode::Entrypoint & genode_ep() override { return _ep; }
};


class Pthread::Factory
{
	private:

		Env &_env;

		Registry<Registered<Pthread::Entrypoint>> _entrypoints;

		Affinity::Space const _affinity_space { _env.cpu().affinity_space() };

		Heap _heap { _env.ram(), _env.rm() };

		bool _use_priorities()
		{
			Attached_rom_dataspace const platform(_env, "platform_info");
			Xml_node const kernel = platform.xml().sub_node("kernel");
			return kernel.attribute_value("name", String<16>("unknown")) == "nova";
		}

	public:

		Factory(Env &env) : _env(env) { }

		Entrypoint & create(Sup::Cpu_index cpu, size_t stack_size, char const *name,
		                    void *(*start_routine) (void *), void *arg)
		{

			Affinity::Location const location =
				_affinity_space.location_of_index(cpu.value);

			return *new Registered<Entrypoint>(_entrypoints, _env, cpu,
			                                   stack_size, name,
			                                   location, start_routine, arg);
		}

		struct Emt_for_cpu_not_found : Exception { };

		Emt & emt_for_cpu(Sup::Cpu_index cpu)
		{
			Entrypoint *found = nullptr;

			_entrypoints.for_each([&] (Entrypoint &ep) {
				if (ep.cpu().value == cpu.value)
					found = &ep;
			});

			if (!found)
				throw Emt_for_cpu_not_found();

			return *found;
		}

		long prio_class(RTTHREADTYPE const type)
		{
			unsigned const VIRTUAL_GENODE_VBOX_LEVELS = 16;
			static_assert (RTTHREADTYPE_END < VIRTUAL_GENODE_VBOX_LEVELS,
			               "prio levels exceeds VIRTUAL_GENODE_VBOX_LEVELS");

			/* evaluate once */
			static bool const priorities = _use_priorities();

			if (!priorities)
				return Cpu_session::DEFAULT_PRIORITY;

			return (VIRTUAL_GENODE_VBOX_LEVELS - type) *
			       Cpu_session::PRIORITY_LIMIT / VIRTUAL_GENODE_VBOX_LEVELS;
		}

		Cpu_connection *cpu_connection(RTTHREADTYPE type)
		{
			using namespace Genode;

			static Cpu_connection * con[RTTHREADTYPE_END - 1];
			static Mutex mutex { };

			Assert(type && type < RTTHREADTYPE_END);

			Mutex::Guard guard(mutex);

			if (con[type - 1])
				return con[type - 1];

			char * data = new (_heap) char[16];

			Genode::snprintf(data, 16, "vbox %u", type);

			con[type - 1] = new (_heap) Cpu_connection(_env, data,
			                                           prio_class(type));

			return con[type - 1];
		}
};


static Pthread::Factory *factory;


Pthread::Emt & Pthread::emt_for_cpu(Sup::Cpu_index cpu)
{
	return factory->emt_for_cpu(cpu);
}


void Pthread::init(Env &env)
{
	factory = new Pthread::Factory(env);
}


long prio_class(RTTHREADTYPE const type)
{
	return factory->prio_class(type);
}


static int create_emt_thread(pthread_t *thread, const pthread_attr_t *attr,
                             void *(*start_routine) (void *),
                             PRTTHREADINT rtthread)
{
	PUVMCPU pUVCpu = (PUVMCPU)rtthread->pvUser;

	Sup::Cpu_index const cpu { pUVCpu->idCpu };

	size_t stack_size = 0;

	/* try to fetch configured stack size form attribute */
	pthread_attr_getstacksize(attr, &stack_size);

	Assert(stack_size);

	Pthread::Entrypoint &ep =
		factory->create(cpu, stack_size, rtthread->szName,
		                start_routine, rtthread);

	*thread = ep.pthread();

	return 0;
}


static int create_thread(pthread_t *thread, const pthread_attr_t *attr,
                         void *(*start_routine) (void *), void *arg)
{
	PRTTHREADINT rtthread = reinterpret_cast<PRTTHREADINT>(arg);

	Assert(rtthread);

	size_t const utcb_size = 4096;

	size_t stack_size = Thread::stack_virtual_size() -
	                    utcb_size - 2 * (1UL << 12);

	if (rtthread->cbStack < stack_size)
		stack_size = rtthread->cbStack;

	/*
	 * Make sure timers run at the same priority as component threads, otherwise
	 * no timer progress can be made. See 'rtTimeNanoTSInternalRef' (timesupref.h)
	 * and 'rtTimerLRThread' (timerlr-generic.cpp)
	 */
	bool const rtthread_timer = rtthread->enmType == RTTHREADTYPE_TIMER;

	if (rtthread_timer) {
		return Libc::pthread_create_from_session(thread, start_routine, arg,
		                                         stack_size, rtthread->szName, nullptr,
		                                         Affinity::Location());

	} else {
		using namespace Genode;
		Cpu_connection *cpu = factory->cpu_connection(rtthread->enmType);
		return cpu->retry_with_upgrade(Ram_quota{8*1024}, Cap_quota{2},
		                               [&] ()
		{
			return Libc::pthread_create_from_session(thread, start_routine, arg,
			                                         stack_size, rtthread->szName, cpu,
			                                         Affinity::Location());
		});
	}

}


extern "C" int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                              void *(*start_routine) (void *), void *arg)
{
	PRTTHREADINT rtthread = reinterpret_cast<PRTTHREADINT>(arg);

	/*
	 * Emulation threads (EMT) represent the guest CPU, so we implement them in
	 * dedicated entrypoints that also handle vCPU events in combination with
	 * user-level threading (i.e., setjmp/longjmp).
	 */
	if (rtthread->enmType == RTTHREADTYPE_EMULATION)
		return create_emt_thread(thread, attr, start_routine, rtthread);

	/* retry thread creation once after CPU session upgrade */
	for (unsigned i = 0; i < 2; i++) {
		using namespace Genode;

		try {
			return create_thread(thread, attr, start_routine, arg); }
		catch (Out_of_ram) {
			log("Upgrading memory for creation of "
			    "thread '", Cstring(rtthread->szName), "'");
			factory->cpu_connection(rtthread->enmType)->upgrade_ram(4096);
		} catch (Out_of_caps) {
			error("out of caps ...");
		}
		catch (...) { break; }
	}

	error("could not create vbox pthread - halt");
	sleep_forever();
	return EAGAIN;
}
