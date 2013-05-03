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


#ifndef QNITPICKERPLATFORMWINDOW_H
#define QNITPICKERPLATFORMWINDOW_H

#include <QtGui/QPlatformWindow>

#include <QDebug>

QT_BEGIN_NAMESPACE

class QNitpickerPlatformWindow : public QPlatformWindow
{
	public:

		QNitpickerPlatformWindow(QWidget *tlw) : QPlatformWindow(tlw) { }

	    QWidget *widget() const
	    {
	    	qDebug() << "QNitpickerPlatformWindow::widget()";
	    	return QPlatformWindow::widget();
	    }

	    void setGeometry(const QRect &rect)
	    {
	    	qDebug() << "QNitpickerPlatformWindow::setGeometry(" << rect << ")";
	    	QPlatformWindow::setGeometry(rect);
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

#endif /* QNITPICKERPLATFORMWINDOW_H */
