/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qplatformdefs.h"
#include "qmutex.h"
#include "qstring.h"

#ifndef QT_NO_THREAD
#include "qatomic.h"
#include "qmutex_p.h"
#include <errno.h>

#ifndef Q_OS_GENODE
#if defined(Q_OS_VXWORKS) && defined(wakeup)
#undef wakeup
#endif
#endif /* Q_OS_GENODE */

QT_BEGIN_NAMESPACE

#ifndef Q_OS_GENODE
static void report_error(int code, const char *where, const char *what)
{
    if (code != 0)
        qWarning("%s: %s failure: %s", where, what, qPrintable(qt_error_string(code)));
}
#endif /* Q_OS_GENODE */

QMutexPrivate::QMutexPrivate()
#ifndef Q_OS_GENODE
    : wakeup(false)
#endif /* Q_OS_GENODE */
{
#ifndef Q_OS_GENODE
    report_error(pthread_mutex_init(&mutex, NULL), "QMutex", "mutex init");
    report_error(pthread_cond_init(&cond, NULL), "QMutex", "cv init");
#endif /* Q_OS_GENODE */
}

QMutexPrivate::~QMutexPrivate()
{
#ifndef Q_OS_GENODE
    report_error(pthread_cond_destroy(&cond), "QMutex", "cv destroy");
    report_error(pthread_mutex_destroy(&mutex), "QMutex", "mutex destroy");
#endif /* Q_OS_GENODE */
}

bool QMutexPrivate::wait(int timeout)
{
#ifdef Q_OS_GENODE
	bool ret;

	if (timeout == 0) {
		ret = false; /* timed out */
	} else if (timeout < 0) {
		sem.down();
		ret = true; /* woken up */
	} else {
		try {
			sem.down(timeout);
			ret = true;
		} catch(Genode::Timeout_exception) {
			ret = false;
		}
	}
#else
    report_error(pthread_mutex_lock(&mutex), "QMutex::lock", "mutex lock");
    int errorCode = 0;
    while (!wakeup) {
        if (timeout < 0) {
            errorCode = pthread_cond_wait(&cond, &mutex);
        } else {
            struct timeval tv;
            gettimeofday(&tv, 0);
            timespec ti;
            ti.tv_nsec = (tv.tv_usec + (timeout % 1000) * 1000) * 1000;
            ti.tv_sec = tv.tv_sec + (timeout / 1000) + (ti.tv_nsec / 1000000000);
            ti.tv_nsec %= 1000000000;
            errorCode = pthread_cond_timedwait(&cond, &mutex, &ti);
        }
        if (errorCode) {
            if (errorCode == ETIMEDOUT) {
                if (wakeup)
                    errorCode = 0;
                break;
            }
            report_error(errorCode, "QMutex::lock()", "cv wait");
        }
    }
    bool ret = wakeup;
    wakeup = false;
    report_error(pthread_mutex_unlock(&mutex), "QMutex::lock", "mutex unlock");
#endif /* Q_OS_GENODE */
    return ret;
}

void QMutexPrivate::wakeUp() Q_DECL_NOTHROW
{
#ifdef Q_OS_GENODE
	sem.up();
#else
    report_error(pthread_mutex_lock(&mutex), "QMutex::unlock", "mutex lock");
    wakeup = true;
    report_error(pthread_cond_signal(&cond), "QMutex::unlock", "cv signal");
    report_error(pthread_mutex_unlock(&mutex), "QMutex::unlock", "mutex unlock");
#endif /* Q_OS_GENODE */
}


QT_END_NAMESPACE

#endif // QT_NO_THREAD
