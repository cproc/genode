/*
 * \brief  seL4-specific signal-source client interface
 * \author Alexander Boettcher
 * \date   2016-07-09
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__SIGNAL_SOURCE__CLIENT_H_
#define _INCLUDE__SIGNAL_SOURCE__CLIENT_H_

#include <base/rpc_client.h>
#include <signal_source/sel4_signal_source.h>

#include <base/internal/capability_space_sel4.h>

namespace Genode {
	class Signal_source_client;
}

class Genode::Signal_source_client : public Rpc_client<SeL4_signal_source>
{
	private:

		Native_capability _notify;

		/**
		 * Request notification object from signal-source server
		 */
		void _init_notify()
		{
			_notify = call<Rpc_request_notify_obj>();
		}

	public:

		/**
		 * Constructor
		 */
		Signal_source_client(Capability<Signal_source> cap)
		: Rpc_client<SeL4_signal_source>(static_cap_cast<SeL4_signal_source>(cap))
		{ _init_notify(); }


		/*****************************
		 ** Signal source interface **
		 *****************************/

		Signal wait_for_signal() override
		{
			unsigned const dst_sel = Capability_space::ipc_cap_data(_notify).sel.value();

			Signal signal;
			do {

				/* block on semaphore until signal context was submitted */
				seL4_CPtr src = dst_sel;
				seL4_Wait(src, nullptr);

				/*
				 * The following request will return immediately and either
				 * return a valid or a null signal. The latter may happen in
				 * the case a submitted signal context was destroyed (by the
				 * submitter) before we have a chance to raise our request.
				 */
				signal = call<Rpc_wait_for_signal>();

			} while (!signal.imprint());

			return signal;
		}
};

#endif /* _INCLUDE__SIGNAL_SOURCE__CLIENT_H_ */
