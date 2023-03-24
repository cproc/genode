/*
 * \brief  pthread_create implementation
 * \author Christian Prochaska
 * \author Christian Helmuth
 * \date   2012-03-12
 *
 * Purpose of a single file for pthread_create is that other application may
 * easily replace this implementation with another one.
 */

/*
 * Copyright (C) 2012-2020 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/mutex.h>
#include <util/string.h>

/* libc includes */
#include <libc/allocator.h>
#include <errno.h>

/* libc-internal includes */
#include <internal/init.h>
#include <internal/thread_create.h>
#include <internal/pthread.h>


using namespace Genode;


struct Placement_policy
{
	struct Placement_by_id : Interface
	{
		unsigned pthread_id;
		unsigned cpu;

		Placement_by_id (unsigned id, unsigned cpu)
		: pthread_id(id), cpu(cpu) { }
	};

	struct Placement_by_name : Interface
	{
		Thread::Name pthread_name;
		unsigned cpu;

		Placement_by_name (Thread::Name name, unsigned cpu)
		: pthread_name(name), cpu(cpu) { }
	};

	Registry<Registered<Placement_by_id> >   _id_policies { };
	Registry<Registered<Placement_by_name> > _name_policies { };

	enum class Policy { ALL, SINGLE, MANUAL };

	Policy _policy { Policy::ALL };

	/*
	 * Default CPU used with manual placement if configured in the policy with
	 * a <thread> node without id or name, otherwise Policy::ALL is applied.
	 */
	unsigned _manual_default_cpu { 0 };
	bool     _manual_default_cpu_set { false };

	void policy(String<32> const &policy_name)
	{
		if (policy_name == "single-cpu")
			_policy = Policy::SINGLE;
		if (policy_name == "manual")
			_policy = Policy::MANUAL;
		if (policy_name == "all-cpus")
			_policy = Policy::ALL;
	}

	unsigned placement(unsigned const pthread_id,
	                   Thread::Name const pthread_name) const
	{
		switch (_policy) {
		case Policy::SINGLE:
			return 0U;
		case Policy::MANUAL: {
			unsigned cpu   = 0U;
			bool     found = false;

			_id_policies.for_each([&](auto const &policy) {
				if (policy.pthread_id == pthread_id) {
					cpu = policy.cpu;
					found = true;
				}
			});

			if (found)
				return cpu;

			_name_policies.for_each([&](auto const &policy) {
				if (policy.pthread_name == pthread_name) {
					cpu = policy.cpu;
					found = true;
				}
			});

			/*
			 * If no entry is found and no default CPU is set, Policy::ALL is
			 * applied.
			 */
			return found ? cpu :
			       (_manual_default_cpu_set ? _manual_default_cpu : pthread_id);
		}
		case Policy::ALL:
		default:
			return pthread_id;
		}
	}

	void add_placement(Genode::Allocator &alloc, unsigned pthread_id,
	                   unsigned cpu)
	{
		new (alloc) Registered<Placement_by_id> (_id_policies, pthread_id,
		                                         cpu);
	}

	void add_placement(Genode::Allocator &alloc, Thread::Name pthread_name,
	                   unsigned cpu)
	{
		new (alloc) Registered<Placement_by_name> (_name_policies,
		                                           pthread_name, cpu);
	}

	void set_manual_default_cpu(unsigned cpu) {
		_manual_default_cpu = cpu;
		_manual_default_cpu_set = true;
	}
};


static Env              *_env_ptr     { nullptr };
static Cpu_session      *_cpu_session { nullptr };
static bool              _verbose     { false };


Placement_policy &placement_policy()
{
	static Placement_policy policy { };
	return policy;
}


