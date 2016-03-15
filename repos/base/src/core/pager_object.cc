/*
 * \brief  Standard implementation of pager object
 * \author Martin Stein
 * \date   2013-11-04
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* core includes */
#include <pager.h>

using namespace Genode;


void Pager_object::wake_up()
{
	/* notify pager to wake up faulter */
	Msgbuf<16> snd, rcv;
	Ipc_marshaller marshaller(snd);
	marshaller.insert(this);
	ipc_call(cap(), snd, rcv, 0);
}


void Pager_object::unresolved_page_fault_occurred()
{
	state.unresolved_page_fault = true;
}
