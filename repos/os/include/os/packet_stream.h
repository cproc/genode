/*
 * \brief  Packet-streaming interface
 * \author Norman Feske
 * \date   2009-11-10
 *
 * This interface support the unidirectional streaming of bulk data between
 * processes via a shared-memory block. The public interface consists of
 * the two class templates 'Packet_stream_source', and 'Packet_stream_sink'.
 * Both communication parties agree on a policy with regard to the
 * organization of the communication buffer by specifying the same
 * 'Packet_stream_policy' as template argument.
 *
 * The communication buffer consists of three parts, a submit queue, an
 * acknowledgement queue, and a bulk buffer. The submit queue contains
 * packets generated by the source to be processed by the sink. The
 * acknowledgement queue contains packets that are processed and acknowledged
 * by the sink. The bulk buffer contains the actual payload. The assignment
 * of packets to bulk-buffer regions is performed by the source.
 *
 * The interplay between source and sink for processing a single packet looks
 * as follows:
 * # The source allocates a region of the bulk buffer for storing the packet
 *   payload using 'alloc_packet'
 * # The source requests the local start address of the payload using
 *   'packet_content' and fills the packet with data
 * # The source submits the packet to the submit queue via 'submit_packet'
 * # The sink requests a packet from the submit queue using 'get_packet'
 * # The sink requests the local start address of the payload using
 *   'packet_content' and processes the contained data
 * # After having finished the processing of the packet, the sink acknowledges
 *   the packet using 'acknowledge_packet', placing the packet into the
 *   acknowledgement queue
 * # The source reads the packet from the acknowledgement queue and releases
 *   the packet using 'release_packet'. Thereby, the region of the bulk buffer
 *   that was used by the packet becomes marked as free.
 *
 * This protocol has four corner cases that are handled by signals:
 *
 * :submit queue is full: when the source is trying to submit a new packet.
 *   In this case, the source needs to stall until the sink has removed packets
 *   from the submit queue. If the sink observes such a condition (calling
 *   'get_packet' on a full submit queue, it delivers a 'ready_to_submit'
 *   signal to wake up the source.
 *
 * :submit queue is empty: when the sink tries to obtain a packet via
 *   'get_packet'. In this case, the sink may go idle. If the source places a
 *   packet into an empty submit queue, it delivers a 'packet_avail'
 *   signal to wake up the sink.
 *
 * :acknowledgement queue is full: when the sink tries to acknowledge a packet
 *   using 'acknowledge_packet'. The sink needs to stall until the source
 *   removes an acknowledged packet from the acknowledgement queue and delivers
 *   a 'ready_to_ack' signal.
 *
 * :acknowledgement queue is empty: when the source tries to obtain an
 *   acknowledged packet using 'get_acked_packet'. In this case, the source
 *   may go idle until the sink places another acknowledged packet into the
 *   empty acknowledgement queue and delivers a 'ack_avail' signal.
 *
 * These conditions must be queried before interacting with the queues by
 * using the methods 'packet_avail', 'ready_to_submit', 'ready_to_ack', and
 * 'ack_avail'.
 */

/*
 * Copyright (C) 2009-2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__OS__PACKET_STREAM_H_
#define _INCLUDE__OS__PACKET_STREAM_H_

/* Genode includes */
#include <base/env.h>
#include <base/signal.h>
#include <base/allocator.h>
#include <base/attached_dataspace.h>
#include <util/string.h>
#include <util/construct_at.h>

namespace Genode {

	class Packet_descriptor;

	template <typename, unsigned> class Packet_descriptor_queue;
	template <typename>           class Packet_descriptor_transmitter;
	template <typename>           class Packet_descriptor_receiver;

	class Packet_stream_base;

	template <typename, unsigned, unsigned, typename>
	struct Packet_stream_policy;

	/**
	 * Default configuration for packet-descriptor queues
	 */
	using Default_packet_stream_policy =
		Packet_stream_policy<Packet_descriptor, 64, 64, char>;

	template <typename POLICY = Default_packet_stream_policy>
	class Packet_stream_source;

	template <typename POLICY = Default_packet_stream_policy>
	class Packet_stream_sink;
}


/**
 * Default packet descriptor
 *
 * A class used as 'PACKET_DESCRIPTOR' arguments to the 'Packet_stream_policy'
 * template must implement the interface of this class.
 */
