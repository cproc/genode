/*
 * \brief  libc_block test
 * \author Josef Soentgen
 * \date   2013-11-04
 */

/*
 * Copyright (C) 2013-2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/attached_rom_dataspace.h>
#include <base/component.h>
#include <base/exception.h>
#include <base/log.h>
#include <base/heap.h>
#include <timer_session/connection.h>

/* libc includes */
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


namespace Test {
	struct Main;
}


struct Close_guard
{
	int fd;

	Close_guard(int fd) : fd(fd) { }
	~Close_guard() { close(fd); }
};


static void test_seq_read(Timer::Connection &timer,
                          off_t offset, size_t length,
                          void *dst, size_t dst_length)
{
	Genode::log("start ", __func__, " length: ", length / 1024,
	            " KiB buffer_length: ", dst_length / 1024, " KiB ...");

	struct stat stat_buf;
	int res = stat("/dev/blkdev", &stat_buf);
	if (res == -1) {
		perror("stat");
		return;
	}

	Genode::warning("size: ", stat_buf.st_size);

	int const fd = open("/dev/blkdev", O_RDWR);
	if (fd == -1) {
		perror("open");
		return;
	}

	Close_guard g(fd);

	offset = lseek(fd, offset, SEEK_SET);
	if (offset == (off_t)-1) {
		perror("lseek");
		return;
	}

	unsigned long const start = timer.elapsed_ms();

	size_t r = 0;
	while (length > 0) {
		ssize_t n = read(fd, dst, dst_length);
		if (n == -1) {
			perror("read");
			return;
		}
		length -= n;
		r      += n;
	}

	unsigned long const stop = timer.elapsed_ms();

	printf("read: %zu KiB in %lu ms throughput: %.02f MiB/s\n",
	       r / 1024, stop - start,
	       ((double)r / (1<<20)) / ((double)(stop - start) / 1000));

	Genode::log("finished ", __func__);
}


static void test_rand_read(Timer::Connection &timer,
                           size_t length, void *dst, size_t dst_length)
{
	Genode::log("start ", __func__, " length: ", length / 1024,
	            " KiB buffer_length: ", dst_length / 1024, " KiB ...");

	int const fd = open("/dev/blkdev", O_RDWR);
	if (fd == -1) {
		perror("open");
		return;
	}

	Close_guard g(fd);

	off_t const end = length - (dst_length*2); /* be on the safe side */
	off_t const offsets[] = {
		1<<20, /* 1 MiB */
		0,
		1024,  /* 1 KiB */
		10 * (1<<20), /* 10 MiB */
		512,   /* 512 bytes */
		20 * (1<<20), /* 10 MiB */
		4096,  /* 4 KiB */
		30 * (1<<20), /* 10 MiB */
		8192,  /* 8 KiB */
		40 * (1<<20), /* 10 MiB */
		16384,  /* 16 KiB */
		0,
		(1<<10) + 4093, /* 1 MiB + devious offset */
		end
	};

	/* measuring the time this way is inaccurate to say the least */
	unsigned long const start = timer.elapsed_ms();

	size_t r = 0;
	for (size_t i = 0; i < sizeof(offsets)/sizeof(offsets[0]); i++) {
		off_t const offset = lseek(fd, offsets[1], SEEK_SET);
		if (offset == (off_t const)-1) {
			perror("lseek");
			return;
		}
		ssize_t const n = read(fd, dst, dst_length);
		if (n == -1) {
			perror("read");
			return;
		}
		r += n;
	}

	unsigned long const stop = timer.elapsed_ms();

	printf("read: %zu KiB in %lu ms throughput: %.02f MiB/s\n",
	       r / 1024, stop - start,
	       ((double)r / (1<<20)) / ((double)(stop - start) / 1000));

	Genode::log("finished ", __func__);
}


static void test_seq_write(Timer::Connection &timer,
                           off_t offset, size_t length,
                           void *src, size_t src_length)
{
	/* fill buffer */
	Genode::memset(src, 23, src_length);

	Genode::log("start ", __func__, " length: ", length / 1024,
	            " KiB buffer_length: ", src_length / 1024, " KiB ...");

	int fd = open("/dev/blkdev", O_RDWR);
	if (fd == -1) {
		perror("open");
		return;
	}

	Close_guard g(fd);

	offset = lseek(fd, offset, SEEK_SET);
	if (offset == (off_t)-1) {
		perror("lseek");
		return;
	}

	unsigned long const start = timer.elapsed_ms();

	size_t written = 0;
	while (length > 0) {
		ssize_t n = write(fd, src, src_length);
		if (n == -1) {
			perror("read");
			return;
		}
		length  -= n;
		written += n;
	}

	unsigned long const stop = timer.elapsed_ms();

	printf("written: %zu KiB in %lu ms throughput: %.02f MiB/s\n",
	       written / 1024, stop - start,
	       ((double)written / (1<<20)) / ((double)(stop - start) / 1000));

	Genode::log("finished ", __func__);
}


struct Test::Main
{
	Genode::Env &env;
	Genode::Heap heap { env.ram(), env.rm() };

	Timer::Connection timer;

	enum {
		KiB = (1<<10), MiB = (1<<20),
	};

	size_t test_size    =   1 * MiB;
	size_t request_size = 128 * KiB;
	bool   test_write   = false;

	void *buffer = nullptr;

	Main(Genode::Env &env) : env(env)
	{
		/* override default settings via config */
		try {
			using namespace Genode;
			Attached_rom_dataspace config(env, "config");
			Xml_node node = config.xml();

			test_size  = node.attribute_value<Number_of_bytes>("test_size", test_size);
			test_write = node.attribute_value<bool>("test_write", test_write);

			request_size = node.attribute_value<Number_of_bytes>("request_size", request_size);
		} catch (...) { }

		if (!heap.alloc(request_size, &buffer)) {
			Genode::error("could not allocate buffer");
			throw Genode::Exception();
		}

		test_seq_read(timer, 0, test_size, buffer, request_size);

		if (test_write) {
			test_seq_write(timer, 0, test_size, buffer, request_size);
		}

		test_rand_read(timer, test_size, buffer, request_size);

		heap.free(buffer, request_size);

		Genode::log("Done");
	}
};


namespace Component {
	Genode::size_t stack_size()      { return 4*1024*sizeof(long);  }
	void construct(Genode::Env &env) { static Test::Main main(env); }
}
