/*
 * \brief  QGenodeClipboard
 * \author Christian Prochaska
 * \date   2015-09-18
 */

/*
 * Copyright (C) 2015 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU General Public License version 2.
 */

#ifndef _QGENODECLIPBOARD_H_
#define _QGENODECLIPBOARD_H_

#include <QtCore/qglobal.h>

#ifndef QT_NO_CLIPBOARD

#include <qpa/qplatformclipboard.h>

QT_BEGIN_NAMESPACE

class QGenodeClipboard : public QPlatformClipboard
{
	private:

		//class MimeData;
		//MimeData *m_mimeData;

	public:

		//QGenodeClipboard();
		//~QGenodeClipboard();
		QMimeData *mimeData(QClipboard::Mode mode = QClipboard::Clipboard);
		void setMimeData(QMimeData *data, QClipboard::Mode mode = QClipboard::Clipboard);
};

QT_END_NAMESPACE

#endif /* QT_NO_CLIPBOARD */
#endif /* _QGENODECLIPBOARD_H_ */
