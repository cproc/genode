/*
 * \brief   Protection-domain facility
 * \author  Martin Stein
 * \author  Stefan Kalkowski
 * \author  Sebastian Sumpf
 * \date    2012-02-12
 */

/*
 * Copyright (C) 2012-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <root/root.h>

/* core includes */
#include <platform_pd.h>
#include <platform_thread.h>

using namespace Core;


/******************************
 ** Cap_space implementation **
 ******************************/

Cap_space::Cap_space() : _slab(nullptr, &_initial_sb) { }


void Cap_space::upgrade_slab(Allocator &alloc)
{
	alloc.try_alloc(Kernel::CAP_SLAB_SIZE).with_result(
		[&] (Memory::Allocation &a) {
			a.deallocate = false;
			_slab.insert_sb(a.ptr);
		},
		[&] (Alloc_error e) { raise(e); });
}


/********************************
 ** Platform_pd implementation **
 ********************************/

bool Platform_pd::map(addr_t virt, addr_t phys,
                      size_t size, Page_flags flags)
{
	using Result = Hw::Page_table::Result;

	Result result = Ok();

	for (;;) {
		bool retry = false;

		{
			Mutex::Guard guard(_mutex);
			_table_alloc.obj([&] (Page_table_array &array) {
				_table.obj([&] (Hw::Page_table &tab) {
					result = tab.insert(virt, phys, size, flags, array.alloc());
				});
			});
		}

		result.with_error([&] (Hw::Page_table_error e) {
			if (e == Hw::Page_table_error::INVALID_RANGE)
				return;

			if (!_warned_once_about_quota_depletion) {
				bool out_of_ram = e == Hw::Page_table_error::OUT_OF_RAM;
				warning("No more ", out_of_ram ? "ram" : "caps",
				        " available in PD session of ", label(),
				        " to resolve page-faults.");
				warning("Will flush page-tables! ",
				        "This is a one-time warning and a hint why ",
				        "performance might be bad...");
				_warned_once_about_quota_depletion = true;
			}

			flush_all();
			retry = true;
		});
		if (!retry)
			break;
	}

	return result.convert<bool>([&] (Ok) -> bool { return true; },
	                            [&] (Hw::Page_table_error) -> bool {
		error("invalid mapping ", Hex(phys), " -> ", Hex(virt), " (", size, ")");
		return false; });
}


void Platform_pd::flush(addr_t virt, size_t size, Core_local_addr)
{
	Mutex::Guard guard(_mutex);

	_table_alloc.obj([&] (Page_table_array &array) {
		_table.obj([&] (Hw::Page_table &tab) {
			tab.remove(virt, size, array.alloc()); });
	});
	if (_kobj.constructed())
		Kernel::invalidate_tlb(*_kobj, virt, size);
}


void Platform_pd::flush_all() {
	flush(platform().vm_start(), platform().vm_size(), Core_local_addr{0}); }


Kernel::Pd::Core_pd_data Platform_pd::_core_pd_data()
{
	void * table_addr = nullptr;
	Hw::Page_table_translator * translator = nullptr;
	_table.obj([&] (Hw::Page_table &tab) { table_addr = &tab; });
	_table_alloc.obj([&] (Page_table_array &array) {
		translator = static_cast<Hw::Page_table_translator*>(&array.alloc()); });
	return Kernel::Pd::Core_pd_data { _table.phys_addr(), table_addr,
	                                  *translator, _slab, _label.string() };
}


Platform_pd::Platform_pd(Rpc_entrypoint &ep,
                         Ram_allocator  &ram,
                         Local_rm       &rm,
                         Allocator      &,
                         char const     *label)
:
	_label(label),
	_ram_alloc(ram),
	_table(ep, ram, rm, *(Hw::Page_table*)Hw::Mm::core_page_tables().base),
	_table_alloc(ep, ram, rm,
		[] (Phys_allocated<Page_table_array> &array, auto *obj_ptr) {
			construct_at<Page_table_array>(obj_ptr, [&] (void *virt) {
				return array.phys_addr() + ((addr_t) obj_ptr - (addr_t)virt);
			});
		}),
	_kobj(_kobj.CALLED_FROM_CORE, _core_pd_data())
{ }


Platform_pd::~Platform_pd()
{
	/* invalidate weak pointers to this object */
	Address_space::lock_for_destruction();
	flush_all();
}


/*************************************
 ** Core_platform_pd implementation **
 *************************************/

Core_platform_pd::Core_platform_pd(Id_allocator &id_alloc)
:
	_table(*(Hw::Page_table*)Hw::Mm::core_page_tables().base),
	_table_alloc(Platform::core_page_table_allocator()),
	_kobj(_kobj.CALLED_FROM_KERNEL,
	      Kernel::Pd::Core_pd_data { Platform::core_page_table(),
	                                 (void*) Hw::Mm::core_page_tables().base,
	                                 _table_alloc, _slab, label().string() },
	      id_alloc)
{ }
