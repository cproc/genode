/*
 * \brief  Intel framebuffer driver session component
 * \author Stefan Kalkowski
 * \date   2015-10-16
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef __COMPONENT_H__
#define __COMPONENT_H__

/* Genode includes */
#include <base/rpc_server.h>
#include <root/component.h>
#include <dataspace/capability.h>
#include <framebuffer_session/framebuffer_session.h>
#include <timer_session/connection.h>

namespace Framebuffer {
	class Session_component;
	class Root;

	extern Root * root;
	Genode::Dataspace_capability framebuffer_dataspace();
}


class Framebuffer::Session_component : public Genode::Rpc_object<Session>
{
	private:

		int                               _height;
		int                               _width;
		Genode::Signal_context_capability _mode_sigh;
		Timer::Connection                 _timer;

	public:

		Session_component() : _height(0), _width(0) {}

		void update(int height, int width)
		{
			_height = height;
			_width  = width;
			if (_mode_sigh.valid())
				Genode::Signal_transmitter(_mode_sigh).submit();
		}


		/***********************************
		 ** Framebuffer session interface **
		 ***********************************/

		Genode::Dataspace_capability dataspace() override {
			return framebuffer_dataspace(); }

		Mode mode() const override {
			return Mode(_width, _height, Mode::RGB565); }

		void mode_sigh(Genode::Signal_context_capability sigh) override {
			_mode_sigh = sigh; }

		void sync_sigh(Genode::Signal_context_capability sigh) override
		{
			_timer.sigh(sigh);
			_timer.trigger_periodic(10*1000);
		}

		void refresh(int x, int y, int w, int h) override { }
};


class Framebuffer::Root
: public Genode::Root_component<Framebuffer::Session_component,
                                Genode::Single_client>
{
	private:

		Session_component _single_session;

		Session_component *_create_session(const char *args) override {
			return &_single_session; }

	public:

		Root(Genode::Rpc_entrypoint *session_ep, Genode::Allocator *md_alloc)
		: Genode::Root_component<Session_component,
		                         Genode::Single_client>(session_ep, md_alloc) {}

		void update(int height, int width) {
			_single_session.update(height, width); }
};

#endif /* __COMPONENT_H__ */
