/*
 * \brief  QNitpickerPlatformWindow
 * \author Christian Prochaska
 * \date   2013-05-08
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */


#ifndef _QNITPICKERPLATFORMWINDOW_H_
#define _QNITPICKERPLATFORMWINDOW_H_

#include <QtGui/QPlatformWindow>

#include <QDebug>

#include "window_slave_policy.h"

QT_BEGIN_NAMESPACE

class QNitpickerPlatformWindow : public QPlatformWindow
{
	private:

		Window_slave_policy _window_slave_policy;
		Genode::Slave       _window_slave;

	public:

		QNitpickerPlatformWindow(QWidget *tlw, Genode::Rpc_entrypoint &ep)
		: QPlatformWindow(tlw),
		  _window_slave_policy(ep),
		  _window_slave(ep, _window_slave_policy, 4*1024*1024) { }

	    QWidget *widget() const
	    {
	    	qDebug() << "QNitpickerPlatformWindow::widget()";
	    	return QPlatformWindow::widget();
	    }

	    void setGeometry(const QRect &rect)
	    {
	    	qDebug() << "QNitpickerPlatformWindow::setGeometry(" << rect << ")";
	    	QPlatformWindow::setGeometry(rect);
	    	_window_slave_policy.geometry(rect.x(), rect.y(), rect.width(), rect.height());
	    }

	    QRect geometry() const
	    {
	    	qDebug() << "QNitpickerPlatformWindow::geometry(): returning" << QPlatformWindow::geometry();
	    	return QPlatformWindow::geometry();
	    }

	    void setVisible(bool visible)
	    {
	    	qDebug() << "QNitpickerPlatformWindow::setVisible(" << visible << ")";
	    	QPlatformWindow::setVisible(visible);
	    }

	    Qt::WindowFlags setWindowFlags(Qt::WindowFlags flags)
	    {
	    	qDebug() << "QNitpickerPlatformWindow::setWindowFlags(" << flags << ")";
	    	return QPlatformWindow::setWindowFlags(flags);
	    }

	    Qt::WindowFlags windowFlags() const
	    {
	    	qDebug() << "QNitpickerPlatformWindow::windowFlags()";
	    	return QPlatformWindow::windowFlags();
	    }

	    WId winId() const
	    {
	    	qDebug() << "QNitpickerPlatformWindow::winId()";
	    	return QPlatformWindow::winId();
	    }

	    void setParent(const QPlatformWindow *window)
	    {
	    	qDebug() << "QNitpickerPlatformWindow::setParent()";
	    	QPlatformWindow::setParent(window);
	    }

	    void setWindowTitle(const QString &title)
	    {
	    	qDebug() << "QNitpickerPlatformWindow::setWindowTitle(" << title << ")";
	    	QPlatformWindow::setWindowTitle(title);
	    }

	    void raise()
	    {
	    	qDebug() << "QNitpickerPlatformWindow::raise()";
	    	QPlatformWindow::raise();
	    }

	    void lower()
	    {
	    	qDebug() << "QNitpickerPlatformWindow::lower()";
	    	QPlatformWindow::lower();
	    }

	    void setOpacity(qreal level)
	    {
	    	qDebug() << "QNitpickerPlatformWindow::setOpacity(" << level << ")";
	    	QPlatformWindow::setOpacity(level);
	    }

	    void requestActivateWindow()
	    {
	    	qDebug() << "QNitpickerPlatformWindow::requestActivateWindow()";
	    	QPlatformWindow::requestActivateWindow();
	    }

};

QT_END_NAMESPACE

#endif /* _QNITPICKERPLATFORMWINDOW_H_ */
