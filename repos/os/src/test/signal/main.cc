/*
 * \brief  Test for signalling framework
 * \author Norman Feske
 * \author Martin Stein
 * \date   2008-09-06
 */

/*
 * Copyright (C) 2008-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/component.h>
#include <base/heap.h>
#include <base/thread.h>
#include <base/registry.h>
#include <timer_session/connection.h>

using namespace Genode;

extern "C" void wait_for_continue();

/**
 * Stress-test 'wait_and_dispatch_one_io_signal' implementation for entrypoints
 *
 * Let multiple entrypoints directly wait and dispatch signals in a
 * highly nested manner and with multiple stressful senders.
 */
struct Nested_stress_test
{
	struct Sender : Thread
	{
		Signal_transmitter transmitter;

		Sender(Env &env, char const *name, Signal_context_capability cap)
		: Thread(env, name, 8*1024), transmitter(cap) { }

		void entry() override
		{
			//wait_for_continue();
			for (;;) { transmitter.submit(); }
		}
	};

	struct Receiver
	{
		Entrypoint ep;

		String<64> const name;

		bool wait_and_dispatch { true };

		Io_signal_handler<Receiver> handler { ep, *this, &Receiver::handle };

		Receiver(Env &env, char const *name)
		: ep(env, 3 * 1024 * sizeof(long), name, Affinity::Location()),
		  name(name) { }

		void handle()
		{
			if (wait_and_dispatch) {
				wait_and_dispatch = false;
				for (;;) ep.wait_and_dispatch_one_io_signal();
			}
		}
	};

	Env                &env;
	Receiver            receiver_1 { env, "receiver-1" };
	Receiver            receiver_2 { env, "receiver-2" };
	Receiver            receiver_3 { env, "receiver-3" };
	Sender              sender_1   { env, "sender-1", receiver_1.handler };
	Sender              sender_2   { env, "sender-2", receiver_2.handler };
	Sender              sender_3   { env, "sender-3", receiver_3.handler };

	Nested_stress_test(Env &env)
	: env(env)
	{
		//wait_for_continue();
		/* let senders start sending signals like crazy */
		sender_1.start();
		sender_2.start();
		sender_3.start();
	}

};

struct Main
{
	Env                  &env;

	Main(Env &env) : env(env)
	{
		log("--- Signalling test ---");
		static Nested_stress_test test(env);
	}
};

void Component::construct(Genode::Env &env) { static Main main(env); }
