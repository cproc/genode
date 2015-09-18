/*
 * \brief  signal handler thread
 * \author Christian Prochaska
 * \date   2015-09-18
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _QSIGNALHANDLERTHREAD_H_
#define _QSIGNALHANDLERTHREAD_H_

/* Genode includes */
#include <base/signal.h>

/* Qt includes */
#include <QThread>

class QSignalHandlerThread : public QThread
{
	Q_OBJECT

	private:

		Genode::Signal_receiver _signal_receiver;

	public:

		Genode::Signal_receiver &signal_receiver()
		{
			return _signal_receiver;
		}
};

#endif /* _QSIGNALHANDLERTHREAD_H_ */
