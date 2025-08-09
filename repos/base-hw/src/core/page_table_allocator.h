/*
 * \brief   Dynamic page table allocator
 * \author  Stefan Kalkowski
 * \date    2025-08-05
 */

/*
 * Copyright (C) 2025 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _CORE__PAGE_TABLE_ALLOCATOR_H_
#define _CORE__PAGE_TABLE_ALLOCATOR_H_

/* base includes */
#include <base/heap.h>
#include <util/dictionary.h>

/* base-hw internal includes */
#include <hw/page_table_allocator.h>
#include <phys_allocated.h>

namespace Core { class Page_table_allocator; }

/**
 * Dynamic page-table allocator for all PDs except core,
 * which uses accountable allocators of the PD session components
 */
class Core::Page_table_allocator : public Hw::Page_table_allocator
{
	private:

		struct Entry;
		struct Key;

		using Dictionary = Genode::Dictionary<Key, addr_t>;

		struct Key : Dictionary::Element
		{
			Entry &entry;

			Key(Dictionary &dict, addr_t addr, Entry &entry)
				: Dictionary::Element(dict, addr), entry(entry) {}
		};

		struct Entry
		{
			struct Table { uint8_t _[get_page_size()]; };
			Phys_allocated<Table> table;

			addr_t virt()
			{
				addr_t ret = 0;
				table.obj([&] (Table &t) { ret = (addr_t)&t; });
				return ret;
			}

			Key v;
			Key p;

			Entry(Rpc_entrypoint &ep,
			      Ram_allocator  &ram,
			      Local_rm       &rm,
			      Dictionary     &virt_dict,
			      Dictionary     &phys_dict)
			:
				table(ep, ram, rm),
				v(virt_dict, virt(), *this),
				p(phys_dict, table.phys_addr(), *this) {}
		};

		Rpc_entrypoint &_ep;
		Ram_allocator  &_ram;
		Local_rm       &_rm;

		static constexpr size_t SLAB_BLOCK_SIZE =
			get_page_size() - Sliced_heap::meta_data_size();
		uint8_t _initial_sb_tables[SLAB_BLOCK_SIZE];
		Tslab<Entry, SLAB_BLOCK_SIZE> _alloc_tables;

		Dictionary _virt_dict {};
		Dictionary _phys_dict {};

	public:

		Page_table_allocator(Rpc_entrypoint &ep,
		                     Ram_allocator  &ram,
		                     Local_rm       &rm,
		                     Allocator      &heap)
		:
			_ep(ep), _ram(ram), _rm(rm),
			_alloc_tables(heap, _initial_sb_tables)
		{}

		Attempt<addr_t, Lookup_error> _phys_addr(addr_t virt_addr) override
		{
			using Result = Attempt<addr_t, Lookup_error>;
			return _virt_dict.with_element(virt_addr,
				[] (Key &k) -> Result { return k.entry.table.phys_addr(); },
				[] ()       -> Result { return Lookup_error(); });
		}

		Attempt<addr_t, Lookup_error> _virt_addr(addr_t phys_addr) override
		{
			using Result = Attempt<addr_t, Lookup_error>;
			return _phys_dict.with_element(phys_addr,
				[] (Key &k) -> Result { return k.entry.virt(); },
				[] ()       -> Result { return Lookup_error(); });
		}

		Allocation::Attempt try_alloc(size_t) override
		{
			return _alloc_tables.try_alloc(sizeof(Entry)).convert<Allocation::Attempt>(
				[&] (auto &res) {
					Entry &entry = *construct_at<Entry>(res.ptr, _ep, _ram, _rm,
					                                    _virt_dict, _phys_dict);
					return entry.table.constructed.convert<Allocation::Attempt>(
						[&] (Ok) -> Allocation::Attempt {
							void *ptr = nullptr;
							entry.table.obj([&] (Entry::Table &t) { ptr = &t; });
							res.deallocate = false;
							return { *this, { ptr, sizeof(Entry::Table) }};
						},
						[&] (Alloc_error e) -> Allocation::Attempt {
							entry.~Entry();
							return e;
						});
				},
				[] (Alloc_error e) { return e; });
		}

		void _free(Allocation &a) override
		{
			_virt_dict.with_element((addr_t)a.ptr,
				[&] (Key &k) {
					void *ptr = &k.entry;
					k.entry.~Entry();
					Allocation { _alloc_tables, { ptr, sizeof(Entry) }}; },
				[] () { });
		}
};

#endif /* _CORE__PAGE_TABLE_ALLOCATOR_H_ */
