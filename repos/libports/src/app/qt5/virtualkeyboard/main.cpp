/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Virtual Keyboard module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QQuickView>
#include <QQuickWidget>
#include <QQuickItem>
#include <QApplication>
#include <QDebug>

static constexpr bool debug = true;

class Im_enabled_item : public QQuickItem
{
	Q_OBJECT

	private:

		QFile       _event_file;
		QTextStream _event_stream;

	protected:

		void keyReleaseEvent(QKeyEvent *e) override
		{
			if (debug)
				qDebug() << "Im_enabled_item::keyReleaseEvent(): " << e;

			if (e->text() != "") {
				_event_stream << e->text();
				_event_stream.flush();
			} else if (e->key() == Qt::Key_Backspace) {
				_event_stream << QString("\x08");
				_event_stream.flush();
			}
		}

	public:

		Im_enabled_item(QQuickItem *parent = nullptr)
		: QQuickItem(parent),
		  _event_file("/event/text"),
		  _event_stream(&_event_file)
		{
			setFlag(ItemAcceptsInputMethod, true);

			if (!_event_file.open(QIODevice::WriteOnly))
				qFatal("ERROR: could not open event VFS file");

			_event_stream.setCodec("UTF-32");
		}

};


int main(int argc, char *argv[])
{
	qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

	QApplication app(argc, argv);

	qmlRegisterType<Im_enabled_item>("Im_enabled_item", 1, 0, "Im_enabled_item");

	QQuickWidget view(QString("qrc:/virtualkeyboard.qml"));

	view.setResizeMode(QQuickWidget::SizeRootObjectToView);

	view.resize(1024, 400);

	view.show();

	return app.exec();
}

#include "main.moc"
