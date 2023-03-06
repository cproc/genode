/*
 * \brief  Trace::Buffer implementation
 * \author Johannes Schlatow
 * \date   2022-02-18
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/trace/buffer.h>
#include <cpu/memory_barrier.h>

/* base-internal includes */
#include <base/internal/spin_lock.h>

using namespace Genode;

/*******************************
 ** Trace::Partitioned_buffer **
 *******************************/

void Trace::Partitioned_buffer::init(size_t size)
{
//Genode::raw("Partitioned_buffer::init(): ", size);
	/* compute number of bytes available for partitions */
	size_t const header_size  = (addr_t)&_primary - (addr_t)this;
	size_t const avail_size   = size - header_size;

	_secondary_offset = align_natural(avail_size / 2);

	_primary    ->init(_secondary_offset);
	_secondary()->init(avail_size - _secondary_offset);

	/* mark first entry in secondary partition as padding instead of head */
	_secondary()->_head_entry()->mark(Simple_buffer::_Entry::PADDING);

	_state = State::Producer::bits(PRIMARY) | State::Consumer::bits(SECONDARY);

	_consumer_lock = SPINLOCK_UNLOCKED;
	_lost_entries = 0;
	_wrapped = 0;
}


Trace::Simple_buffer const &Trace::Partitioned_buffer::_switch_consumer()
{
//Genode::raw("Partitioned_buffer::_switch_consumer()");
	/* first switch atomically */
	bool switched = false;
	while (!switched)
	{
		int const old_state = _state;
		switched = cmpxchg(&_state, old_state, State::toggle_consumer(old_state));
	};

	spinlock_lock(&_consumer_lock);

	/* use spin lock to wait if producer is currently wrapping */

	spinlock_unlock(&_consumer_lock);
//Genode::raw("Partitioned_buffer::_switch_consumer() finished");

	return _consumer();
}


Trace::Simple_buffer &Trace::Partitioned_buffer::_switch_producer()
{
bool blocked = false;
//Genode::raw(&blocked, ": _switch_producer()");
	/* stops consumer from reading after switching */
	_consumer_lock = SPINLOCK_LOCKED;

Trace::Timestamp ts1 = Trace::timestamp_ms();

	bool switched = false;
	while (!switched) {
		int const old_state = _state;

		if (State::Producer::get(old_state) == State::Consumer::get(old_state))
			switched = cmpxchg(&_state, old_state, State::toggle_producer(old_state));
		else {

#if 1
if (!blocked) {
	Genode::raw(&blocked, ": _switch_producer(): blocking");
}
blocked = true;
#endif

#if 0
			/**
			 * consumer may still switch partitions at this point but not continue
			 * reading until we set the new head entry
			 */
			_lost_entries += _producer()._num_entries;
Genode::raw("XXXXX Partitioned_buffer::_switch_producer(): lost entries: ", _lost_entries);
//wait_for_continue();
			switched = true;
#endif
		}
	}

	Trace::Simple_buffer &current = _producer();

	current._buffer_wrapped();

	/* XXX _wrapped only needed for testing */
	if (State::Producer::get(_state) == PRIMARY)
		_wrapped++;

	Genode::memory_barrier();
	_consumer_lock = SPINLOCK_UNLOCKED;

if (blocked) {
	Trace::Timestamp ts2 = Trace::timestamp_ms();
	Genode::raw(&blocked, ": Partitioned_buffer::_switch_producer() finished: ", ts2 - ts1, " ms");
}

	return current;
}


char *Trace::Partitioned_buffer::reserve(size_t len)
{
//Genode::raw("Partitioned_buffer::reserve(): ", len);
//wait_for_continue();
	return _producer()._reserve(len, [&] () -> char* {
		return _switch_producer()._head_entry()->data;
	});
}


void Trace::Partitioned_buffer::commit(size_t len) {
//Genode::raw("Partitioned_buffer::commit(): ", len);
	_producer()._commit(len, [&] () { _switch_producer(); }); }