class Genode::Packet_descriptor
{
	private:

		Genode::off_t  _offset;
		Genode::size_t _size;

	public:

		/**
		 * Default packet alignment used in allocation
		 */
		enum Alignment { PACKET_ALIGNMENT = 0 };

		/**
		 * Exception type thrown by packet streams
		 */
		class Invalid_packet { };

		/**
		 * Constructor
		 */
		Packet_descriptor(Genode::off_t offset, Genode::size_t size) :
			_offset(offset), _size(size) { }

		/**
		 * Default constructor used for instantiating arrays of
		 * packet-descriptors used as submit and ack queues.
		 */
		Packet_descriptor() : _offset(0), _size(0) { }

		Genode::off_t  offset() const { return _offset; }
		Genode::size_t size()   const { return _size; }
};


/**
 * Ring buffer shared between source and sink, containing packet descriptors
 *
 * This class is private to the packet-stream interface.
 */
template <typename PACKET_DESCRIPTOR, unsigned QUEUE_SIZE>
class Genode::Packet_descriptor_queue
{
	private:

		template <typename T>
		class Atomic
		{
			private:

				T _var;

			public:

				Atomic &operator=(T val)
				{
					__atomic_store_n(&_var, val, __ATOMIC_SEQ_CST);
					return *this;
				}

				operator T() const
				{
					return __atomic_load_n(&_var, __ATOMIC_SEQ_CST);
				}
		};


		/*
		 * The anonymous struct is needed to skip the initialization of the
		 * members, which are shared by both sides of the packet stream.
		 */
		struct
		{
			Atomic<unsigned> _head;
			Atomic<unsigned> _tail;
			PACKET_DESCRIPTOR _queue[QUEUE_SIZE];
		};

	public:

		using Packet_descriptor = PACKET_DESCRIPTOR;

		enum Role { PRODUCER, CONSUMER };

		/**
		 * Constructor
		 *
		 * Because the 'Packet_descriptor_queue' is constructed twice (at the
		 * source and at the sink) inside a shared-memory block, the
		 * constructor must know the role of the instance to initialize only
		 * those members that are driven by the respective role.
		 */
		Packet_descriptor_queue(Role role)
		{
			if (role == PRODUCER) {
				_head = 0;
				Genode::memset(_queue, 0, sizeof(_queue));
			} else
				_tail = 0;
		}

		/**
		 * Place packet descriptor into queue
		 *
		 * \return true on success, or
		 *         false if queue is full
		 */
		bool add(PACKET_DESCRIPTOR packet)
		{
			if (full()) return false;

			_queue[_head%QUEUE_SIZE] = packet;
			_head = (_head + 1)%QUEUE_SIZE;
			return true;
		}

		/**
		 * Take packet descriptor from queue
		 *
		 * \return  packet descriptor
		 */
		PACKET_DESCRIPTOR get()
		{
			PACKET_DESCRIPTOR packet = _queue[_tail%QUEUE_SIZE];
			_tail = (_tail + 1)%QUEUE_SIZE;
			return packet;
		}

		/**
		 * Return current packet descriptor
		 */
		PACKET_DESCRIPTOR peek() const
		{
			return _queue[_tail%QUEUE_SIZE];
		}

		/**
		 * Return true if packet-descriptor queue is empty
		 */
		bool empty() { return _tail == _head; }

		/**
		 * Return true if packet-descriptor queue is full
		 */
		bool full() { return (_head + 1)%QUEUE_SIZE == _tail; }

		/**
		 * Return true if a single element is stored in the queue
		 */
		bool single_element() { return (_tail + 1)%QUEUE_SIZE == _head; }


		/**
		 * Return true if a single slot is left to be put into the queue
		 */
		bool single_slot_free() { return (_head + 2)%QUEUE_SIZE == _tail; }

		/**
		 * Return number of slots left to be put into the queue
		 */
		unsigned slots_free() {
			return ((_tail > _head) ? _tail - _head
			                        : QUEUE_SIZE - _head + _tail) - 1; }
};


/**
 * Transmit packet descriptors with data-flow control
 *
 * This class is private to the packet-stream interface.
 */
template <typename TX_QUEUE>
class Genode::Packet_descriptor_transmitter
{
	private:

		/* facility to send ready-to-receive signals */
		Genode::Signal_transmitter _rx_ready { };

