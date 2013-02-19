/*
 * \brief  Expanding RM session client
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

#ifndef _INCLUDE__BASE__EXPANDING_RM_SESSION_CLIENT_H_
#define _INCLUDE__BASE__EXPANDING_RM_SESSION_CLIENT_H_

#include <rm_session/client.h>

namespace Genode {

	class Expanding_rm_session_client : public Rm_session_client
	{
		Rm_session_capability _cap;

		public:

			Expanding_rm_session_client(Rm_session_capability cap)
			: Rm_session_client(cap), _cap(cap) { }

			Local_addr attach(Dataspace_capability ds,
							  size_t size = 0, off_t offset = 0,
							  bool use_local_addr = false,
							  Local_addr local_addr = (addr_t)0,
							  bool executable = false) {

				bool try_again = false;
				do {
					try {
						return Rm_session_client::attach(ds, size, offset,
														 use_local_addr,
														 local_addr,
														 executable);

					} catch (Rm_session::Out_of_metadata) {

						/* give up if the error occurred a second time */
						if (try_again)
							break;

						PINF("upgrading quota donation for Env::RM session");
						env()->parent()->upgrade(_cap, "ram_quota=8K");
						try_again = true;
					}
				} while (try_again);

				return (addr_t)0;
			}

			Pager_capability add_client(Thread_capability thread)
			{
				bool try_again = false;
				do {
					try {
						return Rm_session_client::add_client(thread);
					} catch (Rm_session::Out_of_metadata) {

						/* give up if the error occurred a second time */
						if (try_again)
							break;

						PINF("upgrading quota donation for Env::RM session");
						env()->parent()->upgrade(_cap, "ram_quota=8K");
						try_again = true;
					}
				} while (try_again);

				return Pager_capability();
			}

			void remove_client(Pager_capability pager)
			{
				Rm_session_client::remove_client(pager);
			}
	};

}

#endif /* _INCLUDE__BASE__EXPANDING_RM_SESSION_CLIENT_H_ */
