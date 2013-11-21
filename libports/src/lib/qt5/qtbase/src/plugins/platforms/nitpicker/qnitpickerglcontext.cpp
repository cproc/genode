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

#define EGL_EGLEXT_PROTOTYPES

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <QtPlatformSupport/private/qeglconvenience_p.h>

#include "qnitpickerplatformwindow.h"

#include "qnitpickerglcontext.h"

#include <QDebug>

QT_BEGIN_NAMESPACE
extern "C" void wait_for_continue();
QNitpickerGLContext::QNitpickerGLContext(QOpenGLContext *context)
    : QPlatformOpenGLContext()
{
	PDBG("called");

qDebug() << "context->format() =" << context->format();	

	if (!eglBindAPI(EGL_OPENGL_API)) {
    	PERR("eglBindAPI() failed");
    }

    _egl_display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (_egl_display == EGL_NO_DISPLAY) {
		PERR("eglGetDisplay() failed");
    }

	int major = -1;
	int minor = -1;
	if (!eglInitialize(_egl_display, &major, &minor)) {
		PERR("eglInitialize() failed");
	}

	PDBG("eglInitialize() returned major: %d, minor: %d", major, minor);

    _egl_config = q_configFromGLFormat(_egl_display, context->format(), false, EGL_PBUFFER_BIT);
    if (_egl_config == 0)
        PERR("Could not find a matching EGL config");

	_format = q_glFormatFromConfig(_egl_display, _egl_config);

	_egl_context = eglCreateContext(_egl_display, _egl_config, EGL_NO_CONTEXT, 0);
    if (_egl_context == EGL_NO_CONTEXT) {
        PERR("eglCreateContext() failed");
    }

	int count;
	EGLScreenMESA screen;
	eglGetScreensMESA(_egl_display, &screen, 1, &count);

	enum { MAX_MODES = 100 };

	EGLModeMESA mode[MAX_MODES];
	if (!eglGetModesMESA(_egl_display, screen, mode, MAX_MODES, &count) || count == 0) {
		PERR("eglGetModesMESA() failed");
	}

	EGLint screenAttribs[10];

	/* build up screenAttribs array */
	int i = 0;
	screenAttribs[i++] = EGL_WIDTH;
	screenAttribs[i++] = 640;
	screenAttribs[i++] = EGL_HEIGHT;
	screenAttribs[i++] = 480;
	screenAttribs[i++] = EGL_NONE;

	_egl_surface = eglCreateScreenSurfaceMESA(_egl_display, _egl_config, screenAttribs);
	if (_egl_surface == EGL_NO_SURFACE) {
		PERR("eglCreateScreenSurfaceMESA() failed");
	}
	
	if (!eglShowScreenSurfaceMESA(_egl_display, screen, _egl_surface, mode[0])) {
		PERR("eglShowScreenSurface() failed");
	}

	//EGLSurface egl_surf;
	//egl_surf = eglCreateWindowSurface(_egl_display, egl_config, , 0);


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
	PDBG("called");

	if (!eglMakeCurrent(_egl_display, _egl_surface, _egl_surface, _egl_context)) {
		PERR("eglMakeCurrent() failed");
	}

#if 0
    const EGLint egl_surface_attrs[] =
    {
        EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
        EGL_NONE
    };  

	EGLSurface egl_surface = eglCreateWindowSurface(_egl_display, _egl_config, 0, egl_surface_attrs);
	if (egl_surface == EGL_NO_SURFACE) {
		PERR("eglCreateWindowSurface() failed");
	}

	if (!eglMakeCurrent(_egl_display, egl_surface, egl_surface, _egl_context)) {
		PERR("eglMakeCurrent() failed");
	}
#endif

//QNitpickerPlatformWindow 
	return true;
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
PDBG("called");
	if (!eglMakeCurrent(_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
		PERR("eglMakeCurrent() failed");
	}
}
extern "C" void wait_for_continue();
void QNitpickerGLContext::swapBuffers(QPlatformSurface *surface)
{
	PDBG("called");
	//wait_for_continue();
	if (!eglSwapBuffers(_egl_display, _egl_surface)) {
		PERR("eglSwapBuffers() failed");
	}
}

void (*QNitpickerGLContext::getProcAddress(const QByteArray &procName)) ()
{
	PDBG("procName = %s, pointer = %p", procName.constData(), eglGetProcAddress(procName.constData())); 
	return static_cast<QFunctionPointer>(eglGetProcAddress(procName.constData()));
}

#if 0
EGLContext QKmsContext::eglContext() const
{
    return m_eglContext;
}
#endif
QSurfaceFormat QNitpickerGLContext::format() const
{
    return _format;
}

QT_END_NAMESPACE

