/*
 * \brief  Application child
 * \author Christian Prochaska
 * \date   2009-10-05
 */

/*
 * Copyright (C) 2009-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _APP_CHILD_H_
#define _APP_CHILD_H_

/* Genode includes */
#include <base/child.h>
#include <base/service.h>
#include <init/child_policy.h>
#include <os/session_requester.h>
#include <util/arg_string.h>

/* GDB monitor includes */
#include "genode_child_resources.h"
#include "cpu_session_component.h"
#include "pd_session_component.h"
#include "rom.h"
#include "child_config.h"

namespace Gdb_monitor {
	using namespace Genode;

	class App_child;
}

class Gdb_monitor::App_child : public Child_policy,
                               public Async_service::Wakeup,
                               public Session_state::Ready_callback,
                               public Session_state::Closed_callback
{
	private:

		class Routed_service : public Genode::Child_service
		{
			private:

				Session_state::Factory &_factory;

			public:

				Routed_service(Service::Name      const &name,
				               Id_space<Parent::Server> &server_id_space,
				               Session_state::Factory   &factory,
				               Wakeup                   &wakeup,
				               Pd_session_capability     ram,
				               Pd_session_capability     pd)
				: Child_service(name, server_id_space, factory, wakeup, ram, pd),
				  _factory(factory) { }

				Session_state::Factory &factory() { return _factory; }
		};

		typedef Registered<Genode::Parent_service> Parent_service;
		typedef Registered<Routed_service>         Child_service;

		typedef Registry<Parent_service> Parent_services;
		typedef Registry<Child_service>  Child_services;

		/**
		 * Exception type
		 */
		class Service_not_present : Exception { };

		enum { STACK_SIZE = 4*1024*sizeof(long) };

		Env                                &_env;

		Allocator                          &_alloc;

		Pd_session_capability               _ref_pd_cap { _env.pd_session_cap() };
		Pd_session                         &_ref_pd     { _env.pd() };

		const char                         *_unique_name;

		Dataspace_capability                _elf_ds;

		Region_map                         &_rm;

		Ram_quota                           _ram_quota;
		Cap_quota                           _cap_quota;

		Entrypoint                          _entrypoint;

		Parent_services                     _parent_services;
		Child_services                      _child_services;

		Id_space<Parent::Client>            _client_id_space;

		Init::Child_config                  _child_config;

		Init::Child_policy_provide_rom_file _config_policy;

		Genode_child_resources              _genode_child_resources;

		Signal_dispatcher<App_child>        _unresolved_page_fault_dispatcher;

		Dataspace_pool                      _managed_ds_map;

		Pd_session_component                _pd { _entrypoint.rpc_ep(),
		                                          _env,
		                                          _alloc,
		                                          _unique_name,
		                                          _managed_ds_map };

		Pd_service::Single_session_factory  _pd_factory { _pd };
		Pd_service                          _pd_service { _pd_factory };

		Local_cpu_factory                   _cpu_factory;
		Cpu_service                         _cpu_service { _cpu_factory };

		Local_rom_factory                   _rom_factory;
		Rom_service                         _rom_service { _rom_factory };

		Attached_rom_dataspace              _session_requests { _env, "session_requests" };

		Signal_handler<App_child>           _session_request_handler { _entrypoint,
		                                                               *this,
		                                                               &App_child::_handle_session_requests };

		Genode::Session_requester           _session_requester { _entrypoint.rpc_ep(),
		                                                         _env.ram(),
		                                                         _env.rm() };

		Child                              *_child;

		void _dispatch_unresolved_page_fault(unsigned)
		{
			_genode_child_resources.cpu_session_component().handle_unresolved_page_fault();
		}

		template <typename T>
		static T *_find_service(Registry<T> &services,
		                              Service::Name const &name)
		{
			T *service = nullptr;
			services.for_each([&] (T &s) {
				if (!service && (s.name() == name))
					service = &s; });
			return service;
		}

		struct Route
		{
			Routed_service &service;
			Session_label   label;
		};

		Route _resolve_session_request(Service::Name const &service_name,
		                               Session_label const &label)
		{
			Routed_service *service = _find_service(_child_services, service_name);

			if (!service)
				throw Service_not_present();	

			return Route { *service, label };
		}

		/* functions copied from repos/os/src/init/server.cc */
		/* static */ void close_session(Genode::Session_state &session);
		void session_ready(Session_state &session);
		void _close_session(Session_state &session,
		                    Parent::Session_response response);
		void session_closed(Session_state &session);
		void _handle_create_session_request(Xml_node request,
		                                    Parent::Client::Id id);
		void _handle_upgrade_session_request(Xml_node request,
		                                     Parent::Client::Id id);
		void _handle_close_session_request(Xml_node request,
		                                   Parent::Client::Id id);
		void _handle_session_request(Xml_node request);
		void _handle_session_requests();

		/**
		 * Child_service::Wakeup callback
		 */
		void wakeup_async_service() override
		{
			_session_requester.trigger_update();
		}

	public:

		/**
		 * Constructor
		 */
		App_child(Env             &env,
		          Allocator       &alloc,
		          char const      *unique_name,
		          Ram_quota        ram_quota,
		          Cap_quota        cap_quota,
		          Signal_receiver &signal_receiver,
		          Xml_node         target_node)
		:
			_env(env),
			_alloc(alloc),
			_unique_name(unique_name),
			_rm(_env.rm()),
			_ram_quota(ram_quota), _cap_quota(cap_quota),
			_entrypoint(_env, STACK_SIZE, "GDB monitor entrypoint"),
			_child_config(env.ram(), _rm, target_node),
			_config_policy("config", _child_config.dataspace(), &_entrypoint.rpc_ep()),
			_unresolved_page_fault_dispatcher(signal_receiver,
			                                  *this,
			                                  &App_child::_dispatch_unresolved_page_fault),
			_cpu_factory(_env, _entrypoint.rpc_ep(), _alloc, _pd.core_pd_cap(),
			             signal_receiver, &_genode_child_resources),
			_rom_factory(env, _entrypoint.rpc_ep(), _alloc)
		{
			_genode_child_resources.region_map_component(&_pd.region_map());
			_pd.region_map().fault_handler(_unresolved_page_fault_dispatcher);
			_session_requests.sigh(_session_request_handler);
		}

		~App_child()
		{
			destroy(_alloc, _child);
		}

		Genode_child_resources *genode_child_resources()
		{
			return &_genode_child_resources;
		}

		void start()
		{
			_child = new (_alloc) Child(_rm, _entrypoint.rpc_ep(), *this);
		}

		/****************************
		 ** Child-policy interface **
		 ****************************/

		Name name() const override { return _unique_name; }

		Pd_session &ref_pd() override { return _ref_pd; }

		Pd_session_capability ref_pd_cap() const override { return _ref_pd_cap; }

		Id_space<Parent::Server> &server_id_space() override
		{
			return _session_requester.id_space();
		}

		void init(Pd_session &session,
		          Pd_session_capability cap) override
		{
			session.ref_account(_ref_pd_cap);

			_entrypoint.rpc_ep().apply(cap, [&] (Pd_session_component *pd) {
				if (pd) {
					_ref_pd.transfer_quota(pd->core_pd_cap(), _cap_quota);
					_ref_pd.transfer_quota(pd->core_pd_cap(), _ram_quota);
				}
			});
		}

		Service &resolve_session_request(Service::Name const &service_name,
		                                 Session_state::Args const &args) override
		{
			Service *service = nullptr;

			/* check for config file request */
			if ((service = _config_policy.resolve_session_request(service_name.string(), args.string())))
					return *service;

			/* check for "session_requests" ROM request */
			Genode::Session_label const label(Genode::label_from_args(args.string()));
			if (service_name == Genode::Rom_session::service_name()
			 && label.last_element() == Genode::Session_requester::rom_name())
				return _session_requester.service();

			if (service_name == "CPU")
				return _cpu_service;

			if (service_name == "PD")
				return _pd_service;

			if (service_name == "ROM")
				return _rom_service;

			service = _find_service(_parent_services, service_name);
			if (!service)
				service = new (_alloc) Parent_service(_parent_services, _env, service_name);

			if (!service)
				throw Service_denied();

			return *service;
		}

		void announce_service(Service::Name const &service_name) override
		{
			if (_find_service(_child_services, service_name)) {
				Genode::warning(name(), ": service ", service_name, " is already registered");
				return;
			}

			new (_alloc)
				Child_service(_child_services, service_name,
				              _session_requester.id_space(),
				              _child->session_factory(), *this,
				              _child->ram_session_cap(),
				              _child->pd_session_cap());

			_env.parent().announce(service_name.string());

			_handle_session_requests();
		}
};

