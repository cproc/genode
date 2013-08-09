/*
 * \brief  Event tracing infrastructure
 * \author Norman Feske
 * \date   2013-08-09
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BASE__TRACE__POLICY_H_
#define _INCLUDE__BASE__TRACE__POLICY_H_

#include <base/stdint.h>

namespace Genode {

	class Msg_buf_base;
	class Signal;
	class Rpc_object_base;

	namespace Trace { class Policy; }
}


/**
 * Header of tracing policy
 */
struct Genode::Trace::Policy
{
	size_t (*max_event_size)  ();
	size_t (*rpc_call)        (char *, char const *, Msg_buf_base const &);
	size_t (*rpc_returned)    (char *, char const *, Msg_buf_base const &);
	size_t (*rpc_dispatch)    (char *, char const *, Rpc_object_base const &, Msg_buf_base const &);
	size_t (*rpc_reply)       (char *, char const *, Rpc_object_base const &, Msg_buf_base const &);
	size_t (*signal_submit)   (char *, unsigned const);
	size_t (*signal_received) (char *, Signal const &);
};

#endif /* _INCLUDE__BASE__TRACE__POLICY_H_ */
