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

/* libc includes */
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>


int main(int argc, char **argv)
{
	int fd = open("/dev/dsp", O_RDWR);

	printf("fd: %d\n", fd);

	static char buf[2048];
#if 0
	for (size_t i = 0; i < sizeof(buf); i++) {
		buf[i] = i;
	}
#endif
	ssize_t total_bytes_read = 0;
	ssize_t total_bytes_written = 0;

	for (int i = 0; /*i < 512*/; i++) {
		ssize_t bytes_read = read(fd, buf, sizeof(buf));
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
