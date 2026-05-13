/*
 * \brief  Test for using libc with VFS pipe plugin
 * \author Christian Prochaska
 * \date   2016-04-24
 */

/*
 * Copyright (C) 2016-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */


/* libc includes */
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


enum { BUF_SIZE = 128*1024 };
static char buf[BUF_SIZE];

static int pipefd[2];

int main(int argc, char *argv[])
{
	pipe2(pipefd, O_NONBLOCK);

	ssize_t bytes_written_total = 0;
	ssize_t bytes_read_total = 0;

	for (int i = 0; i < 2; i++) {

		fprintf(stderr, "writing up to %d bytes\n", BUF_SIZE);

		ssize_t bytes_written = write(pipefd[1], buf, BUF_SIZE);

		if (bytes_written > 0)
			bytes_written_total += bytes_written;

		fprintf(stderr, "wrote %zd bytes (buffered: %zd)\n", bytes_written, bytes_written_total - bytes_read_total);

		sleep(1);

		fprintf(stderr, "reading up to %d bytes\n", BUF_SIZE);

		ssize_t bytes_read = read(pipefd[0], buf, BUF_SIZE);

		if (bytes_read > 0)
			bytes_read_total += bytes_read;

		fprintf(stderr, "read %zd bytes (buffered: %zd)\n", bytes_read, bytes_written_total - bytes_read_total);

		sleep(1);
	}

	return 0;
}
