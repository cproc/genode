/*
 * \brief  Block session replay
 * \author Josef Soentgen
 * \date   201-07-04
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


static bool verbose = false;
static bool bulk    = false;


namespace Test {

	using namespace Genode;

	struct Result;
	struct Replay;
	struct Main;
}


struct Test::Result
{
	uint64_t integer;
	uint64_t fraction;

	Result(uint64_t amount, unsigned long start, unsigned long end)
	{
		/* fixed-point magic */
		enum { SHIFT = 32U, MASK = (1UL<<SHIFT)-1, };
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
	unsigned              total_request_num { 0 };
	Genode::Fifo<Request> requests;

	uint64_t amount { 0 };

	Timer::Connection  timer { env };
	unsigned long      start_time { 0UL };

	void print_result(uint64_t amount, unsigned start_time)
	{
		unsigned long end_time = timer.elapsed_ms();
		Result result(amount, start_time, end_time);

		Genode::log(amount / 1024, " KiB in ", end_time - start_time, " ms ",
		            "(", result.integer, ".", result.fraction, " MiB/s)");
	}

	Allocator_avl      block_alloc { &alloc };
	Block::Connection *block;
	size_t             block_size;
	Block::sector_t    block_count;

	Genode::Signal_handler<Replay> ack_dispatcher {
		env.ep(), *this, &Replay::ack_avail };

	Genode::Signal_handler<Replay> submit_dispatcher {
		env.ep(), *this, &Replay::ready_to_submit };

	void submit()
	{
		bool more = true;

		try {
			while (block->tx()->ready_to_submit() && more) {
				Request *req = requests.dequeue();
				if (!req) {
					/* done */
					return;
				}

				if (verbose) {
					static unsigned count = 0;
					Genode::log(++count, " request op: ",
					            req->op == Block::Packet_descriptor::READ ? "read" : "write",
					            " nr: ", req->nr, " count: ", req->count);
				}


				Block::Packet_descriptor p(
					block->tx()->alloc_packet(req->count * block_size),
					req->op, req->nr, req->count);

				block->tx()->submit_packet(p);

				//Genode::destroy(&alloc, req);
				requests.enqueue(req);
				more = bulk;
			}
		} catch (...) { Genode::warning("could not submit request"); }
	}

	void ready_to_submit() { submit(); }

	void ack_avail()
	{
		while (block->tx()->ack_avail()) {

			Block::Packet_descriptor p = block->tx()->get_acked_packet();
			if (!p.succeeded()) {
				Genode::error("packet failed block_nr: ", p.block_number(),
				              " block_count: ", p.block_count());
			} else {
				if (verbose) {
					Genode::log("request succeeded size: ", p.size());
				}
				amount += p.size();

				if (--request_num == 0) {
					print_result(amount, start_time);
					//env.parent().exit(0);
					request_num = total_request_num;
					amount = 0;
					start_time = timer.elapsed_ms();
				} else {
					if (verbose) {
						Genode::log("remaining requests: ", request_num);
					}
				}

			}

			block->tx()->release_packet(p);
		}

		submit();
	}

	Replay(Genode::Env &env, Genode::Allocator &alloc, Genode::Xml_node config)
	: env(env), alloc(alloc)
	{
		verbose = config.attribute_value<bool>("verbose", false);
		bulk    = config.attribute_value<bool>("bulk", false);

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
		} catch (...) {
			Genode::error("could not read <request_list>");
			return;
		}

		total_request_num = request_num;

		size_t const tx_size = 2*1<<20;
		try {
			block = new (&alloc) Block::Connection(env, &block_alloc, tx_size);

			block->tx_channel()->sigh_ack_avail(ack_dispatcher);
			block->tx_channel()->sigh_ready_to_submit(submit_dispatcher);

			Block::Session::Operations block_ops;
			block->info(&block_count, &block_size, &block_ops);
		} catch (...) {
			Genode::error("could not open Block session");
			return;
		}

		Genode::log("start replaying ", request_num, " requests bulk=",
		            bulk ? "yes" : "no", " tx_size=", tx_size);

		start_time = timer.elapsed_ms();

		/* initial submit */
		submit();
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
	Genode::size_t stack_size()      { return 2*1024*sizeof(long);  }
	void construct(Genode::Env &env) { static Test::Main main(env); }
}
