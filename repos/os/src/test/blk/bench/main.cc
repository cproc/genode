/*
 * \brief  Block session benchmark test
 * \author Sebastian Sumpf
 * \author Josef Soentgen
 * \date   2015-03-24
 */

/*
 * Copyright (C) 2015-2016 Genode Labs GmbH
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


enum {
	TEST_WRITE   = false,
	TEST_SIZE    = 1024 * 1024 * 1024,
	REQUEST_SIZE = 8 * 512,
};


namespace Test {

	using namespace Genode;

	class Throughput;
	struct Main;
}


class Test::Throughput
{
	private:

		Genode::Env &_env;

		size_t _test_size    = TEST_SIZE;
		bool   _test_write   = TEST_WRITE;
		size_t _request_size = REQUEST_SIZE;

		Allocator_avl      _alloc;
		Block::Connection *_block;
		Timer::Connection  _timer;

		Signal_handler<Throughput> _disp_ack;
		Signal_handler<Throughput> _disp_submit;
		bool                       _read_done  = false;
		bool                       _write_done = false;

		unsigned long   _start = 0;
		unsigned long   _stop  = 0;
		size_t          _bytes = 0;
		Block::sector_t _current = 0;

		size_t          _blk_size;
		Block::sector_t _blk_count;

		void _submit()
		{
			static size_t count = _request_size / _blk_size;

			if (_read_done && (_write_done || !_test_write))
				return;

			try {
				while (_block->tx()->ready_to_submit()) {
					Block::Packet_descriptor p(
						_block->tx()->alloc_packet(_request_size),
						!_read_done ? Block::Packet_descriptor::READ : Block::Packet_descriptor::WRITE,
						_current, count);

					_block->tx()->submit_packet(p);

					/* increment for next read */
					_current += count;
					if (_current + count >= _blk_count)
						_current = 0;
				}
			} catch (...) { }
		}

		void _ready_to_submit() { _submit(); }

		void _ack_avail()
		{
			while (_block->tx()->ack_avail()) {

				Block::Packet_descriptor p = _block->tx()->get_acked_packet();
				if (!p.succeeded()) {
					Genode::error("packet error block: ", p.block_number(),
					              " count: ", p.block_count());
				}

				if (!_read_done
				    || (_read_done && p.operation() == Block::Packet_descriptor::WRITE))
					_bytes += p.size();

				_block->tx()->release_packet(p);
			}

			if (_bytes >= _test_size) {
				_finish();
				return;
			}

			_submit();
		}

		void _finish()
		{
			if (_read_done && (_write_done || !_test_write))
				return;

			_stop = _timer.elapsed_ms();

			/* fixed-point magic */
			enum { SHIFT = 32U, MASK = (1UL<<SHIFT)-1, };
			uint64_t kib_per_msec = ((_bytes/1024) << SHIFT) / ((_stop - _start));
			uint64_t i = kib_per_msec >> SHIFT;
			uint64_t f = ((kib_per_msec & MASK) * 100) >> SHIFT;

			Genode::log(!_read_done ? "Read" : "Wrote", " ",
			            _bytes / 1024, " KiB in ", _stop - _start, " ms ",
			            "(", i, ".", f, " MiB/s)");

			/* start write */
			if (!_read_done ) {
				_read_done  = true;
				_start      = _timer.elapsed_ms();
				_bytes      = 0;
				_current    = 0;
				if (_test_write)
					_submit();
				else
					Genode::log("Done");
			} else if (!_write_done && _test_write) {
				_write_done = true;
				Genode::log("Done");
			}
		}

	public:

		Throughput(Genode::Env &env, Genode::Allocator &heap)
		:
			_env(env), _alloc(&heap),
			_disp_ack(_env.ep(), *this, &Throughput::_ack_avail),
			_disp_submit(_env.ep(), *this, &Throughput::_ready_to_submit)
		{
			/* override default settings via config */
			try {
				Attached_rom_dataspace config(_env, "config");
				Xml_node node = config.xml();

				_test_size  = node.attribute_value<Number_of_bytes>("test_size", _test_size);
				_test_write = node.attribute_value<bool>("write", _test_write);

				_request_size = node.attribute_value<Number_of_bytes>("request_size", _request_size);
			} catch (...) { }

			size_t const tx_size = Block::Session::TX_QUEUE_SIZE * _request_size;
			if (tx_size > _env.ram().avail()) {
				Genode::error("available RAM quota: ", _env.ram().avail(),
				              " too small to satisfy tx_size: ", tx_size);
				throw Genode::Exception();
			}
			_block = new (&heap) Block::Connection(_env, &_alloc, tx_size);

			_block->tx_channel()->sigh_ack_avail(_disp_ack);
			_block->tx_channel()->sigh_ready_to_submit(_disp_submit);

			Block::Session::Operations blk_ops;
			_block->info(&_blk_count, &_blk_size, &blk_ops);

			Genode::log("Session block count: ", _blk_count, " size: ", _blk_size);
			Genode::log("Read", _test_write ? "/write " : " ", _test_size / 1024, " KiB in ",
			            _request_size/1024, " KiB blocks ...");

			_start = _timer.elapsed_ms();
			_submit();
		}
};


struct Test::Main
{
	Genode::Env &env;
	Genode::Heap heap { env.ram(), env.rm() };

	Throughput tp { env, heap };

	Main(Genode::Env &env) : env(env) { }
};


namespace Component {
	Genode::size_t stack_size()      { return 2*1024*sizeof(long);  }
	void construct(Genode::Env &env) { static Test::Main main(env); }
}
