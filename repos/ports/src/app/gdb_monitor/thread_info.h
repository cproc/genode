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

namespace Gdb_monitor
{
	class Thread_event_signal_context;
	class Exception_signal_context;
	class Sigstop_signal_context;
}

class Gdb_monitor::Thread_event_signal_context : public Signal_context
{
	private:

		Thread_info *_thread_info;

	public:

		Thread_event_signal_context(Thread_info *thread_info)
		: _thread_info(thread_info) { }

		Thread_info *thread_info() { return _thread_info; }
};

struct Gdb_monitor::Exception_signal_context : Thread_event_signal_context
{
	Exception_signal_context(Thread_info *thread_info)
	: Thread_event_signal_context(thread_info) { }
};

struct Gdb_monitor::Sigstop_signal_context : Thread_event_signal_context
{
	Sigstop_signal_context(Thread_info *thread_info)
	: Thread_event_signal_context(thread_info) { }
};

class Gdb_monitor::Thread_info : public Append_list<Thread_info>::Element
{
	private:

		Cpu_session_component    *_cpu_session_component;
		Thread_capability         _thread_cap;
		unsigned long             _lwpid;
		
		Exception_signal_context  _exception_signal_context;
		Signal_context_capability _exception_signal_context_cap;
		Sigstop_signal_context    _sigstop_signal_context;
		Signal_context_capability _sigstop_signal_context_cap;
		
		int                       _pipefd[2];
		Lock                      _pipe_lock;
		bool                      _initial_sigtrap_pending = true;

		Signal_context_capability _manage(Signal_context *_signal_context)
		{
			return _cpu_session_component->exception_signal_receiver()->
			           manage(_signal_context);
		}

		void _dissolve(Signal_context *_signal_context)
		{
			_cpu_session_component->exception_signal_receiver()->
			    dissolve(_signal_context);
		}

	public:


		Thread_info(Cpu_session_component *cpu_session_component,
					Thread_capability thread_cap,
					unsigned long lwpid)
		: _cpu_session_component(cpu_session_component),
		  _thread_cap(thread_cap),
		  _lwpid(lwpid),
		  _exception_signal_context(this),
		  _exception_signal_context_cap(_manage(&_exception_signal_context)),
		  _sigstop_signal_context(this),
		  _sigstop_signal_context_cap(_manage(&_sigstop_signal_context))
		{
			if (pipe(_pipefd) != 0)
				PERR("could not create pipe");
		}

		~Thread_info()
		{
			close(_pipefd[0]);
			close(_pipefd[1]);

			_dissolve(&_exception_signal_context);
			_dissolve(&_sigstop_signal_context);
		}
#if 0
		Exception_signal_context *exception_signal_context()
		{
			return &_exception_signal_context;
		}
#endif
		Signal_context_capability exception_signal_context_cap()
		{
			return _exception_signal_context_cap;
		}
#if 0
		Sigstop_signal_context *sigstop_signal_context()
		{
			return &_sigstop_signal_context;
		}
#endif
		Signal_context_capability sigstop_signal_context_cap()
		{
			return _sigstop_signal_context_cap;
		}

		Thread_capability thread_cap() { return _thread_cap; }
		unsigned long lwpid() { return _lwpid; }

		int signal_pipe_read_fd() { return _pipefd[0]; }

		int deliver_signal(int signo, unsigned long *payload)
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

			return 0;
		}
};

#endif /* _THREAD_INFO_H_ */
