/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qplatformdefs.h"
#include "qwaitcondition.h"
#include "qmutex.h"
#include "qreadwritelock.h"
#include "qatomic.h"
#include "qstring.h"

#include "qmutex_p.h"
#include "qreadwritelock_p.h"

#include <errno.h>

#ifdef Q_OS_GENODE
#include <os/timed_semaphore.h>
#endif /* Q_OS_GENODE */

#ifndef QT_NO_THREAD

QT_BEGIN_NAMESPACE

#ifndef Q_OS_GENODE
static void report_error(int code, const char *where, const char *what)
{
    if (code != 0)
        qWarning("%s: %s failure: %s", where, what, qPrintable(qt_error_string(code)));
}
#endif /* Q_OS_GENODE */



struct QWaitConditionPrivate {
#ifdef Q_OS_GENODE
    Genode::Lock mutex;
    Genode::Timed_semaphore sem;
#else
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int waiters;
    int wakeups;

    bool wait(unsigned long time)
    {
        int code;
        forever {
            if (time != ULONG_MAX) {
                struct timeval tv;
                gettimeofday(&tv, 0);

                timespec ti;
                ti.tv_nsec = (tv.tv_usec + (time % 1000) * 1000) * 1000;
                ti.tv_sec = tv.tv_sec + (time / 1000) + (ti.tv_nsec / 1000000000);
                ti.tv_nsec %= 1000000000;

                code = pthread_cond_timedwait(&cond, &mutex, &ti);
            } else {
                code = pthread_cond_wait(&cond, &mutex);
            }
            if (code == 0 && wakeups == 0) {
                // many vendors warn of spurios wakeups from
                // pthread_cond_wait(), especially after signal delivery,
                // even though POSIX doesn't allow for it... sigh
                continue;
            }
            break;
        }

        Q_ASSERT_X(waiters > 0, "QWaitCondition::wait", "internal error (waiters)");
        --waiters;
        if (code == 0) {
            Q_ASSERT_X(wakeups > 0, "QWaitCondition::wait", "internal error (wakeups)");
            --wakeups;
        }
        report_error(pthread_mutex_unlock(&mutex), "QWaitCondition::wait()", "mutex unlock");

        if (code && code != ETIMEDOUT)
            report_error(code, "QWaitCondition::wait()", "cv wait");

        return (code == 0);
    }
#endif /* Q_OS_GENODE */

};


QWaitCondition::QWaitCondition()
{
    d = new QWaitConditionPrivate;
#ifndef Q_OS_GENODE
    report_error(pthread_mutex_init(&d->mutex, NULL), "QWaitCondition", "mutex init");
    report_error(pthread_cond_init(&d->cond, NULL), "QWaitCondition", "cv init");
    d->waiters = d->wakeups = 0;
#endif /* Q_OS_GENODE */
}


QWaitCondition::~QWaitCondition()
{
#ifndef Q_OS_GENODE
    report_error(pthread_cond_destroy(&d->cond), "QWaitCondition", "cv destroy");
    report_error(pthread_mutex_destroy(&d->mutex), "QWaitCondition", "mutex destroy");
#endif /* Q_OS_GENODE */
    delete d;
}

void QWaitCondition::wakeOne()
{
#ifdef Q_OS_GENODE
    Genode::Lock::Guard lock_guard(d->mutex);

    if (d->sem.cnt() < 0) {
      d->sem.up();
    }
#else
    report_error(pthread_mutex_lock(&d->mutex), "QWaitCondition::wakeOne()", "mutex lock");
    d->wakeups = qMin(d->wakeups + 1, d->waiters);
    report_error(pthread_cond_signal(&d->cond), "QWaitCondition::wakeOne()", "cv signal");
    report_error(pthread_mutex_unlock(&d->mutex), "QWaitCondition::wakeOne()", "mutex unlock");
#endif /* Q_OS_GENODE */
}

void QWaitCondition::wakeAll()
{
#ifdef Q_OS_GENODE
  Genode::Lock::Guard lock_guard(d->mutex);

  while (d->sem.cnt() < 0) {
    d->sem.up();
  }
#else
    report_error(pthread_mutex_lock(&d->mutex), "QWaitCondition::wakeAll()", "mutex lock");
    d->wakeups = d->waiters;
    report_error(pthread_cond_broadcast(&d->cond), "QWaitCondition::wakeAll()", "cv broadcast");
    report_error(pthread_mutex_unlock(&d->mutex), "QWaitCondition::wakeAll()", "mutex unlock");
#endif /* Q_OS_GENODE */
}

bool QWaitCondition::wait(QMutex *mutex, unsigned long time)
{
    if (! mutex)
        return false;
    if (mutex->d->recursive) {
        qWarning("QWaitCondition: cannot wait on recursive mutexes");
        return false;
    }

#ifndef Q_OS_GENODE
    report_error(pthread_mutex_lock(&d->mutex), "QWaitCondition::wait()", "mutex lock");
    ++d->waiters;
#endif /* Q_OS_GENODE */
    
    mutex->unlock();

#ifdef Q_OS_GENODE
    bool returnValue;
    
    if (time == ULONG_MAX) {
    	/* Timeout: never */
    	d->sem.down();
    	returnValue = true;
    } else {
    	try {
    	  d->sem.down((Genode::Alarm::Time) time);
          returnValue = true;
    	} catch (Genode::Timeout_exception) {
          returnValue = false;
    	}
    }
#else
    bool returnValue = d->wait(time);
#endif /* Q_OS_GENODE */
    mutex->lock();

    return returnValue;
}

#ifndef Q_OS_GENODE
bool QWaitCondition::wait(QReadWriteLock *readWriteLock, unsigned long time)
{
    if (!readWriteLock || readWriteLock->d->accessCount == 0)
        return false;
    if (readWriteLock->d->accessCount < -1) {
        qWarning("QWaitCondition: cannot wait on QReadWriteLocks with recursive lockForWrite()");
        return false;
    }

    report_error(pthread_mutex_lock(&d->mutex), "QWaitCondition::wait()", "mutex lock");
    ++d->waiters;

    int previousAccessCount = readWriteLock->d->accessCount;
    readWriteLock->unlock();

    bool returnValue = d->wait(time);

    if (previousAccessCount < 0)
        readWriteLock->lockForWrite();
    else
        readWriteLock->lockForRead();

    return returnValue;
}
#endif /* Q_OS_GENODE */

QT_END_NAMESPACE

#endif // QT_NO_THREAD
