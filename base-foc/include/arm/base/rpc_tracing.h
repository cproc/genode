/*
 * \brief  Tracing of RPC client calls
 * \author Josef Soentgen
 * \date   2013-01-06
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _INCLUDE__BASE__RPC_TRACING_H_
#define _INCLUDE__BASE__RPC_TRACING_H_

#include <base/thread.h>
#include <base/snprintf.h>
#include <base/printf.h>
#include <util/string.h>

namespace Fiasco {
#include <l4/sys/ktrace.h>
}

namespace Genode {

#if defined(RPC_TRACING)
/**
 * Dump tracebuffer to serial kernel console if number of entries
 * is greater than n.
 */
inline void _dump_tracebuffer(unsigned int n)
{
	using namespace Fiasco;
	static unsigned long count = 0;
	l4_tracebuffer_status_t *status;
	status = fiasco_tbuf_get_status();
	if (status) {
		/*
		volatile l4_tracebuffer_entry_t *cur;

		cur = status->current_entry;
		unsigned long num = cur->number;

		if (num > (count + n)) {
			//PDBG("%lu", num);
			count = num;
			fiasco_tbuf_dump();
			fiasco_tbuf_clear();
		}
		*/
	}
}

inline void rpc_trace_call(Native_capability const &cap, const char *rname,
                           const char *suffix)
{
	using namespace Fiasco;
	char buffer[128];

	snprintf(buffer, sizeof (buffer) - 1, " C%s %d %s", rname, cap.local_name(),
	         suffix);

	fiasco_tbuf_log(buffer);

	if (*suffix == '<')
		_dump_tracebuffer(1024 * 8);
}

inline void rpc_trace_dispatch(Native_capability const &cap, const char *rname)
{
	using namespace Fiasco;
	char buffer[128];

	snprintf(buffer, sizeof (buffer) - 1, " D%s %d", rname, cap.local_name());

	fiasco_tbuf_log(buffer);
}
#else
#define rpc_trace_call(...)
#define rpc_trace_dispatch(...)
#endif

}

#endif /* _INCLUDE__BASE__RPC_TRACING_H_ */
