/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Copyright (C) 2016 Intel Corporation.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qthread.h"

#include "qplatformdefs.h"

#include <private/qcoreapplication_p.h>
#include <private/qcore_unix_p.h>

#if defined(Q_OS_DARWIN)
#  include <private/qeventdispatcher_cf_p.h>
#else
#  if !defined(QT_NO_GLIB)
#    include "../kernel/qeventdispatcher_glib_p.h"
#  endif
#endif

#include <private/qeventdispatcher_unix_p.h>

#include "qthreadstorage.h"

#include "qthread_p.h"

#include "qdebug.h"

#ifndef Q_OS_GENODE
#include <sched.h>
#include <errno.h>

#ifdef Q_OS_BSD4
#include <sys/sysctl.h>
#endif
#ifdef Q_OS_VXWORKS
#  if (_WRS_VXWORKS_MAJOR > 6) || ((_WRS_VXWORKS_MAJOR == 6) && (_WRS_VXWORKS_MINOR >= 6))
#    include <vxCpuLib.h>
#    include <cpuset.h>
#    define QT_VXWORKS_HAS_CPUSET
#  endif
#endif

#ifdef Q_OS_HPUX
#include <sys/pstat.h>
#endif

#if defined(Q_OS_MAC)
# ifdef qDebug
#   define old_qDebug qDebug
#   undef qDebug
# endif

# ifdef old_qDebug
#   undef qDebug
#   define qDebug QT_NO_QDEBUG_MACRO
#   undef old_qDebug
# endif
#endif

#if defined(Q_OS_LINUX) && !defined(QT_LINUXBASE)
#include <sys/prctl.h>
#endif

#if defined(Q_OS_LINUX) && !defined(SCHED_IDLE)
// from linux/sched.h
# define SCHED_IDLE    5
#endif

#if defined(Q_OS_DARWIN) || !defined(Q_OS_ANDROID) && !defined(Q_OS_OPENBSD) && defined(_POSIX_THREAD_PRIORITY_SCHEDULING) && (_POSIX_THREAD_PRIORITY_SCHEDULING-0 >= 0)
#define QT_HAS_THREAD_PRIORITY_SCHEDULING
#endif

#endif /* Q_OS_GENODE */

QT_BEGIN_NAMESPACE

#ifndef QT_NO_THREAD

#ifdef Q_OS_GENODE

#include <base/env.h>

#include <assert.h>

Genode::Env *QThreadPrivate::_env = nullptr;
QHash<Qt::HANDLE, struct QThreadPrivate::tls_struct> QThreadPrivate::tls;

void initialize_qt_core(Genode::Env &env) { QThreadPrivate::env(env); }

#else

Q_STATIC_ASSERT(sizeof(pthread_t) <= sizeof(Qt::HANDLE));

enum { ThreadPriorityResetFlag = 0x80000000 };

#if defined(Q_OS_LINUX) && defined(__GLIBC__) && (defined(Q_CC_GNU) || defined(Q_CC_INTEL)) && !defined(QT_LINUXBASE)
/* LSB doesn't have __thread, https://lsbbugs.linuxfoundation.org/show_bug.cgi?id=993 */
#define HAVE_TLS
#endif
#if defined(Q_CC_XLC) || defined (Q_CC_SUN)
#define HAVE_TLS
#endif

#ifdef HAVE_TLS
static __thread QThreadData *currentThreadData = 0;
#endif

static pthread_once_t current_thread_data_once = PTHREAD_ONCE_INIT;
static pthread_key_t current_thread_data_key;

static void destroy_current_thread_data(void *p)
{
#if defined(Q_OS_VXWORKS)
    // Calling setspecific(..., 0) sets the value to 0 for ALL threads.
    // The 'set to 1' workaround adds a bit of an overhead though,
    // since this function is called twice now.
    if (p == (void *)1)
        return;
#endif
    // POSIX says the value in our key is set to zero before calling
    // this destructor function, so we need to set it back to the
    // right value...
    pthread_setspecific(current_thread_data_key, p);
    QThreadData *data = static_cast<QThreadData *>(p);
    if (data->isAdopted) {
        QThread *thread = data->thread;
        Q_ASSERT(thread);
        QThreadPrivate *thread_p = static_cast<QThreadPrivate *>(QObjectPrivate::get(thread));
        Q_ASSERT(!thread_p->finished);
        thread_p->finish(thread);
    }
    data->deref();

    // ... but we must reset it to zero before returning so we aren't
    // called again (POSIX allows implementations to call destructor
    // functions repeatedly until all values are zero)
    pthread_setspecific(current_thread_data_key,
#if defined(Q_OS_VXWORKS)
                                                 (void *)1);
#else
                                                 0);
