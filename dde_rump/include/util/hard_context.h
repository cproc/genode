/**
 * \brief  Hard-context for use within rump kernel
 * \author Sebastian Sumpf
 * \date   2014-02-05
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__HARD_CONTEXT_H_
#define _INCLUDE__HARD_CONTEXT_H_

extern "C" {
#include <sys/cdefs.h>
#include <sys/types.h>
#include <rump/rump.h>
}

#include <base/thread.h>


/*************
 ** Threads **
 *************/

typedef void *(*func)(void *);

namespace Timer {
	class Connection;
};

class Hard_context : public Genode::Thread<sizeof(Genode::addr_t) * 2048>
{
	private:

		func                      _func;
		void                     *_arg;
		int                       _cookie;
		lwp                      *_lwp;

	protected:

		void entry()
		{
			_func(_arg);
			PDBG("Returned from func");
		}

	public:

		Hard_context(char const *name, func f, void *arg, int cookie, bool run = true)
		: Thread(name),
			_func(f), _arg(arg), _cookie(cookie), _lwp(0) { if (run) start(); }

		void set_lwp(lwp *l) { _lwp = l; }
		lwp *get_lwp() { return _lwp; }

		static Timer::Connection *timer();
};

#endif /* _INCLUDE__HARD_CONTEXT_H_ */
