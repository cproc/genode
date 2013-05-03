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
#include <os/slave.h>

using namespace Genode;

class Window_slave_policy : /*public QObject,*/ public Genode::Slave_policy
{
	//Q_OBJECT

	private:

		Genode::Root_capability _input_root;
		Genode::Root_capability _framebuffer_root;

		QByteArray _config_byte_array;

		const char *_config(int xpos, int ypos, int width, int height)
		{
			QDomDocument config_doc;

			QDomElement config_node = config_doc.createElement("config");
			config_node.setAttribute("xpos", QString::number(xpos));
			config_node.setAttribute("ypos", QString::number(ypos));
			config_node.setAttribute("width", QString::number(width));
			config_node.setAttribute("height", QString::number(height));
			config_node.setAttribute("refresh_rate", "25");
			config_doc.appendChild(config_node);

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
		: Genode::Slave_policy("nit_fb", ep, env()->ram_session())
		{
			PDBG("called");
			configure(_config(0, 0, 0, 0));
		}


		bool announce_service(const char            *name,
                              Root_capability        root,
                              Allocator             *alloc,
                              Server                *server)
		{
			PDBG("name = %s", name);

			if (Genode::strcmp(name, "Input") == 0) {
				_input_root = root;
				return true;
			}

			if (Genode::strcmp(name, "Framebuffer") == 0) {
				_framebuffer_root = root;
				return true;
			}

			return Slave_policy::announce_service(name, root, alloc, server);
		}


		void geometry(int x, int y, int width, int height)
		{
			configure(_config(x, y, width, height));
		}
};

#endif /* _UNDECORATED_WINDOW_POLICY_H_ */
