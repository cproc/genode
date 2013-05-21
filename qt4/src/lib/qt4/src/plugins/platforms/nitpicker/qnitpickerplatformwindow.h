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
#include <qevdevkeyboardhandler_p.h>

/* Qoost includes */
#include <qoost/qmember.h>

#include "qnitpickerwindowsurface.h"
#include "window_slave_policy.h"

QT_BEGIN_NAMESPACE

class QNitpickerPlatformWindow : public QObject, public QPlatformWindow
{
	Q_OBJECT

	private:

		Window_slave_policy      _window_slave_policy;
		Genode::Slave            _window_slave;
		QMember<QTimer>          _timer;
		Qt::MouseButtons         _mouse_button_state;
		QEvdevKeyboardHandler    _keyboard_handler;
		QByteArray               _title;
		QNitpickerWindowSurface *_window_surface;
		bool                     _resize_handle;
		bool                     _decoration;

		void _process_mouse_event(Input::Event *ev)
		{
			QPoint local_position(ev->ax(), ev->ay());
			QPoint global_position (geometry().x() + local_position.x(),
			                        geometry().y() + local_position.y());

			//qDebug() << "local_position =" << local_position;
			//qDebug() << "global_position =" << global_position;

			switch (ev->type()) {

				case Input::Event::PRESS:
					PDBG("PRESS");
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
					PDBG("RELEASE");
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

				case Input::Event::WHEEL:
					PDBG("WHEEL");
					QWindowSystemInterface::handleWheelEvent(widget(),
					                                         local_position,
					                                         local_position,
					                                         ev->ry() * 120,
					                                         Qt::Vertical);
					return;

				default:
					break;
			}

			QWindowSystemInterface::handleMouseEvent(widget(),
			                                         local_position,
			                                         global_position,
			                                         _mouse_button_state);
		}

		void _process_key_event(Input::Event *ev)
		{
			const bool pressed = (ev->type() == Input::Event::PRESS);
			const int keycode = ev->code();
			_keyboard_handler.processKeycode(keycode, pressed, false);
		}

	public:

		QNitpickerPlatformWindow(QWidget *tlw, Genode::Rpc_entrypoint &ep,
		                         int screen_width, int screen_height)
		: QPlatformWindow(tlw),
		  _window_slave_policy(ep, screen_width, screen_height),
		  _window_slave(ep, _window_slave_policy, 9*1024*1024),
		  _timer(this),
		  _keyboard_handler("", -1, false, false, ""),
		  _window_surface(0),
		  _resize_handle(true),
		  _decoration(true)
		{
			_window_slave_policy.wait_for_service_announcements();

			connect(_timer, SIGNAL(timeout()), this, SLOT(handle_events()));
			_timer->start(10);
		}

		void window_surface(QNitpickerWindowSurface *window_surface)
		{
			_window_surface = window_surface;
		}

	    QWidget *widget() const
	    {
	    	//qDebug() << "QNitpickerPlatformWindow::widget()";
	    	return QPlatformWindow::widget();
	    }

	    void setGeometry(const QRect &rect)
	    {
	    	qDebug() << "QNitpickerPlatformWindow::setGeometry(" << rect << ")";
	    	QPlatformWindow::setGeometry(rect);
	    	_window_slave_policy.configure(rect.x(), rect.y(), rect.width(),
	    	                               rect.height(), _title.constData(),
	    	                               _resize_handle, _decoration);
			_window_surface->framebuffer_changed();
	    }

	    QRect geometry() const
	    {
	    	//qDebug() << "QNitpickerPlatformWindow::geometry(): returning" << QPlatformWindow::geometry();
	    	return QPlatformWindow::geometry();
	    }

	    void setVisible(bool visible)
	    {
	    	qDebug() << "QNitpickerPlatformWindow::setVisible(" << visible << ")";
	    	QPlatformWindow::setVisible(visible);
	    	QRect g = geometry();
	    	if (visible) {
		    	_window_slave_policy.configure(g.x(), g.y(),
		    	                               g.width(), g.height(),
		    	                               _title.constData(),
		    	                               _resize_handle, _decoration);
	    	} else {
	    		_window_slave_policy.configure(g.x() + 100000, g.y(),
	    		                               g.width(), g.height(),
	    		                               _title.constData(),
	    		                               _resize_handle, _decoration);
	    	}
			_window_surface->framebuffer_changed();
	    }

	    Qt::WindowFlags setWindowFlags(Qt::WindowFlags flags)
	    {
	    	qDebug() << "QNitpickerPlatformWindow::setWindowFlags(" << flags << ")";

	    	_resize_handle = true;
	    	_decoration = true;

	    	if (flags.testFlag(Qt::Popup)) {
	    		_resize_handle = false;
	    		_decoration = false;
	    	}

	    	QRect g = geometry();
    		_window_slave_policy.configure(g.x(), g.y(), g.width(), g.height(),
    		                               _title.constData(), _resize_handle,
    		                               _decoration);

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
	    	return WId(this);
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
	    	_title = title.toUtf8();
	    	QRect g = geometry();
	    	_window_slave_policy.configure(g.x(), g.y(),
	    	                               g.width(), g.height(),
	    	                               _title.constData(),
	    	                               _resize_handle, _decoration);
			_window_surface->framebuffer_changed();
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
				_window_surface->framebuffer_changed();
			}

			/* handle input events */
			Input::Session_client input(_window_slave_policy.input_session());
			if (input.is_pending()) {
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
