/*
 * \brief  Trace-event definitions
 * \author Norman Feske
 * \date   2013-08-09
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BASE__TRACE__EVENTS_H_
#define _INCLUDE__BASE__TRACE__EVENTS_H_

#include <base/thread.h>
#include <base/trace/policy.h>

namespace Genode { namespace Trace {
	struct Rpc_call;
	struct Rpc_returned;
	struct Rpc_dispatch;
	struct Rpc_reply;
	struct Signal_submit;
	struct Signal_received;
} }


struct Genode::Trace::Rpc_call
{
	char         const *rpc_name;
	Msg_buf_base const &msg;

	Rpc_call(char const *rpc_name, Msg_buf_base const &msg)
	: rpc_name(rpc_name), msg(msg)
	{
		Thread_base::trace(this);
	}

	size_t generate(Policy &policy, char *dst) const {
		return policy.rpc_call(dst, rpc_name, msg); }
};


struct Genode::Trace::Rpc_returned
{
	char         const *rpc_name;
	Msg_buf_base const &msg;

	Rpc_returned(char const *rpc_name, Msg_buf_base const &msg)
	: rpc_name(rpc_name), msg(msg)
	{
		Thread_base::trace(this);
	}

	size_t generate(Policy &policy, char *dst) const {
		return policy.rpc_returned(dst, rpc_name, msg); }
};


struct Genode::Trace::Rpc_dispatch
{
	char            const *rpc_name;
	Rpc_object_base const &obj;
	Msg_buf_base    const &msg;

	Rpc_dispatch(char const *rpc_name, Rpc_object_base const &obj,
	             Msg_buf_base const &msg)
	:
		rpc_name(rpc_name), obj(obj), msg(msg)
	{
		Thread_base::trace(this);
	}

	size_t generate(Policy &policy, char *dst) const {
		return policy.rpc_dispatch(dst, rpc_name, obj, msg); }
};


struct Genode::Trace::Rpc_reply
{
	char            const *rpc_name;
	Rpc_object_base const &obj;
	Msg_buf_base    const &msg;

	Rpc_reply(char const *rpc_name, Rpc_object_base const &obj,
	          Msg_buf_base const &msg)
	:
		rpc_name(rpc_name), obj(obj), msg(msg)
	{
		Thread_base::trace(this);
	}

	size_t generate(Policy &policy, char *dst) const {
		return policy.rpc_reply(dst, rpc_name, obj, msg); }
};


struct Genode::Trace::Signal_submit
{
	unsigned const num;

	Signal_submit(unsigned const num) : num(num)
	{ Thread_base::trace(this); }

	size_t generate(Policy &policy, char *dst) const {
		return policy.signal_submit(dst, num); }
};


struct Genode::Trace::Signal_received
{
	Signal const &signal;;

	Signal_received(Signal const &signal) : signal(signal)
	{ Thread_base::trace(this); }

	size_t generate(Policy &policy, char *dst) const {
		return policy.signal_received(dst, signal); }
};


#endif /* _INCLUDE__BASE__TRACE__EVENTS_H_ */
