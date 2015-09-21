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

#include "qgenodeclipboard.h"

#ifndef QT_NO_CLIPBOARD

#include <QMimeData>

QT_BEGIN_NAMESPACE

QMimeData *QGenodeClipboard::mimeData(QClipboard::Mode mode)
{
	qDebug("*** mimeData() ***");
	return QPlatformClipboard::mimeData(mode);
}

void QGenodeClipboard::setMimeData(QMimeData *data, QClipboard::Mode mode)
{
	if (!data || !data->hasText() || !supportsMode(mode))
		return;

	QString text = data->text();
	QByteArray utf8text = text.toUtf8();
	qDebug("*** setMimeData(%s) ***", utf8text.constData());

	QPlatformClipboard::setMimeData(data, mode);
}

QT_END_NAMESPACE

#endif /* QT_NO_CLIPBOARD */
