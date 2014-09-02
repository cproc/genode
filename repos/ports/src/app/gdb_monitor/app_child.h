/*
 * \brief  Application child
 * \author Christian Prochaska
 * \date   2009-10-05
 */

/*
 * Copyright (C) 2009-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _APP_CHILD_H_
#define _APP_CHILD_H_

#include <stdio.h>
#include <sys/stat.h>

#include <base/child.h>
#include <base/service.h>
#include <pd_session/connection.h>

#include <init/child_config.h>
#include <init/child_policy.h>

#include <util/arg_string.h>

#include "cpu_root.h"
#include "genode_child_resources.h"
#include "ram_root.h"
#include "rm_root.h"
#include "rom.h"


namespace Gdb_monitor { class App_child; }

class Gdb_monitor::App_child : public Child_policy,
                               public Init::Child_policy_enforce_labeling
{
	private:

		enum { STACK_SIZE = 4*1024*sizeof(long) };

		const char                    *_unique_name;

		Genode::Dataspace_capability  _elf_ds;

		Rpc_entrypoint                _entrypoint;

		Service_registry              *_parent_services;
		Service_registry              _local_services;

		Genode::Rpc_entrypoint       *_root_ep;

		Init::Child_config            _child_config;

		Init::Child_policy_provide_rom_file _binary_policy;
		Init::Child_policy_provide_rom_file _config_policy;

		Object_pool<Dataspace_object> _managed_ds_map;

		Genode_child_resources        _genode_child_resources;

		Signal_dispatcher<App_child>  _unresolved_page_fault_dispatcher;

		Rm_root                       _rm_root;
		Rm_session_capability         _rm_session_cap;

		Cpu_root                      _cpu_root;
		Cpu_session_capability        _cpu_session_cap;

		Ram_session_capability        _ram_session_cap;

		Pd_connection                 _pd;

		Child                        *_child;

		Rom_service                   _rom_service;

		class Rm_service : public Genode::Service
		{
			private:

				Rm_root _rm_root;

			public:

				Rm_service(Genode::Rpc_entrypoint        *entrypoint,
						   Genode::Allocator             *md_alloc,
						   Object_pool<Dataspace_object> *managed_ds_map)
				:
					Genode::Service("RM"),
					_rm_root(entrypoint, md_alloc, managed_ds_map)
				{ }

				Genode::Session_capability session(const char *args,
												   Genode::Affinity const &affinity)
				{
					return _rm_root.session(args, affinity);
				}

				void upgrade(Genode::Session_capability, const char *) { }

				void close(Genode::Session_capability cap)
				{
					_rm_root.close(cap);
				}
		} _rm_service;

		Rm_session_capability _get_rm_session_cap()
		{
			_entrypoint.manage(&_rm_root);
			Capability<Rm_session> cap = static_cap_cast<Rm_session>
										 (_rm_root.session("ram_quota=512K", Affinity()));
			Rm_session_client rm(cap);

			rm.fault_handler(_unresolved_page_fault_dispatcher);
			return cap;
		}

		Cpu_session_capability _get_cpu_session_cap()
		{
			_entrypoint.manage(&_cpu_root);
			char args[64];
			Genode::snprintf(args, sizeof(args), "ram_quota=32K, label=\"%s\"", _unique_name);
			return static_cap_cast<Cpu_session>(_cpu_root.session(args, Affinity()));
		}

		/**
		 * Proxy for a service provided by the child
		 */
		class Child_service_root : public Genode::Rpc_object<Genode::Root>
		{
			private:

				/**
				 * Root interface of the real service, provided by the child
				 */
				Genode::Root_client _child_root;

				/**
				 * Child's RAM session used for quota transfers
				 */
				Genode::Ram_session_capability _child_ram;

				struct Child_session;
				typedef Genode::Object_pool<Child_session> Session_pool;

				/**
				 * Per-session meta data
				 *
				 * For each session, we need to keep track of the quota
				 * attached to it - in order to revert the quota donation
				 * when the session gets closed.
				 */
				struct Child_session : public Session_pool::Entry
				{
					Genode::size_t ram_quota;

					Child_session(Genode::Session_capability cap,
								  Genode::size_t ram_quota)
					: Session_pool::Entry(cap), ram_quota(ram_quota) { }
				};

				/**
				 * Data base containing per-session meta data
				 */
				Session_pool _sessions;

			public:

				/**
				 * Constructor
				 */

				Child_service_root(Genode::Ram_session_capability child_ram,
					               Genode::Root_capability        child_root)
				: _child_root(child_root), _child_ram(child_ram)
				{ }

				/********************
				 ** Root interface **
				 ********************/

				Session_capability session(Session_args const &args,
					                       Affinity const &affinity)
				{
					using namespace Genode;

					Genode::size_t ram_quota =
						Arg_string::find_arg(args.string(),
							                 "ram_quota").ulong_value(0);

					/* forward session quota to child */
					env()->ram_session()->transfer_quota(_child_ram, ram_quota);

					Session_capability cap = _child_root.session(args, affinity);

					/*
					 * Keep information about donated quota in '_sessions'
					 * data base.
					 */
					_sessions.insert(new (env()->heap())
						             Child_session(cap, ram_quota));
					return cap;
				}

				void upgrade(Session_capability session_cap,
					         Upgrade_args const &args)
				{
					using namespace Genode;

					auto lambda = [&] (Child_session *session) {
						if (!session) {
							PERR("attempt to upgrade unknown session");
							return;
						}

						Genode::size_t ram_quota =
							Arg_string::find_arg(args.string(),
								                 "ram_quota").ulong_value(0);

						/* forward session quota to child */
						env()->ram_session()->transfer_quota(_child_ram, ram_quota);

						session->ram_quota += ram_quota;

						/* inform child about quota upgrade */
						_child_root.upgrade(session_cap, args);
					};

					_sessions.apply(session_cap, lambda);
				}

				void close(Session_capability session_cap)
				{
					using namespace Genode;

					Child_session *session;

					auto lambda = [&] (Child_session *s) {
						session = s;

						if (!session) {
							PERR("attempt to close unknown session");
							return;
						}
						_sessions.remove(session);
					};
					_sessions.apply(session_cap, lambda);

					Genode::size_t ram_quota = session->ram_quota;
					destroy(env()->heap(), session);

					_child_root.close(session_cap);

					/*
					 * The child is expected to free enough quota to revert
					 * the quota donation.
					 */

					Ram_session_client child_ram(_child_ram);
					child_ram.transfer_quota(env()->ram_session_cap(), ram_quota);
				}
		};

		void _dispatch_unresolved_page_fault(unsigned)
		{
			_genode_child_resources.cpu_session_component()->handle_unresolved_page_fault();
		}

	public:

		/**
		 * Constructor
		 *
		 * \param root_ep  entrypoint serving the root interfaces of the
		 *                 services provided by the child and announced
		 *                 towards the parent of GDB monitor
		 */
		App_child(const char                     *unique_name,
				  Genode::Dataspace_capability    elf_ds,
				  Genode::Ram_session_capability  ram_session,
				  Genode::Cap_session            *cap_session,
				  Service_registry               *parent_services,
				  Genode::Rpc_entrypoint         *root_ep,
				  Signal_receiver                *signal_receiver,
				  Xml_node                        target_node)
		: Init::Child_policy_enforce_labeling(unique_name),
		  _unique_name(unique_name),
		  _elf_ds(elf_ds),
		  _entrypoint(cap_session, STACK_SIZE, "GDB monitor entrypoint name"),
		  _parent_services(parent_services),
		  _root_ep(root_ep),
		  _child_config(ram_session, target_node),
		  _binary_policy("binary", elf_ds, &_entrypoint),
		  _config_policy("config", _child_config.dataspace(), &_entrypoint),
		  _unresolved_page_fault_dispatcher(*signal_receiver,
		                                    *this,
		                                    &App_child::_dispatch_unresolved_page_fault),
		  _rm_root(&_entrypoint, env()->heap(),
				   &_managed_ds_map, &_genode_child_resources),
		  _rm_session_cap(_get_rm_session_cap()),
		  _cpu_root(&_entrypoint, env()->heap(),
					signal_receiver, &_genode_child_resources),
		  _cpu_session_cap(_get_cpu_session_cap()),
		  _ram_session_cap(ram_session),
		  _pd(unique_name),
		  _rom_service(&_entrypoint, env()->heap()),
		  _rm_service(&_entrypoint, env()->heap(), &_managed_ds_map)
		{
			_local_services.insert(&_rm_service);
			_local_services.insert(&_rom_service);
		}

		~App_child()
		{
			destroy(env()->heap(), _child);
			_rm_root.close(_rm_session_cap);
		}

		Genode_child_resources *genode_child_resources()
		{
			return &_genode_child_resources;
		}

		void start()
		{
			_child = new (env()->heap()) Child(_elf_ds,
			                                   _pd.cap(),
			                                   _ram_session_cap,
			                                   _cpu_session_cap,
		                                       _rm_session_cap,
		                                       &_entrypoint,
		                                       this);
		}

		/****************************
		 ** Child-policy interface **
		 ****************************/

		const char *name() const { return _unique_name; }

		void filter_session_args(const char *, char *args, Genode::size_t args_len)
		{
			Init::Child_policy_enforce_labeling::filter_session_args(0, args, args_len);
		}

		Service *resolve_session_request(const char *service_name,
										 const char *args)
		{
			Service *service = 0;

			/* check for binary file request */
			if ((service = _binary_policy.resolve_session_request(service_name, args)))
				return service;

			/* check for config file request */
			if ((service = _config_policy.resolve_session_request(service_name, args)))
					return service;

			service = _local_services.find(service_name);
			if (service)
				return service;

			service = _parent_services->find(service_name);
			if (!service) {
				service = new (env()->heap()) Parent_service(service_name);
				_parent_services->insert(service);
			}

			return service;
		}

		bool announce_service(const char     *name,
							  Root_capability root,
							  Allocator      *alloc,
							  Server         *server)
		{
			/* create and announce proxy for the child's root interface */
			Child_service_root *r = new (alloc)
				Child_service_root(_ram_session_cap, root);

			Genode::env()->parent()->announce(name, _root_ep->manage(r));
			return true;
		}
};

#endif /* _APP_CHILD_H_ */
