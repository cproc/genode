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

#include "qthread.h"

#include "qplatformdefs.h"

#include <private/qcoreapplication_p.h>
#ifdef Q_OS_GENODE
#include <private/qeventdispatcher_genode_p.h>
#else
#if !defined(QT_NO_GLIB)
#  include "../kernel/qeventdispatcher_glib_p.h"
#endif
#include <private/qeventdispatcher_unix_p.h>
#endif /* Q_OS_GENODE */

#include "qthreadstorage.h"

#include "qthread_p.h"

#include "qdebug.h"

#ifndef Q_OS_GENODE
#include <sched.h>
#include <errno.h>

#ifdef Q_OS_BSD4
#include <sys/sysctl.h>
#endif

#if defined(Q_OS_MAC)
# ifdef qDebug
#   define old_qDebug qDebug
#   undef qDebug
# endif
# include <CoreServices/CoreServices.h>

# ifdef old_qDebug
#   undef qDebug
#   define qDebug QT_NO_QDEBUG_MACRO
#   undef old_qDebug
# endif
#endif
#endif /* Q_OS_GENODE */

QT_BEGIN_NAMESPACE

#ifndef QT_NO_THREAD

#ifdef Q_OS_GENODE

#include <base/env.h>
#include <timer_session/connection.h>

QHash<Qt::HANDLE, struct QThreadPrivate::tls_struct> QThreadPrivate::tls;

#else

static pthread_once_t current_thread_data_once = PTHREAD_ONCE_INIT;
static pthread_key_t current_thread_data_key;

static void destroy_current_thread_data(void *p)
{
    // POSIX says the value in our key is set to zero before calling
    // this destructor function, so we need to set it back to the
    // right value...
    pthread_setspecific(current_thread_data_key, p);
    reinterpret_cast<QThreadData *>(p)->deref();
    // ... but we must reset it to zero before returning so we aren't
    // called again (POSIX allows implementations to call destructor
    // functions repeatedly until all values are zero)
    pthread_setspecific(current_thread_data_key, 0);
}

static void create_current_thread_data_key()
{
    pthread_key_create(&current_thread_data_key, destroy_current_thread_data);
}

#endif /* Q_OS_GENODE */

QThreadData *QThreadData::current()
{
#ifdef Q_OS_GENODE
	// create an entry for the thread-specific data of the current thread
	if (!QThreadPrivate::tls.contains(QThread::currentThreadId())) {
		QThreadPrivate::tls.insert(QThread::currentThreadId(),
                               (struct QThreadPrivate::tls_struct){0, true});
	}
    QThreadData *data = QThreadPrivate::tls.value(QThread::currentThreadId()).data;
#else
    pthread_once(&current_thread_data_once, create_current_thread_data_key);

    QThreadData *data = reinterpret_cast<QThreadData *>(pthread_getspecific(current_thread_data_key));
#endif /* Q_OS_GENODE */
    if (!data) {
        void *a;
        if (QInternal::activateCallbacks(QInternal::AdoptCurrentThread, &a)) {
            QThread *adopted = static_cast<QThread*>(a);
            Q_ASSERT(adopted);
            data = QThreadData::get2(adopted);
#ifdef Q_OS_GENODE
            struct QThreadPrivate::tls_struct tls_elem = 
                QThreadPrivate::tls.value(QThread::currentThreadId());
            tls_elem.data = data;
            QThreadPrivate::tls.insert(QThread::currentThreadId(), tls_elem);
#else
            pthread_setspecific(current_thread_data_key, data);
#endif /* Q_OS_GENODE */
            adopted->d_func()->running = true;
            adopted->d_func()->finished = false;
            static_cast<QAdoptedThread *>(adopted)->init();
        } else {
            data = new QThreadData;
#ifdef Q_OS_GENODE
            struct QThreadPrivate::tls_struct tls_elem = 
            QThreadPrivate::tls.value(QThread::currentThreadId());
            tls_elem.data = data;
            QThreadPrivate::tls.insert(QThread::currentThreadId(), tls_elem);
#else
            pthread_setspecific(current_thread_data_key, data);
#endif /* Q_OS_GENODE */
            data->thread = new QAdoptedThread(data);
#ifdef Q_OS_GENODE
            data->threadId = QThread::currentThreadId();
#endif /* Q_OS_GENODE */
            data->deref();
        }
        if (!QCoreApplicationPrivate::theMainThread)
            QCoreApplicationPrivate::theMainThread = data->thread;
    }
    return data;
}


