/*
 * \brief   QtQuick test
 * \author  Christian Prochaska
 * \date    2013-11-26
 */

#include <QGuiApplication>
#include <QQuickView>

class Qml_signal_handler : public QObject
{
	Q_OBJECT

	public Q_SLOTS:

		void button1_clicked()
		{
			qDebug() << "button1_clicked()";
		}
};

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    Qml_signal_handler qml_signal_handler;

    QQuickView view;
    view.setSource(QUrl("qrc:/qt_quick.qml"));

    QObject *root_object = (QObject *)view.rootObject();

    QObject *button1 = root_object->findChild<QObject*>("button1");

    QObject::connect(button1, SIGNAL(clicked()),
                     &qml_signal_handler, SLOT(button1_clicked()));

    view.show();

    return app.exec();
}

#include "main.moc"
