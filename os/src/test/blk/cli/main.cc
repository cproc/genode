/**
 * \brief  Block session tests - client side.
 * \author Stefan Kalkowski
 * \date   2013-12-10
 */

#include <base/allocator_avl.h>
#include <block_session/connection.h>
#include <timer_session/connection.h>
#include <os/ring_buffer.h>

static Genode::size_t             blk_sz;   /* block size of the device   */
static Block::sector_t            blk_cnt;  /* number of blocks of device */
static Block::Session::Operations blk_ops;  /* supported operations       */

extern "C" int raw_write_str(char const *str);

/**
 * Virtual base class of all test scenarios, provides basic signal handling
 */
class Test
{
	public:

		void perform()
		{
			while(1) {

				raw_write_str("allocating packet descriptor\n");
				Block::Packet_descriptor p(
					_session.dma_alloc_packet(blk_sz),
					Block::Packet_descriptor::READ, 1, 1);

				raw_write_str("calling submit_packet()\n");
				_session.tx()->submit_packet(p);

				raw_write_str("calling wait_for_signal()\n");
				Genode::Signal s = _receiver.wait_for_signal();

				raw_write_str("got a signal\n");
				static_cast<Genode::Signal_dispatcher_base *>
					(s.context())->dispatch(s.num());

			}
		}

		void _ack_avail(unsigned)
		{
				raw_write_str("calling get_acked_packet()\n");
				Block::Packet_descriptor p = _session.tx()->get_acked_packet();
				raw_write_str("calling release_packet()\n");
				_session.tx()->release_packet(p);
				raw_write_str("release_packet() returned\n");
		}

	protected:

		Genode::Allocator_avl           _alloc;
		Block::Connection               _session;
		Genode::Signal_receiver         _receiver;
		Genode::Signal_dispatcher<Test> _disp_ack;
		bool                            _handle;

		Genode::size_t _shared_buffer_size(Genode::size_t bulk)
		{
			return bulk +
			       sizeof(Block::Session::Tx_policy::Ack_queue) +
			       sizeof(Block::Session::Tx_policy::Submit_queue) +
			       (1 << Block::Packet_descriptor::PACKET_ALIGNMENT) - 1;
		}

	public:

		Test()
		: _alloc(Genode::env()->heap()),
		  _session(&_alloc, _shared_buffer_size(blk_sz)),
		  _disp_ack(_receiver, *this, &Test::_ack_avail)
		{
			_session.tx_channel()->sigh_ack_avail(_disp_ack);
		}
};

int main()
{
	try {
		/**
		 * First we ask for the block size of the driver to dimension
		 * the queue size for our tests. Moreover, we implicitely test,
		 * whether closing and opening again works for the driver
		 */
		{
			Genode::Allocator_avl alloc(Genode::env()->heap());
			Block::Connection blk(&alloc);
			blk.info(&blk_cnt, &blk_sz, &blk_ops);
		}

		PINF("block device with block size %zd sector count %lld",
		     blk_sz, blk_cnt);

		Test t;
		t.perform();

		PINF("Tests finished successfully!");
	} catch(Genode::Parent::Service_denied) {
		PERR("Opening block session was denied!");
		return -1;
	}
	
	return 0;
}
