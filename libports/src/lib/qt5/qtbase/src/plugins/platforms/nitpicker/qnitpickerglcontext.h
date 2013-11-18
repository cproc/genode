/*
 * \brief  QNitpickerGLContext
 * \author Christian Prochaska
 * \date   2013-11-18
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef QNITPICKERGLCONTEXT_H
#define QNITPICKERGLCONTEXT_H

#include <QOpenGLContext>

#include <qpa/qplatformopenglcontext.h>

#include <EGL/egl.h>


QT_BEGIN_NAMESPACE


class QNitpickerGLContext : public QPlatformOpenGLContext
{
	public:

		QNitpickerGLContext(QOpenGLContext *context);

		QSurfaceFormat format() const;

		void swapBuffers(QPlatformSurface *surface);

		bool makeCurrent(QPlatformSurface *surface);
	    
		void doneCurrent();

		void (*getProcAddress(const QByteArray &procName)) ();

#if 0
    bool isValid() const;
#endif


#if 0
    EGLContext eglContext() const;

private:
    EGLContext m_eglContext;
    QSurfaceFormat m_format;

    QKmsDevice *m_device;
#endif
};

QT_END_NAMESPACE

#endif // QNITPICKERGLCONTEXT_H
