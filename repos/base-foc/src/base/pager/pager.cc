/*
 * \brief  Fiasco pager framework
 * \author Norman Feske
 * \author Christian Helmuth
 * \author Stefan Kalkowski
 * \date   2006-07-14
 *
 * FIXME Isn't this file generic?
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/pager.h>
#include <base/env.h>
#include <base/printf.h>
#include <base/lock.h>

/* Fiasco.OC includes */
namespace Fiasco {
#include <l4/sys/factory.h>
#include <l4/sys/ipc.h>
}

using namespace Genode;


/**********************
 ** Pager activation **
 **********************/

void Pager_activation_base::entry()
{
	Ipc_pager pager;
	_cap = Native_capability(Thread_base::_thread_cap);
	_cap_valid.unlock();

	bool reply_pending = false;
	while (1) {

		if (reply_pending)
			pager.reply_and_wait_for_fault();
		else
			pager.wait_for_fault();

		reply_pending = false;

		if (!_ep) {
			PWRN("Pager entrypoint not yet defined");
			continue;
		}

		/* lookup referenced object */
		Object_pool<Pager_object>::Guard obj(_ep->lookup_and_lock(pager.badge()));

		/* the pager_object might be destroyed, while we got the message */
		if (!obj) {
			PWRN("No pager object found!");
			continue;
		}

		switch (pager.msg_type()) {

		case Ipc_pager::PAGEFAULT:
		case Ipc_pager::EXCEPTION:
			{
				if (pager.is_exception()) {
					Lock::Guard guard(obj->state.lock);
					pager.copy_regs(&obj->state);
					obj->state.exceptions++;
					obj->state.in_exception = true;
					obj->submit_exception_signal();
					continue;
				}

				/* handle request */
				if (obj->pager(pager)) {
					/* could not resolv - leave thread in pagefault */
					PDBG("Could not resolve pf=%p ip=%p",
					     (void*)pager.fault_addr(), (void*)pager.fault_ip());
				} else {
					pager.set_reply_dst(obj->badge());
					reply_pending = true;
					continue;
				}
				break;
			}

		case Ipc_pager::WAKE_UP:
			{
				/*
				 * We got a request from one of cores region-manager sessions
				 * to answer the pending page fault of a resolved region-manager
				 * client, or to resume a previously paused thread. Hence, we
				 * have to send a reply to the specified thread and answer the
				 * call.
				 */

				/* send reply to the caller */
				pager.set_reply_dst(Native_thread());
				pager.acknowledge_wakeup();

				/* revert exception flag */
				{
					Lock::Guard guard(obj->state.lock);
					obj->state.in_exception = false;
				}

				/* send wake up message to requested thread */
				pager.set_reply_dst(obj->badge());
				pager.acknowledge_wakeup();
				break;
			}

		/*
		 * Handle exceptions that are artificially generated by the pause
		 * function of the CPU service.
		 */
		case Ipc_pager::PAUSE:
			{
				Lock::Guard guard(obj->state.lock);
				pager.copy_regs(&obj->state);

				obj->state.exceptions++;
				obj->state.in_exception = true;

				/*
				 * It might occur that the thread raises an exception,
				 * after it already got resumed by the cpu_session, in
				 * that case we unblock it immediately.
				 */
				if (!obj->state.paused) {
					pager.set_reply_dst(obj->badge());
					reply_pending = true;
				}
				break;
			}

		default:
			PERR("Got unknown message type %x!", pager.msg_type());
		}
	};
}


/**********************
 ** Pager entrypoint **
 **********************/

Pager_entrypoint::Pager_entrypoint(Cap_session           *cap_session,
                                   Pager_activation_base *a)
: _activation(a), _cap_session(cap_session)
{
	_activation->ep(this);
}


void Pager_entrypoint::dissolve(Pager_object *obj)
{
	/* cleanup at cap session */
	_cap_session->free(obj->Object_pool<Pager_object>::Entry::cap());

	remove_locked(obj);
}


Pager_capability Pager_entrypoint::manage(Pager_object *obj)
{
	using namespace Fiasco;

	/* return invalid capability if no activation is present */
	if (!_activation) return Pager_capability();

	Native_capability c = _activation->cap();
	Native_capability cap(_cap_session->alloc(c));

	/* add server object to object pool */
	obj->cap(cap);
	insert(obj);

	/* return capability that uses the object id as badge */
	return reinterpret_cap_cast<Pager_object>(cap);
}
