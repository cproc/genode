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

#include <base/printf.h>

#include "qnitpickercontext.h"

#include <QtPlatformSupport/private/qeglconvenience_p.h>

QT_BEGIN_NAMESPACE

QNitpickerGLContext::QNitpickerGLContext(QOpenGLContext *context)
    : QPlatformOpenGLContext()
{
	PDBG("called");

	EGLBoolean res = eglBindAPI(EGL_OPENGL_ES_API);
	if (res != EGL_TRUE)
    	PERR("Failed to set EGL API, err=%d", eglGetError());

	QSurfaceFormat platformFormat;
	platformFormat.setDepthBufferSize(16);
	platformFormat.setRedBufferSize(5);
	platformFormat.setGreenBufferSize(6);
	platformFormat.setBlueBufferSize(5);
	//m_depth = 16;
	//m_format = QImage::Format_RGB16;

    EGLConfig config = q_configFromGLFormat(EGL_NO_DISPLAY, platformFormat);
    if (config == 0)
        qFatal("QNitpickerGLContext: failed to find EGL config");
   
	//Initialize EGLContext
	static EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, context->format().majorVersion(),
		EGL_NONE
	};

    EGLContext eglcontext = eglCreateContext(EGL_NO_DISPLAY, config, 0, contextAttribs);
    if (eglcontext == EGL_NO_CONTEXT) {
        qWarning("QNitpickerGLContext::QNitpickerGLContext(): eglError: %x, this: %p",
                 eglGetError(), this);
        eglcontext = 0;
    }
#	
#if 0
    EGLDisplay display = m_device->eglDisplay();
    EGLConfig config = q_configFromGLFormat(display, QKmsScreen::tweakFormat(context->format()), true);
    m_format = q_glFormatFromConfig(display, config);

    //Initialize EGLContext
    static EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, context->format().majorVersion(),
        EGL_NONE
    };

    m_eglContext = eglCreateContext(display, config, 0, contextAttribs);
    if (m_eglContext == EGL_NO_CONTEXT) {
        qWarning("QKmsContext::QKmsContext(): eglError: %x, this: %p",
                 eglGetError(), this);
        m_eglContext = 0;
    }
#endif
}

#if 0
bool QKmsContext::isValid() const
{
    return m_eglContext != EGL_NO_CONTEXT;
}
#endif

bool QNitpickerGLContext::makeCurrent(QPlatformSurface *surface)
{
PDBG("not implemented yet");

	return false;
#if 0
    Q_ASSERT(surface->surface()->surfaceType() == QSurface::OpenGLSurface);

    EGLDisplay display = m_device->eglDisplay();

    QPlatformWindow *window = static_cast<QPlatformWindow *>(surface);
    QKmsScreen *screen = static_cast<QKmsScreen *> (QPlatformScreen::platformScreenForWindow(window->window()));

    EGLSurface eglSurface = screen->eglSurface();

    screen->waitForPageFlipComplete();

    bool ok = eglMakeCurrent(display, eglSurface, eglSurface, m_eglContext);
    if (!ok)
        qWarning("QKmsContext::makeCurrent(): eglError: %x, this: %p",
                 eglGetError(), this);

    return true;
#endif
}

void QNitpickerGLContext::doneCurrent()
{
PDBG("not implemented yet");

#if 0
    bool ok = eglMakeCurrent(m_device->eglDisplay(), EGL_NO_SURFACE, EGL_NO_SURFACE,
                             EGL_NO_CONTEXT);
    if (!ok)
        qWarning("QKmsContext::doneCurrent(): eglError: %x, this: %p",
                 eglGetError(), this);
#endif
}

void QNitpickerGLContext::swapBuffers(QPlatformSurface *surface)
{
PDBG("not implemented yet");

#if 0
    //Cast context to a window surface and get the screen the context
    //is on and call swapBuffers on that screen.
    QPlatformWindow *window = static_cast<QPlatformWindow *>(surface);
    QKmsScreen *screen = static_cast<QKmsScreen *> (QPlatformScreen::platformScreenForWindow(window->window()));
    screen->swapBuffers();
#endif
}

void (*QNitpickerGLContext::getProcAddress(const QByteArray &procName)) ()
{
PDBG("not implemented yet");

	return 0;
#if 0
    return eglGetProcAddress(procName.data());
#endif
}

#if 0
EGLContext QKmsContext::eglContext() const
{
    return m_eglContext;
}
#endif
QSurfaceFormat QNitpickerGLContext::format() const
{
PDBG("not implemented yet");

	return QSurfaceFormat();
#if 0
    return m_format;
#endif
}

QT_END_NAMESPACE

