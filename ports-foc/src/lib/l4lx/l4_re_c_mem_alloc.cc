/*
 * \brief  L4Re functions needed by L4Linux.
 * \author Stefan Kalkowski
 * \date   2011-04-17
 */

/*
 * Copyright (C) 2011-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */
#include <base/printf.h>
#include <base/capability.h>
#include <os/config.h>
#include <rm_session/connection.h>

/* L4lx includes */
#include <dataspace.h>
#include <env.h>

namespace Fiasco {
#include <l4/re/c/mem_alloc.h>
}

using namespace Fiasco;

static const bool DEBUG = false;

extern "C" {

	static const unsigned long _chunk_size()
	{
		enum { DEFAULT_CHUNK_SIZE = 16*1024*1024 };

		Genode::Number_of_bytes result = DEFAULT_CHUNK_SIZE;

		try {
			Genode::config()->xml_node().sub_node("ram")
			                            .attribute("chunk_size")
			                            .value(&result);
		} catch(...) { }

		return result;
	}


	long l4re_ma_alloc(unsigned long size, l4re_ds_t const mem,
	                   unsigned long flags)
	{
		static const unsigned long chunk_size = _chunk_size();

		using namespace L4lx;

		if (DEBUG)
			PDBG("size=%lx mem=%lx flags=%lx", size, mem, flags);

		Dataspace *ds;
		if (size > chunk_size) {
			ds = new (Genode::env()->heap())
				Chunked_dataspace("lx_memory", size, mem, chunk_size);
		} else {
			Genode::Dataspace_capability cap =
				Genode::env()->ram_session()->alloc(size);
			ds = new (Genode::env()->heap())
				Single_dataspace("lx_memory", size, cap, mem);
		}
		Env::env()->dataspaces()->insert(ds);
		return 0;
	}


	long l4re_ma_alloc_align(unsigned long size, l4re_ds_t const mem,
	                         unsigned long flags, unsigned long align) {
		return l4re_ma_alloc(size, mem, flags); }


	long l4re_ma_free(l4re_ds_t const mem)
	{
		PWRN("%s: Not implemented yet!",__func__);
		return 0;
	}

}
