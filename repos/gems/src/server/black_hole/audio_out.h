/*
 * \brief  'Audio_out' part of black hole component
 * \author Christian Prochaska
 * \date   2021-07-07
 *
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _AUDIO_OUT_H_
#define _AUDIO_OUT_H_

/* Genode includes */
#include <audio_out_session/rpc_object.h>
#include <timer_session/connection.h>

namespace Audio_out
{
	class Session_component;
	class Root;
}


/**************************************
 ** Audio_out session implementation **
 **************************************/

class Audio_out::Session_component : public Audio_out::Session_rpc_object
{
	private:

		Timer::One_shot_timeout<Audio_out::Session_component> _timeout;

		Genode::Duration _delay { Genode::Milliseconds { 10 } };

		void _handle_timeout(Genode::Duration)
		{
			Genode::log(this, ": _handle_timeout(): queued: ", stream()->queued(), ", empty: ", stream()->empty(), ", full: ", stream()->full());	

			if (stream()->empty())
				return;

			bool const full = stream()->full();

			Packet *p = stream()->get(stream()->pos());
			p->invalidate();
			p->mark_as_played();
			stream()->increment_position();
			progress_submit();

			if (full) alloc_submit();

			if (!stream()->empty()) 
				_timeout.schedule(_delay.trunc_to_plain_us());

			Genode::warning("_handle_timeout() finished: queued: ", stream()->queued(), ", empty: ", stream()->empty(), ", full: ", stream()->full());	
		}

		Genode::Constructible<Genode::Signal_handler<Audio_out::Session_component>> _data_available_handler { };

		Genode::Signal_context_capability _data_available_cap(Genode::Env &env)
		{
			_data_available_handler.construct(env.ep(),
			                                  *this,
			                                  &Audio_out::Session_component::_handle_data_available);
			return *_data_available_handler;
		}

		void _handle_data_available()
		{
			Genode::log("_handle_data_available(): queued: ", stream()->queued(), ", empty: ", stream()->empty(), ", full: ", stream()->full());	
			_timeout.schedule(_delay.trunc_to_plain_us());
		}

	public:

		Session_component(Genode::Env &env, Timer::Connection &timer)
		: Session_rpc_object(env, _data_available_cap(env)),
		  _timeout(timer, *this, &Audio_out::Session_component::_handle_timeout)
		{ }

		~Session_component()
		{
Genode::log(this, ": ~Session_component()");
			if (Session_rpc_object::active()) stop();
		}

		void start() override
		{
			Session_rpc_object::start();
			stream()->pos(0);
		}

		void stop() override
		{
			Session_rpc_object::stop();
		}
};


namespace Audio_out {
	typedef Genode::Root_component<Session_component, Genode::Multiple_clients> Root_component;
}


class Audio_out::Root : public Audio_out::Root_component
{
	private:

		Genode::Env &_env;
		Timer::Connection _timer;

		Session_component *_create_session(const char *args) override
		{
			using namespace Genode;
Genode::log("_create_session(): args: ", Genode::Cstring(args));

			size_t ram_quota =
				Arg_string::find_arg(args, "ram_quota").ulong_value(0);

			size_t session_size = align_addr(sizeof(Session_component), 12);
Genode::log("session_size: ", session_size);
Genode::log("sizeof(Stream): ", sizeof(Stream));
			if ((ram_quota < session_size) ||
			    (sizeof(Stream) > ram_quota - session_size)) {
				Genode::error("insufficient 'ram_quota', got ", ram_quota, ", "
				              "need ", sizeof(Stream) + session_size);
				throw Insufficient_ram_quota();
			}

			Session_component *session = new (md_alloc())
				Session_component(_env, _timer);

			return session;

		}

		void _destroy_session(Session_component *session) override
		{
Genode::log("_destroy_session()");
			Genode::destroy(md_alloc(), session);
Genode::log("_destroy_session() finished");
		}

	public:

		Root(Genode::Env        &env,
		     Genode::Allocator  &md_alloc)
		: Root_component(env.ep(), md_alloc),
		  _env(env), _timer(env) { }
};

#endif /* _AUDIO_OUT_H_ */