#endif
}

static void create_current_thread_data_key()
{
    pthread_key_create(&current_thread_data_key, destroy_current_thread_data);
}

static void destroy_current_thread_data_key()
{
    pthread_once(&current_thread_data_once, create_current_thread_data_key);
    pthread_key_delete(current_thread_data_key);

    // Reset current_thread_data_once in case we end up recreating
    // the thread-data in the rare case of QObject construction
    // after destroying the QThreadData.
    pthread_once_t pthread_once_init = PTHREAD_ONCE_INIT;
    current_thread_data_once = pthread_once_init;
}
Q_DESTRUCTOR_FUNCTION(destroy_current_thread_data_key)

#endif /* Q_OS_GENODE */

// Utility functions for getting, setting and clearing thread specific data.
static QThreadData *get_thread_data()
{
#ifdef Q_OS_GENODE
	if (!QThreadPrivate::tls.contains(QThread::currentThreadId())) {
		// create an entry for the thread-specific data of the current thread
		QThreadPrivate::tls.insert(QThread::currentThreadId(),
                                   (struct QThreadPrivate::tls_struct){0, true});
	}
    return QThreadPrivate::tls.value(QThread::currentThreadId()).data;
#else
#ifdef HAVE_TLS
    return currentThreadData;
#else
    pthread_once(&current_thread_data_once, create_current_thread_data_key);
    return reinterpret_cast<QThreadData *>(pthread_getspecific(current_thread_data_key));
#endif
#endif /* Q_OS_GENODE */
}

static void set_thread_data(QThreadData *data)
{
#ifdef Q_OS_GENODE
    struct QThreadPrivate::tls_struct tls_elem =
        QThreadPrivate::tls.value(QThread::currentThreadId());
    tls_elem.data = data;
    QThreadPrivate::tls.insert(QThread::currentThreadId(), tls_elem);
#else
#ifdef HAVE_TLS
    currentThreadData = data;
#endif
    pthread_once(&current_thread_data_once, create_current_thread_data_key);
    pthread_setspecific(current_thread_data_key, data);
#endif /* Q_OS_GENODE */
}

static void clear_thread_data()
{
#ifdef Q_OS_GENODE
	QThreadPrivate::tls.remove(QThread::currentThreadId());
#else
#ifdef HAVE_TLS
    currentThreadData = 0;
#endif
    pthread_setspecific(current_thread_data_key, 0);
#endif /* Q_OS_GENODE */
}

template <typename T>
static typename std::enable_if<QTypeInfo<T>::isIntegral, Qt::HANDLE>::type to_HANDLE(T id)
{
    return reinterpret_cast<Qt::HANDLE>(static_cast<intptr_t>(id));
}

template <typename T>
static typename std::enable_if<QTypeInfo<T>::isIntegral, T>::type from_HANDLE(Qt::HANDLE id)
{
    return static_cast<T>(reinterpret_cast<intptr_t>(id));
}

template <typename T>
static typename std::enable_if<QTypeInfo<T>::isPointer, Qt::HANDLE>::type to_HANDLE(T id)
{
    return id;
}

template <typename T>
static typename std::enable_if<QTypeInfo<T>::isPointer, T>::type from_HANDLE(Qt::HANDLE id)
{
    return static_cast<T>(id);
}

void QThreadData::clearCurrentThreadData()
{
    clear_thread_data();
}

QThreadData *QThreadData::current(bool createIfNecessary)
{
    QThreadData *data = get_thread_data();
    if (!data && createIfNecessary) {
        data = new QThreadData;
        QT_TRY {
            set_thread_data(data);
            data->thread = new QAdoptedThread(data);
        } QT_CATCH(...) {
            clear_thread_data();
            data->deref();
            data = 0;
            QT_RETHROW;
        }
        data->deref();
        data->isAdopted = true;
#ifdef Q_OS_GENODE
        data->threadId.store(QThread::currentThreadId());
#else
        data->threadId.store(to_HANDLE(pthread_self()));
#endif /* Q_OS_GENODE */
        if (!QCoreApplicationPrivate::theMainThread)
            QCoreApplicationPrivate::theMainThread = data->thread.load();
    }
    return data;
}


