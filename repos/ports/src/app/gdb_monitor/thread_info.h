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

#include <base/thread.h>

#include <unistd.h>

#include "append_list.h"

namespace Gdb_monitor {

	using namespace Genode;

	class Thread_info : public Signal_context, public Append_list<Thread_info>::Element
	{
		private:

			Thread_capability      _thread_cap;
			unsigned long          _lwpid;
			int                    _pipefd[2];
			Lock                   _pipe_lock;

		public:

			Thread_info(Thread_capability thread_cap, unsigned long lwpid)
			: _thread_cap(thread_cap),
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
				Lock::Guard pipe_lock_guard(_pipe_lock);

				write(_pipefd[1], &signo, sizeof(signo));

				if (payload)
					write(_pipefd[1], payload, sizeof(*payload));
			}
	};
}

#endif /* _THREAD_INFO_H_ */
