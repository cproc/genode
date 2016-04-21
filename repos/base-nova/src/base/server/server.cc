/*
 * \brief  NOVA-specific support code for the server-side RPC API
 * \author Norman Feske
 * \author Sebastian Sumpf
 * \author Alexander Boettcher
 * \date   2010-01-13
 */

/*
 * Copyright (C) 2010-2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/printf.h>
#include <base/rpc_server.h>
#include <base/env.h>

/* base-internal includes */
#include <base/internal/stack.h>
#include <base/internal/ipc.h>

/* NOVA includes */
#include <nova/syscalls.h>
#include <nova/util.h>
#include <nova/native_thread.h>

using namespace Genode;


/***********************
 ** Server entrypoint **
 ***********************/

Untyped_capability Rpc_entrypoint::_manage(Rpc_object_base *obj)
{
	using namespace Nova;

	Untyped_capability ec_cap;

	/* _ec_sel is invalid until thread gets started */
	if (native_thread().ec_sel != Native_thread::INVALID_INDEX)
		ec_cap = Native_capability(native_thread().ec_sel);
	else
		ec_cap = _thread_cap;

	Untyped_capability obj_cap = _alloc_rpc_cap(_pd_session, ec_cap,
	                                            (addr_t)&_activation_entry);
	if (!obj_cap.valid())
		return obj_cap;

	/* add server object to object pool */
	obj->cap(obj_cap);
	insert(obj);

	/* return object capability managed by entrypoint thread */
	return obj_cap;
}


void Rpc_entrypoint::_dissolve(Rpc_object_base *obj)
{
	/* de-announce object from cap_session */
	_free_rpc_cap(_pd_session, obj->cap());

	/* avoid any incoming IPC */
	Nova::revoke(Nova::Obj_crd(obj->cap().local_name(), 0), true);

	/* make sure nobody is able to find this object */
	remove(obj);

	/* effectively invalidate the capability used before */
	obj->cap(Untyped_capability());

	/*
	 * The activation may execute a blocking operation in a dispatch function.
	 * Before resolving the corresponding object, we need to ensure that it is
	 * no longer used by an activation. Therefore, we to need cancel an
	 * eventually blocking operation and let the activation leave the context
	 * of the object.
	 */
	using namespace Nova;

	Utcb *utcb = reinterpret_cast<Utcb *>(Thread_base::myself()->utcb());
	/* don't call ourself */
	if (utcb == reinterpret_cast<Utcb *>(this->utcb()))
		return;

	/*
	 * Required outside of core. E.g. launchpad needs it to forcefully kill
	 * a client which blocks on a session opening request where the service
	 * is not up yet.
	 */
	cancel_blocking();

	/* activate entrypoint now - otherwise cleanup call will block forever */
	_delay_start.unlock();

	/* make a IPC to ensure that cap() identifier is not used anymore */
	utcb->msg[0] = 0xdead;
	utcb->set_msg_word(1);
	if (uint8_t res = call(_cap.local_name()))
		PERR("%8p - could not clean up entry point of thread 0x%p - res %u",
		     utcb, this->utcb(), res);
}


static void reply(Nova::Utcb &utcb, Rpc_exception_code exc, Msgbuf_base &snd_msg)
{
	copy_msgbuf_to_utcb(utcb, snd_msg, exc.value);

	Nova::reply(Thread_base::myself()->stack_top());
}