/* functions copied from repos/os/src/init/server.cc */

/* static */ void Gdb_monitor::App_child::close_session(Genode::Session_state &session)
{
	session.phase = Genode::Session_state::CLOSE_REQUESTED;
	session.service().initiate_request(session);
	session.service().wakeup();
}


void Gdb_monitor::App_child::session_ready(Session_state &session)
{
	/* _report_update_trigger.trigger_report_update(); */

	/*
	 * If 'session_ready' is called as response to a session-quota upgrade,
	 * the 'phase' is set to 'CAP_HANDED_OUT' by 'Child::session_response'.
	 * We just need to forward the state change to our parent.
	 */
	if (session.phase == Session_state::CAP_HANDED_OUT) {
		Parent::Server::Id id { session.id_at_client().value };
		_env.parent().session_response(id, Parent::SESSION_OK);
	}

	if (session.phase == Session_state::AVAILABLE) {
		Parent::Server::Id id { session.id_at_client().value };
		_env.parent().deliver_session_cap(id, session.cap);
		session.phase = Session_state::CAP_HANDED_OUT;
	}

	if (session.phase == Session_state::SERVICE_DENIED)
		_close_session(session, Parent::SERVICE_DENIED);
}


void Gdb_monitor::App_child::_close_session(Session_state &session,
                                            Parent::Session_response response)
{
	/* _report_update_trigger.trigger_report_update(); */

	Ram_transfer::Account &service_ram_account = session.service();
	Cap_transfer::Account &service_cap_account = session.service();

	service_ram_account.try_transfer(_env.ram_session_cap(),
	                                 session.donated_ram_quota());

	service_cap_account.try_transfer(_env.pd_session_cap(),
	                                 session.donated_cap_quota());

	Parent::Server::Id id { session.id_at_client().value };

	session.destroy();

	_env.parent().session_response(id, response);
}


