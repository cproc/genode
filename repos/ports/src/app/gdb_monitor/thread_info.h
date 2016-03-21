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

class Gdb_monitor::Thread_info : public Append_list<Thread_info>::Element
{
	private:

		static constexpr bool _verbose = false;

		Cpu_session_component          *_cpu_session_component;
		Thread_capability               _thread_cap;
		unsigned long                   _lwpid;
		
		/*
		 * SIGTRAP, SIGSTOP and SIGINT must get delivered to the gdbserver code
		 * in the same order that they were generated. Since these signals are
		 * generated by different threads, the exception signal receiver is
		 * used as synchronization point. 
		 */
		Signal_dispatcher<Thread_info>  _exception_dispatcher;
		Signal_dispatcher<Thread_info>  _sigstop_dispatcher;
		Signal_dispatcher<Thread_info>  _sigint_dispatcher;

		int                             _pipefd[2];
		Lock                            _pipe_lock;
		bool                            _initial_sigtrap_pending = true;

		void _dispatch_exception(unsigned)
		{
			deliver_signal(SIGTRAP, 0);
		}

		void _dispatch_sigstop(unsigned)
		{
			deliver_signal(SIGSTOP, 0);
		}

		void _dispatch_sigint(unsigned)
		{
			deliver_signal(SIGINT, 0);
		}

	public:

		Thread_info(Cpu_session_component *cpu_session_component,
					Thread_capability thread_cap,
					unsigned long lwpid)
		: _cpu_session_component(cpu_session_component),
		  _thread_cap(thread_cap),
		  _lwpid(lwpid),
		  _exception_dispatcher(
		      *_cpu_session_component->exception_signal_receiver(),
		      *this,
		      &Thread_info::_dispatch_exception),
		  _sigstop_dispatcher(
		      *_cpu_session_component->exception_signal_receiver(),
		      *this,
		      &Thread_info::_dispatch_sigstop),
		  _sigint_dispatcher(
		      *_cpu_session_component->exception_signal_receiver(),
		      *this,
		      &Thread_info::_dispatch_sigint)
		{
			if (pipe(_pipefd) != 0)
				PERR("could not create pipe");
		}

		~Thread_info()
		{
			close(_pipefd[0]);
			close(_pipefd[1]);
		}

		Signal_context_capability exception_signal_context_cap()
		{
			return _exception_dispatcher;
		}
		
		Signal_context_capability sigstop_signal_context_cap()
		{
			return _sigstop_dispatcher;
		}

		Signal_context_capability sigint_signal_context_cap()
		{
			return _sigint_dispatcher;
		}

		Thread_capability thread_cap() { return _thread_cap; }
		unsigned long lwpid() { return _lwpid; }

		int signal_pipe_read_fd() { return _pipefd[0]; }

		int deliver_signal(int signo, unsigned long *payload)
		{
			if ((signo == SIGTRAP) && _initial_sigtrap_pending) {

				_initial_sigtrap_pending = false;

				_cpu_session_component->remove_breakpoint_at_first_instruction();

				/*
				 * The lock guard prevents an interruption by
				 * 'genode_stop_all_threads()', which could cause
				 * the new thread to be resumed when it should be
				 * stopped.
				 */

				Lock::Guard stop_new_threads_lock_guard(
					_cpu_session_component->stop_new_threads_lock());

				if (!_cpu_session_component->stop_new_threads())
					_cpu_session_component->resume(_thread_cap);

				/*
				 * gdbserver expects SIGSTOP as first signal of a new thread,
				 * but we cannot write SIGSTOP here, because waitpid() would
				 * detect that the thread is in an exception state and wait
				 * for the SIGTRAP. So SIGINFO ist used for this purpose.
				 */
				signo = SIGINFO;
			}

			switch (signo) {
				case SIGSTOP:
					if (_verbose)
						PDBG("delivering SIGSTOP to thread %lu", _lwpid);
					break;
				case SIGTRAP:
					if (_verbose)
						PDBG("delivering SIGTRAP to thread %lu", _lwpid);
					break;
				case SIGSEGV:
					if (_verbose)
						PDBG("delivering SIGSEGV to thread %lu", _lwpid);
					break;
				case SIGINT:
					if (_verbose)
						PDBG("delivering SIGINT to thread %lu", _lwpid);
					break;
				case SIGINFO:
					if (_verbose)
						PDBG("delivering initial SIGSTOP to thread %lu", _lwpid);
					break;
				default:
					PERR("unexpected signal %d", signo);
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