		Genode::Mutex  _tx_queue_mutex { };
		TX_QUEUE      *_tx_queue;
		bool           _tx_wakeup_needed = false;

		/*
		 * Noncopyable
		 */
		Packet_descriptor_transmitter(Packet_descriptor_transmitter const &);
		Packet_descriptor_transmitter &operator = (Packet_descriptor_transmitter const &);

	public:

		class Saturated_tx_queue : Exception { };

		/**
		 * Constructor
		 */
		Packet_descriptor_transmitter(TX_QUEUE *tx_queue)
		:
			_tx_queue(tx_queue)
		{ }

		void register_rx_ready_cap(Genode::Signal_context_capability cap)
		{
			_rx_ready.context(cap);

			/*
			 * if a packet was already put into the queue
			 * before a signal handler was registered,
			 * a signal has to be send again
			 */
			if (!_tx_queue->empty())
				_rx_ready.submit();
		}

		bool ready_for_tx()
		{
			Genode::Mutex::Guard mutex_guard(_tx_queue_mutex);
			return !_tx_queue->full();
		}

		void tx(typename TX_QUEUE::Packet_descriptor packet)
		{
			Genode::Mutex::Guard mutex_guard(_tx_queue_mutex);

			if (_tx_queue->full())
				throw Saturated_tx_queue();

			_tx_queue->add(packet);

			if (_tx_queue->single_element())
				_rx_ready.submit();
		}

		bool try_tx(typename TX_QUEUE::Packet_descriptor packet)
		{
			Genode::Mutex::Guard mutex_guard(_tx_queue_mutex);

			if (_tx_queue->full())
				return false;

			_tx_queue->add(packet);

			if (_tx_queue->single_element())
				_tx_wakeup_needed = true;

			return true;
		}

		bool tx_wakeup()
		{
			Genode::Mutex::Guard mutex_guard(_tx_queue_mutex);

			bool signal_submitted = false;

			if (_tx_wakeup_needed) {
				_rx_ready.submit();
				signal_submitted = true;
			}

			_tx_wakeup_needed = false;
			return signal_submitted;
		}

		/**
		 * Return number of slots left to be put into the tx queue
		 */
		unsigned tx_slots_free() { return _tx_queue->slots_free(); }
};


/**
 * Receive packet descriptors with data-flow control
 *
 * This class is private to the packet-stream interface.
 */
template <typename RX_QUEUE>
class Genode::Packet_descriptor_receiver
{
	private:

		/* facility to send ready-to-transmit signals */
		Genode::Signal_transmitter _tx_ready { };

		Genode::Mutex mutable  _rx_queue_mutex { };
		RX_QUEUE              *_rx_queue;
		bool                   _rx_wakeup_needed = false;

		/*
		 * Noncopyable
		 */
		Packet_descriptor_receiver(Packet_descriptor_receiver const &);
		Packet_descriptor_receiver &operator = (Packet_descriptor_receiver const &);

	public:

		class Empty_rx_queue : Exception { };

		/**
		 * Constructor
		 */
		Packet_descriptor_receiver(RX_QUEUE *rx_queue)
		:
			_rx_queue(rx_queue)
		{ }

		void register_tx_ready_cap(Genode::Signal_context_capability cap)
		{
			_tx_ready.context(cap);

			/*
			 * if a packet was already put into the queue
			 * before a signal handler was registered,
			 * a signal has to be send again
			 */
			if (!_rx_queue->empty())
				_tx_ready.submit();
		}

		bool ready_for_rx()
		{
			Genode::Mutex::Guard mutex_guard(_rx_queue_mutex);
			return !_rx_queue->empty();
		}

		void rx(typename RX_QUEUE::Packet_descriptor *out_packet)
		{
			Genode::Mutex::Guard mutex_guard(_rx_queue_mutex);

			if (_rx_queue->empty())
				throw Empty_rx_queue();

			*out_packet = _rx_queue->get();

			if (_rx_queue->single_slot_free())
				_tx_ready.submit();
		}

		typename RX_QUEUE::Packet_descriptor try_rx()
		{
			Genode::Mutex::Guard mutex_guard(_rx_queue_mutex);

			typename RX_QUEUE::Packet_descriptor packet { };

			if (!_rx_queue->empty())
				packet = _rx_queue->get();

			if (_rx_queue->single_slot_free())
				_rx_wakeup_needed = true;

			return packet;
		}