void Rpc_entrypoint::_activation_entry()
{
	/* retrieve portal id from eax/rdi */
#ifdef __x86_64__
	addr_t id_pt; asm volatile ("" : "=D" (id_pt));
#else
	addr_t id_pt; asm volatile ("" : "=a" (id_pt));
#endif

	Rpc_entrypoint &ep   = *static_cast<Rpc_entrypoint *>(Thread_base::myself());
	Nova::Utcb     &utcb = *(Nova::Utcb *)Thread_base::myself()->utcb();

	Receive_window &rcv_window = ep.native_thread().rcv_window;
	rcv_window.post_ipc(utcb);

	/* handle ill-formed message */
	if (utcb.msg_words() < 2) {
		ep._rcv_buf.word(0) = ~0UL; /* invalid opcode */
	} else {
		copy_utcb_to_msgbuf(utcb, rcv_window, ep._rcv_buf);
	}

	Ipc_unmarshaller unmarshaller(ep._rcv_buf);

	Rpc_opcode opcode(0);
	unmarshaller.extract(opcode);

	/* default return value */
	Rpc_exception_code exc = Rpc_exception_code(Rpc_exception_code::INVALID_OBJECT);

	/* in case of a portal cleanup call we are done here - just reply */
	if (ep._cap.local_name() == id_pt) {
		if (!rcv_window.prepare_rcv_window(utcb))
			PWRN("out of capability selectors for handling server requests");

		ep._rcv_buf.reset();
		reply(utcb, exc, ep._snd_buf);
	}
	{
		/* potentially delay start */
		Lock::Guard lock_guard(ep._delay_start);
	}

	/* atomically lookup and lock referenced object */
	auto lambda = [&] (Rpc_object_base *obj) {
		if (!obj) {
			PERR("could not look up server object, return from call id_pt=%lx",
			     id_pt);
			return;
		}

		/*
		 * Inhibit removal of capabilities sent as results of client requests.
		 * This prevents the recursive revocation of NOVA portal caps and,
		 * therefore, permits clients to use result capabilities after server
		 * code dropped all references.
		 */
		for (unsigned i = 0; i < ep._snd_buf.used_caps(); ++i)
			ep._snd_buf.cap(i).keep_if_last_reference();

		/* dispatch request */
		ep._snd_buf.reset();
		try { exc = obj->dispatch(opcode, unmarshaller, ep._snd_buf); }
		catch (Blocking_canceled) { }
	};
	ep.apply(id_pt, lambda);

	if (!rcv_window.prepare_rcv_window(*(Nova::Utcb *)ep.utcb()))
		PWRN("out of capability selectors for handling server requests");

	ep._rcv_buf.reset();
	reply(utcb, exc, ep._snd_buf);
}


void Rpc_entrypoint::entry()
{
	/*
	 * Thread entry is not used for activations on NOVA
	 */
}


void Rpc_entrypoint::_block_until_cap_valid() { }


void Rpc_entrypoint::activate()
{
	/*
	 * In contrast to a normal thread, a server activation is created at
	 * construction time. However, it executes no code because processing
	 * time is always provided by the caller of the server activation. To
	 * delay the processing of requests until the 'activate' function is
	 * called, we grab the '_delay_start' lock on construction and release it
	 * here.
	 */
	_delay_start.unlock();
}


Rpc_entrypoint::Rpc_entrypoint(Pd_session *pd_session, size_t stack_size,
                               const char  *name, bool start_on_construction,
                               Affinity::Location location)
:
	Thread_base(Cpu_session::DEFAULT_WEIGHT, name, stack_size),
	_delay_start(Lock::LOCKED),
	_pd_session(*pd_session)
{
	/* when not running in core set the affinity via cpu session */
	if (native_thread().ec_sel == Native_thread::INVALID_INDEX) {

		/* place new thread on the specified CPU */
		if (location.valid())
			_cpu_session->affinity(_thread_cap, location);

		/* magic value evaluated by thread_nova.cc to start a local thread */
		native_thread().ec_sel = Native_thread::INVALID_INDEX - 1;
		native_thread().initial_ip = (addr_t)&_activation_entry;
	} else {
		/* tell affinity CPU in 'core' via stack */
		reinterpret_cast<Affinity::Location *>(stack_base())[0] = location;
	}

	/* required to create a 'local' EC */
	Thread_base::start();

	/* create cleanup portal */
	_cap = _alloc_rpc_cap(_pd_session, Native_capability(native_thread().ec_sel),
	                      (addr_t)_activation_entry);
	if (!_cap.valid())
		throw Cpu_session::Thread_creation_failed();

	Receive_window &rcv_window = Thread_base::native_thread().rcv_window;

	/* prepare portal receive window of new thread */
	if (!rcv_window.prepare_rcv_window(*(Nova::Utcb *)&_stack->utcb()))
		throw Cpu_session::Thread_creation_failed();

	if (start_on_construction)
		activate();
}


Rpc_entrypoint::~Rpc_entrypoint()
{
	typedef Object_pool<Rpc_object_base> Pool;

	Pool::remove_all([&] (Rpc_object_base *obj) {
		PWRN("Object pool not empty in %s", __func__);
		_dissolve(obj);
	});

	if (!_cap.valid())
		return;

	_free_rpc_cap(_pd_session, _cap);
}