void Gdb_monitor::App_child::session_closed(Session_state &session)
{
	_close_session(session, Parent::SESSION_CLOSED);
}


void Gdb_monitor::App_child::_handle_create_session_request(Xml_node request,
                                                            Parent::Client::Id id)
{
	/*
	 * Ignore requests that are already successfully forwarded (by a prior call
	 * of '_handle_create_session_request') but still remain present in the
	 *  'session_requests' ROM because the server child has not responded yet.
	 */
	try {
		_client_id_space.apply<Parent::Client>(id, [&] (Parent::Client const &) { });
		return;
	} catch (Id_space<Parent::Client>::Unknown_id) { /* normal case */ }

	if (!request.has_sub_node("args"))
		return;

	typedef Session_state::Args Args;
	Args const args = request.sub_node("args").decoded_content<Args>();

	Service::Name const name = request.attribute_value("service", Service::Name());

	Session_label const label = label_from_args(args.string());

	try {
		Route const route = _resolve_session_request(name, label);

		/*
		 * Reduce session quota by local session costs
		 */
		char argbuf[Parent::Session_args::MAX_SIZE];
		strncpy(argbuf, args.string(), sizeof(argbuf));

		Cap_quota const cap_quota = cap_quota_from_args(argbuf);
		Ram_quota const ram_quota = ram_quota_from_args(argbuf);

		size_t const keep_quota = route.service.factory().session_costs();

		if (ram_quota.value < keep_quota)
			throw Genode::Insufficient_ram_quota();

		Ram_quota const forward_ram_quota { ram_quota.value - keep_quota };

		Arg_string::set_arg(argbuf, sizeof(argbuf), "ram_quota", forward_ram_quota.value);

		Session_state &session =
			route.service.create_session(route.service.factory(),
		                                 _client_id_space, id, route.label,
		                                 argbuf, Affinity());

		/* transfer session quota */
		try {
			Ram_transfer::Remote_account env_ram_account(_env.ram(), _env.ram_session_cap());
			Cap_transfer::Remote_account env_cap_account(_env.pd(),  _env.pd_session_cap());

			Ram_transfer ram_transfer(forward_ram_quota, env_ram_account, route.service);
			Cap_transfer cap_transfer(cap_quota,         env_cap_account, route.service);

			ram_transfer.acknowledge();
			cap_transfer.acknowledge();
		}
		catch (...) {
			/*
			 * This should never happen unless our parent missed to
			 * transfor the session quota to us prior issuing the session
			 * request.
			 */
			warning("unable to transfer session quota "
			        "(", ram_quota, " bytes, ", cap_quota, " caps) "
			        "of forwarded ", name, " session");
			session.destroy();
			throw Service_denied();
		}

		session.ready_callback  = this;
		session.closed_callback = this;

		/* initiate request */
		route.service.initiate_request(session);

		/* if request was not handled synchronously, kick off async operation */
		if (session.phase == Session_state::CREATE_REQUESTED)
			route.service.wakeup();

		if (session.phase == Session_state::SERVICE_DENIED)
			throw Service_denied();

		if (session.phase == Session_state::INSUFFICIENT_RAM_QUOTA)
			throw Insufficient_ram_quota();

		if (session.phase == Session_state::INSUFFICIENT_CAP_QUOTA)
			throw Insufficient_cap_quota();
	}
	catch (Service_denied) {
		_env.parent().session_response(Parent::Server::Id { id.value },
		                               Parent::SERVICE_DENIED); }
	catch (Insufficient_ram_quota) {
		_env.parent().session_response(Parent::Server::Id { id.value },
		                               Parent::INSUFFICIENT_RAM_QUOTA); }
	catch (Insufficient_cap_quota) {
		_env.parent().session_response(Parent::Server::Id { id.value },
		                               Parent::INSUFFICIENT_CAP_QUOTA); }
	catch (Service_not_present) { /* keep request pending */ }
}


