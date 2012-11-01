/*
 * \brief  Client-side ROM session interface
 * \author Norman Feske
 * \date   2006-07-06
 */

/*
 * Copyright (C) 2006-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__ROM_SESSION__CLIENT_H_
#define _INCLUDE__ROM_SESSION__CLIENT_H_

#include <rom_session/capability.h>
#include <base/rpc_client.h>

namespace Genode {

	struct Rom_session_client : Rpc_client<Rom_session>
	{
		explicit Rom_session_client(Rom_session_capability session)
		: Rpc_client<Rom_session>(session) { }

		Rom_dataspace_capability dataspace() {
			PDBG("calling call<Rpc_dataspace>()");
			Rom_dataspace_capability ds_cap = call<Rpc_dataspace>();
			PDBG("call<Rpc_dataspace>() returned, ds_cap.local_name() = %d", ds_cap.local_name());
			return ds_cap;
		 }

		void sigh(Signal_context_capability cap) { call<Rpc_sigh>(cap); }
	};
}

#endif /* _INCLUDE__ROM_SESSION__CLIENT_H_ */
