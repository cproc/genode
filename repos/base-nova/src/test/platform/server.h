/*
 * \brief  Dummy server interface
 * \author Alexander Boettcher
 */

/*
 * Copyright (C) 2013-2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#pragma once

/* Genode includes */
#include <base/printf.h>
#include <base/thread.h>
#include <base/env.h>
#include <base/sleep.h>

#include <cap_session/connection.h>
#include <base/rpc_server.h>

namespace Test { struct Session; struct Client; struct Component; }

/**
 * Test session interface definition
 */
struct Test::Session : Genode::Session
{
	static const char *service_name() { return "TEST"; }

	GENODE_RPC(Rpc_cap_void, bool, cap_void,
	           Genode::Native_capability);
	GENODE_RPC(Rpc_void_cap, Genode::Native_capability,
	           void_cap);
	GENODE_RPC(Rpc_leak_utcb_address, Genode::addr_t, leak_utcb_address);

	GENODE_RPC_INTERFACE(Rpc_cap_void, Rpc_void_cap, Rpc_leak_utcb_address);
};

struct Test::Client : Genode::Rpc_client<Session>
{
	Client(Capability<Session> cap) : Rpc_client<Session>(cap) { }

	bool cap_void(Genode::Native_capability cap) {
		return call<Rpc_cap_void>(cap); }

	Genode::Native_capability void_cap() {
		return call<Rpc_void_cap>(); }

	Genode::addr_t leak_utcb_address() {
		return call<Rpc_leak_utcb_address>(); }
};

struct Test::Component : Genode::Rpc_object<Test::Session, Test::Component>
{
	/* Test to transfer a object capability during send */
	bool cap_void(Genode::Native_capability);
	/* Test to transfer a object capability during reply */
	Genode::Native_capability void_cap();
	/* Leak utcb address of entrypoint to manipulate utcb receive window */
	Genode::addr_t leak_utcb_address();
};

namespace Test { typedef Genode::Capability<Test::Session> Capability; }

/**
 * Session implementation
 */
bool Test::Component::cap_void(Genode::Native_capability got_cap) {
	if (!got_cap.valid())
		return false;

	/* be evil and keep this cap by manually incrementing the ref count */
	Genode::Cap_index idx(Genode::cap_map()->find(got_cap.local_name()));
	idx.inc();

	return true;
}

Genode::Native_capability Test::Component::void_cap() {
	Genode::Native_capability send_cap = cap();

	/* XXX this code does does no longer work since the removal of 'solely_map' */
#if 0
	/* be evil and switch translation off - client ever uses a new selector */
	send_cap.solely_map();
#endif

	return send_cap;
}

Genode::addr_t Test::Component::leak_utcb_address() {
	return reinterpret_cast<Genode::addr_t>(Genode::Thread_base::myself()->utcb()); }