void QAdoptedThread::init()
{
#ifdef Q_OS_GENODE
    d_func()->thread_id = QThread::currentThreadId();
#else
    d_func()->thread_id = pthread_self();
#endif /* Q_OS_GENODE */
}

/*
   QThreadPrivate
*/

#if defined(Q_C_CALLBACKS)
extern "C" {
#endif

typedef void*(*QtThreadCallback)(void*);

#if defined(Q_C_CALLBACKS)
}
#endif

#endif // QT_NO_THREAD

void QThreadPrivate::createEventDispatcher(QThreadData *data)
{
#ifdef Q_OS_GENODE
    data->eventDispatcher = new QEventDispatcherGenode;
#else
#if !defined(QT_NO_GLIB)
    if (qgetenv("QT_NO_GLIB").isEmpty()
        && qgetenv("QT_NO_THREADED_GLIB").isEmpty()
        && QEventDispatcherGlib::versionSupported())
        data->eventDispatcher = new QEventDispatcherGlib;
    else
#endif
        data->eventDispatcher = new QEventDispatcherUNIX;
#endif /* Q_OS_GENODE */
    data->eventDispatcher->startingUp();
}

#ifndef QT_NO_THREAD

void *QThreadPrivate::start(void *arg)
{
#ifndef Q_OS_GENODE
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    pthread_cleanup_push(QThreadPrivate::finish, arg);
#endif /* Q_OS_GENODE */

    QThread *thr = reinterpret_cast<QThread *>(arg);
    QThreadData *data = QThreadData::get2(thr);

#ifdef Q_OS_GENODE
    thr->d_func()->thread_id = QThread::currentThreadId();
    QThread::setTerminationEnabled(false);

    // create an entry for the thread-specific data of the current thread
    if (!QThreadPrivate::tls.contains(QThread::currentThreadId())) {
        QThreadPrivate::tls.insert(QThread::currentThreadId(),
                                   (struct QThreadPrivate::tls_struct){0, true});
    }

    struct QThreadPrivate::tls_struct tls_elem = 
        QThreadPrivate::tls.value(QThread::currentThreadId());
    tls_elem.data = data;
    QThreadPrivate::tls.insert(QThread::currentThreadId(), tls_elem);
#else
    pthread_once(&current_thread_data_once, create_current_thread_data_key);
    pthread_setspecific(current_thread_data_key, data);
#endif /* Q_OS_GENODE */

    data->ref();
    data->quitNow = false;

    // ### TODO: allow the user to create a custom event dispatcher
    createEventDispatcher(data);

    emit thr->started();
#ifdef Q_OS_GENODE
    QThread::setTerminationEnabled(true);
#else
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();
#endif /* Q_OS_GENODE */
    thr->run();
#ifndef Q_OS_GENODE
    pthread_cleanup_pop(1);
#endif /* Q_OS_GENODE */
    return 0;
}

void QThreadPrivate::finish(void *arg)
{
    QThread *thr = reinterpret_cast<QThread *>(arg);
    QThreadPrivate *d = thr->d_func();
    QMutexLocker locker(&d->mutex);

    d->priority = QThread::InheritPriority;
    d->running = false;
    d->finished = true;
    if (d->terminated)
        emit thr->terminated();
    d->terminated = false;
    emit thr->finished();

    if (d->data->eventDispatcher) {
        d->data->eventDispatcher->closingDown();
        QAbstractEventDispatcher *eventDispatcher = d->data->eventDispatcher;
        d->data->eventDispatcher = 0;
        delete eventDispatcher;
    }

    void *data = &d->data->tls;
    QThreadStorageData::finish((void **)data);

#ifdef Q_OS_GENODE
    QThreadPrivate::tls.remove(QThread::currentThreadId());
#endif /* Q_OS_GENODE */

    d->thread_id = 0;
    d->thread_done.wakeAll();
}




