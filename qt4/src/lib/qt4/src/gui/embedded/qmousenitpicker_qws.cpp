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

#ifndef QT_NO_QWS_MOUSE_NITPICKER

#include <input/keycodes.h>

#include <QtGui>

#include <qwindowsystem_qws.h>

#include <qmousenitpicker_qws.h>

QNitpickerMouseHandler::QNitpickerMouseHandler(const QString &driver, const QString &device)
    : QWSMouseHandler(driver, device)
{
qDebug() << "QNitpickerMouseHandler()";
	state = Qt::NoButton;
}

QNitpickerMouseHandler::~QNitpickerMouseHandler()
{
}

void QNitpickerMouseHandler::resume()
{
}

void QNitpickerMouseHandler::suspend()
{
}

void QNitpickerMouseHandler::processMouseEvent(Input::Event *ev)
{
	QPoint position(ev->ax(), ev->ay());

	int wheel = 0;

	switch (ev->type()) {
		case Input::Event::PRESS:
			switch (ev->code()) {
				case Input::BTN_LEFT:
					state |= Qt::LeftButton;
					break;
				case Input::BTN_RIGHT:
					state |= Qt::RightButton;
					break;
				case Input::BTN_MIDDLE:
					state |= Qt::MidButton;
					break;
				case Input::BTN_SIDE:
					state |= Qt::XButton1;
					break;
				case Input::BTN_EXTRA:
					state |= Qt::XButton2;
					break;
			}
			break;
		case Input::Event::RELEASE:
			switch (ev->code()) {
				case Input::BTN_LEFT:
					state &= ~Qt::LeftButton;
					break;
				case Input::BTN_RIGHT:
					state &= ~Qt::RightButton;
					break;
				case Input::BTN_MIDDLE:
					state &= ~Qt::MidButton;
					break;
				case Input::BTN_SIDE:
					state &= ~Qt::XButton1;
					break;
				case Input::BTN_EXTRA:
					state &= ~Qt::XButton2;
					break;
			}
			break;
		case Input::Event::WHEEL:
			wheel = ev->ry() * 120; // WHEEL_DELTA
			break;
		default:
			break;
	}

	mouseChanged(position, state, wheel);
}

#endif // QT_NO_QWS_MOUSE_NITPICKER
