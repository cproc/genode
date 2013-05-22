/*
 * \brief  QNitpickerWindowSurface
 * \author Christian Prochaska
 * \date   2013-05-08
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */


#include <QtCore/qdebug.h>
#include <QtGui/private/qapplication_p.h>

#include "qnitpickerplatformwindow.h"

#include "qnitpickerwindowsurface.h"


QT_BEGIN_NAMESPACE

QNitpickerWindowSurface::QNitpickerWindowSurface(QWidget *window)
    : QWindowSurface(window), _framebuffer_changed(false)
{
    //qDebug() << "QNitpickerWindowSurface::QNitpickerWindowSurface:" << (long)this;

    _platform_window = static_cast<QNitpickerPlatformWindow*>(window->platformWindow());
    _platform_window->window_surface(this);
}

QNitpickerWindowSurface::~QNitpickerWindowSurface()
{
}

QPaintDevice *QNitpickerWindowSurface::paintDevice()
{
    qDebug() << "QNitpickerWindowSurface::paintDevice()";
    if (_framebuffer_changed) {
    	PDBG("framebuffer changed");
    	_framebuffer_changed = false;
    	/*
    	 * It can happen that 'resize()' was not called yet, so the size needs
    	 * to be obtained from the window.
    	 */
        QImage::Format format = QApplicationPrivate::platformIntegration()->screens().first()->format();
        QRect geo = _platform_window->geometry();
        _image = QImage(_platform_window->framebuffer(), geo.width(), geo.height(), 2*geo.width(), format);
        qDebug() << "QNitpickerWindowSurface::paintDevice(): w =" << geo.width() << ", h =" << geo.height();
    }
    qDebug() << "QNitpickerWindowSurface::paintDevice() finished";
    return &_image;
}

void QNitpickerWindowSurface::flush(QWidget *widget, const QRegion &region, const QPoint &offset)
{
    Q_UNUSED(widget);
    Q_UNUSED(region);
    Q_UNUSED(offset);

    qDebug() << "QNitpickerWindowSurface::flush()";

    _platform_window->refresh(0, 0, size().width(), size().height());
}

void QNitpickerWindowSurface::resize(const QSize &size)
{
    qDebug() << "QNitpickerWindowSurface::resize:" << this << size;
    QWindowSurface::resize(size);
}

void QNitpickerWindowSurface::framebuffer_changed()
{
	_framebuffer_changed = true;
}

QT_END_NAMESPACE