/**************************************************************************
 ** QThread
 *************************************************************************/

Qt::HANDLE QThread::currentThreadId()
{
#ifdef Q_OS_GENODE
    return (Qt::HANDLE)QThreadPrivate::Genode_thread::myself();
#else
    // requires a C cast here otherwise we run into trouble on AIX
    return (Qt::HANDLE)pthread_self();
#endif /* Q_OS_GENODE */
}

#ifndef Q_OS_GENODE
#if defined(QT_LINUXBASE) && !defined(_SC_NPROCESSORS_ONLN)
// LSB doesn't define _SC_NPROCESSORS_ONLN.
#  define _SC_NPROCESSORS_ONLN 84
#endif
#endif /* Q_OS_GENODE */

int QThread::idealThreadCount()
{
    int cores = -1;

#if defined(Q_OS_MAC)
    // Mac OS X
    cores = MPProcessorsScheduled();
#elif defined(Q_OS_HPUX)
    // HP-UX
    struct pst_dynamic psd;
    if (pstat_getdynamic(&psd, sizeof(psd), 1, 0) == -1) {
        perror("pstat_getdynamic");
        cores = -1;
    } else {
        cores = (int)psd.psd_proc_cnt;
    }
#elif defined(Q_OS_BSD4)
    // FreeBSD, OpenBSD, NetBSD, BSD/OS
    size_t len = sizeof(cores);
    int mib[2];
    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    if (sysctl(mib, 2, &cores, &len, NULL, 0) != 0) {
        perror("sysctl");
        cores = -1;
    }
#elif defined(Q_OS_IRIX)
    // IRIX
    cores = (int)sysconf(_SC_NPROC_ONLN);
#elif defined(Q_OS_INTEGRITY)
    // ### TODO - how to get the amound of CPUs on INTEGRITY?
#elif defined(Q_OS_GENODE)
	cores = 1;
#else
    // the rest: Linux, Solaris, AIX, Tru64
    cores = (int)sysconf(_SC_NPROCESSORS_ONLN);
#endif

    return cores;
}

void QThread::yieldCurrentThread()
{
#ifndef Q_OS_GENODE
    sched_yield();
#endif /* Q_OS_GENODE */
}

#ifndef Q_OS_GENODE
/*  \internal
    helper function to do thread sleeps, since usleep()/nanosleep()
    aren't reliable enough (in terms of behavior and availability)
*/
static void thread_sleep(struct timespec *ti)
{
    pthread_mutex_t mtx;
    pthread_cond_t cnd;

    pthread_mutex_init(&mtx, 0);
    pthread_cond_init(&cnd, 0);

    pthread_mutex_lock(&mtx);
    (void) pthread_cond_timedwait(&cnd, &mtx, ti);
    pthread_mutex_unlock(&mtx);

    pthread_cond_destroy(&cnd);
    pthread_mutex_destroy(&mtx);
}
#endif /* Q_OS_GENODE */

void QThread::sleep(unsigned long secs)
{
#ifdef Q_OS_GENODE
	static Timer::Connection timer;
	timer.msleep(secs * 1000);
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    struct timespec ti;
    ti.tv_sec = tv.tv_sec + secs;
    ti.tv_nsec = (tv.tv_usec * 1000);
    thread_sleep(&ti);
#endif /* Q_OS_GENODE */
}

void QThread::msleep(unsigned long msecs)
{
#ifdef Q_OS_GENODE
	static Timer::Connection timer;
	timer.msleep(msecs);
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    struct timespec ti;

    ti.tv_nsec = (tv.tv_usec + (msecs % 1000) * 1000) * 1000;
    ti.tv_sec = tv.tv_sec + (msecs / 1000) + (ti.tv_nsec / 1000000000);
    ti.tv_nsec %= 1000000000;
    thread_sleep(&ti);
#endif /* Q_OS_GENODE */
}

void QThread::usleep(unsigned long usecs)
{
#ifdef Q_OS_GENODE
	static Timer::Connection timer;
	timer.msleep(usecs / 1000);
#else
    struct timeval tv;
    gettimeofday(&tv, 0);
    struct timespec ti;

    ti.tv_nsec = (tv.tv_usec + (usecs % 1000000)) * 1000;
    ti.tv_sec = tv.tv_sec + (usecs / 1000000) + (ti.tv_nsec / 1000000000);
    ti.tv_nsec %= 1000000000;
    thread_sleep(&ti);
#endif /* Q_OS_GENODE */
}

