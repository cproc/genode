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

QNitpickerGLContext::QNitpickerGLContext(QOpenGLContext *context)
    : QPlatformOpenGLContext()
{
	PDBG("called");

	if (!eglBindAPI(EGL_OPENGL_ES_API)) {
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

	EGLint num_configs;
	if (!eglGetConfigs(_egl_display, 0, 0, &num_configs)) {
		PERR("eglGetConfigs() failed");
	}

	PDBG("eglGetConfigs() found %d configuration(s)", num_configs);

	EGLConfig *configs = new EGLConfig[num_configs];
	if (!eglGetConfigs(_egl_display, configs, num_configs, &num_configs)) {
		PERR("eglGetConfigs() failed");
	}

	PDBG("eglGetConfigs() got %d configuration(s)", num_configs);

	int value;
	for (int i = 0; i < num_configs; i++) {
		PDBG("config #%d", i);
		eglGetConfigAttrib(_egl_display, configs[i], EGL_BUFFER_SIZE, &value);
		PDBG("Buffer Size: %d", value);
		eglGetConfigAttrib(_egl_display, configs[i], EGL_RED_SIZE, &value);
		PDBG("Red Size: %d", value);
		eglGetConfigAttrib(_egl_display, configs[i], EGL_GREEN_SIZE, &value);
		PDBG("Green Size: %d", value);
		eglGetConfigAttrib(_egl_display, configs[i], EGL_BLUE_SIZE, &value);
		PDBG("Blue Size: %d", value);
		eglGetConfigAttrib(_egl_display, configs[i], EGL_ALPHA_SIZE, &value);
		PDBG("Alpha Size: %d", value);
		eglGetConfigAttrib(_egl_display, configs[i], EGL_CONFIG_CAVEAT, &value);
 	 	switch (value) {
			case  EGL_NONE: PDBG("EGL_CONFIG_CAVEAT EGL_NONE"); break;
			case  EGL_SLOW_CONFIG: PDBG("EGL_CONFIG_CAVEAT EGL_SLOW_CONFIG"); break;
		}
 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_CONFIG_ID, &value);
 	 	PDBG("Config ID: %d", value);

 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_DEPTH_SIZE, &value);
 	 	PDBG("Depth size: %d", value);

 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_MAX_PBUFFER_WIDTH, &value);
 	 	PDBG("Max pbuffer width: %d", value);
 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_MAX_PBUFFER_HEIGHT, &value);
 	 	PDBG("Max pbuffer height %d", value);
 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_MAX_PBUFFER_PIXELS, &value);
 	 	PDBG("Max pbuffer pixels: %d", value);
 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_NATIVE_RENDERABLE, &value);
 	 	PDBG("Native renderable: %s", value ? "true" : "false");
 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_NATIVE_VISUAL_ID, &value);
 	 	PDBG("Native visual ID: %d", value);
 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_NATIVE_VISUAL_TYPE, &value);
 	 	PDBG("Native visual type: %d", value);
 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_SAMPLE_BUFFERS, &value);
 	 	PDBG("Sample Buffers: %d", value);
 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_SAMPLES, &value);
 	 	PDBG("Samples: %d", value);
 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_SURFACE_TYPE, &value);
 	 	PDBG("Surface type: %d", value);
 	 	eglGetConfigAttrib(_egl_display, configs[i], EGL_TRANSPARENT_TYPE, &value);

	}	

	/* FIXME */
#if 0
	QSurfaceFormat platformFormat;
	platformFormat.setDepthBufferSize(16);
	platformFormat.setRedBufferSize(5);
	platformFormat.setGreenBufferSize(6);
	platformFormat.setBlueBufferSize(5);
	//m_depth = 16;
	//m_format = QImage::Format_RGB16;

    EGLConfig egl_config = q_configFromGLFormat(_egl_display, platformFormat);
    //if (egl_config == 0)
        //qFatal("QNitpickerGLContext: failed to find EGL config");
#endif

	static EGLint context_attrs[] = {
		EGL_CONTEXT_CLIENT_VERSION, context->format().majorVersion(),
		EGL_NONE
	};

	_egl_config = configs[0];

	//delete[] configs;

	_format = q_glFormatFromConfig(_egl_display, _egl_config);

	_egl_context = eglCreateContext(_egl_display, _egl_config, EGL_NO_CONTEXT, context_attrs);
    if (_egl_context == EGL_NO_CONTEXT) {
        qWarning("QNitpickerGLContext::QNitpickerGLContext(): eglError: %x, this: %p",
                 eglGetError(), this);
        _egl_context = 0;
    }

	EGLSurface egl_surf;
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

	/* TODO: create surface */

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
	if (!eglMakeCurrent(_egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) {
		PERR("eglMakeCurrent() failed");
	}
}

void QNitpickerGLContext::swapBuffers(QPlatformSurface *surface)
{
	PDBG("called");
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

