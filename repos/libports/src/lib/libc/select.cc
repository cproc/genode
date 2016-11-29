/*
 * \brief  select() implementation
 * \author Christian Prochaska
 * \date   2010-01-21
 *
 * the 'select()' implementation is partially based on the lwip version as
 * implemented in 'src/api/sockets.c'
 */

/*
 * Copyright (C) 2010-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <base/log.h>
#include <os/timed_semaphore.h>

#include <libc-plugin/plugin_registry.h>
#include <libc-plugin/plugin.h>

#include <sys/select.h>
#include <signal.h>
#include <base/debug.h>

using namespace Libc;


void (*libc_select_notify)() __attribute__((weak));


/** Description for a task waiting in select */
struct libc_select_cb
{
	struct libc_select_cb *next;
	int nfds;
	int nready;
	fd_set readset;
	fd_set writeset;
	fd_set exceptset;
	/** don't signal the same semaphore twice: set to 1 when signalled */
	int sem_signalled;
	/** semaphore to wake up a task waiting for select */
	Timed_semaphore *sem;
};


/** The global list of tasks waiting for select */
static struct libc_select_cb *select_cb_list;


static Genode::Lock &select_cb_list_lock()
{
	static Genode::Lock _select_cb_list_lock;
	return _select_cb_list_lock;
}


/* poll plugin select() functions */
/* input fds may not be NULL */
static int selscan(int nfds, fd_set *in_readfds, fd_set *in_writefds,
                   fd_set *in_exceptfds, fd_set *out_readfds,
                   fd_set *out_writefds, fd_set *out_exceptfds)
{
	PDBG("");
	int nready = 0;

	 /* zero timeout for polling of the plugins' select() functions */
	struct timeval tv_0 = {0, 0};

	/* temporary fd sets that are passed to the plugins */
	fd_set plugin_readfds;
	fd_set plugin_writefds;
	fd_set plugin_exceptfds;
	int plugin_nready;

	if (out_readfds)
		FD_ZERO(out_readfds);
	if (out_writefds)
		FD_ZERO(out_writefds);
	if (out_exceptfds)
		FD_ZERO(out_exceptfds);

	for (Plugin *plugin = plugin_registry()->first(); plugin; plugin = plugin->next()) {
		if (plugin->supports_select(nfds, in_readfds, in_writefds, in_exceptfds, &tv_0)) {

			plugin_readfds = *in_readfds;
			plugin_writefds = *in_writefds;
			plugin_exceptfds = *in_exceptfds;

			plugin_nready = plugin->select(nfds, &plugin_readfds, &plugin_writefds, &plugin_exceptfds, &tv_0);

			if (plugin_nready > 0) {
				for (int libc_fd = 0; libc_fd < nfds; libc_fd++) {
					if (out_readfds && FD_ISSET(libc_fd, &plugin_readfds)) {
						FD_SET(libc_fd, out_readfds);
					}
					if (out_writefds && FD_ISSET(libc_fd, &plugin_writefds)) {
						FD_SET(libc_fd, out_writefds);
					}
					if (out_exceptfds && FD_ISSET(libc_fd, &plugin_exceptfds)) {
						FD_SET(libc_fd, out_exceptfds);
					}
				}
				nready += plugin_nready;
			} else if (plugin_nready < 0) {
				Genode::error("plugin->select() returned error value ", plugin_nready);
			}
		}
	}

	return nready;
}


/* this function gets called by plugin backends when file descripors become ready */
static void select_notify()
{
	PDBG("");
	struct libc_select_cb *scb;
	int nready;
	fd_set tmp_readfds, tmp_writefds, tmp_exceptfds;

	/* check for each waiting select() function if one of its fds is ready now
	 * and if so, wake this select() function up */
	while (1) {
		select_cb_list_lock().lock();
		for (scb = select_cb_list; scb; scb = scb->next) {
			if (scb->sem_signalled == 0) {
				FD_ZERO(&tmp_readfds);
				FD_ZERO(&tmp_writefds);
				FD_ZERO(&tmp_exceptfds);
				nready = selscan(scb->nfds, &scb->readset, &scb->writeset,
			                     &scb->exceptset, &tmp_readfds, &tmp_writefds,
			                     &tmp_exceptfds);
				if (nready > 0)
					break;
			}
		}

		if (scb) {
			scb->sem_signalled = 1;
			scb->nready = nready;
			scb->readset = tmp_readfds;
			scb->writeset = tmp_writefds;
			scb->exceptset = tmp_exceptfds;
			scb->sem->up();
			select_cb_list_lock().unlock();
		} else {
			select_cb_list_lock().unlock();
			break;
		}
	}
}


extern "C" int
__attribute__((weak))
_select(int nfds, fd_set *readfds, fd_set *writefds,
        fd_set *exceptfds, struct timeval *timeout)
{
	if (timeout) {
		PDBG("enter with ", nfds,", timeout is ",timeout->tv_sec,":",timeout->tv_usec);
	} else {
		PDBG("enter with ", nfds,", timeout is NULL");
	}

	int nready = 0;
	for (Plugin *plugin = plugin_registry()->first(); plugin; plugin = plugin->next())
		if (plugin->supports_select(nfds, readfds, writefds, exceptfds, timeout))
			nready += plugin->select(nfds, readfds, writefds, exceptfds, timeout);
	return nready;
}


extern "C" int
__attribute__((weak))
select(int nfds, fd_set *readfds, fd_set *writefds,
       fd_set *exceptfds, struct timeval *timeout)
{
	PDBG("");
	return _select(nfds, readfds, writefds, exceptfds, timeout);
}


extern "C" int
__attribute__((weak))
_pselect(int nfds, fd_set *readfds, fd_set *writefds,
         fd_set *exceptfds, const struct timespec *timeout,
         const sigset_t *sigmask)
{
	PDBG("");
	struct timeval tv;
	sigset_t origmask;
	int nready;

	if (timeout) {
		tv.tv_usec = timeout->tv_nsec / 1000;
		tv.tv_sec = timeout->tv_sec;
	}

	if (sigmask)
		sigprocmask(SIG_SETMASK, sigmask, &origmask);
	nready = select(nfds, readfds, writefds, exceptfds, &tv);
	if (sigmask)
		sigprocmask(SIG_SETMASK, &origmask, NULL);

	return nready;
}


extern "C" int
__attribute__((weak))
pselect(int nfds, fd_set *readfds, fd_set *writefds,
        fd_set *exceptfds, const struct timespec *timeout,
        const sigset_t *sigmask)
{
	PDBG("");
	return _pselect(nfds, readfds, writefds, exceptfds, timeout, sigmask);
}
