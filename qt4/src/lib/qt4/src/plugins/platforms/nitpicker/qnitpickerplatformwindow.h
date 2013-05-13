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

/* Genode includes */
#include <input/keycodes.h>

/* Qt includes */
#include <QtGui/QPlatformWindow>
#include <QtGui/QWindowSystemInterface>
#include <QTimer>
#include <QDebug>

/* Qoost includes */
#include <qoost/qmember.h>

#include "window_slave_policy.h"

QT_BEGIN_NAMESPACE

class QNitpickerPlatformWindow : public QObject, public QPlatformWindow
{
	Q_OBJECT

	private:

		Window_slave_policy  _window_slave_policy;
		Genode::Slave        _window_slave;
		QMember<QTimer>      _timer;
		Qt::MouseButtons     _mouse_button_state;

		void _process_mouse_event(Input::Event *ev)
		{
			QPoint position(ev->ax(), ev->ay());

			int wheel = 0;

			switch (ev->type()) {

				case Input::Event::PRESS:
					switch (ev->code()) {
						case Input::BTN_LEFT:
							_mouse_button_state |= Qt::LeftButton;
							break;
						case Input::BTN_RIGHT:
							_mouse_button_state |= Qt::RightButton;
							break;
						case Input::BTN_MIDDLE:
							_mouse_button_state |= Qt::MidButton;
							break;
						case Input::BTN_SIDE:
							_mouse_button_state |= Qt::XButton1;
							break;
						case Input::BTN_EXTRA:
							_mouse_button_state |= Qt::XButton2;
							break;
					}
					break;

				case Input::Event::RELEASE:
					switch (ev->code()) {
						case Input::BTN_LEFT:
							_mouse_button_state &= ~Qt::LeftButton;
							break;
						case Input::BTN_RIGHT:
							_mouse_button_state &= ~Qt::RightButton;
							break;
						case Input::BTN_MIDDLE:
							_mouse_button_state &= ~Qt::MidButton;
							break;
						case Input::BTN_SIDE:
							_mouse_button_state &= ~Qt::XButton1;
							break;
						case Input::BTN_EXTRA:
							_mouse_button_state &= ~Qt::XButton2;
							break;
					}
					break;
#if 0
				case Input::Event::WHEEL:
					wheel = ev->ry() * 120; // WHEEL_DELTA
					break;
#endif
				default:
					break;
			}

			QWindowSystemInterface::handleMouseEvent(widget(), position, position, _mouse_button_state);
		}

		void _process_key_event(Input::Event *ev)
		{
			QEvent::Type type;

			if (ev->type() == Input::Event::RELEASE)
				type = QEvent::KeyRelease;
			else
				type = QEvent::KeyPress;

			int keycode = ev->code();

			/* FIXME */
			QWindowSystemInterface::handleKeyEvent(widget(), type, ev->code(),
			                                       Qt::KeyboardModifiers(Qt::NoModifier));
		}

	public:

		QNitpickerPlatformWindow(QWidget *tlw, Genode::Rpc_entrypoint &ep,
		                         int screen_width, int screen_height)
		: QPlatformWindow(tlw),
		  _window_slave_policy(ep, screen_width, screen_height),
		  _window_slave(ep, _window_slave_policy, 6*1024*1024),
		  _timer(this)
		{
			_window_slave_policy.wait_for_service_announcements();
			connect(_timer, SIGNAL(timeout()), this, SLOT(handle_events()));
			_timer->start(10);
		}

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

	    unsigned char *framebuffer()
	    {
	    	return _window_slave_policy.framebuffer();
	    }

		void refresh(int x, int y, int w, int h)
		{
			_window_slave_policy.refresh(x, y, w, h);
		}

	private slots:

		void handle_events()
		{
			/* handle framebuffer mode change events */
			if (_window_slave_policy.mode_changed()) {
				int new_width;
				int new_height;
				_window_slave_policy.size(new_width, new_height);
				PDBG("mode change detected: %d, %d", new_width, new_height);
				QRect geo = geometry();
				QWindowSystemInterface::handleGeometryChange(widget(),
				                                             QRect(geo.x(),
				                                                   geo.y(),
				                                                   new_width,
				                                                   new_height));
			}

			/* handle input events */
			Input::Session_client input(_window_slave_policy.input_session());
			if (input.is_pending()) {
				int num_ev;
				Input::Event *ev_buf = _window_slave_policy.ev_buf();
				for (int i = 0, num_ev = input.flush(); i < num_ev; i++) {

					Input::Event *ev = &ev_buf[i];

					bool const is_key_event = ev->type() == Input::Event::PRESS ||
					                          ev->type() == Input::Event::RELEASE;

					bool const is_mouse_button_event =
						is_key_event && (ev->code() == Input::BTN_LEFT ||
					                     ev->code() == Input::BTN_MIDDLE ||
					                     ev->code() == Input::BTN_RIGHT);

					if (ev->type() == Input::Event::MOTION ||
					    ev->type() == Input::Event::WHEEL ||
					    is_mouse_button_event) {

						_process_mouse_event(ev);

					} else if (is_key_event && (ev->code() < 128)) {

						_process_key_event(ev);

					}
				}

			}


		}

};

QT_END_NAMESPACE

#endif /* _QNITPICKERPLATFORMWINDOW_H_ */
