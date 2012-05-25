/*
 * \brief   Framebuffer session component
 * \author  Christian Prochaska
 * \date    2014-09-21
 */

/*
 * Copyright (C) 2014 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */


#ifndef _FRAMEBUFFER_SESSION_COMPONENT_H_
#define _FRAMEBUFFER_SESSION_COMPONENT_H_

/* Genode includes */
#include <base/rpc_server.h>
#include <framebuffer_session/framebuffer_session.h>

/* libvncserver includes */
#include <rfb/rfb.h>

enum { MODE_W = 1024, MODE_H = 768 };

namespace Framebuffer {

	class Session_component : public Genode::Rpc_object<Session>
	{
		private:

			Genode::Ram_dataspace_capability  _framebuffer_ds_cap;
			char                             *_framebuffer_ds_addr;

		public:

			/**
			 * Constructor
			 */
			Session_component(const char *args);
			~Session_component();

			Genode::Dataspace_capability dataspace();
			Mode mode() const;
			void mode_sigh(Genode::Signal_context_capability sigh_cap);
			void refresh(int x, int y, int w, int h);
			void sync_sigh(Genode::Signal_context_capability sigh_cap);
	};

}

#endif /* _FRAMEBUFFER_SESSION_COMPONENT_H_ */
