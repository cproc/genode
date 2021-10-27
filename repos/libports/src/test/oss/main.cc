/*
 * \brief  OSS test
 * \author Christian Prochaska
 * \date   2021-10-07
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/log.h>

/* libc includes */
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char **argv)
{
	int fd = open("/dev/dsp", O_RDWR);

	printf("fd: %d\n", fd);

	static char buf[2048];

#if 1
	int CHANNELS = 2;
	for (size_t i = 0; i < sizeof(buf) / sizeof(int16_t) / CHANNELS; i++) {
		for (int c = 0; c < CHANNELS; c++) {
			((int16_t*)buf)[(i * CHANNELS) + c] = (((i*2)+1) << 8) | (((i*2)+0) & 0xff);
		}
	}
#endif

	ssize_t total_bytes_read = 0;
	ssize_t total_bytes_written = 0;

	for (int i = 0; /*i < 512*/; i++) {

		memset(buf, 0, sizeof(buf));

static bool started = false;
if (!started) {
	Genode::trace("start");
	buf[0] = 128;
	started = true;
}

		size_t bytes_read = 0;
#if 0
		static constexpr int num_pieces = 1;
		for (int piece = 0; piece < num_pieces; piece++) {
			bytes_read += read(fd,
			                   &buf[piece * (sizeof(buf) / num_pieces)],
			                   sizeof(buf) / num_pieces);
		}
#else
		bytes_read += sizeof(buf);
#endif
		total_bytes_read += bytes_read;
//printf("bytes read: %zd, %zd\n", bytes_read, total_bytes_read);
		if (bytes_read > 0) {
			ssize_t bytes_written = write(fd, buf, sizeof(buf));
			total_bytes_written += bytes_written;
//printf("bytes written: %zd, %zd\n", bytes_written, total_bytes_written);
		}
	}

	return 0;
}
