/*
 * \brief  Init component
 * \author Norman Feske
 * \date   2010-04-27
 */

/*
 * Copyright (C) 2010-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Genode includes */
#include <base/component.h>
#include <base/attached_rom_dataspace.h>

/* local includes */
#include <child_registry.h>
#include <child.h>
#include <alias.h>
#include <state_reporter.h>
#include <server.h>

namespace Init { struct Main; }


struct Init::Main : State_reporter::Producer, Child::Default_route_accessor,
                    Child::Ram_limit_accessor
{
	Env &_env;

	Registry<Init::Parent_service> _parent_services;
	Registry<Routed_service>       _child_services;
	Child_registry                 _children;

	Heap _heap { _env.ram(), _env.rm() };

	Attached_rom_dataspace _config { _env, "config" };

	Reconstructible<Verbose> _verbose { _config.xml() };

	Constructible<Buffered_xml> _default_route;

	unsigned _child_cnt = 0;

	static Ram_quota _preserved_ram_from_config(Xml_node config)
	{
		Number_of_bytes preserve { 40*sizeof(long)*1024 };

		config.for_each_sub_node("resource", [&] (Xml_node node) {
			if (node.attribute_value("name", String<16>()) == "RAM")
				preserve = node.attribute_value("preserve", preserve); });

		return Ram_quota { preserve };
	}

	Ram_quota _avail_ram()
	{
		Ram_quota const preserved_ram = _preserved_ram_from_config(_config.xml());

		Ram_quota avail_ram { _env.ram().avail() };

		if (preserved_ram.value > avail_ram.value) {
			error("RAM preservation exceeds available memory");
			return Ram_quota { 0 };
		}

		/* deduce preserved quota from available quota */
		return Ram_quota { avail_ram.value - preserved_ram.value };
	}

	/**
	 * Child::Ram_limit_accessor interface
	 */
	Ram_quota ram_limit() override { return _avail_ram(); }

	void _handle_resource_avail() { }

	void produce_state_report(Xml_generator &xml, Report_detail const &detail) const
	{
		if (detail.init_ram())
			xml.node("ram", [&] () { generate_ram_info(xml, _env.ram()); });

		if (detail.children())
			_children.report_state(xml, detail);
	}

	/**
	 * Default_route_accessor interface
	 */
	Xml_node default_route() override
	{
		return _default_route.constructed() ? _default_route->xml()
		                                    : Xml_node("<empty/>");
	}

	State_reporter _state_reporter { _env, *this };

	Signal_handler<Main> _resource_avail_handler {
		_env.ep(), *this, &Main::_handle_resource_avail };

	void _update_aliases_from_config();
	void _update_parent_services_from_config();
	void _abandon_obsolete_children();
	void _update_children_config();
	void _destroy_abandoned_parent_services();
	void _handle_config();

	Signal_handler<Main> _config_handler {
		_env.ep(), *this, &Main::_handle_config };

	Server _server { _env, _heap, _child_services, _state_reporter };

	Main(Env &env) : _env(env)
	{
		_config.sigh(_config_handler);

		/* prevent init to block for resource upgrades (never satisfied by core) */
		_env.parent().resource_avail_sigh(_resource_avail_handler);

		_handle_config();
	}
};


void Init::Main::_update_parent_services_from_config()
{
	Xml_node const node = _config.xml().has_sub_node("parent-provides")
	                    ? _config.xml().sub_node("parent-provides")
	                    : Xml_node("<empty/>");

	/* remove services that are no longer present in config */
	_parent_services.for_each([&] (Parent_service &service) {

		Service::Name const name = service.name();

		bool obsolete = true;
		node.for_each_sub_node("service", [&] (Xml_node service) {
			if (name == service.attribute_value("name", Service::Name())) {
				obsolete = false; }});

		if (obsolete)
			service.abandon();
	});

	if (_verbose->enabled())
		log("parent provides");

	/* register new services */
	node.for_each_sub_node("service", [&] (Xml_node service) {

		Service::Name const name = service.attribute_value("name", Service::Name());

		bool registered = false;
		_parent_services.for_each([&] (Parent_service const &service) {
			if (service.name() == name)
				registered = true; });

		if (!registered) {
			new (_heap) Init::Parent_service(_parent_services, _env, name);
			if (_verbose->enabled())
				log("  service \"", name, "\"");
		}
	});
}


void Init::Main::_destroy_abandoned_parent_services()
{
	_parent_services.for_each([&] (Parent_service &service) {
		if (service.abandoned())
			destroy(_heap, &service); });
}


void Init::Main::_update_aliases_from_config()
{
	/* remove all known aliases */
	while (_children.any_alias()) {
		Init::Alias *alias = _children.any_alias();
		_children.remove_alias(alias);
		destroy(_heap, alias);
	}

	/* create aliases */
	_config.xml().for_each_sub_node("alias", [&] (Xml_node alias_node) {

		try {
			_children.insert_alias(new (_heap) Alias(alias_node)); }
		catch (Alias::Name_is_missing) {
			warning("missing 'name' attribute in '<alias>' entry"); }
		catch (Alias::Child_is_missing) {
			warning("missing 'child' attribute in '<alias>' entry"); }
	});
}


