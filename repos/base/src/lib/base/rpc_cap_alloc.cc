/*
 * \brief  RPC entrypoint support for allocating RPC object capabilities
 * \author Norman Feske
 * \date   2016-01-19
 */

/*
 * Copyright (C) 2016-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/env.h>
#include <util/retry.h>
#include <base/rpc_server.h>
#include <pd_session/client.h>

using namespace Genode;


Native_capability Rpc_entrypoint::_alloc_rpc_cap(Pd_session &pd,
                                                 Native_capability ep, addr_t)
{
	Untyped_capability new_obj_cap =
		retry<Genode::Pd_session::Out_of_metadata>(
			[&] () { return pd.alloc_rpc_cap(_cap); },
			[&] () { env_deprecated()->parent()->upgrade(Parent::Env::pd(),
			                                             "ram_quota=16K"); });

	return new_obj_cap;
}


void Rpc_entrypoint::_free_rpc_cap(Pd_session &pd, Native_capability cap)
{
	return pd.free_rpc_cap(cap);
}
