/*
 * \brief  Low-level test for TRACE service
 * \author Norman Feske
 * \date   2013-08-12
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <trace_session/connection.h>
#include <timer_session/connection.h>


static char const *state_name(Genode::Trace::Subject_info::State state)
{
	switch (state) {
	case Genode::Trace::Subject_info::INVALID:  return "INVALID";
	case Genode::Trace::Subject_info::UNTRACED: return "UNTRACED";
	case Genode::Trace::Subject_info::TRACED:   return "TRACED";
	case Genode::Trace::Subject_info::FOREIGN:  return "FOREIGN";
	case Genode::Trace::Subject_info::ERROR:    return "ERROR";
	case Genode::Trace::Subject_info::DEAD:     return "DEAD";
	}
	return "undefined";
}


int main(int argc, char **argv)
{
	using namespace Genode;

	printf("--- test-trace started ---\n");

	static Genode::Trace::Connection trace(1024*1024, 64*1024, 0);

	static Timer::Connection timer;

	for (;;) {

		timer.msleep(3000);

		Trace::Subject_id subjects[32];
		size_t num_subjects = trace.subjects(subjects, 32);

		printf("%zd tracing subjects present\n", num_subjects);

		for (size_t i = 0; i < num_subjects; i++) {

			Trace::Subject_info info = trace.subject_info(subjects[i]);
			printf("ID:%d label:\"%s\" name:\"%s\" state:%s policy:%d\n",
			       subjects[i].id,
			       info.session_label().string(),
			       info.thread_name().string(),
			       state_name(info.state()),
			       info.policy_id().id);
		}
	}

	printf("--- test-trace finished ---\n");
	return 0;
}
