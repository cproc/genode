/*
 * \brief  Connection to PD service
 * \author Norman Feske
 * \date   2008-08-22
 */

/*
 * Copyright (C) 2008-2012 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__PD_SESSION__CONNECTION_H_
#define _INCLUDE__PD_SESSION__CONNECTION_H_

#include <pd_session/client.h>
#include <base/connection.h>

namespace Genode {

	struct Pd_connection : Connection<Pd_session>, Pd_session_client
	{
		/**
		 * Constructor
		 *
		 * \param label  session label
		 * \param root   chroot path (only on Linux)
		 */
		Pd_connection(char const *label = "", char const *root = "")
		:
			Connection<Pd_session>(session("ram_quota=4K, label=\"%s\"%s%s%s",
			                               label,
			                               (root && root[0]) ? ", root=\"" : "",
			                               (root && root[0]) ? root        : "",
			                               (root && root[0]) ? "\""        : "")),
			Pd_session_client(cap())
		{ }
	};
}

#endif /* _INCLUDE__PD_SESSION__CONNECTION_H_ */
