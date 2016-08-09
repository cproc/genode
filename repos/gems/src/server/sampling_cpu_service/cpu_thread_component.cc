/*
 * \brief  Cpu_thread_component implementation
 * \author Christian Prochaska
 * \date   2016-01-19
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/snprintf.h>

/* libc includes */
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

/* local includes */
#include "cpu_session_component.h"


static constexpr bool verbose = true;
static constexpr bool verbose_thread_not_started_yet = false;


using namespace Genode;


static inline void sanitize_path_component(char *c)
{
	while (*c) {

		if ((*c == '/')
		 || (*c == '\\')
		 || (*c == ':'))
			*c = '_';

		c++;
	}
}


Sampling_cpu_service::Cpu_thread_component::Cpu_thread_component(
                                  Cpu_session_component &cpu_session_component,
                                  Pd_session_capability pd,
                                  Cpu_session::Name const &name,
                                  Affinity::Location affinity,
                                  Cpu_session::Weight weight,
                                  addr_t utcb,
                                  char const *thread_name,
                                  unsigned int thread_id)
: _cpu_session_component(cpu_session_component),
  _parent_cpu_thread(
      _cpu_session_component.parent_cpu_session().create_thread(pd,
                                                                name,
                                                                affinity,
                                                                weight,
                                                                utcb))
{
	snprintf(_label, sizeof(_label), "%s -> %s",
	         _cpu_session_component.session_label().string(),
	         thread_name);

	chdir("/");

	char dir_name[LABEL_LEN] = "";
	unsigned int dir_name_index = 0;

	char const *c = _cpu_session_component.session_label().string();

	for (;;) {

		if ((c[0] == '\0')
		 || ((c[0] == ' ')
		  && (c[1] == '-')
		  && (c[2] == '>')
		  && (c[3] == ' '))) {

			dir_name[dir_name_index] = '\0';
			sanitize_path_component(dir_name);

			int res = mkdir(dir_name, 0777);
			if ((res == -1) && (errno != EEXIST)) {
				PERR("Could not create (sub)directory: %s", dir_name);
				return;
			}

			chdir(dir_name);

			if (c[0] == '\0')
				break;

			dir_name_index = 0;

			c += 4;

		} else {

			dir_name[dir_name_index] = *c;
			dir_name_index++;
			c++;

		}
	}

	char file_name[LABEL_LEN];
	snprintf(file_name, sizeof(file_name), "%s.%u", thread_name, thread_id);

	sanitize_path_component(file_name);

	fd = open(file_name, O_CREAT | O_WRONLY, 0666);
	if (fd == -1)
		PERR("Could not create/open file: %s/%s", dir_name, file_name);

	_cpu_session_component.thread_ep().manage(this);
}


Sampling_cpu_service::Cpu_thread_component::~Cpu_thread_component()
{
	flush();
	_cpu_session_component.thread_ep().dissolve(this);
}


void Sampling_cpu_service::Cpu_thread_component::take_sample()
{
	if (!_started) {
		if (verbose_thread_not_started_yet)
			PDBG("cannot take sample, thread not started yet");
		return;
	}

	try {

		_parent_cpu_thread.pause();

		Thread_state thread_state = _parent_cpu_thread.state();

		_parent_cpu_thread.resume();

		char const *format_string;

		if (sizeof(addr_t) == 8)
			format_string = "%16lX\n";
		else
			format_string = "%8X\n";

		snprintf(&_sample_buf[_sample_buf_index], SAMPLE_SIZE + 1,
	         	 format_string, thread_state.ip);

#if 0
		if (verbose)
			PDBG("%lx", thread_state.ip);
#endif

		if (_sample_buf_index + SAMPLE_SIZE + 1 >= SAMPLE_BUF_SIZE)
			flush();
		else
			_sample_buf_index += SAMPLE_SIZE;

	} catch (Cpu_thread::State_access_failed) {

		if (verbose)
			PDBG("state access failed");

	}
}


void Sampling_cpu_service::Cpu_thread_component::reset()
{
	_sample_buf_index = 0;
	lseek(fd, 0, SEEK_SET);
}


void Sampling_cpu_service::Cpu_thread_component::flush()
{
	if (_sample_buf_index == 0)
		return;

	if (verbose)
		PDBG("\n%s---", _sample_buf);

	//write(fd, _sample_buf, _sample_buf_index);
	_sample_buf_index = 0;
}


Dataspace_capability
Sampling_cpu_service::Cpu_thread_component::utcb()
{
	return _parent_cpu_thread.utcb();
}


void Sampling_cpu_service::Cpu_thread_component::start(addr_t ip, addr_t sp)
{
	_parent_cpu_thread.start(ip, sp);
	_started = true;
}


void Sampling_cpu_service::Cpu_thread_component::pause()
{
	_parent_cpu_thread.pause();
}


void Sampling_cpu_service::Cpu_thread_component::resume()
{
	_parent_cpu_thread.resume();
}


void Sampling_cpu_service::Cpu_thread_component::single_step(bool enable)
{
	_parent_cpu_thread.single_step(enable);
}


void Sampling_cpu_service::Cpu_thread_component::cancel_blocking()
{
	_parent_cpu_thread.cancel_blocking();
}


Thread_state
Sampling_cpu_service::Cpu_thread_component::state()
{
	return _parent_cpu_thread.state();
}


void Sampling_cpu_service::Cpu_thread_component::state(Thread_state const &state)
{
	_parent_cpu_thread.state(state);
}


void Sampling_cpu_service::Cpu_thread_component::exception_sigh(Signal_context_capability sigh_cap)
{
	_parent_cpu_thread.exception_sigh(sigh_cap);
}


void Sampling_cpu_service::Cpu_thread_component::affinity(Affinity::Location location)
{
	_parent_cpu_thread.affinity(location);
}


unsigned Sampling_cpu_service::Cpu_thread_component::trace_control_index()
{
	return _parent_cpu_thread.trace_control_index();
}


Dataspace_capability
Sampling_cpu_service::Cpu_thread_component::trace_buffer()
{
	return _parent_cpu_thread.trace_buffer();
}


Dataspace_capability
Sampling_cpu_service::Cpu_thread_component::trace_policy()
{
	return _parent_cpu_thread.trace_policy();
}
