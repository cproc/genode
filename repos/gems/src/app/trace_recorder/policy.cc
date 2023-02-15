/*
 * \brief  Installs and maintains a tracing policy
 * \author Johannes Schlatow
 * \date   2022-05-10
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* local includes */
#include <policy.h>

using namespace Genode;
using namespace Genode::Trace;

Trace_recorder::Policy::Policy(Env                &env,
                               Trace::Connection  &trace,
                               Policy::Name const &name,
                               Policies           &policies)
:
	Policies::Element(policies, name),
	_env(env), _trace(trace), _rom(env, name.string())
{
		Dataspace_capability dst_ds = _trace.policy(_id);

		enum {
			MAX_SIZE = 0, NO_OFFSET = 0, ANY_LOCAL_ADDR = false,
			EXECUTABLE = true
		};

		Policy_module *dst = _env.rm().attach(dst_ds, MAX_SIZE, NO_OFFSET,
		                                      ANY_LOCAL_ADDR, nullptr,
		                                      EXECUTABLE);

		void *src = _env.rm().attach(_ds);
		memcpy(dst, src, _size);

#if 0
		/*
		 * Locally relocate the function pointers and call the
		 * enable functions.
		 */

		Policy_module policy_module(*dst);

		for (unsigned i = 0; i < sizeof(Trace::Policy_module) / sizeof(void *); i++) {
			((addr_t *)&policy_module)[i] += (addr_t)(dst);
		}

		policy_module.enable_rpc_call();
#endif

		dst->rpc_call_enabled = true;

		_env.rm().detach(dst);
		_env.rm().detach(src);
}
