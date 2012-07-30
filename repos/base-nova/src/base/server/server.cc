/*
 * \brief  NOVA-specific support code for the server-side RPC API
 * \author Norman Feske
 * \author Sebastian Sumpf
 * \date   2010-01-13
 */

/*
 * Copyright (C) 2010-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/printf.h>
#include <base/rpc_server.h>
#include <base/env.h>
#include <base/cap_sel_alloc.h>

/* NOVA includes */
#include <nova/syscalls.h>
#include <base/nova_util.h>
#include <nova_cpu_session/connection.h>


using namespace Genode;

/***********************
 ** Server entrypoint **
 ***********************/

Untyped_capability Rpc_entrypoint::_manage(Rpc_object_base *obj)
{
	using namespace Nova;

	Untyped_capability ep_cap, new_obj_cap;                                 

	/* _ec_sel is invalid until thread gets started */
	if (tid().ec_sel != ~0UL)
		ec_cap = Native_capability(tid().ec_sel);
	else
		ec_cap = _thread_cap;

	ep_cap = _cap_session->alloc(ec_cap, (addr_t)_activation_entry);

	/* add server object to object pool */
	obj->cap(new_obj_cap);
	insert(obj);

	/* return capability that uses the local object id as badge */
	return new_obj_cap;
}


void Rpc_entrypoint::_dissolve(Rpc_object_base *obj)
{
	/* Avoid any incoming IPC early, keep local selector */
	Nova::revoke(Nova::Obj_crd(obj->cap().local_name(), 0), false);

	/* make sure nobody is able to find this object */
	remove(obj);

	/*
	 * The activation may execute a blocking operation
	 * in a dispatch function. Before resolving the
	 * corresponding object, we need to ensure that
	 * it is no longer used by an activation. Therefore,
	 * we to need cancel an eventually blocking operation
	 * and let the activation leave the context of the
	 * object.
	 */
	_leave_server_object(obj);

	/* wait until nobody is inside dispatch */
	obj->lock();

	/* De-announce object from cap_session */
	_cap_session->free(obj->cap());

	/* Revoke local selector finally */
	Nova::revoke(Nova::Obj_crd(obj->cap().local_name(), 0), true);
	/* free cap selector ??? XXX */
	//cap_selector_allocator()->free(obj->cap().local_name(), 0);

}

void Rpc_entrypoint::_activation_entry()
{
#ifdef __x86_64__
	addr_t id_pt; asm volatile ("" : "=D" (id_pt));
#else
	addr_t id_pt; asm volatile ("" : "=a" (id_pt));
#endif
	/* retrieve portal id from eax */
	Rpc_entrypoint *ep = static_cast<Rpc_entrypoint *>(Thread_base::myself());

	Ipc_server srv(&ep->_snd_buf, &ep->_rcv_buf);
	ep->_rcv_buf.post_ipc(reinterpret_cast<Nova::Utcb *>(ep->utcb()));

	/* destination of next reply */
	srv.dst(Native_capability(id_pt));

	int opcode = 0;

	srv >> IPC_WAIT >> opcode;

	/* set default return value */
	srv.ret(ERR_INVALID_OBJECT);

	/* atomically lookup and lock referenced object */
	{
		Lock::Guard lock_guard(ep->_curr_obj_lock);

		ep->_curr_obj = ep->obj_by_id(id_pt);
		if (!ep->_curr_obj || !id_pt) {
/*
			ep->_curr_obj = ep->obj_by_id(srv.badge());
			if (!ep->_curr_obj) {
*/
				PERR("could not look up server object, "
				     " return from call badge=%lx id_pt=%lx",
				     srv.badge(), id_pt);
				ep->_curr_obj_lock.unlock();
				srv << IPC_REPLY;
//			}
		}

		ep->_curr_obj->lock();
	}

	/* dispatch request */
	try { srv.ret(ep->_curr_obj->dispatch(opcode, srv, srv)); }
	catch (Blocking_canceled) { }

	ep->_curr_obj->unlock();
	ep->_curr_obj = 0;

	ep->_rcv_buf.rcv_prepare_pt_sel_window((Nova::Utcb *)ep->utcb());
	srv << IPC_REPLY;
}


void Rpc_entrypoint::entry()
{
	/*
	 * Thread entry is not used for activations on NOVA
	 */
}


void Rpc_entrypoint::_leave_server_object(Rpc_object_base *obj)
{
	{
		Lock::Guard lock_guard(_curr_obj_lock);

		if (obj == _curr_obj)
			cancel_blocking();
	}

	Nova::Utcb *utcb = reinterpret_cast<Nova::Utcb *>(
	                   Thread_base::myself()->utcb());
	/* don't call ourself */
	if (utcb != reinterpret_cast<Nova::Utcb *>(&_context->utcb)) {
		utcb->set_msg_word(0);
		if (uint8_t res = Nova::call(obj->cap().local_name()))
			PERR("could not clean up entry point - %u", res);
	}
}


void Rpc_entrypoint::_block_until_cap_valid() { }


void Rpc_entrypoint::activate()
{
	/*
	 * In contrast to a normal thread, a server activation is created at
	 * construction time. However, it executes no code because processing
	 * time is always provided by the caller of the server activation. To
	 * delay the processing of requests until the 'activate' function is
	 * called, we grab the '_curr_obj_lock' on construction and release it
	 * here.
	 */
	_curr_obj_lock.unlock();
}


Rpc_entrypoint::Rpc_entrypoint(Cap_session *cap_session, size_t stack_size,
                               const char  *name, bool start_on_construction)
:
	Thread_base(name, stack_size),
	_curr_obj(0),
	_curr_obj_lock(Lock::LOCKED),
	_cap_session(cap_session)
{
	/**
	 * Create thread if we aren't running in core.
	 *
	 * For core this code can't be performed since the sessions aren't
	 * setup in the early bootstrap phase of core. In core the thread
	 * is created 'manually'.
	 */
	if (_tid.ec_sel == ~0UL) {
		/* create new pager object and assign it to the new thread */
		Pager_capability pager_cap =
			env()->rm_session()->add_client(_thread_cap);
		env()->cpu_session()->set_pager(_thread_cap, pager_cap);

		addr_t thread_sp = (addr_t)&_context->stack[-4];
		Genode::Nova_cpu_connection cpu;
		cpu.start_exc_base_vcpu(_thread_cap, 0, thread_sp,
		                        _tid.exc_pt_sel);

		request_event_portal(pager_cap, _tid.exc_pt_sel,
		                     Nova::PT_SEL_STARTUP);
		request_event_portal(pager_cap, _tid.exc_pt_sel,
		                     Nova::PT_SEL_PAGE_FAULT);

		/**
		 * Request native thread cap, _thread_cap only a token.
		 * The native thread cap is required to attach new rpc objects
		 * (to create portals bound to the ec)
		 */
		Native_capability ec_cap = cpu.native_cap(_thread_cap);
		_tid.ec_sel = ec_cap.local_name();
	}

	_rcv_buf.rcv_prepare_pt_sel_window((Nova::Utcb *)&_context->utcb);

	if (start_on_construction)
		activate();
}
