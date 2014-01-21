/*
 * \brief  Aggregate
 * \author Norman Feske
 * \date   2014-01-15
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#include <aggregate.h>
#include <resource.h>
#include <type.h>

using namespace Monitor;


void Aggregate_instance::handle_config(unsigned)
{
	PINF("re-attaching config dataspace");
	try {
		_config_ds.construct(_config_rom_session.dataspace()); }
	catch (...) {
		PERR("cannot access config ROM for aggregate");
		return; }

	try {
		Xml_node config(_config_ds->local_addr<char>(), _config_ds->size());

		_configured_type_registry.update(_alloc, config); }
	catch (...) {
		PERR("spurious exception during type update");
		throw; }

	try {
		/*
		 * Now that we know the type definition of the 'configured' type,
		 * we can instantiate it. In order to do that, we first need a
		 * declaration of the type, which is represented by
		 * '_configured_resource'. Finally, we construct an instance of
		 * the declared resource.
		 */
		static Xml_node node("<resource type=\"configured\" name=\"configured\" />");
		_configured_resource.construct(_alloc, node,
		                               _configured_type_registry,
		                               _configured_type_args);
		_configured_resource->update(_alloc, node, _outer_resources);
		_configured_instance.construct(_alloc, *_configured_resource);
		_configured_instance->update(_alloc); }
	catch (...) {
		PERR("spurious exception during instance update"); }
}


void Aggregate_instance::dump_state() const
{
	/*
	 * Dump top-level type
	 */
	try {
		static char buf[10000];
		Xml_generator xml(buf, sizeof(buf), "resource", [&] () {
			_configured_resource->type_ptr()->serialize(xml); });

		Genode::printf("--- configured type ---\n%s\n\n", buf);
	} catch (Registry<Type>::Lookup_failed) { }
}
