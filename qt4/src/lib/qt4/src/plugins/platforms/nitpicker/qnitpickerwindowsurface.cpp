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
    : QWindowSurface(window)
{
    //qDebug() << "QNitpickerWindowSurface::QNitpickerWindowSurface:" << (long)this;

    _platform_window = static_cast<QNitpickerPlatformWindow*>(window->platformWindow());
}

QNitpickerWindowSurface::~QNitpickerWindowSurface()
{
}

QPaintDevice *QNitpickerWindowSurface::paintDevice()
{
    //qDebug() << "QNitpickerWindowSurface::paintDevice";
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
    QImage::Format format = QApplicationPrivate::platformIntegration()->screens().first()->format();
    if (_image.size() != size)
        _image = QImage(_platform_window->framebuffer(), size.width(), size.height(), 2*/*size.width()*/1024, format);
}

QT_END_NAMESPACE
