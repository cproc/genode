/*
 * \brief  Input session
 * \author Norman Feske
 * \date   2014-04-26
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INPUT_SESSION_H_
#define _INPUT_SESSION_H_

#include <base/rpc_server.h>
#include <input_session/input_session.h>
#include <os/attached_ram_dataspace.h>
#include <os/ring_buffer.h>

namespace Desktop { namespace Input {

	using namespace ::Input;
	class Session_component;
} }


class Desktop::Input::Session_component : public Genode::Rpc_object<Input::Session>
{
	private:

		enum { EVENT_QUEUE_SIZE = 200U };

		Genode::Attached_ram_dataspace _ds { Genode::env()->ram_session(),
		                                     EVENT_QUEUE_SIZE*sizeof(Input::Event) };

		typedef Ring_buffer<Input::Event, EVENT_QUEUE_SIZE> Event_queue;
		
		Event_queue _event_queue;

	public:

		typedef Event_queue::Overflow Overflow;

		/**
		 * Submit input event
		 *
		 * \throw Overflow
		 */
		void submit(Input::Event event) { _event_queue.add(event); }


		/******************************
		 ** Input::Session interface **
		 ******************************/

		Genode::Dataspace_capability dataspace() override { return _ds.cap(); }

		bool is_pending() const override { return !_event_queue.empty(); }

		int flush() override
		{
			Input::Event *dst = _ds.local_addr<Input::Event>();
			
			unsigned cnt = 0;
			for (; cnt < EVENT_QUEUE_SIZE && !_event_queue.empty(); cnt++)
				*dst++ = _event_queue.get();

			return cnt;
		}
};

#endif /* _INPUT_SESSION_H_ */
