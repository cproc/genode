/*
 * \brief  Thread info class for GDB monitor
 * \author Christian Prochaska
 * \date   2011-09-09
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _THREAD_INFO_H_
#define _THREAD_INFO_H_

/* Genode includes */
#include <base/thread.h>

/* libc includes */
#include <unistd.h>

#include "append_list.h"
#include "cpu_session_component.h"

namespace Gdb_monitor { class Thread_info; }

class Gdb_monitor::Thread_info : public Signal_context, public Append_list<Thread_info>::Element
{
	private:

		Cpu_session_component *_cpu_session_component;
		Thread_capability      _thread_cap;
		unsigned long          _lwpid;
		int                    _pipefd[2];
		Lock                   _pipe_lock;
		bool                   _initial_sigtrap_pending = true;

	public:

		Thread_info(Cpu_session_component *cpu_session_component,
					Thread_capability thread_cap,
					unsigned long lwpid)
		: _cpu_session_component(cpu_session_component),
		  _thread_cap(thread_cap),
		  _lwpid(lwpid)
		{
			if (pipe(_pipefd) != 0)
				PERR("could not create pipe");
		}

		~Thread_info()
		{
			close(_pipefd[0]);
			close(_pipefd[1]);
		}

		Thread_capability thread_cap() { return _thread_cap; }
		unsigned long lwpid() { return _lwpid; }

		int signal_pipe_read_fd() { return _pipefd[0]; }

		void deliver_signal(int signo, unsigned long *payload)
		{
			if ((signo == SIGTRAP) && _initial_sigtrap_pending) {

				_initial_sigtrap_pending = false;

				/*
				 * The lock guard prevents an interruption by
				 * 'genode_stop_all_threads()', which could cause
				 * the new thread to be resumed when it should be
				 * stopped.
				 */

				Lock::Guard stop_new_threads_lock_guard(
					_cpu_session_component->stop_new_threads_lock());

				if (!_cpu_session_component->stop_new_threads()) {
					_cpu_session_component->single_step(_thread_cap, false);
					_cpu_session_component->resume(_thread_cap);
				}
			}

			/*
			 * The lock guard ensures that no other signal gets written between
			 * this signal and the payload.
			 */
			Lock::Guard pipe_lock_guard(_pipe_lock);

			write(_pipefd[1], &signo, sizeof(signo));

			if (payload)
				write(_pipefd[1], payload, sizeof(*payload));
		}
};

#endif /* _THREAD_INFO_H_ */
