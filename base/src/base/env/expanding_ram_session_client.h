/*
 * \brief  Expanding RAM session client
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

#ifndef _INCLUDE__BASE__EXPANDING_RAM_SESSION_CLIENT_H_
#define _INCLUDE__BASE__EXPANDING_RAM_SESSION_CLIENT_H_

#include <ram_session/client.h>

namespace Genode {

	class Expanding_ram_session_client : public Ram_session_client
	{
		Ram_session_capability _cap;

		public:

			Expanding_ram_session_client(Ram_session_capability cap)
			: Ram_session_client(cap), _cap(cap) { }

			Ram_dataspace_capability alloc(size_t size, bool cached) {
				bool try_again = false;
				do {
					try {
						return Ram_session_client::alloc(size, cached);

					} catch (Ram_session::Out_of_metadata) {

						/* give up if the error occurred a second time */
						if (try_again)
							break;

						PINF("upgrading quota donation for Env::RAM session");
						env()->parent()->upgrade(_cap, "ram_quota=8K");
						try_again = true;
					}
				} while (try_again);

				return Ram_dataspace_capability();
			}
	};

}

#endif /* _INCLUDE__BASE__EXPANDING_RAM_SESSION_CLIENT_H_ */