void QThread::start(Priority priority)
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);
    if (d->running)
        return;

    d->running = true;
    d->finished = false;
    d->terminated = false;

#ifndef Q_OS_GENODE
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
#endif /* Q_OS_GENODE */

    d->priority = priority;

#ifdef Q_OS_GENODE

    d->genode_thread = new QThreadPrivate::Genode_thread(this);

    if (d->genode_thread) {

        if (d->stackSize > 0) {
            if (!d->genode_thread->set_stack_size(d->stackSize)) {
                qWarning("QThread::start: Thread stack size error");

                // we failed to set the stacksize, and as the documentation states,
                // the thread will fail to run...
                d->running = false;
                d->finished = false;
                return;
            }
        }

        d->genode_thread->start();

    } else {
        qWarning("QThread::start: Thread creation error");

        d->running = false;
        d->finished = false;
        d->thread_id = 0;
    }

#else

#if defined(Q_OS_DARWIN) || !defined(Q_OS_OPENBSD) && defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING-0 >= 0)
    switch (priority) {
    case InheritPriority:
        {
            pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
            break;
        }

    default:
        {
            int sched_policy;
            if (pthread_attr_getschedpolicy(&attr, &sched_policy) != 0) {
                // failed to get the scheduling policy, don't bother
                // setting the priority
                qWarning("QThread::start: Cannot determine default scheduler policy");
                break;
            }

            int prio_min = sched_get_priority_min(sched_policy);
            int prio_max = sched_get_priority_max(sched_policy);
            if (prio_min == -1 || prio_max == -1) {
                // failed to get the scheduling parameters, don't
                // bother setting the priority
                qWarning("QThread::start: Cannot determine scheduler priority range");
                break;
            }

            int prio;
            switch (priority) {
            case IdlePriority:
                prio = prio_min;
                break;

            case TimeCriticalPriority:
                prio = prio_max;
                break;

            default:
                // crudely scale our priority enum values to the prio_min/prio_max
                prio = (priority * (prio_max - prio_min) / TimeCriticalPriority) + prio_min;
                prio = qMax(prio_min, qMin(prio_max, prio));
                break;
            }

            sched_param sp;
            sp.sched_priority = prio;

            if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0
                || pthread_attr_setschedpolicy(&attr, sched_policy) != 0
                || pthread_attr_setschedparam(&attr, &sp) != 0) {
                // could not set scheduling hints, fallback to inheriting them
                pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
            }
            break;
        }
    }
#endif // _POSIX_THREAD_PRIORITY_SCHEDULING

    if (d->stackSize > 0) {
#if defined(_POSIX_THREAD_ATTR_STACKSIZE) && (_POSIX_THREAD_ATTR_STACKSIZE-0 > 0)
        int code = pthread_attr_setstacksize(&attr, d->stackSize);
#else
        int code = ENOSYS; // stack size not supported, automatically fail
#endif // _POSIX_THREAD_ATTR_STACKSIZE

        if (code) {
            qWarning("QThread::start: Thread stack size error: %s",
                     qPrintable(qt_error_string(code)));

            // we failed to set the stacksize, and as the documentation states,
            // the thread will fail to run...
            d->running = false;
            d->finished = false;
            return;
        }
    }

    int code =
        pthread_create(&d->thread_id, &attr, QThreadPrivate::start, this);
    if (code == EPERM) {
        // caller does not have permission to set the scheduling
        // parameters/policy
        pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
        code =
            pthread_create(&d->thread_id, &attr, QThreadPrivate::start, this);
    }

    pthread_attr_destroy(&attr);

    if (code) {
        qWarning("QThread::start: Thread creation error: %s", qPrintable(qt_error_string(code)));

        d->running = false;
        d->finished = false;
        d->thread_id = 0;
    }

#endif /* Q_OS_GENODE */

}

void QThread::terminate()
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);

