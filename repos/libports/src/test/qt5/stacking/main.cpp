/*
 * \brief   Window stacking test
 * \author  Christian Prochaska
 * \date    2021-06-14
 */

/*
 * Copyright (C) 2021 Genode Labs GmbH
 *
 * This file is part of the Genode OS framework, which is distributed
 * under the terms of the GNU Affero General Public License version 3.
 */

/* Qt includes */
#include <QtGui>
#include <QtWidgets>
#include <QApplication>

/* Qoost includes */
#include <qoost/compound_widget.h>

/* qt5_component includes */
#include <qt5_component/qpa_init.h>


class Main_window : public Compound_widget<QWidget, QVBoxLayout>
{
	Q_OBJECT

	private:

		QMember<QPushButton> _button { "Push" };
		QMember<QTimer> _timer;
		QMember<QWidget> _w;
		QWidget *_w2 { nullptr };

	private slots:

		void _clicked()
		{
			QMessageBox msgBox;
			msgBox.setText("Message");
			msgBox.exec();
		}

		void _timeout()
		{
/* show and hide same window controlled by timer */
#if 1
			_w->setVisible(!_w->isVisible());
#endif

/* show new window controlled by timer */
#if 0
			static unsigned int id = 0;
			delete _w2;
			_w2 = new QWidget();
			_w2->resize(200, 100);

/* give each new window a new ID*/
#if 0
			_w2->setWindowTitle(QString::number(id));
#endif

			id++;
			_w2->setVisible(true);
#endif
		}

	public:

		Main_window()
		{

/* add a push button which shows a new message window when clicked */
#if 0
			layout()->addWidget(_button);
			_button->setMinimumSize(400, 400);
			connect(_button, SIGNAL(clicked()), this, SLOT(_clicked()));
#endif

			_w->resize(100, 100);

			connect(_timer, SIGNAL(timeout()), this, SLOT(_timeout()));
			_timer->setInterval(1000);
			_timer->start();
		}
};


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Main_window *w = new Main_window;
	w->setMinimumSize(400, 400);
	w->show();

	return app.exec();
}

#include "main.moc"