		bool rx_wakeup(bool omit_signal)
		{
			Genode::Mutex::Guard mutex_guard(_rx_queue_mutex);

			bool signal_submitted = false;

			if (_rx_wakeup_needed && !omit_signal) {
				_tx_ready.submit();
				signal_submitted = true;
			}

			_rx_wakeup_needed = false;

			return signal_submitted;
		}

		typename RX_QUEUE::Packet_descriptor rx_peek() const
		{
			Genode::Mutex::Guard mutex_guard(_rx_queue_mutex);
			return _rx_queue->peek();
		}
};


/**
 * Common base of 'Packet_stream_source' and 'Packet_stream_sink'
 */
class Genode::Packet_stream_base
{
	public:

		/**
		 * Exception type
		 */
		class Transport_dataspace_too_small { };

	private:

		/*
		 * Noncopyable
		 */
		Packet_stream_base(Packet_stream_base const &);
		Packet_stream_base &operator = (Packet_stream_base const &);

	protected:

		Genode::Region_map          &_rm;
		Genode::Dataspace_capability _ds_cap;
		Genode::Attached_dataspace   _ds { _rm, _ds_cap };

		Genode::off_t  _submit_queue_offset;
		Genode::off_t  _ack_queue_offset;
		Genode::off_t  _bulk_buffer_offset;
		Genode::size_t _bulk_buffer_size { 0 };

		/**
		 * Constructor
		 *
		 * \param submit_queue_size  submit queue size in bytes
		 * \param ack_queue_size     acknowledgement queue size in bytes
		 * \throw                    'Transport_dataspace_too_small'
		 */
		Packet_stream_base(Genode::Dataspace_capability transport_ds,
		                   Genode::Region_map &rm,
		                   Genode::size_t submit_queue_size,
		                   Genode::size_t ack_queue_size)
		:
			_rm(rm), _ds_cap(transport_ds),

			/* map dataspace locally */
			_submit_queue_offset(0),
			_ack_queue_offset(_submit_queue_offset + submit_queue_size),
			_bulk_buffer_offset(align_addr(_ack_queue_offset + ack_queue_size, 6))
		{
			if ((Genode::size_t)_bulk_buffer_offset >= _ds.size())
				throw Transport_dataspace_too_small();

			_bulk_buffer_size = _ds.size() - _bulk_buffer_offset;
		}

		void *_submit_queue_local_base() {
			return _ds.local_addr<char>() + _submit_queue_offset; }

		void *_ack_queue_local_base() {
			return _ds.local_addr<char>() + _ack_queue_offset; }

		Genode::addr_t _bulk_buffer_local_base() {
			return (Genode::addr_t)_ds.local_addr<char>() + _bulk_buffer_offset; }

		/**
		 * Hook for unit testing
		 */
		void _debug_print_buffers();

		/**
		 * Return communication buffer
		 */
		Genode::Dataspace_capability _dataspace() { return _ds_cap; }

		bool packet_valid(Packet_descriptor packet)
		{
			return !packet.size() || (packet.offset() >= _bulk_buffer_offset
				 && packet.offset() < _bulk_buffer_offset + (Genode::off_t)_bulk_buffer_size
				 && packet.offset() + packet.size() <= _bulk_buffer_offset + _bulk_buffer_size);
		}

		template<typename CONTENT_TYPE>
		CONTENT_TYPE *packet_content(Packet_descriptor packet)
		{
			if (!packet.size()) return nullptr;

			if (!packet_valid(packet) || packet.size() < sizeof(CONTENT_TYPE))
				throw Packet_descriptor::Invalid_packet();

			return (CONTENT_TYPE *)(_ds.local_addr<char>() + packet.offset());
		}

		Genode::addr_t ds_local_base() const { return (Genode::addr_t)_ds.local_addr<char>(); }
		Genode::addr_t ds_size()       const { return _ds.size(); }
};


/**
 * Policy used by both sides source and sink
 */
template <typename PACKET_DESCRIPTOR,
          unsigned SUBMIT_QUEUE_SIZE,
          unsigned ACK_QUEUE_SIZE,
          typename CONTENT_TYPE>
struct Genode::Packet_stream_policy
{
	using Content_type      = CONTENT_TYPE;
	using Packet_descriptor = PACKET_DESCRIPTOR;

