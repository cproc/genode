/*
 * \brief  'Mem_alloc' content copied from parent via 'fork'
 * \author Norman Feske
 * \author Christian Prochaska
 * \date   2022-03-21
 */

/*
 * Copyright (C) 2022 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _LIBC__INTERNAL__CLONED_MEM_ALLOC_RANGE_H_
#define _LIBC__INTERNAL__CLONED_MEM_ALLOC_RANGE_H_

/* Genode includes */
#include <region_map/region_map.h>

/* libc-internal includes */
#include <internal/clone_session.h>

namespace Libc { struct Cloned_mem_alloc_range; }


struct Libc::Cloned_mem_alloc_range
{
	Ram_dataspace_capability ds;

	using Range = Region_map::Range;

	Range const range;

	Cloned_mem_alloc_range(Ram_allocator &ram, Region_map &rm, Range const range)
	:
		ds(ram.alloc(range.num_bytes)), range(range)
	{
		rm.attach(ds, {
			.size       = { },
			.offset     = { },
			.use_at     = true,
			.at         = range.start,
			.executable = { },
			.writeable  = true
		}).with_result(
			[&] (Range) { },
			[&] (Region_map::Attach_error e) {
				using Error = Region_map::Attach_error;
				switch (e) {
				case Error::OUT_OF_RAM:        throw Out_of_ram();
				case Error::OUT_OF_CAPS:       throw Out_of_caps();
				case Error::INVALID_DATASPACE: break;
				case Error::REGION_CONFLICT:   break;
				}
				error("failed to clone Mem_alloc region ",
				      Hex_range(range.start, range.num_bytes));
			}
		);
	}

	void import_content(Clone_connection &clone_connection)
	{
		clone_connection.memory_content((void *)range.start, range.num_bytes);
	}

	virtual ~Cloned_mem_alloc_range() { }
};

#endif /* _LIBC__INTERNAL__CLONED_MEM_ALLOC_RANGE_H_ */
