/*
 * \brief  QNitpickerWindowSurface
 * \author Christian Prochaska
 * \date   2013-05-08
 */

/*
 * Copyright (C) 2013 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */


#ifndef _QNITPICKERWINDOWSURFACE_H_
#define _QNITPICKERWINDOWSURFACE_H_

#include <QtGui/private/qwindowsurface_p.h>

#include <QtGui/QPlatformWindow>

QT_BEGIN_NAMESPACE

class QNitpickerWindowSurface : public QWindowSurface
{
	private:

		QImage _image;

	public:

		QNitpickerWindowSurface(QWidget *window);
		~QNitpickerWindowSurface();

		QPaintDevice *paintDevice();
		void flush(QWidget *widget, const QRegion &region, const QPoint &offset);
		void resize(const QSize &size);

};

QT_END_NAMESPACE

#endif /* _QNITPICKERWINDOWSURFACE_H_ */
