/*
 * \brief  Platform driver - Device abstraction
 * \author Stefan Kalkowski
 * \date   2020-04-30
 */

/*
 * Copyright (C) 2020 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#include <device.h>
#include <session_component.h>

Driver::Device::Name Driver::Device::name() const { return _name; }


bool Driver::Device::acquire(Session_component & sc)
{
	if (_session.valid() && _session != sc.label()) { return false; }

	/**
	 * FIXME: By now, we cannot use the connection objects for IRQ and
	 *        IOMEM and propagate missing resources when opening the
	 *        sessions, because the combination of Genode::Env and
	 *        Genode::Connection object transparently does quota upgrades.
	 *        If we like to account those costs per client, we actually
	 *        need to explicitely forward exceptions during session
	 *        requests.
	 *        For now, we try to measure the probable costs.
	 */
	Cap_quota_guard::Reservation caps(sc.cap_quota_guard(),
	                                  Cap_quota{_cap_quota_required()});
	Ram_quota_guard::Reservation ram(sc.ram_quota_guard(),
	                                 Ram_quota{_ram_quota_required()});

	_session = sc.label();

	caps.acknowledge();
	ram.acknowledge();
	return true;
}


void Driver::Device::release(Session_component & sc)
{
	if (_session != sc.label()) { return; }

	sc.replenish(Cap_quota{_cap_quota_required()});
	sc.replenish(Ram_quota{_ram_quota_required()});

	_io_mem_list.for_each([&] (Io_mem & io_mem) {
		if (io_mem.io_mem) {
			destroy(sc.heap(), io_mem.io_mem);
			io_mem.io_mem = nullptr;
		}
	});

	_irq_list.for_each([&] (Irq & irq) {
		if (irq.irq) {
			destroy(sc.heap(), irq.irq);
			irq.irq = nullptr;
		}
	});

	_session = Platform::Session::Label();;
}


Genode::Irq_session_capability Driver::Device::irq(unsigned idx,
                                                   Session_component & sc)
{
	Irq_session_capability cap;

	if (_session != sc.label()) { return cap; }

	unsigned i = 0;
	_irq_list.for_each([&] (Irq & irq)
	{
		if (i++ != idx) return;

		if (!irq.irq) {
			irq.irq = new (sc.heap())
				Irq_connection(sc.env().env, irq.number);
		}
		cap = irq.irq->cap();
	});

	return cap;
}


Genode::Io_mem_session_capability
Driver::Device::io_mem(unsigned idx, Cache_attribute attr,
                       Session_component & sc)
{
	Io_mem_session_capability cap;

	if (_session != sc.label()) return cap;

	unsigned i = 0;
	_io_mem_list.for_each([&] (Io_mem & io_mem)
	{
		if (i++ != idx) return;

		if (!io_mem.io_mem) {
			io_mem.io_mem = new (sc.heap())
				Io_mem_connection(sc.env().env, io_mem.base, io_mem.size,
				                          (attr == WRITE_COMBINED));
		}
		cap = io_mem.io_mem->cap();
	});

	return cap;
}


void Driver::Device::report(Xml_generator & xml, Session_component & sc)
{
	unsigned io_mem_id = 0;
	unsigned irq_id    = 0;

	static constexpr addr_t page_off_mask = (addr_t)((1 << 12) - 1);

	xml.node("device", [&] () {
		xml.attribute("name", name());
		_io_mem_list.for_each([&] (Io_mem & io_mem) {
			xml.node("io_mem", [&] () {
				xml.attribute("id",   io_mem_id++);
				xml.attribute("size", io_mem.size);
				xml.attribute("page_offset",
				              io_mem.base & page_off_mask);
			});
		});
		_irq_list.for_each([&] (Irq &) {
			xml.node("irq", [&] () {
				xml.attribute("id", irq_id++); });
		});
		_property_list.for_each([&] (Property & p) {
			xml.node("property", [&] () {
				xml.attribute("name",  p.name);
				xml.attribute("value", p.value);
			});
		});
		_report_platform_specifics(xml, sc);
	});
}


Genode::size_t Driver::Device::_cap_quota_required()
{
	size_t total = 0;
	_io_mem_list.for_each([&] (Io_mem &) {
		total += Io_mem_session::CAP_QUOTA; });
	return total;
}


Genode::size_t Driver::Device::_ram_quota_required()
{
	size_t total = 0;
	_io_mem_list.for_each([&] (Io_mem & io_mem) {
		total += io_mem.size + 2*1024; });
	return total;
}


Driver::Device::Device(Name name)
: _name(name) { }


Driver::Device::~Device()
{
	if (_session.valid()) {
		error("Device to be destroyed, still obtained by session ",
		      _session); }
}
