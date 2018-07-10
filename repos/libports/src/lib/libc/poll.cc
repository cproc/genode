/*
 * \brief  poll() implementation
 * \author Josef Soentgen
 * \date   2012-07-12
 *
 * this 'poll()' implementation is based on OpenSSHp's and
 * uses our 'select()' function internally.
 */

/*
 * Copyright (C) 2010-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Libc includes */
#include <libc-plugin/plugin_registry.h>
#include <libc-plugin/plugin.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <stdlib.h>

using namespace Libc;


/**
 * The poll function was taken from OpenSSH portable (bsd-poll.c) and adepted
 * to better fit within Genode's libc.
 *
 * Copyright (c) 2004, 2005, 2007 Darren Tucker (dtucker at zip com au).
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
extern "C" int
__attribute__((weak))
poll(struct pollfd fds[], nfds_t nfds, int timeout)
{
	nfds_t i;
	int ret, fd, maxfd = 0;
	fd_set readfds, writefds, exceptfds;
	struct timeval tv, *tvp = NULL;

	for (i = 0; i < nfds; i++) {
		fd = fds[i].fd;
		if (fd >= (int)FD_SETSIZE) {
			/*errno = EINVAL;*/
			return -1;
		}
		maxfd = MAX(maxfd, fd);
	}

	/* populate event bit vectors for the events we're interested in */

	FD_ZERO(&readfds);
	FD_ZERO(&writefds);
	FD_ZERO(&exceptfds);

	for (i = 0; i < nfds; i++) {
		fd = fds[i].fd;
		if (fd == -1)
			continue;
		if (fds[i].events & POLLIN) {
			FD_SET(fd, &readfds);
			FD_SET(fd, &exceptfds);
		}
		if (fds[i].events & POLLOUT) {
			FD_SET(fd, &writefds);
			FD_SET(fd, &exceptfds);
		}
	}

	/* poll timeout is msec, select is timeval (sec + usec) */
	if (timeout >= 0) {
		tv.tv_sec = timeout / 1000;
		tv.tv_usec = (timeout % 1000) * 1000;
		tvp = &tv;
	}
	ret = select(maxfd + 1, &readfds, &writefds, &exceptfds, tvp);
	/*saved_errno = errno;*/
	/* scan through select results and set poll() flags */
	for (i = 0; i < nfds; i++) {
		fd = fds[i].fd;
		fds[i].revents = 0;
		if (fd == -1)
			continue;
		if ((fds[i].events & POLLIN) && FD_ISSET(fd, &readfds)) {
			fds[i].revents |= POLLIN;
		}
		if ((fds[i].events & POLLOUT) && FD_ISSET(fd, &writefds)) {
			fds[i].revents |= POLLOUT;
		}
		if (FD_ISSET(fd, &exceptfds)) {
			fds[i].revents |= POLLERR;
		}
	}

	/*
	if (ret == -1)
		errno = saved_errno;
	*/
	return ret;
}
