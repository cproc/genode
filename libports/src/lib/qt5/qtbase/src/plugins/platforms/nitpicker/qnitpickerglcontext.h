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
	private:

		QSurfaceFormat _format;

		EGLDisplay _egl_display;
		EGLContext _egl_context;
		EGLConfig  _egl_config;
		EGLSurface _egl_surface;

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
#endif
};

QT_END_NAMESPACE

#endif // QNITPICKERGLCONTEXT_H
