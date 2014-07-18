/*
 * \brief  RM root interface
 * \author Christian Helmuth
 * \date   2006-07-17
 */

/*
 * Copyright (C) 2006-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _RM_ROOT_H_
#define _RM_ROOT_H_

/* Genode includes */
#include <root/component.h>

#include "genode_child_resources.h"
#include "rm_session_component.h"

namespace Gdb_monitor { class Rm_root; }

class Gdb_monitor::Rm_root : public Root_component<Rm_session_component>
{
	private:

		Object_pool<Dataspace_object> *_managed_ds_map;
		Genode_child_resources *_genode_child_resources;

	protected:

		Rm_session_component *_create_session(const char *args)
		{
			Rm_session_component *rm_session_component =
				new (md_alloc()) Rm_session_component(_managed_ds_map, args);

			if (_genode_child_resources)
				_genode_child_resources->rm_session_component(rm_session_component);

			return rm_session_component;
		}

	public:

		/**
		 * Constructor
		 *
		 * \param session_ep   entry point for managing RM session objects
		 * \param md_alloc     meta data allocator to be used by root component
		 */
		Rm_root(Rpc_entrypoint  *session_ep,
				Allocator       *md_alloc,
				Object_pool<Dataspace_object> *managed_ds_map,
				Genode_child_resources *genode_child_resources = 0)
		:
			Root_component<Rm_session_component>(session_ep, md_alloc),
			_managed_ds_map(managed_ds_map),
			_genode_child_resources(genode_child_resources)
		{ }
};

#endif /* _RM_ROOT_H_ */
