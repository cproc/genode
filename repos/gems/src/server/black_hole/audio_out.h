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
			Genode::log("_handle_data_available(): empty: ", stream()->empty(), ", full: ", stream()->full());	
			bool const full = stream()->full();
			while (!stream()->empty()) {
				Genode::log("_handle_data_available(): while: pos: ", stream()->pos(), ", empty: ", stream()->empty(), ", full: ", stream()->full());
				unsigned pos = stream()->pos();
				Genode::log("_handle_data_available(): while check 1");
				Packet *p = stream()->get(pos);
				Genode::log("_handle_data_available(): while check 2");
				p->invalidate();
				Genode::log("_handle_data_available(): while check 3");
				p->mark_as_played();
				Genode::log("_handle_data_available(): while check 4");
				stream()->increment_position();
				Genode::log("_handle_data_available(): while check 5");
				progress_submit();
				alloc_submit();
				Genode::log("_handle_data_available(): while end");
			}
			if (full) alloc_submit();
			Genode::log("_handle_data_available() finished: pos: ", stream()->pos(), ", full: ", stream()->full());	
		}

	public:

		Session_component(Genode::Env &env)
		: Session_rpc_object(env, _data_available_cap(env)) { }

		~Session_component()
		{
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
				Session_component(_env);

			return session;

		}

		void _destroy_session(Session_component *session) override
		{
			Genode::destroy(md_alloc(), session);
		}

	public:

		Root(Genode::Env        &env,
		     Genode::Allocator  &md_alloc)
		: Root_component(env.ep(), md_alloc), _env(env) { }
};

#endif /* _AUDIO_OUT_H_ */
