/*
 * \brief  Input service test program
 * \author Christian Helmuth
 * \date   2010-06-15
 */

/*
 * Copyright (C) 2010-2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/component.h>
#include <base/env.h>
#include <base/log.h>
#include <base/printf.h>
#include <input_session/connection.h>
#include <input/event.h>


using namespace Genode;

static char const * ev_type(Input::Event::Type type)
{
	switch (type) {
	case Input::Event::INVALID: return "INVALID";
	case Input::Event::MOTION:  return "MOTION ";
	case Input::Event::PRESS:   return "PRESS  ";
	case Input::Event::RELEASE: return "RELEASE";
	case Input::Event::WHEEL:   return "WHEEL  ";
	case Input::Event::FOCUS:   return "FOCUS  ";
	case Input::Event::LEAVE:   return "LEAVE  ";
	case Input::Event::TOUCH:   return "TOUCH  ";
	}

	return "";
}


static char const * key_name(Input::Event *ev)
{
	if (ev->type() == Input::Event::MOTION)
		return "";

	return Input::key_name(static_cast<Input::Keycode>(ev->code()));
}


class Test_environment
{
	private:

		Genode::Env &_env;

		Input::Connection _input;

		Input::Event *_ev_buf = { _env.rm().attach(_input.dataspace()) };

		Genode::Signal_handler<Test_environment> _input_sigh;

		unsigned int event_count = 0;

		void _handle_input();

	public:

		Test_environment(Genode::Env &env)
		: _env(env),
		  _input_sigh(env.ep(), *this, &Test_environment::_handle_input)
		{
			Genode::log("--- Input test is up ---");

			Genode::log("input buffer at ", _ev_buf);

			_input.sigh(_input_sigh);
		}
};


void Test_environment::_handle_input()
{
	/*
	 * Handle input events
	 */
	int key_cnt = 0;

	for (int i = 0, num_ev = _input.flush(); i < num_ev; ++i) {

		event_count++;

		Input::Event *ev = &_ev_buf[i];

		if (ev->type() == Input::Event::PRESS)   key_cnt++;
		if (ev->type() == Input::Event::RELEASE) key_cnt--;

		/* log event */
		Genode::log("Input event #", event_count,
		            "\ttype=", ev_type(ev->type()),
		            "\tcode=", ev->code(),
		            "\trx=", ev->rx(),
		            "\try=", ev->ry(),
		            "\tax=", ev->ax(),
		            "\tay=", ev->ay(),
		            "\tkey_cnt=", key_cnt,
		            "\t", key_name(ev));
	}
}

void Component::construct(Genode::Env &env)
{
	using namespace Genode;

	Genode::log("--- Test input ---\n");
	static Test_environment te(env);
}


Genode::size_t Component::stack_size() {
	return 4*1024*sizeof(long); }