void QAdoptedThread::init()
{
#ifdef Q_OS_GENODE
    d_func()->thread_id = QThread::currentThreadId();
#endif /* Q_OS_GENODE */
}

/*
   QThreadPrivate
*/

extern "C" {
typedef void*(*QtThreadCallback)(void*);
}

#endif // QT_NO_THREAD

void QThreadPrivate::createEventDispatcher(QThreadData *data)
{
#if defined(Q_OS_DARWIN)
    bool ok = false;
    int value = qEnvironmentVariableIntValue("QT_EVENT_DISPATCHER_CORE_FOUNDATION", &ok);
    if (ok && value > 0)
        data->eventDispatcher.storeRelease(new QEventDispatcherCoreFoundation);
    else
        data->eventDispatcher.storeRelease(new QEventDispatcherUNIX);
#elif !defined(QT_NO_GLIB)
    if (qEnvironmentVariableIsEmpty("QT_NO_GLIB")
        && qEnvironmentVariableIsEmpty("QT_NO_THREADED_GLIB")
        && QEventDispatcherGlib::versionSupported())
        data->eventDispatcher.storeRelease(new QEventDispatcherGlib);
    else
        data->eventDispatcher.storeRelease(new QEventDispatcherUNIX);
#else
    data->eventDispatcher.storeRelease(new QEventDispatcherUNIX);
#endif

    data->eventDispatcher.load()->startingUp();
}

#ifndef QT_NO_THREAD

#ifndef Q_OS_GENODE
#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX))
static void setCurrentThreadName(pthread_t threadId, const char *name)
{
#  if defined(Q_OS_LINUX) && !defined(QT_LINUXBASE)
    Q_UNUSED(threadId);
    prctl(PR_SET_NAME, (unsigned long)name, 0, 0, 0);
#  elif defined(Q_OS_MAC)
    Q_UNUSED(threadId);
    pthread_setname_np(name);
#  elif defined(Q_OS_QNX)
    pthread_setname_np(threadId, name);
#  endif
}
#endif
#endif /* Q_OS_GENODE */

void *QThreadPrivate::start(void *arg)
{
#ifndef Q_OS_GENODE
#if !defined(Q_OS_ANDROID)
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
#endif
    pthread_cleanup_push(QThreadPrivate::finish, arg);
#endif /* Q_OS_GENODE */

    QThread *thr = reinterpret_cast<QThread *>(arg);
    QThreadData *data = QThreadData::get2(thr);

    {
        QMutexLocker locker(&thr->d_func()->mutex);

#ifdef Q_OS_GENODE
        thr->d_func()->thread_id = QThread::currentThreadId();
        set_thread_data(data);
        QThread::setTerminationEnabled(false);
#else
        // do we need to reset the thread priority?
        if (int(thr->d_func()->priority) & ThreadPriorityResetFlag) {
            thr->d_func()->setPriority(QThread::Priority(thr->d_func()->priority & ~ThreadPriorityResetFlag));
        }

        data->threadId.store(to_HANDLE(pthread_self()));
        set_thread_data(data);
#endif /* Q_OS_GENODE */

        data->ref();
        data->quitNow = thr->d_func()->exited;
    }

    if (data->eventDispatcher.load()) // custom event dispatcher set?
        data->eventDispatcher.load()->startingUp();
    else
        createEventDispatcher(data);

#ifdef Q_OS_GENODE
    QThread::setTerminationEnabled(true);
    emit thr->started(QThread::QPrivateSignal());
#else
#if (defined(Q_OS_LINUX) || defined(Q_OS_MAC) || defined(Q_OS_QNX))
    {
        // sets the name of the current thread.
        QString objectName = thr->objectName();

        pthread_t thread_id = from_HANDLE<pthread_t>(data->threadId.load());
        if (Q_LIKELY(objectName.isEmpty()))
            setCurrentThreadName(thread_id, thr->metaObject()->className());
        else
            setCurrentThreadName(thread_id, objectName.toLocal8Bit());
    }
#endif

    emit thr->started(QThread::QPrivateSignal());
#if !defined(Q_OS_ANDROID)
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();
#endif
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

    d->isInFinish = true;
    d->priority = QThread::InheritPriority;
    void *data = &d->data->tls;
    locker.unlock();
    emit thr->finished(QThread::QPrivateSignal());
    QCoreApplication::sendPostedEvents(0, QEvent::DeferredDelete);
    QThreadStorageData::finish((void **)data);
    locker.relock();

    QAbstractEventDispatcher *eventDispatcher = d->data->eventDispatcher.load();
    if (eventDispatcher) {
        d->data->eventDispatcher = 0;
        locker.unlock();
        eventDispatcher->closingDown();
        delete eventDispatcher;
        locker.relock();
    }

    d->running = false;
    d->finished = true;
    d->interruptionRequested = false;

#ifdef Q_OS_GENODE
    QThreadPrivate::tls.remove(QThread::currentThreadId());
#endif /* Q_OS_GENODE */

    d->isInFinish = false;
    d->thread_done.wakeAll();
}




