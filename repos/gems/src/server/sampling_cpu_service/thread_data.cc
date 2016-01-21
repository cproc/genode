/*
 * \brief  Thread_data implementation
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


static constexpr bool verbose = false;


Sampling_cpu_service::Thread_data::Thread_data(Cpu_session_component &cpu_session_component,
                                               Thread_capability thread_cap,
                                               char const *thread_name,
                                               unsigned int thread_id)
: _cpu_session_component(cpu_session_component),
  _thread_cap(thread_cap)
{
	snprintf(_label, sizeof(_label), "%s -> %s",
	         _cpu_session_component.session_label().string(),
	         thread_name);

	char dir_name[Cpu_session_component::LABEL_LEN] = "/";
	unsigned int dir_name_index = 1;

	char const *c = _cpu_session_component.session_label().string();

	while (*c != '\0') {

		if ((c[0] == ' ') &&
			(c[1] == '-') &&
			(c[2] == '>') &&
			(c[3] == ' ')) {

			int res = mkdir(dir_name, 0777);
			if ((res == -1) && (errno != EEXIST)) {
				PERR("Could not create (sub)directory: %s", dir_name);
				return;
			}

			chdir(dir_name);

			dir_name[0] = '\0';
			dir_name_index = 0;

			c += 4;

		} else {

			dir_name[dir_name_index++] = *c;
			c++;

		}
	}

	char file_name[LABEL_LEN];
	snprintf(file_name, sizeof(file_name), "%s.%u", thread_name, thread_id);

	fd = open(file_name, O_CREAT | O_WRONLY, 0666);
	if (fd == -1)
		PERR("Could not create/open file: %s/%s", dir_name, file_name);
}


void Sampling_cpu_service::Thread_data::take_sample()
{
	try {

		_cpu_session_component.pause(_thread_cap);

		Thread_state thread_state = _cpu_session_component.state(_thread_cap);

		char const *format_string;

		if (sizeof(addr_t) == 8)
			format_string = "%16lX\n";
		else
			format_string = "%8X\n";

		snprintf(&_sample_buf[_sample_buf_index], SAMPLE_SIZE + 1,
	         	 format_string, thread_state.ip);

		if (_sample_buf_index + SAMPLE_SIZE + 1 >= SAMPLE_BUF_SIZE)
			flush();
		else
			_sample_buf_index += SAMPLE_SIZE;

		_cpu_session_component.resume(_thread_cap);

	} catch (Cpu_session::State_access_failed) {

		if (verbose)
			PDBG("state access failed");

	}
}


void Sampling_cpu_service::Thread_data::reset()
{
	_sample_buf_index = 0;
	lseek(fd, 0, SEEK_SET);
}


void Sampling_cpu_service::Thread_data::flush()
{
	if (verbose)
		PDBG("%s", _sample_buf);

	write(fd, _sample_buf, _sample_buf_index);
	_sample_buf_index = 0;
}