void Libc::init_pthread_support(Genode::Env &env,
                                Cpu_session &cpu_session,
                                Xml_node const &node,
                                Genode::Allocator &alloc)
{
	_env_ptr = &env;

	_cpu_session = &cpu_session;

	_verbose = node.attribute_value("verbose", false);

	String<32> const policy_name = node.attribute_value("placement",
	                                                    String<32>("all-cpus"));
	placement_policy().policy(policy_name);

	node.for_each_sub_node("thread", [&](Xml_node &policy) {

		if (policy.has_attribute("cpu")) {

			unsigned const cpu = policy.attribute_value("cpu", 0U);

			if (policy.has_attribute("id")) {
				unsigned const id = policy.attribute_value("id", 0U);

				if (_verbose)
					log("pthread with id ", id, " -> cpu ", cpu);

				placement_policy().add_placement(alloc, id, cpu);
			} else if (policy.has_attribute("name")) {
				Thread::Name const name = policy.attribute_value("name",
				                                                 Thread::Name());
				if (_verbose)
					log("pthread with name ", name, " -> cpu ", cpu);

				placement_policy().add_placement(alloc, name, cpu);
			} else {
				if (_verbose)
					log("pthread default -> cpu ", cpu);

				placement_policy().set_manual_default_cpu(cpu);
			}
		}
	});
}


static unsigned pthread_id()
{
	static Mutex mutex;

	static unsigned id = 0;

	Mutex::Guard guard(mutex);

	return id++;
}


int Libc::pthread_create_from_session(pthread_t *thread,
                                      void *(*start_routine) (void *),
                                      void *arg,
                                      size_t stack_size,
                                      char const *name,
                                      Cpu_session *cpu,
                                      Affinity::Location location)
{
	Libc::Allocator alloc { };

	Cpu_local_storage *cls =
		cpu_local_storage_registry().first() ?
		cpu_local_storage_registry().first()->find_by_location(location) :
		nullptr;

	if (!cls) {
		cls = new (alloc) Cpu_local_storage(*_env_ptr, location);
		cpu_local_storage_registry().insert(cls);
	}

	pthread_t thread_obj = new (alloc)
	                       pthread(start_routine, arg,
	                               stack_size, name, cpu, location, *cls);
	if (!thread_obj)
		return EAGAIN;

	*thread = thread_obj;

	thread_obj->start();

	return 0;
}


int Libc::pthread_create_from_thread(pthread_t *thread, Thread &t, void *stack_address)
{
	Libc::Allocator alloc { };

	Cpu_local_storage *cls =
		cpu_local_storage_registry().first() ?
		cpu_local_storage_registry().first()->find_by_location(t.affinity()) :
		nullptr;

	if (!cls) {
		cls = new (alloc) Cpu_local_storage(*_env_ptr, t.affinity());
		cpu_local_storage_registry().insert(cls);
	}

	pthread_t thread_obj = new (alloc) pthread(t, stack_address, *cls);

	if (!thread_obj)
		return EAGAIN;

	*thread = thread_obj;
	return 0;
}


int Libc::pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                         void *(*start_routine) (void *), void *arg,
                         char const *name)
{
	if (!_cpu_session || !start_routine || !thread)
		return EINVAL;

	size_t const stack_size = (attr && *attr && (*attr)->stack_size)
	                        ? (*attr)->stack_size
	                        : Libc::Component::stack_size();

	Thread::Name const pthread_name {
		(attr && *attr) ? (*attr)->name.string() : "pthread" };

	unsigned const id { pthread_id() };
	unsigned const cpu = placement_policy().placement(id, pthread_name);
	Thread::Name const pthread_name_with_id { pthread_name, ".", id };
	Affinity::Space space { _cpu_session->affinity_space() };
	Affinity::Location location { space.location_of_index(cpu) };

	if (_verbose)
		log("create ", pthread_name_with_id, " -> cpu ", cpu);

	int result =
		Libc::pthread_create_from_session(thread, start_routine, arg,
	                                      stack_size,
	                                      name ? : pthread_name_with_id.string(),
	                                      _cpu_session, location);

	if ((result == 0) && attr && *attr &&
	    ((*attr)->detach_state == PTHREAD_CREATE_DETACHED))
		pthread_detach(*thread);

	return result;
}


extern "C" int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                              void *(*start_routine) (void *), void *arg)
{
	return Libc::pthread_create(thread, attr, start_routine, arg, nullptr);
}


extern "C" typeof(pthread_create) _pthread_create __attribute__((alias("pthread_create")));
