/*
 * \brief  Signal service on the HW-core
 * \author Martin stein
 * \date   2012-05-05
 */

/*
 * Copyright (C) 2012-2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _CORE__INCLUDE__SIGNAL_SESSION_COMPONENT_H_
#define _CORE__INCLUDE__SIGNAL_SESSION_COMPONENT_H_

/* Genode includes */
#include <signal_session/signal_session.h>
#include <base/rpc_server.h>
#include <base/slab.h>
#include <base/allocator_guard.h>

namespace Genode
{
	/**
	 * Provides the signal service
	 */
	class Signal_session_component : public Rpc_object<Signal_session>
	{
		public:

			enum {
				RECEIVERS_SB_SIZE = 4096,
				CONTEXTS_SB_SIZE  = 4096,
			};

		private:

			Allocator_guard _md_alloc;
			Slab            _receivers_slab;
			Slab            _contexts_slab;
			char            _initial_receivers_sb [RECEIVERS_SB_SIZE];
			char            _initial_contexts_sb  [CONTEXTS_SB_SIZE];

		public:

			/**
			 * Constructor
			 *
			 * \param md         Metadata allocator
			 * \param ram_quota  Amount of RAM quota donated to this session
			 */
			Signal_session_component(Allocator * const md,
			                         size_t const ram_quota);

			/**
			 * Destructor
			 */
			~Signal_session_component();

			/**
			 * Raise the quota of this session by 'q'
			 */
			void upgrade_ram_quota(size_t const q) { _md_alloc.upgrade(q); }

			/******************************
			 ** Signal_session interface **
			 ******************************/

			Signal_receiver_capability alloc_receiver();

			Signal_context_capability
			alloc_context(Signal_receiver_capability const r,
			              unsigned const imprint);
			void free_context(Signal_context_capability context_cap);
	};
}

#endif /* _CORE__INCLUDE__CAP_SESSION_COMPONENT_H_ */

