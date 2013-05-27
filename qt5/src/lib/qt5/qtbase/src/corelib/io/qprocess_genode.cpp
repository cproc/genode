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

//#define QPROCESS_DEBUG
#include "qdebug.h"

#ifndef QT_NO_PROCESS

#include "qplatformdefs.h"

#include "qprocess.h"
#include "qprocess_p.h"
#include "private/qcore_unix_p.h"

#ifdef Q_OS_MAC
#include <private/qcore_mac_p.h>
#endif

#include <private/qcoreapplication_p.h>
#include <private/qthread_p.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qlist.h>
#include <qhash.h>
#include <qmutex.h>
#include <qsemaphore.h>
#include <qsocketnotifier.h>
#include <qthread.h>
#include <qelapsedtimer.h>

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#ifdef Q_OS_QNX
#include "qvarlengtharray.h"

#include <spawn.h>
#include <sys/neutrino.h>
#endif

QT_BEGIN_NAMESPACE

// POSIX requires PIPE_BUF to be 512 or larger
// so we will use 512
static const int errorBufferMax = 512;

static inline void add_fd(int &nfds, int fd, fd_set *fdset)
{
    FD_SET(fd, fdset);
    if ((fd) > nfds)
        nfds = fd;
}

struct QProcessInfo {
    QProcess *process;
    int deathPipe;
    int exitResult;
    pid_t pid;
    int serialNumber;
};


void QProcessPrivate::destroyPipe(int *pipe)
{
#if defined QPROCESS_DEBUG
qDebug() << "destroyPipe()";
#endif
}

void QProcessPrivate::startProcess()
{
    Q_Q(QProcess);

#if defined (QPROCESS_DEBUG)
    qDebug("QProcessPrivate::startProcess()");
#endif

    // Start the process (platform dependent)
    q->setProcessState(QProcess::Starting);


    launchpad_child = launchpad()->start_child(program.toLocal8Bit().constData(),
    		                                   ram_quota_hash()->value(program),
    		                                   Genode::Dataspace_capability());
    if (launchpad_child) {
      _q_startupNotification();
    }
}


bool QProcessPrivate::processStarted()
{
#if defined QPROCESS_DEBUG
qDebug() << "QProcessPrivate::processStarted()";
#endif
return false;
}

qint64 QProcessPrivate::bytesAvailableFromStdout() const
{
#if defined QPROCESS_DEBUG
qDebug() << "QProcessPrivate::bytesAvailableFromStdout()";
#endif
return 0;
}

qint64 QProcessPrivate::bytesAvailableFromStderr() const
{
#if defined QPROCESS_DEBUG
qDebug() << "QProcessPrivate::bytesAvailableFromStderr()";
#endif
return 0;
}

qint64 QProcessPrivate::readFromStdout(char *data, qint64 maxlen)
{
#if defined QPROCESS_DEBUG
qDebug() << "QProcessPrivate::readFromStdout()";
#endif
return 0;
}

qint64 QProcessPrivate::readFromStderr(char *data, qint64 maxlen)
{
#if defined QPROCESS_DEBUG
qDebug() << "QProcessPrivate::readFromStderr()";
#endif
return 0;
}

qint64 QProcessPrivate::writeToStdin(const char *data, qint64 maxlen)
{
#if defined QPROCESS_DEBUG
qDebug() << "writeToStdin()";
#endif
return 0;
}

void QProcessPrivate::terminateProcess()
{
#if defined (QPROCESS_DEBUG)
    qDebug("QProcessPrivate::terminateProcess()");
#endif
}

void QProcessPrivate::killProcess()
{
#if defined (QPROCESS_DEBUG)
    qDebug("QProcessPrivate::killProcess()");
#endif

    if (launchpad_child) {
        launchpad()->exit_child(launchpad_child);
    }
}

bool QProcessPrivate::waitForStarted(int msecs)
{
    Q_Q(QProcess);

#if defined (QPROCESS_DEBUG)
    qDebug("QProcessPrivate::waitForStarted(%d) waiting for child to start (fd = %d)", msecs,
	   childStartedPipe[0]);
#endif

return false;
}

bool QProcessPrivate::waitForReadyRead(int msecs)
{
    Q_Q(QProcess);
#if defined (QPROCESS_DEBUG)
    qDebug("QProcessPrivate::waitForReadyRead(%d)", msecs);
#endif

return false;
}

bool QProcessPrivate::waitForBytesWritten(int msecs)
{
    Q_Q(QProcess);
#if defined (QPROCESS_DEBUG)
    qDebug("QProcessPrivate::waitForBytesWritten(%d)", msecs);
#endif

return false;
}

bool QProcessPrivate::waitForFinished(int msecs)
{
    Q_Q(QProcess);
#if defined (QPROCESS_DEBUG)
    qDebug("QProcessPrivate::waitForFinished(%d)", msecs);
#endif

return false;
}

void QProcessPrivate::findExitCode()
{
#ifdef QPROCESS_DEBUG
qDebug() << "QProcessPrivate::findExitCode()";
#endif
}

bool QProcessPrivate::waitForDeadChild()
{
#ifdef QPROCESS_DEBUG
qDebug() << QProcessPrivate::waitForDeadChild();
#endif

return false;
}

void QProcessPrivate::_q_notified()
{
#ifdef QPROCESS_DEBUG
qDebug() << "QProcessPrivate::_q_notified()";
#endif
}

bool QProcessPrivate::startDetached(const QString &program, const QStringList &arguments, const QString &workingDirectory, qint64 *pid)
{
#ifdef QPROCESS_DEBUG
qDebug() << "QProcessPrivate::startDetached()";
#endif

return false;
}

void QProcessPrivate::initializeProcessManager()
{
#ifdef QPROCESS_DEBUG
qDebug() << "QProcessPrivate::initializeProcessManager()";
#endif
}

QT_END_NAMESPACE

#endif // QT_NO_PROCESS
