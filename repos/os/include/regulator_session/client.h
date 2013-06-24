/*
 * \brief  Client-side regulator session interface
 * \author Stefan Kalkowski
 * \date   2013-06-13
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__REGULATOR_SESSION__CLIENT_H_
#define _INCLUDE__REGULATOR_SESSION__CLIENT_H_

#include <base/rpc_client.h>
#include <regulator_session/capability.h>

namespace Regulator {

	class Session_client : public Genode::Rpc_client<Session>
	{
		public:

			/**
			 * Constructor
			 *
			 * \param session  session capability
			 */
			Session_client(Session_capability session)
			: Genode::Rpc_client<Session>(session) { }


			/*********************************
			 ** Regulator session interface **
			 *********************************/

			void level(unsigned long level) { call<Rpc_set_level>(level);  }
			unsigned long level()           { return call<Rpc_level>();    }
			void state(bool enable)         { call<Rpc_set_state>(enable); }
			bool state()                    { return call<Rpc_state>();    }
	};
}

#endif /* _INCLUDE__REGULATOR_SESSION__CLIENT_H_ */