#ifdef Q_OS_GENODE
    if (QThreadPrivate::tls.value(QThread::currentThreadId()).termination_enabled) {

        if (d->genode_thread) {
            delete d->genode_thread;
            d->genode_thread = 0;
        }

        d->terminated = true;
        d->running = false;
    }
#else
    if (!d->thread_id)
        return;

    int code = pthread_cancel(d->thread_id);
    if (code) {
        qWarning("QThread::start: Thread termination error: %s",
                 qPrintable(qt_error_string((code))));
    } else {
        d->terminated = true;
    }
#endif /* Q_OS_GENODE */
}

#ifdef Q_OS_GENODE
static inline void join_and_delete_genode_thread(QThreadPrivate *d)
{
    if (d->genode_thread) {
        d->genode_thread->join();
        delete d->genode_thread;
        d->genode_thread = 0;
    }
}
#endif /* Q_OS_GENODE */

bool QThread::wait(unsigned long time)
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);

#ifdef Q_OS_GENODE
    if (d->thread_id == QThread::currentThreadId()) {
#else
    if (d->thread_id == pthread_self()) {
#endif /* Q_OS_GENODE */
        qWarning("QThread::wait: Thread tried to wait on itself");
        return false;
    }

    if (d->finished || !d->running) {
#ifdef Q_OS_GENODE
        join_and_delete_genode_thread(d);
#endif /* Q_OS_GENODE */
        return true;
    }

    while (d->running) {
        if (!d->thread_done.wait(locker.mutex(), time))
            return false;
    }

#ifdef Q_OS_GENODE
    join_and_delete_genode_thread(d);
#endif /* Q_OS_GENODE */

    return true;
}

void QThread::setTerminationEnabled(bool enabled)
{
    Q_ASSERT_X(currentThread() != 0, "QThread::setTerminationEnabled()",
               "Current thread was not started with QThread.");
#ifdef Q_OS_GENODE
    struct QThreadPrivate::tls_struct tls_elem = 
        QThreadPrivate::tls.value(QThread::currentThreadId());
    tls_elem.termination_enabled = enabled;
    QThreadPrivate::tls.insert(QThread::currentThreadId(), tls_elem);
#else
    pthread_setcancelstate(enabled ? PTHREAD_CANCEL_ENABLE : PTHREAD_CANCEL_DISABLE, NULL);
    if (enabled)
        pthread_testcancel();
#endif /* Q_OS_GENODE */
}

void QThread::setPriority(Priority priority)
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);
    if (!d->running) {
        qWarning("QThread::setPriority: Cannot set priority, thread is not running");
        return;
    }

    d->priority = priority;

#ifndef Q_OS_GENODE

    // copied from start() with a few modifications:

#if defined(Q_OS_DARWIN) || !defined(Q_OS_OPENBSD) && defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING-0 >= 0)
    int sched_policy;
    sched_param param;

    if (pthread_getschedparam(d->thread_id, &sched_policy, &param) != 0) {
        // failed to get the scheduling policy, don't bother setting
        // the priority
        qWarning("QThread::setPriority: Cannot get scheduler parameters");
        return;
    }

    int prio_min = sched_get_priority_min(sched_policy);
    int prio_max = sched_get_priority_max(sched_policy);
    if (prio_min == -1 || prio_max == -1) {
        // failed to get the scheduling parameters, don't
        // bother setting the priority
        qWarning("QThread::setPriority: Cannot determine scheduler priority range");
        return;
    }

    int prio;
    switch (priority) {
    case InheritPriority:
        qWarning("QThread::setPriority: Argument cannot be InheritPriority");
        return;

    case IdlePriority:
        prio = prio_min;
        break;

    case TimeCriticalPriority:
        prio = prio_max;
        break;

    default:
        // crudely scale our priority enum values to the prio_min/prio_max
        prio = (priority * (prio_max - prio_min) / TimeCriticalPriority) + prio_min;
        prio = qMax(prio_min, qMin(prio_max, prio));
        break;
    }

    param.sched_priority = prio;
    pthread_setschedparam(d->thread_id, sched_policy, &param);
#endif

#endif /* Q_OS_GENODE */
}

#endif // QT_NO_THREAD

QT_END_NAMESPACE