/**************************************************************************
 ** QThread
 *************************************************************************/

Qt::HANDLE QThread::currentThreadId() Q_DECL_NOTHROW
{
#ifdef Q_OS_GENODE
    return (Qt::HANDLE)QThreadPrivate::Genode_thread::myself();
#else
    // requires a C cast here otherwise we run into trouble on AIX
    return to_HANDLE(pthread_self());
#endif /* Q_OS_GENODE */
}

#ifndef Q_OS_GENODE
#if defined(QT_LINUXBASE) && !defined(_SC_NPROCESSORS_ONLN)
// LSB doesn't define _SC_NPROCESSORS_ONLN.
#  define _SC_NPROCESSORS_ONLN 84
#endif
#endif /* Q_OS_GENODE */

int QThread::idealThreadCount() Q_DECL_NOTHROW
{
    int cores = 1;

#if defined(Q_OS_HPUX)
    // HP-UX
    struct pst_dynamic psd;
    if (pstat_getdynamic(&psd, sizeof(psd), 1, 0) == -1) {
        perror("pstat_getdynamic");
    } else {
        cores = (int)psd.psd_proc_cnt;
    }
#elif defined(Q_OS_BSD4)
    // FreeBSD, OpenBSD, NetBSD, BSD/OS, OS X, iOS
    size_t len = sizeof(cores);
    int mib[2];
    mib[0] = CTL_HW;
    mib[1] = HW_NCPU;
    if (sysctl(mib, 2, &cores, &len, NULL, 0) != 0) {
        perror("sysctl");
    }
#elif defined(Q_OS_IRIX)
    // IRIX
    cores = (int)sysconf(_SC_NPROC_ONLN);
#elif defined(Q_OS_INTEGRITY)
#if (__INTEGRITY_MAJOR_VERSION >= 10)
    // Integrity V10+ does support multicore CPUs
    Value processorCount;
    if (GetProcessorCount(CurrentTask(), &processorCount) == 0)
        cores = processorCount;
    else
#endif
    // as of aug 2008 Integrity only supports one single core CPU
    cores = 1;
#elif defined(Q_OS_VXWORKS)
    // VxWorks
#  if defined(QT_VXWORKS_HAS_CPUSET)
    cpuset_t cpus = vxCpuEnabledGet();
    cores = 0;

    // 128 cores should be enough for everyone ;)
    for (int i = 0; i < 128 && !CPUSET_ISZERO(cpus); ++i) {
        if (CPUSET_ISSET(cpus, i)) {
            CPUSET_CLR(cpus, i);
            cores++;
        }
    }
#  else
    // as of aug 2008 VxWorks < 6.6 only supports one single core CPU
    cores = 1;
#  endif
#elif defined(Q_OS_GENODE)
	cores = 1;
#else
    // the rest: Linux, Solaris, AIX, Tru64
    cores = (int)sysconf(_SC_NPROCESSORS_ONLN);
    if (cores == -1)
        return 1;
#endif
    return cores;
}

void QThread::yieldCurrentThread()
{
#ifndef Q_OS_GENODE
    sched_yield();
#endif /* Q_OS_GENODE */
}

