/*
 * \brief  base-hw specific part of RPC framework
 * \author Stefan Kalkowski
 * \date   2015-03-05
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/rpc_server.h>
#include <base/sleep.h>
#include <base/env.h>
#include <util/retry.h>

/* base-internal includes */
#include <base/internal/ipc_server.h>

using namespace Genode;


/***********************
 ** Server entrypoint **
 ***********************/

Untyped_capability Rpc_entrypoint::_manage(Rpc_object_base *obj)
{
	Untyped_capability new_obj_cap = _alloc_rpc_cap(_pd_session, _cap);

	/* add server object to object pool */
	obj->cap(new_obj_cap);
	insert(obj);

	/* return capability that uses the object id as badge */
	return new_obj_cap;
}


void Rpc_entrypoint::entry()
{
	Native_connection_state cs;
	Ipc_server srv(cs, _snd_buf, _rcv_buf);
	_ipc_server = &srv;
	_cap = srv;
	_cap_valid.unlock();

	/*
	 * Now, the capability of the server activation is initialized
	 * an can be passed around. However, the processing of capability
	 * invocations should not happen until activation-using server
	 * is completely initialized. Thus, we wait until the activation
	 * gets explicitly unblocked by calling 'Rpc_entrypoint::activate()'.
	 */
	_delay_start.lock();

	while (!_exit_handler.exit) {

		Rpc_opcode opcode(0);

		srv.reply_wait();
		srv.extract(opcode);

		/* set default return value */
		srv.ret(Rpc_exception_code(Rpc_exception_code::INVALID_OBJECT));

		/* atomically lookup and lock referenced object */
		apply(srv.badge(), [&] (Rpc_object_base *curr_obj) {
			if (!curr_obj) return;

			/* dispatch request */
			try { srv.ret(curr_obj->dispatch(opcode, srv, srv)); }
			catch (Blocking_canceled) { }
		});
	}

	/* answer exit call, thereby wake up '~Rpc_entrypoint' */
	srv.reply();

	/* defer the destruction of 'Ipc_server' until '~Rpc_entrypoint' is ready */
	_delay_exit.lock();
}
