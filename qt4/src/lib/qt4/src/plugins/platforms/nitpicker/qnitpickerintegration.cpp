/*
 * \brief  QNitpickerIntegration
 * \author Christian Prochaska
 * \date   2013-05-08
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

/* Genode includes */

#include <base/rpc_server.h>
#include <cap_session/connection.h>

/* Qt includes */
#include "qnitpickerintegration.h"
#include "qnitpickerplatformwindow.h"
#include "qnitpickerscreen.h"
#include "qnitpickerwindowsurface.h"
#include "qbasicunixfontdatabase.h"

#include <QtGui/private/qpixmap_raster_p.h>


static Genode::Rpc_entrypoint &_entrypoint()
{
	enum { STACK_SIZE = 2*1024*sizeof(Genode::addr_t) };
	static Genode::Cap_connection cap;
	static Genode::Rpc_entrypoint entrypoint(&cap, STACK_SIZE, "qt_window_ep");
	return entrypoint;
}


QNitpickerIntegration::QNitpickerIntegration()
{
    _screens.append(_nitpicker_screen);
}


bool QNitpickerIntegration::hasCapability(QPlatformIntegration::Capability cap) const
{
	switch (cap) {
		case ThreadedPixmaps: return true;
		default: return QPlatformIntegration::hasCapability(cap);
	}
}


QPixmapData *QNitpickerIntegration::createPixmapData(QPixmapData::PixelType type) const
{
    return new QRasterPixmapData(type);
}


QPlatformWindow *QNitpickerIntegration::createPlatformWindow(QWidget *widget, WId winId) const
{
    Q_UNUSED(winId);
    qDebug() << "QNitpickerIntegration::createPlatformWindow(" << widget << "," << winId << ")";
    return new QNitpickerPlatformWindow(widget, _entrypoint());
}


QWindowSurface *QNitpickerIntegration::createWindowSurface(QWidget *widget, WId winId) const
{
    Q_UNUSED(winId);
    qDebug() << "QNitpickerIntegration::createWindowSurface(" << widget << "," << winId << ")";
    return new QNitpickerWindowSurface(widget);
}


QPlatformFontDatabase *QNitpickerIntegration::fontDatabase() const
{
    static QBasicUnixFontDatabase db;
    return &db;
}