void Init::Main::_abandon_obsolete_children()
{
	_children.for_each_child([&] (Child &child) {

		Child_policy::Name const name = child.name();

		bool obsolete = true;
		_config.xml().for_each_sub_node("start", [&] (Xml_node node) {
			if (node.attribute_value("name", Child_policy::Name()) == name)
				obsolete = false; });

		if (obsolete)
			child.abandon();
	});
}


void Init::Main::_update_children_config()
{
	for (;;) {

		/*
		 * Children are abandoned if any of their client sessions can no longer
		 * be routed or result in a different route. As each child may be a
		 * service, an avalanche effect may occur. It stops if no update causes
		 * a potential side effect in one iteration over all chilren.
		 */
		bool side_effects = false;

		_config.xml().for_each_sub_node("start", [&] (Xml_node node) {

			Child_policy::Name const start_node_name =
				node.attribute_value("name", Child_policy::Name());

			_children.for_each_child([&] (Child &child) {
				if (child.name() == start_node_name) {
					switch (child.apply_config(node)) {
					case Child::NO_SIDE_EFFECTS: break;
					case Child::MAY_HAVE_SIDE_EFFECTS: side_effects = true; break;
					};
				}
			});
		});

		if (!side_effects)
			break;
	}
}


void Init::Main::_handle_config()
{
	_config.update();

	_verbose.construct(_config.xml());
	_state_reporter.apply_config(_config.xml());

	/* determine default route for resolving service requests */
	try {
		_default_route.construct(_heap, _config.xml().sub_node("default-route")); }
	catch (...) { }

	Prio_levels     const prio_levels    = prio_levels_from_xml(_config.xml());
	Affinity::Space const affinity_space = affinity_space_from_xml(_config.xml());

	_update_aliases_from_config();
	_update_parent_services_from_config();
	_abandon_obsolete_children();
	_update_children_config();

	/* kill abandoned children */
	_children.for_each_child([&] (Child &child) {
		if (child.abandoned()) {
			_children.remove(&child);
			destroy(_heap, &child);
		}
	});

	_destroy_abandoned_parent_services();

	/* initial RAM limit before starting new children */
	Ram_quota const avail_ram = _avail_ram();

	/* variable used to track the RAM taken by new started children */
	Ram_quota used_ram { 0 };

	/* create new children */
	try {
		_config.xml().for_each_sub_node("start", [&] (Xml_node start_node) {

			/* skip start node if corresponding child already exists */
			bool exists = false;
			_children.for_each_child([&] (Child const &child) {
				if (child.name() == start_node.attribute_value("name", Child_policy::Name()))
					exists = true; });
			if (exists) {
				return;
			}

			if (used_ram.value > avail_ram.value) {
				error("RAM exhausted while starting childen");
				throw Ram_session::Alloc_failed();
			}

			try {
				Init::Child &child = *new (_heap)
					Init::Child(_env, _heap, *_verbose,
					            Init::Child::Id { ++_child_cnt }, _state_reporter,
					            start_node, *this, _children,
					            Ram_quota { avail_ram.value - used_ram.value },
					             *this, prio_levels, affinity_space,
					            _parent_services, _child_services);
				_children.insert(&child);

				/* account for the start XML node buffered in the child */
				size_t const metadata_overhead = start_node.size()
				                               + sizeof(Init::Child);
				/* track used memory and RAM limit */
				used_ram = Ram_quota { used_ram.value
				                     + child.ram_quota().value
				                     + metadata_overhead };
			}
			catch (Rom_connection::Rom_connection_failed) {
				/*
				 * The binary does not exist. An error message is printed
				 * by the Rom_connection constructor.
				 */
			}
			catch (Allocator::Out_of_memory) {
				warning("local memory exhausted during child creation"); }
			catch (Ram_session::Alloc_failed) {
				warning("failed to allocate memory during child construction"); }
			catch (Child::Missing_name_attribute) {
				warning("skipped startup of nameless child"); }
			catch (Region_map::Attach_failed) {
				warning("failed to attach dataspace to local address space "
				        "during child construction"); }
			catch (Parent::Service_denied) {
				warning("failed to create session during child construction"); }
		});
	}
	catch (Xml_node::Nonexistent_sub_node) { error("no children to start"); }
	catch (Xml_node::Invalid_syntax) { error("config has invalid syntax"); }
	catch (Init::Child_registry::Alias_name_is_not_unique) { }

	/*
	 * Initiate RAM sessions of all new children
	 */
	_children.for_each_child([&] (Child &child) {
		child.initiate_env_ram_session(); });

	/*
	 * Initiate remaining environment sessions of all new children
	 */
	_children.for_each_child([&] (Child &child) {
		child.initiate_env_sessions(); });

	/*
	 * (Re-)distribute RAM among the childen, given their resource assignments
	 * and the available slack memory. We first apply possible downgrades to
	 * free as much memory as we can. This memory is then incorporated in the
	 * subsequent upgrade step.
	 */
	_children.for_each_child([&] (Child &child) { child.apply_ram_downgrade(); });
	_children.for_each_child([&] (Child &child) { child.apply_ram_upgrade(); });

	_server.apply_config(_config.xml());
}


void Component::construct(Genode::Env &env) { static Init::Main main(env); }

