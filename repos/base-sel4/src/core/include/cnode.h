/*
 * \brief   Utilities for manipulating seL4 CNodes
 * \author  Norman Feske
 * \date    2015-05-04
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _CORE__INCLUDE__CNODE_H_
#define _CORE__INCLUDE__CNODE_H_

/* Genode includes */
#include <base/exception.h>
#include <base/allocator.h>

/* core includes */
#include <untyped_address.h>

namespace Genode {

	class Cnode_base;
	class Cnode;
}


class Genode::Cnode_base
{
	private:

		unsigned const _sel;
		size_t   const _size_log2;

	public:

		unsigned sel()       const { return _sel; }
		size_t   size_log2() const { return _size_log2; }

		/**
		 * Return size of underlying backing store in bytes
		 *
		 * One cnode entry takes 16 (2^4) bytes.
		 */
		size_t mem_size_log2() const { return _size_log2 + 4; }

		/**
		 * Copy selector from another CNode
		 */
		void copy(Cnode_base const &from, unsigned from_idx, unsigned to_idx)
		{
			seL4_CNode     const service    = sel();
			seL4_Word      const dest_index = to_idx;
			uint8_t        const dest_depth = size_log2();
			seL4_CNode     const src_root   = from.sel();
			seL4_Word      const src_index  = from_idx;
			uint8_t        const src_depth  = from.size_log2();
			seL4_CapRights const rights     = seL4_AllRights;

			int const ret = seL4_CNode_Copy(service, dest_index, dest_depth,
			                                src_root, src_index, src_depth, rights);
			if (ret != 0) {
				PWRN("%s: seL4_CNode_Copy (0x%x) returned %d", __FUNCTION__,
				     from_idx, ret);
			}
		}

		void copy(Cnode_base const &from, unsigned idx) { copy(from, idx, idx); }

		/**
		 * Move selector from another CNode
		 */
		void move(Cnode_base const &from, unsigned from_idx, unsigned to_idx)
		{
			seL4_CNode const service    = sel();
			seL4_Word  const dest_index = to_idx;
			uint8_t    const dest_depth = size_log2();
			seL4_CNode const src_root   = from.sel();
			seL4_Word  const src_index  = from_idx;
			uint8_t    const src_depth  = from.size_log2();

			int const ret = seL4_CNode_Move(service, dest_index, dest_depth,
			                                src_root, src_index, src_depth);
			if (ret != 0) {
				PWRN("%s: seL4_CNode_Move (0x%x) returned %d", __FUNCTION__,
				     from_idx, ret);
			}
		}

		void move(Cnode_base const &from, unsigned idx) { move(from, idx, idx); }

		/**
		 * Constructor
		 */
		Cnode_base(unsigned sel, size_t size_log2)
		: _sel(sel), _size_log2(size_log2) { }
};


class Genode::Cnode : public Cnode_base
{
	public:

		class Phys_alloc_failed     : Exception { };
		class Untyped_lookup_failed : Exception { };
		class Retype_untyped_failed : Exception { };

		/**
		 * Constructor
		 *
		 * \param dst_sel     designated selector referring to the created
		 *                    CNode
		 * \param size_log2   number of entries in CNode
		 * \param phys_alloc  physical-memory allocator used for allocating
		 *                    the CNode backing store
		 *
		 * \throw Phys_alloc_failed
		 * \throw Untyped_address::Lookup_failed
		 */
		Cnode(unsigned dst_sel, size_t size_log2, Range_allocator &phys_alloc)
		:
			Cnode_base(dst_sel, size_log2)
		{
			/*
			 * Allocate natually-aligned physical memory for cnode
			 *
			 * The natual alignment is needed to ensure that the backing store is
			 * contained in a single untyped memory region.
			 */
			void *out_ptr = nullptr;
			size_t const mem_size = 1UL << mem_size_log2();
			Range_allocator::Alloc_return alloc_ret =
				phys_alloc.alloc_aligned(mem_size, &out_ptr, mem_size_log2());
			addr_t const phys_addr = (addr_t)out_ptr;

			if (alloc_ret.is_error()) {
				PERR("%s: allocation of backing store for cnode failed", __FUNCTION__);
				throw Phys_alloc_failed();
			}

			Untyped_address const untyped_addr(phys_addr, mem_size);

			seL4_Untyped const service     = untyped_addr.sel();
			int          const type        = seL4_CapTableObject;
			int          const offset      = untyped_addr.offset();
			int          const size_bits   = size_log2;
			seL4_CNode   const root        = seL4_CapInitThreadCNode;
			int          const node_index  = 0;
			int          const node_depth  = 0;
			int          const node_offset = dst_sel;
			int          const num_objects = 1;

			int const ret = seL4_Untyped_RetypeAtOffset(service,
			                                            type,
			                                            offset,
			                                            size_bits,
			                                            root,
			                                            node_index,
			                                            node_depth,
			                                            node_offset,
			                                            num_objects);
			if (ret != 0) {
				PERR("seL4_Untyped_RetypeAtOffset (CapTable) returned %d", ret);
				throw Retype_untyped_failed();
			}
		}

		~Cnode()
		{
			/* convert CNode back to untyped memory */

			/* revert phys allocation */

			PDBG("not implemented");
		}
};

#endif /* _CORE__INCLUDE__CNODE_H_ */
