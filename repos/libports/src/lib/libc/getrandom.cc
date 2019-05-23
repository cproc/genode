/*
 * \brief  C-library back end
 * \author Emery Hemingway
 * \date   2019-05-07
 */

/*
 * Copyright (C) 2019 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Libc includes */
extern "C" {
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/random.h>
}

/* local includes */
#include "libc_errno.h"

/* Genode includes */
#include <trace/timestamp.h>
#include <base/log.h>
#include <util/string.h>

namespace Libc { extern char const *config_rng(); }

static
ssize_t read_rng(char *buf, size_t buflen)
{
	static int rng_fd { -1 };
	static bool fallback { false };

	if (fallback) {
		size_t off = 0;
		while (off < buflen) {
			/* collect 31 bits of random */
			unsigned const nonce = random();
			size_t n = Genode::min(4, buflen-off);
			Genode::memcpy(buf+off, &nonce, n);
			off += n;
		}
		return buflen;
	}

	if (rng_fd == -1) {
		if (!Genode::strcmp(Libc::config_rng(), "")) {
			Genode::warning("Libc RNG not configured");

			/* initialize the FreeBSD random facility */
			srandom(Genode::Trace::timestamp()|1);
			fallback = true;

			return read_rng(buf, buflen);
		}

		rng_fd = open(Libc::config_rng(), O_RDONLY);
		if (rng_fd == -1) {
			Genode::error("RNG device ", Genode::Cstring(Libc::config_rng()), " not readable!");
			exit(~0);
		}
	}

	return read(rng_fd, buf, buflen);
}


extern "C" ssize_t __attribute__((weak))
getrandom(void *buf, size_t buflen, unsigned int flags)
{
	size_t off = 0;
	while (off < buflen && off < 256) {
		ssize_t n = read_rng((char*)buf+off, buflen-off);
		if (n < 1) return Libc::Errno(EIO);
		off += n;
	}
	return off;
}


extern "C" int __attribute__((weak))
getentropy(void *buf, size_t buflen)
{
	if (256 < buflen) return Libc::Errno(EIO);

	size_t off = 0;
	while (off < buflen) {
		ssize_t n = read_rng((char*)buf+off, buflen-off);
		if (n < 1) return Libc::Errno(EIO);
		off += n;
	}
	return 0;
}
