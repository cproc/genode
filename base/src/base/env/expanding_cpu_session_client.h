/*
 * \brief  Expanding CPU session client
 * \author Norman Feske
 * \author Christian Helmuth
 * \date   2006-07-28
 *
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BASE__EXPANDING_CPU_SESSION_CLIENT_H_
#define _INCLUDE__BASE__EXPANDING_CPU_SESSION_CLIENT_H_

#include <cpu_session/client.h>

namespace Genode {

	template <typename CPU_SESSION_CLIENT, typename CPU_SESSION_CAPABILITY>
	class Expanding_cpu_session_client : public CPU_SESSION_CLIENT
	{
		CPU_SESSION_CAPABILITY _cap;

		public:

			Expanding_cpu_session_client(CPU_SESSION_CAPABILITY cap)
			: CPU_SESSION_CLIENT(cap), _cap(cap) { }

			Thread_capability create_thread(Cpu_session::Name const &name, addr_t utcb) {
				bool try_again = false;
				do {
					try {
						return CPU_SESSION_CLIENT::create_thread(name, utcb);
					} catch (Cpu_session::Out_of_metadata) {

						/* give up if the error occurred a second time */
						if (try_again)
							break;

						PINF("upgrading quota donation for Env::CPU session");
						env()->parent()->upgrade(_cap, "ram_quota=8K");
						try_again = true;
					}
				} while (try_again);

				return Thread_capability();
			}
	};

}

#endif /* _INCLUDE__BASE__EXPANDING_CPU_SESSION_CLIENT_H_ */
