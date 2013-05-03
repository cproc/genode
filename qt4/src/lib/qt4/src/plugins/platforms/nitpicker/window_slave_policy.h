/*
 * \brief   Slave policy for an undecorated window
 * \author  Christian Prochaska
 * \date    2013-05-08
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _UNDECORATED_WINDOW_POLICY_H_
#define _UNDECORATED_WINDOW_POLICY_H_

/* Qt4 includes */
#include <QDebug>
#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>

/* Genode includes */
#include <cap_session/connection.h>
#include <framebuffer_session/client.h>
#include <os/slave.h>

using namespace Genode;

class Window_slave_policy : /*public QObject,*/ public Genode::Slave_policy
{
	//Q_OBJECT

	private:

		Framebuffer::Session_capability  _framebuffer_session;
		Genode::Lock                     _framebuffer_ready_lock;
		unsigned char                   *_framebuffer;

		QByteArray _config_byte_array;

		const char *_config(int xpos, int ypos, int width, int height)
		{
			QDomDocument config_doc;

			QDomElement config_node = config_doc.createElement("config");
			config_doc.appendChild(config_node);

			config_node.setAttribute("xpos", QString::number(xpos));
			config_node.setAttribute("ypos", QString::number(ypos));
			config_node.setAttribute("width", QString::number(width));
			config_node.setAttribute("height", QString::number(height));
			config_node.setAttribute("animate", "off");
			//config_node.setAttribute("refresh_rate", "25");

			_config_byte_array = config_doc.toByteArray(4);

			qDebug() << _config_byte_array;

			return _config_byte_array.constData();
		}

	protected:

		const char **_permitted_services() const
		{
			static const char *permitted_services[] = {
				"CAP", "LOG", "RM", "ROM", "SIGNAL",
				"Timer", "Nitpicker", 0 };

			return permitted_services;
		};

	public:

		Window_slave_policy(Genode::Rpc_entrypoint &ep)
		: Genode::Slave_policy("liquid_fb", ep, env()->ram_session()),
		  _framebuffer_ready_lock(Genode::Lock::LOCKED),
		  _framebuffer(0)
		{
			configure(_config(0, 0, 550, 370));
		}


		~Window_slave_policy()
		{
			if (_framebuffer)
				Genode::env()->rm_session()->detach(_framebuffer);
		}

		bool announce_service(const char            *name,
                              Root_capability        root,
                              Allocator             *alloc,
                              Server                *server)
		{
			PDBG("name = %s", name);

			if (Genode::strcmp(name, "Input") == 0) {
				return true;
			}

			if (Genode::strcmp(name, "Framebuffer") == 0) {
				Genode::Session_capability session_cap =
					Genode::Root_client(root).session("ram_quota=8K");
				_framebuffer_session =
					Genode::static_cap_cast<Framebuffer::Session>(session_cap);
				Framebuffer::Session_client session_client(_framebuffer_session);
				_framebuffer = Genode::env()->rm_session()->attach(session_client.dataspace());

				Framebuffer::Mode const scr_mode = session_client.mode();
				PDBG("_framebuffer = %p, width = %d, height = %d", _framebuffer, scr_mode.width(), scr_mode.height());

				_framebuffer_ready_lock.unlock();
				return true;
			}

			return Slave_policy::announce_service(name, root, alloc, server);
		}


		void wait_for_service_announcements()
		{
			Lock_guard<Lock> framebuffer_ready_lock_guard(_framebuffer_ready_lock);
		}


		void geometry(int x, int y, int width, int height)
		{
			Framebuffer::Session_client session_client(_framebuffer_session);

			Signal_context signal_context;
			Signal_receiver signal_receiver;
			Signal_context_capability signal_context_capability =
				signal_receiver.manage(&signal_context);

			session_client.mode_sigh(signal_context_capability);

			configure(_config(x, y, width, height));

			PDBG("waiting for mode change signal");

			//signal_receiver.wait_for_signal();

			//PDBG("received mode change signal");

			session_client.mode_sigh(Signal_context_capability());
		}


		unsigned char *framebuffer()
		{
			Framebuffer::Session_client session_client(_framebuffer_session);

			if (_framebuffer) {
				Genode::env()->rm_session()->detach(_framebuffer);
				session_client.release();
			}

			_framebuffer = Genode::env()->rm_session()->attach(session_client.dataspace());

			Framebuffer::Mode const scr_mode = session_client.mode();
			PDBG("_framebuffer = %p, width = %d, height = %d", _framebuffer, scr_mode.width(), scr_mode.height());

			return _framebuffer;
		}


		void refresh(int x, int y, int w, int h)
		{
			Framebuffer::Session_client session_client(_framebuffer_session);
			session_client.refresh(x, y, w, h);
		}
};

#endif /* _UNDECORATED_WINDOW_POLICY_H_ */
