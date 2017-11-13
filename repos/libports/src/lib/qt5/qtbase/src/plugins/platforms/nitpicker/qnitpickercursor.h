/*
 * \brief  QNitpickerCursor
 * \author Christian Prochaska
 * \date   2017-11-13
 */

/*
 * Copyright (C) 2017 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */


#ifndef _QNITPICKERCURSOR_H_
#define _QNITPICKERCURSOR_H_

/* Genode includes */
#include <base/log.h>
#include <os/reporter.h>
#include <report_session/connection.h>
#include <vbox_pointer/shape_report.h>

/* Qt includes */
#include <QtGui/QBitmap>
#include <QtGui/private/qguiapplication_p.h>
#include <qpa/qplatformcursor.h>

class QNitpickerCursor : public QPlatformCursor
{
	private:

		Report::Connection          _shape_report_connection;
		Genode::Attached_dataspace  _shape_report_ds;
		Vbox_pointer::Shape_report *_shape_report;

	public:

		QNitpickerCursor(Genode::Env &env)
		: _shape_report_connection(env, "shape", sizeof(Vbox_pointer::Shape_report)),
		  _shape_report_ds(env.rm(), _shape_report_connection.dataspace()),
		  _shape_report(_shape_report_ds.local_addr<Vbox_pointer::Shape_report>()) { }

		virtual void changeCursor(QCursor *widgetCursor, QWindow *window) override
		{
			Genode::log(__PRETTY_FUNCTION__, " called");

			Q_UNUSED(window);

#ifndef QT_NO_CURSOR
			const Qt::CursorShape shape = widgetCursor ? widgetCursor->shape() : Qt::ArrowCursor;

			// XXX: detect visibility
			_shape_report->visible = true;

			QImage cursor;

			if (shape == Qt::BitmapCursor) {
				// application supplied cursor
				cursor = widgetCursor->pixmap().toImage();
				_shape_report->x_hot = widgetCursor->hotSpot().x();
				_shape_report->y_hot = widgetCursor->hotSpot().y();
			} else {
				// system cursor
				QPlatformCursorImage platformImage(0, 0, 0, 0, 0, 0);
				platformImage.set(shape);
				cursor = *platformImage.image();
				_shape_report->x_hot = platformImage.hotspot().x();
				_shape_report->y_hot = platformImage.hotspot().y();
			}

			_shape_report->width = cursor.width();
			_shape_report->height = cursor.height();

			_shape_report_connection.submit(sizeof(Vbox_pointer::Shape_report));

#else // !QT_NO_CURSOR
			Q_UNUSED(widgetCursor);
#endif
		}

		virtual QPoint pos() const override
		{
			Genode::log(__PRETTY_FUNCTION__, " called");
			return QGuiApplicationPrivate::lastCursorPosition.toPoint();
		}

		virtual void setPos(const QPoint &pos) override
		{
			Genode::log(__PRETTY_FUNCTION__, " called");
		}
};

QT_END_NAMESPACE

#endif /* _QNITPICKERCURSOR_H_ */
