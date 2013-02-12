/*
 * \brief  Native types
 * \author Norman Feske
 * \date   2007-10-15
 */

/*
 * Copyright (C) 2007-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BASE__NATIVE_TYPES_H_
#define _INCLUDE__BASE__NATIVE_TYPES_H_

#include <util/string.h>
#include <base/native_capability.h>
#include <base/stdint.h>

/*
 * We cannot just include <semaphore.h> and <pthread.h> here
 * because this would impy the nested inclusion of a myriad
 * of Linux types and would pollute the namespace for everyone
 * who includes this header file. We want to cleanly separate
 * Genode from POSIX.
 */

namespace Genode {

	/**
	 * Native lock type
	 *
	 * We are using a sleeping spinlock as lock implementation on Linux. This
	 * is a temporary solution until we have implemented futex-based locking.
	 * In a previous version, we have relied on POSIX semaphores as provided by
	 * the glibc. However, relying on the glibc badly interferes with a custom
	 * libc implementation. The glibc semaphore implementation expects to find
	 * a valid pthread structure via the TLS pointer. We do not have such a
	 * structure because we create threads via the 'clone' system call rather
	 * than 'pthread_create'. Hence we have to keep the base framework clean
	 * from glibc usage altogether.
	 */
	typedef volatile int Native_lock;

	/**
	 * Thread ID used in lock implementation
	 *
	 * Unfortunately, both - PID and TID - are needed for lx_tgkill() in
	 * thread_check_stopped_and_restart().
	 */
	struct Native_thread_id
	{
		unsigned int tid;  /* Native thread ID type as returned by the
		                      'clone' system call */
		unsigned int pid;  /* process ID (resp. thread-group ID) */

		/*
		 * The Linux-specific part of the Genode lock implementation uses the
		 * 'futex' syscall to block and unblock a thread. This syscall uses a
		 * thread-specific, natively aligned memory location to identify the
		 * thread to be woken up. Usually, this memory location also gets used
		 * as a counter variable, but the Genode lock implementation does not
		 * use it for this purpose and does not write to it. Therefore, no
		 * extra space needs to be allocated for this variable if there's an
		 * existing memory location with the correct alignment and whose
		 * content does not get changed unexpectedly by another thread.
		 * Currently, the address of the first word on the thread's stack
		 * gets used.
		 */
		int *dummy_futex_counter;

		Native_thread_id() : tid(0), pid(0), dummy_futex_counter(0) { }
		Native_thread_id(unsigned int tid, unsigned int pid, int *dummy_futex_counter)
		: tid(tid), pid(pid), dummy_futex_counter(dummy_futex_counter) { }

		bool operator == (Native_thread_id &t) {
			return ((tid == t.tid) && (pid == t.pid));
		}

		bool operator != (Native_thread_id &t) {
			return ((tid != t.tid) || (pid != t.pid));
		}

	};

	struct Thread_meta_data;

	/**
	 * Native thread contains more thread-local data than just the ID
	 *
	 * FIXME doc
	 * A thread needs two sockets as it may be a server that depends on another
	 * service during request processing. If the server socket would be used for
	 * the client call, the server thread may be unblocked by further requests
	 * from its clients. In other words, the additional client socket provides
	 * closed-receive semantics in calls. An even better solution is to use
	 * SCM_RIGHTS messages to send a client socket descriptor with the request.
	 */
	struct Native_thread : Native_thread_id
	{
		bool is_ipc_server;

		/**
		 * Opaque pointer to additional thread-specific meta data
		 *
		 * This pointer is used by hybrid Linux/Genode program to maintain
		 * POSIX-thread-related meta data. For non-hybrid Genode programs, it
		 * remains unused.
		 */
		Thread_meta_data *meta_data;

		Native_thread() : is_ipc_server(false), meta_data(0) { }
	};

	inline bool operator == (Native_thread_id t1, Native_thread_id t2) {
		return (t1.tid == t2.tid) && (t1.pid == t2.pid); }

	inline bool operator != (Native_thread_id t1, Native_thread_id t2) {
		return (t1.tid != t2.tid) || (t1.pid != t2.pid); }

	struct Cap_dst_policy
	{
		struct Dst
		{
			int socket;

			/**
			 * Default constructor creates invalid destination
			 */
			Dst() : socket(-1) { }

			explicit Dst(int socket) : socket(socket) { }
		};

		static bool valid(Dst id) { return id.socket != -1; }
		static Dst  invalid()     { return Dst(); }
		static void copy(void* dst, Native_capability_tpl<Cap_dst_policy>* src);
	};

	/**
	 * Empty UTCB type expected by the thread library, unused on Linux
	 */
	typedef struct { } Native_utcb;

	typedef Native_capability_tpl<Cap_dst_policy> Native_capability;

	/**
	 * The connection state is the socket handle of the RPC entrypoint
	 */
	struct Native_connection_state
	{
		int server_sd;
		int client_sd;

		Native_connection_state() : server_sd(-1), client_sd(-1) { }
	};

	enum { PARENT_SOCKET_HANDLE = 100 };

	struct Native_config
	{
		/**
		 * Thread-context area configuration.
		 *
		 * Please update platform-specific files after changing these
		 * values, e.g., 'base-linux/src/platform/context_area.*.ld'.
		 */
		static addr_t context_area_virtual_base() { return 0x40000000UL; }
		static addr_t context_area_virtual_size() { return 0x10000000UL; }

		/**
		 * Size of virtual address region holding the context of one thread
		 */
		static addr_t context_virtual_size() { return 0x00100000UL; }
	};

	class Native_pd_args
	{
		public:

			enum { ROOT_PATH_MAX_LEN = 256 };

		private:

			char _root[ROOT_PATH_MAX_LEN];

			unsigned _uid;
			unsigned _gid;

		public:

			Native_pd_args() : _uid(0), _gid(0) { _root[0] = 0; }

			Native_pd_args(char const *root, unsigned uid, unsigned gid)
			:
				_uid(uid), _gid(gid)
			{
				Genode::strncpy(_root, root, sizeof(_root));
			}

			char const *root() const { return _root; }
			unsigned    uid()  const { return _uid;  }
			unsigned    gid()  const { return _gid;  }
	};
}

#endif /* _INCLUDE__BASE__NATIVE_TYPES_H_ */
