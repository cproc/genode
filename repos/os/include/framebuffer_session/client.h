/*
 * \brief  Client-side framebuffer interface
 * \author Norman Feske
 * \date   2006-07-10
 */

/*
 * Copyright (C) 2006-2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

#ifndef _INCLUDE__FRAMEBUFFER_SESSION__CLIENT_H_
#define _INCLUDE__FRAMEBUFFER_SESSION__CLIENT_H_

#include <framebuffer_session/capability.h>
#include <base/rpc_client.h>

namespace Framebuffer { struct Session_client; }


struct Framebuffer::Session_client : Rpc_client<Session>
{
	explicit Session_client(Session_capability session)
	: Rpc_client<Session>(session) { }

	Dataspace_capability dataspace() override {
		return call<Rpc_dataspace>(); }

	Mode mode() const override { return call<Rpc_mode>(); }

	void mode_sigh(Signal_context_capability sigh) override {
		call<Rpc_mode_sigh>(sigh); }

	void sync_sigh(Signal_context_capability sigh) override {
		call<Rpc_sync_sigh>(sigh); }

	void refresh(Rect rect) override { call<Rpc_refresh>(rect); }

	/**
	 * Flush specified pixel region
	 *
	 * \deprecated
	 * \noapi
	 */
	void refresh(int x, int y, int w, int h)
	{
		refresh(Rect { { x, y }, { unsigned(w), unsigned(h) } });
	}

};

#endif /* _INCLUDE__FRAMEBUFFER_SESSION__CLIENT_H_ */