static timespec makeTimespec(time_t secs, long nsecs)
{
    struct timespec ts;
    ts.tv_sec = secs;
    ts.tv_nsec = nsecs;
    return ts;
}

void QThread::sleep(unsigned long secs)
{
    qt_nanosleep(makeTimespec(secs, 0));
}

void QThread::msleep(unsigned long msecs)
{
    qt_nanosleep(makeTimespec(msecs / 1000, msecs % 1000 * 1000 * 1000));
}

void QThread::usleep(unsigned long usecs)
{
    qt_nanosleep(makeTimespec(usecs / 1000 / 1000, usecs % (1000*1000) * 1000));
}

#ifndef Q_OS_GENODE
#ifdef QT_HAS_THREAD_PRIORITY_SCHEDULING
// Does some magic and calculate the Unix scheduler priorities
// sched_policy is IN/OUT: it must be set to a valid policy before calling this function
// sched_priority is OUT only
static bool calculateUnixPriority(int priority, int *sched_policy, int *sched_priority)
{
#ifdef SCHED_IDLE
    if (priority == QThread::IdlePriority) {
        *sched_policy = SCHED_IDLE;
        *sched_priority = 0;
        return true;
    }
    const int lowestPriority = QThread::LowestPriority;
#else
    const int lowestPriority = QThread::IdlePriority;
#endif
    const int highestPriority = QThread::TimeCriticalPriority;

    int prio_min;
    int prio_max;
#if defined(Q_OS_VXWORKS) && defined(VXWORKS_DKM)
    // for other scheduling policies than SCHED_RR or SCHED_FIFO
    prio_min = SCHED_FIFO_LOW_PRI;
    prio_max = SCHED_FIFO_HIGH_PRI;

    if ((*sched_policy == SCHED_RR) || (*sched_policy == SCHED_FIFO))
#endif
    {
    prio_min = sched_get_priority_min(*sched_policy);
    prio_max = sched_get_priority_max(*sched_policy);
    }

    if (prio_min == -1 || prio_max == -1)
        return false;

    int prio;
    // crudely scale our priority enum values to the prio_min/prio_max
    prio = ((priority - lowestPriority) * (prio_max - prio_min) / highestPriority) + prio_min;
    prio = qMax(prio_min, qMin(prio_max, prio));

    *sched_priority = prio;
    return true;
}
#endif
#endif /* Q_OS_GENODE */

void QThread::start(Priority priority)
{
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);

    if (d->isInFinish)
        d->thread_done.wait(locker.mutex());

    if (d->running)
        return;

#ifdef Q_OS_GENODE
    if (d->finished) {
        /**
          * Thread is to be restarted.
          *
          * Since a new Genode thread is going to be created below, the old one
          * needs to be destroyed first.
          */
        delete d->genode_thread;
        d->genode_thread = 0;
	}
#endif

    d->running = true;
    d->finished = false;
    d->returnCode = 0;
    d->exited = false;
    d->interruptionRequested = false;

#ifndef Q_OS_GENODE
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
#endif /* Q_OS_GENODE */

    d->priority = priority;

#ifdef Q_OS_GENODE

    assert(d->_env != nullptr);

    d->genode_thread = new QThreadPrivate::Genode_thread(*d->_env, this);

    if (d->genode_thread) {

        if (d->stackSize > 0) {
            try {
                d->genode_thread->stack_size(d->stackSize);
            } catch (...) {
                qWarning("QThread::start: Thread stack allocation error");

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

#if defined(QT_HAS_THREAD_PRIORITY_SCHEDULING)
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

            int prio;
            if (!calculateUnixPriority(priority, &sched_policy, &prio)) {
                // failed to get the scheduling parameters, don't
                // bother setting the priority
                qWarning("QThread::start: Cannot determine scheduler priority range");
                break;
            }

            sched_param sp;
            sp.sched_priority = prio;

            if (pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED) != 0
                || pthread_attr_setschedpolicy(&attr, sched_policy) != 0
                || pthread_attr_setschedparam(&attr, &sp) != 0) {
                // could not set scheduling hints, fallback to inheriting them
                // we'll try again from inside the thread
                pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
                d->priority = Priority(priority | ThreadPriorityResetFlag);
            }
            break;
        }
    }
