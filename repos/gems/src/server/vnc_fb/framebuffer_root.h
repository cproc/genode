/*
 * \brief   Framebuffer root
 * \author  Christian Prochaska
 * \date    2012-04-10
 */

/*
 * Copyright (C) 2012-2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */


#ifndef _FRAMEBUFFER_ROOT_H_
#define _FRAMEBUFFER_ROOT_H_

/* Genode includes */
#include <root/component.h>

#include "framebuffer_session_component.h"

namespace Framebuffer {

	/**
	 * Shortcut for single-client root component
	 */
	typedef Genode::Root_component<Session_component, Genode::Single_client> Root_component;


	class Root : public Root_component
	{
		protected:

			Session_component *_create_session(const char *args)
			{
				return new (md_alloc())
				  Session_component(args);
			}

		public:

			Root(Genode::Rpc_entrypoint *session_ep,
			     Genode::Allocator      *md_alloc)
			: Root_component(session_ep, md_alloc) { }

	};

}

#endif /* _FRAMEBUFFER_ROOT_H_ */