void Gdb_monitor::App_child::_handle_upgrade_session_request(Xml_node request,
                                                             Parent::Client::Id id)
{
	_client_id_space.apply<Session_state>(id, [&] (Session_state &session) {

		Ram_quota const ram_quota { request.attribute_value("ram_quota", 0UL) };
		Cap_quota const cap_quota { request.attribute_value("cap_quota", 0UL) };

		session.phase = Session_state::UPGRADE_REQUESTED;

		try {
			Ram_transfer::Remote_account env_ram_account(_env.ram(), _env.ram_session_cap());
			Cap_transfer::Remote_account env_cap_account(_env.pd(),  _env.pd_session_cap());

			Ram_transfer ram_transfer(ram_quota, env_ram_account, session.service());
			Cap_transfer cap_transfer(cap_quota, env_cap_account, session.service());

			ram_transfer.acknowledge();
			cap_transfer.acknowledge();
		}
		catch (...) {
			warning("unable to upgrade session quota "
			        "(", ram_quota, " bytes, ", cap_quota, " caps) "
			        "of forwarded ", session.service().name(), " session");
			return;
		}

		session.increase_donated_quota(ram_quota, cap_quota);
		session.service().initiate_request(session);
		session.service().wakeup();
	});
}


void Gdb_monitor::App_child::_handle_close_session_request(Xml_node request,
                                                           Parent::Client::Id id)
{
	_client_id_space.apply<Session_state>(id, [&] (Session_state &session) {
		close_session(session); });
}


void Gdb_monitor::App_child::_handle_session_request(Xml_node request)
{
	if (!request.has_attribute("id"))
		return;

	/*
	 * We use the 'Parent::Server::Id' of the incoming request as the
	 * 'Parent::Client::Id' of the forwarded request.
	 */
	Parent::Client::Id const id { request.attribute_value("id", 0UL) };

	if (request.has_type("create"))
		_handle_create_session_request(request, id);

	if (request.has_type("upgrade"))
		_handle_upgrade_session_request(request, id);

	if (request.has_type("close"))
		_handle_close_session_request(request, id);
}


void Gdb_monitor::App_child::_handle_session_requests()
{
	_session_requests.update();

	Xml_node const requests = _session_requests.xml();

	requests.for_each_sub_node([&] (Xml_node request) {
		_handle_session_request(request); });

	/* _report_update_trigger.trigger_report_update(); */
}

#endif /* _APP_CHILD_H_ */
