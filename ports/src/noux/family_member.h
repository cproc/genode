/*
 * \brief  Helper for handling the relationship between Noux processes
 * \author Norman Feske
 * \date   2012-02-25
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _NOUX__FAMILY_MEMBER_H_
#define _NOUX__FAMILY_MEMBER_H_

/* Genode includes */
#include <util/list.h>
#include <base/lock.h>

/* Noux includes */
#include <parent_execve.h>

namespace Noux {

	class Family_member : public List<Family_member>::Element,
	                      public Parent_execve
	{
		private:

			int             const _pid;
			Lock                  _lock;
			List<Family_member>   _list;
			Family_member * const _parent;
			bool                  _has_exited;
			int                   _exit_status;
			Semaphore             _wait4_blocker;

			void _wakeup_wait4() { _wait4_blocker.up(); }

		public:

			Family_member(int pid, Family_member *parent)
			: _pid(pid), _parent(parent), _has_exited(false), _exit_status(0)
			{ }

			virtual ~Family_member() { }

			int pid() const { return _pid; }

			Family_member *parent() { return _parent; }

			int exit_status() const { return _exit_status; }

			/**
			 * Called by the parent at creation time of the process
			 */
			void insert(Family_member *member)
			{
				Lock::Guard guard(_lock);
				_list.insert(member);
			}

			/**
			 * Called by the parent from the return path of the wait4 syscall
			 */
			void remove(Family_member *member)
			{
				Lock::Guard guard(_lock);
				_list.remove(member);
			}

			/* Called by the parent from 'deliver_kill()' */
			virtual void submit_signal(Noux::Sysio::Signal sig) = 0;

			/**
			 * Called by the parent (originates from Kill_broadcaster)
			 */
			bool deliver_kill(int pid, Noux::Sysio::Signal sig)
			{
				Lock::Guard guard(_lock);

				if (pid == _pid) {
					submit_signal(sig);
					return true;
				}

				bool result = false;

				for (Family_member *child = _list.first(); child; child = child->next())
					if (child->deliver_kill(pid, sig))
						result = true;

				return result;
			}

			/**
			 * Parent_execve interface
			 */

			/* Called by the parent from 'execve_child()' */
			virtual Family_member *do_execve(const char *filename,
			                                 Args const &args,
			                                 Sysio::Env const &env,
			                                 bool verbose) = 0;

			/* Called by the child on the parent (via Parent_execve) */
			void execve_child(Family_member &child,
			                  const char *filename,
			                  Args const &args,
			                  Sysio::Env const &env,
			                  bool verbose)
			{
				Lock::Guard guard(_lock);
				Family_member *new_child = child.do_execve(filename,
				                                           args,
				                                           env,
				                                           verbose);
				_list.insert(new_child);
				_list.remove(&child);
			}

			/**
			 * Tell the parent that we exited
			 */
			void wakeup_parent(int exit_status)
			{
				_exit_status = exit_status;
				_has_exited  = true;
				if (_parent)
					_parent->_wakeup_wait4();
			}

			Family_member *poll4()
			{
				Lock::Guard guard(_lock);

				/* check if any of our children has exited */
				Family_member *curr = _list.first();
				for (; curr; curr = curr->next()) {
					if (curr->_has_exited)
						return curr;
				}
				return 0;
			}

			/**
			 * Wait for the exit of any of our children
			 */
			Family_member *wait4()
			{
				for (;;) {
					Family_member *result = poll4();
					if (result)
						return result;

					_wait4_blocker.down();
				}
			}
	};
}

#endif /* _NOUX__FAMILY_MEMBER_H_ */