	using Submit_queue = Packet_descriptor_queue<PACKET_DESCRIPTOR, SUBMIT_QUEUE_SIZE>;
	using Ack_queue    = Packet_descriptor_queue<PACKET_DESCRIPTOR, ACK_QUEUE_SIZE>;
};


/**
 * Originator of a packet stream
 */
template <typename POLICY>
class Genode::Packet_stream_source : private Packet_stream_base
{
	public:

		using Packet_descriptor = typename POLICY::Packet_descriptor;

	private:

		using Submit_queue = typename POLICY::Submit_queue;
		using Ack_queue    = typename POLICY::Ack_queue;
		using Content_type = typename POLICY::Content_type;

		enum { PACKET_ALIGNMENT = POLICY::Packet_descriptor::PACKET_ALIGNMENT };

		Genode::Range_allocator &_packet_alloc;

		Packet_descriptor_transmitter<Submit_queue> _submit_transmitter;
		Packet_descriptor_receiver<Ack_queue>       _ack_receiver;

	public:

		/**
		 * Exception type
		 */
		class Packet_alloc_failed    : Exception { };
		class Saturated_submit_queue : Exception { };
		class Empty_ack_queue        : Exception { };

		enum class Alloc_packet_error { FAILED };

		using Alloc_packet_result = Attempt<Packet_descriptor, Alloc_packet_error>;

		/**
		 * Constructor
		 *
		 * \param transport_ds  dataspace used for communication buffer shared
		 *                      between source and sink
		 * \param rm            region to map buffer dataspace into
		 * \param packet_alloc  allocator for managing packet allocation within
		 *                      the shared communication buffer
		 *
		 * The 'packet_alloc' must not be pre-initialized. It will be
		 * initialized by the constructor using dataspace-relative offsets
		 * rather than pointers.
		 */
		Packet_stream_source(Genode::Dataspace_capability  transport_ds_cap,
		                     Genode::Region_map           &rm,
		                     Genode::Range_allocator      &packet_alloc)
		:
			Packet_stream_base(transport_ds_cap, rm,
			                   sizeof(Submit_queue),
			                   sizeof(Ack_queue)),
			_packet_alloc(packet_alloc),

			/* construct packet-descriptor queues */
			_submit_transmitter(construct_at<Submit_queue>(_submit_queue_local_base(),
			                                               Submit_queue::PRODUCER)),
			_ack_receiver(construct_at<Ack_queue>(_ack_queue_local_base(),
			                                      Ack_queue::CONSUMER))
		{
			/* initialize packet allocator */
			_packet_alloc.add_range(_bulk_buffer_offset,
			                         _bulk_buffer_size);
		}

		~Packet_stream_source()
		{
			_packet_alloc.remove_range(_bulk_buffer_offset,
			                            _bulk_buffer_size);
		}

		using Packet_stream_base::packet_valid;

		/**
		 * Return the size of the bulk buffer.
		 */
		Genode::size_t bulk_buffer_size() { return _bulk_buffer_size; }

		/**
		 * Register signal handler for receiving the signal that new packets
		 * are available in the submit queue.
		 */
		void register_sigh_packet_avail(Genode::Signal_context_capability cap)
		{
			_submit_transmitter.register_rx_ready_cap(cap);
		}

		/**
		 * Register signal handler for receiving the signal that there is new
		 * space for new acknowledgements in the ack queue.
		 */
		void register_sigh_ready_to_ack(Genode::Signal_context_capability cap)
		{
			_ack_receiver.register_tx_ready_cap(cap);
		}

		/**
		 * Allocate packet
		 *
		 * \param size   size of packet in bytes
		 * \param align  alignment of packet as log2 value, default is 1 byte
		 * \throws       'Packet_alloc_failed'
		 * \return       packet descriptor with an assigned range within the
		 *               bulk buffer shared between source and sink
		 */
		Packet_descriptor alloc_packet(Genode::size_t size, unsigned align = PACKET_ALIGNMENT)
		{
			if (size == 0)
				return Packet_descriptor(0, 0);

			return _packet_alloc.alloc_aligned(size, align).convert<Packet_descriptor>(

				[&] (void *base) {
					return Packet_descriptor((Genode::off_t)base, size); },

				[&] (Allocator::Alloc_error) -> Packet_descriptor {
					throw Packet_alloc_failed(); });
		}