#endif // QT_HAS_THREAD_PRIORITY_SCHEDULING


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

    pthread_t threadId;
    int code = pthread_create(&threadId, &attr, QThreadPrivate::start, this);
    if (code == EPERM) {
        // caller does not have permission to set the scheduling
        // parameters/policy
#if defined(QT_HAS_THREAD_PRIORITY_SCHEDULING)
        pthread_attr_setinheritsched(&attr, PTHREAD_INHERIT_SCHED);
#endif
        code = pthread_create(&threadId, &attr, QThreadPrivate::start, this);
    }
    d->data->threadId.store(to_HANDLE(threadId));

    pthread_attr_destroy(&attr);

    if (code) {
        qWarning("QThread::start: Thread creation error: %s", qPrintable(qt_error_string(code)));

        d->running = false;
        d->finished = false;
        d->data->threadId.store(nullptr);
    }

#endif /* Q_OS_GENODE */

}

void QThread::terminate()
{
#if !defined(Q_OS_ANDROID)
    Q_D(QThread);
    QMutexLocker locker(&d->mutex);

#ifdef Q_OS_GENODE
    if (QThreadPrivate::tls.value(QThread::currentThreadId()).termination_enabled) {

        if (d->genode_thread) {
            delete d->genode_thread;
            d->genode_thread = 0;
        }

        d->exited = true;
        d->running = false;
    }
#else
    if (!d->data->threadId.load())
        return;

    int code = pthread_cancel(from_HANDLE<pthread_t>(d->data->threadId.load()));
    if (code) {
        qWarning("QThread::start: Thread termination error: %s",
                 qPrintable(qt_error_string((code))));
    }
#endif
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
    if (from_HANDLE<pthread_t>(d->data->threadId.load()) == pthread_self()) {
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
    QThread *thr = currentThread();
    Q_ASSERT_X(thr != 0, "QThread::setTerminationEnabled()",
               "Current thread was not started with QThread.");

    Q_UNUSED(thr)
#ifdef Q_OS_GENODE
    struct QThreadPrivate::tls_struct tls_elem = 
        QThreadPrivate::tls.value(QThread::currentThreadId());
    tls_elem.termination_enabled = enabled;
    QThreadPrivate::tls.insert(QThread::currentThreadId(), tls_elem);
#else
#if defined(Q_OS_ANDROID)
    Q_UNUSED(enabled);
#else
    pthread_setcancelstate(enabled ? PTHREAD_CANCEL_ENABLE : PTHREAD_CANCEL_DISABLE, NULL);
    if (enabled)
        pthread_testcancel();
#endif
#endif /* Q_OS_GENODE */
}

// Caller must lock the mutex
void QThreadPrivate::setPriority(QThread::Priority threadPriority)
{
    priority = threadPriority;

#ifndef Q_OS_GENODE

    // copied from start() with a few modifications:

#ifdef QT_HAS_THREAD_PRIORITY_SCHEDULING
    int sched_policy;
    sched_param param;

    if (pthread_getschedparam(from_HANDLE<pthread_t>(data->threadId.load()), &sched_policy, &param) != 0) {
        // failed to get the scheduling policy, don't bother setting
        // the priority
        qWarning("QThread::setPriority: Cannot get scheduler parameters");
        return;
    }

    int prio;
    if (!calculateUnixPriority(priority, &sched_policy, &prio)) {
        // failed to get the scheduling parameters, don't
        // bother setting the priority
        qWarning("QThread::setPriority: Cannot determine scheduler priority range");
        return;
    }

    param.sched_priority = prio;
    int status = pthread_setschedparam(from_HANDLE<pthread_t>(data->threadId.load()), sched_policy, &param);

# ifdef SCHED_IDLE
    // were we trying to set to idle priority and failed?
    if (status == -1 && sched_policy == SCHED_IDLE && errno == EINVAL) {
        // reset to lowest priority possible
        pthread_getschedparam(from_HANDLE<pthread_t>(data->threadId.load()), &sched_policy, &param);
        param.sched_priority = sched_get_priority_min(sched_policy);
        pthread_setschedparam(from_HANDLE<pthread_t>(data->threadId.load()), sched_policy, &param);
    }
# else
    Q_UNUSED(status);
# endif // SCHED_IDLE
#endif

#endif /* Q_OS_GENODE */
}

#endif // QT_NO_THREAD

QT_END_NAMESPACE

