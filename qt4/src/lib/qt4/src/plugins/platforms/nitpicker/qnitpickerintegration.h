/*
 * \brief  QNitpickerIntegration
 * \author Christian Prochaska
 * \date   2013-05-08
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */


#ifndef QNITPICKERINTEGRATION_H
#define QNITPICKERINTEGRATION_H

#include <QtGui/QPlatformIntegration>
#include <QtGui/QPlatformScreen>

#include <QDebug>

#include "qnitpickerscreen.h"

/* Qoost includes */
#include <qoost/qmember.h>

QT_BEGIN_NAMESPACE

class QNitpickerIntegration : public QPlatformIntegration
{
	private:

		QMember<QNitpickerScreen> _nitpicker_screen;
	    QList<QPlatformScreen *>  _screens;

	public:

		QNitpickerIntegration();

		bool hasCapability(QPlatformIntegration::Capability cap) const;

		QPixmapData *createPixmapData(QPixmapData::PixelType type) const;
		QPlatformWindow *createPlatformWindow(QWidget *widget, WId winId) const;
		QWindowSurface *createWindowSurface(QWidget *widget, WId winId) const;

		QList<QPlatformScreen *> screens() const { return _screens; }
};

QT_END_NAMESPACE

#endif /* QNITPICKERINTEGRATION_H */