		/**
		 * Allocate packet without throwing exceptions
		 *
		 * \param size   size of packet in bytes
		 * \param align  alignment of packet as log2 value, default is 1 byte
		 * \return       an Attempt object that either contains an error or a
		 *               packet descriptor with an assigned range within the
		 *               bulk buffer shared between source and sink
		 */
		Alloc_packet_result alloc_packet_attempt(Genode::size_t size, unsigned align = PACKET_ALIGNMENT)
		{
			if (size == 0)
				return Packet_descriptor(0, 0);

			return _packet_alloc.alloc_aligned(size, align).convert<Alloc_packet_result>(

				[&] (void *base) {
					return Packet_descriptor((Genode::off_t)base, size); },

				[&] (Allocator::Alloc_error) {
					return Alloc_packet_error::FAILED; });
		}

		/**
		 * Get pointer to the content of the specified packet
		 *
		 * \throw Packet_descriptor::Invalid_packet  raise an exception if
		                                             the packet is invalid
		 */
		Content_type *packet_content(Packet_descriptor packet) {
			return Packet_stream_base::packet_content<Content_type>(packet); }

		/**
		 * Return true if submit queue can hold 'count' additional packets
		 */
		bool ready_to_submit(unsigned count = 1)
		{
			return _submit_transmitter.tx_slots_free() >= count;
		}

		/**
		 * Tell sink about a packet to process
		 */
		void submit_packet(Packet_descriptor packet)
		{
			if (!ready_to_submit()) {
				error("attempt to add packet into saturated submit queue");
				throw Saturated_submit_queue();
			}

			_submit_transmitter.tx(packet);
		}

		/**
		 * Submit the specified packet to the server if possible
		 *
		 * \return false if the submit queue is congested
		 *
		 * This method never blocks.
		 */
		bool try_submit_packet(Packet_descriptor packet)
		{
			return _submit_transmitter.try_tx(packet);
		}

		/**
		 * Wake up the packet sink if needed
		 *
		 * This method assumes that the same signal handler is used for
		 * the submit transmitter and the ack receiver. The ack receiver is not
		 * signalled if the submit transmitter was already signalled.
		 */
		void wakeup()
		{
			/* submit only one signal, prefer submit transmitter over ack receiver */
			_ack_receiver.rx_wakeup(_submit_transmitter.tx_wakeup());
		}

		/**
		 * Returns true if one or more packet acknowledgements are available
		 */
		bool ack_avail() { return _ack_receiver.ready_for_rx(); }

		/**
		 * Get acknowledged packet
		 */
		Packet_descriptor get_acked_packet()
		{
			if (!ack_avail()) {
				error("attempt to retrieve packet from empty acknowledgement queue");
				throw Empty_ack_queue();
			}

			Packet_descriptor packet;
			_ack_receiver.rx(&packet);
			return packet;
		}

		/**
		 * Return next acknowledgement from sink, or an invalid packet
		 *
		 * This method never blocks.
		 */
		Packet_descriptor try_get_acked_packet()
		{
			return _ack_receiver.try_rx();
		}

		/**
		 * Release bulk-buffer space consumed by the packet
		 */
		void release_packet(Packet_descriptor packet)
		{
			if (packet.size())
				_packet_alloc.free((void *)packet.offset(), packet.size());
		}

		void debug_print_buffers() {
			Packet_stream_base::_debug_print_buffers(); }

		Genode::Dataspace_capability dataspace() {
			return Packet_stream_base::_dataspace(); }

		Genode::addr_t ds_local_base() const { return (Genode::addr_t)_ds.local_addr<void>(); }
		Genode::addr_t ds_size()       const { return Packet_stream_base::_ds.size(); }
};


/**
 * Receiver of a packet stream
 */
template <typename POLICY>
class Genode::Packet_stream_sink : private Packet_stream_base
{
	public:

		using Submit_queue      = typename POLICY::Submit_queue;
		using Ack_queue         = typename POLICY::Ack_queue;
		using Packet_descriptor = typename POLICY::Packet_descriptor;
		using Content_type      = typename POLICY::Content_type;

		/**
		 * Exception types
		 */
		class Saturated_ack_queue : Exception { };
		class Empty_submit_queue  : Exception { };

	private:

		Packet_descriptor_receiver<Submit_queue> _submit_receiver;
		Packet_descriptor_transmitter<Ack_queue> _ack_transmitter;

	public:

		/**
		 * Constructor
		 *
		 * \param transport_ds  dataspace used for communication buffer shared between
		 *                      source and sink
		 */
		Packet_stream_sink(Genode::Dataspace_capability transport_ds,
		                   Genode::Region_map &rm)
		:
			Packet_stream_base(transport_ds, rm, sizeof(Submit_queue), sizeof(Ack_queue)),

			/* construct packet-descriptor queues */
			_submit_receiver(construct_at<Submit_queue>(_submit_queue_local_base(),
			                                            Submit_queue::CONSUMER)),
			_ack_transmitter(construct_at<Ack_queue>(_ack_queue_local_base(),
			                                         Ack_queue::PRODUCER))
		{ }

		using Packet_stream_base::packet_valid;

		/**
		 * Register signal handler to notify that new acknowledgements
		 * are available in the ack queue.
		 */
		void register_sigh_ack_avail(Genode::Signal_context_capability cap)
		{
			_ack_transmitter.register_rx_ready_cap(cap);
		}

		/**
		 * Register signal handler to notify that new packets
		 * can be submitted into the submit queue.
		 */
		void register_sigh_ready_to_submit(Genode::Signal_context_capability cap)
		{
			_submit_receiver.register_tx_ready_cap(cap);
		}

		/**
		 * Return true if a packet is available
		 */
		bool packet_avail() { return _submit_receiver.ready_for_rx(); }

		/**
		 * Get next packet from source
		 *
		 * This method blocks if no packets are available.
		 */
		Packet_descriptor get_packet()
		{
			if (!packet_avail()) {
				error("attempt to retrieve packet from empty submit queue");
				throw Empty_submit_queue();
			}

			Packet_descriptor packet;
			_submit_receiver.rx(&packet);
			return packet;
		}

		/**
		 * Return next packet from source, or an invalid packet
		 *
		 * This method never blocks.
		 */
		Packet_descriptor try_get_packet()
		{
			return _submit_receiver.try_rx();
		}

		/**
		 * Wake up the packet source if needed
		 *
		 * This method assumes that the same signal handler is used for
		 * the submit receiver and the ack transmitter. The submit receiver
		 * is not signalled if the ack transmitter was already signalled.
		 */
		void wakeup()
		{
			/* submit only one signal, prefer ack_avail signal over ready_to_submit */
			_submit_receiver.rx_wakeup(_ack_transmitter.tx_wakeup());
		}

		/**
		 * Return but do not dequeue next packet
		 *
		 * If there is no packet, an invalid packet descriptor is returned.
		 */
		Packet_descriptor peek_packet() const
		{
			return _submit_receiver.rx_peek();
		}

		/**
		 * Get pointer to the content of the specified packet
		 *
		 * \throw Packet_descriptor::Invalid_packet  raise an exception if
		                                             the packet is invalid
		 */
		Content_type *packet_content(Packet_descriptor packet) {
			return Packet_stream_base::packet_content<Content_type>(packet); }

		/**
		 * Returns true if no further acknowledgements can be submitted
		 */
		bool ready_to_ack() { return _ack_transmitter.ready_for_tx(); }

		/**
		 * Returns number of slots left in the the ack queue
		 */
		unsigned ack_slots_free() {
			return _ack_transmitter.tx_slots_free(); }

		/**
		 * Tell the source that the processing of the specified packet is completed
		 *
		 * This method blocks if the acknowledgement queue is full.
		 */
		void acknowledge_packet(Packet_descriptor packet)
		{
			if (!ack_slots_free()) {
				error("attempt to add packet to saturated acknowledgement queue");
				throw Saturated_ack_queue();
			}

			_ack_transmitter.tx(packet);
		}

		/**
		 * Acknowledge the specified packet to the client if possible
		 *
		 * \return false if the acknowledgement queue is congested
		 *
		 * This method never blocks.
		 */
		bool try_ack_packet(Packet_descriptor packet)
		{
			return _ack_transmitter.try_tx(packet);
		}

		void debug_print_buffers() {
			Packet_stream_base::_debug_print_buffers(); }

		Genode::Dataspace_capability dataspace() {
			return Packet_stream_base::_dataspace(); }

		Genode::addr_t ds_local_base() const { return (Genode::addr_t)_ds.local_addr<char>(); }
		Genode::addr_t ds_size()       const { return Packet_stream_base::_ds.size(); }
};

#endif /* _INCLUDE__OS__PACKET_STREAM_H_ */

