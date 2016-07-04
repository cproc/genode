/*
 * \brief  Libc block I/O replay
 * \author Josef Soentgen
 * \date   2016-07-04
 */

/*
 * Copyright (C) 2016 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/allocator_avl.h>
#include <base/attached_rom_dataspace.h>
#include <base/component.h>
#include <base/heap.h>
#include <base/log.h>
#include <block_session/connection.h>
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


static bool verbose = false;
static bool bulk    = false;


namespace Test {

	using namespace Genode;

	struct Result;
	struct Replay;
	struct Main;

	using Genode::uint64_t;
}


struct Test::Result
{
	uint64_t integer;
	uint64_t fraction;

	Result(uint64_t amount, unsigned long start, unsigned long end)
	{
		/* fixed-point magic */
		enum { SHIFT = 32ULL, MASK = (1ULL<<SHIFT)-1, };
		uint64_t kib_per_msec = ((amount/1024) << SHIFT) / ((end - start));

		integer  = kib_per_msec >> SHIFT;
		fraction = ((kib_per_msec & MASK) * 100) >> SHIFT;
	}
};


struct Test::Replay
{
	Genode::Env       &env;
	Genode::Allocator &alloc;

	struct Request : Genode::Fifo<Request>::Element
	{
		Block::Packet_descriptor::Opcode op;
		Block::sector_t nr;
		Genode::size_t  count;

		Request(Block::Packet_descriptor::Opcode op,
		        Block::sector_t nr, Genode::size_t count)
		: op(op), nr(nr), count(count) { }
	};

	unsigned              request_num { 0 };
	Genode::Fifo<Request> requests;

	uint64_t amount { 0 };

	Timer::Connection  timer { env };
	unsigned long      start_time { 0UL };

	int fd { -1 };
	unsigned block_size { 512 };

	enum { BUFFER_SIZE = 4 * (1u<<20), };
	void *buffer;

	void print_result(uint64_t amount, unsigned start_time)
	{
		unsigned long end_time = timer.elapsed_ms();
		Result result(amount, start_time, end_time);

		Genode::log(amount / 1024, " KiB in ", end_time - start_time, " ms ",
		            "(", result.integer, ".", result.fraction, " MiB/s)");
	}

	void replay(int const fd)
	{
		for (Request *req = requests.dequeue(); req; req = requests.dequeue()) {

			if (verbose) {
				static unsigned count = 0;
				Genode::log(++count, " request op: ",
				            req->op == Block::Packet_descriptor::READ ? "read" : "write",
				            " nr: ", req->nr, " count: ", req->count);
			}

			::off_t const offset = lseek(fd, req->nr * block_size, SEEK_SET);
			if (offset == (::off_t const)-1) {
				perror("lseek");
				return;
			}

			ssize_t n = 0;
			if (req->op == Block::Packet_descriptor::READ) {
				n = read(fd, buffer, req->count*block_size);
				if (n == -1) {
					perror("read");
					return;
				}
				amount += n;
			} else if (req->op == Block::Packet_descriptor::WRITE) {
				n = write(fd, buffer, req->count*block_size);
				if (n == -1) {
					perror("write");
					return;
				}
				amount += n;
			}

			if (verbose) {
				Genode::log("request succeeded size: ", n);
				Genode::log("remaining requests: ", request_num);
			}

		}
	}

	Replay(Genode::Env &env, Genode::Allocator &alloc, Genode::Xml_node config)
	: env(env), alloc(alloc)
	{
		verbose = config.attribute_value<bool>("verbose", false);

		try {
			Xml_node request_list = config.sub_node("request_list");
			request_list.for_each_sub_node("request", [&](Xml_node request) {
				Block::Packet_descriptor::Opcode op;
				Block::sector_t nr { 0 };
				Genode::size_t  count { 0 };

				try {
					request.attribute("block_nr").value(&nr);
					request.attribute("block_count").value(&count);

					Genode::String<8> tmp;
					request.attribute("type").value(&tmp);
					if      (tmp == "read")  { op = Block::Packet_descriptor::READ; }
					else if (tmp == "write") { op = Block::Packet_descriptor::WRITE; }
					else { throw -1; }

					Request *req = new (&alloc) Request(op, nr, count);
					requests.enqueue(req);
					++request_num;
				} catch (...) { return; }
			});

			buffer = alloc.alloc(BUFFER_SIZE);
		} catch (...) {
			Genode::error("could not read <request_list>");
			return;
		}

		Genode::log("start replaying ", request_num, " requests bulk=",
		            bulk ? "yes" : "no", " buffer_size=", (unsigned)BUFFER_SIZE);

		fd = open("/dev/blkdev", O_RDWR);
		if (fd == -1) {
			perror("open");
			return;
		}

		start_time = timer.elapsed_ms();

		replay(fd);
		print_result(amount, start_time);

		close(fd);
		alloc.free(buffer, BUFFER_SIZE);

		env.parent().exit(0);
	}
};



struct Test::Main
{
	Genode::Env &env;
	Genode::Heap heap { env.ram(), env.rm() };

	Genode::Attached_rom_dataspace config_rom { env, "config" };

	Replay replay { env, heap, config_rom.xml() };

	Main(Genode::Env &env) : env(env) { }
};


namespace Component {
	Genode::size_t stack_size()      { return 8*1024*sizeof(long);  }
	void construct(Genode::Env &env) { static Test::Main main(env); }
}
