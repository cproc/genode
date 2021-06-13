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

	private slots:

		void _clicked()
		{
			QMessageBox msgBox;
			msgBox.setText("Message");
			msgBox.exec();
		}

	public:

		Main_window()
		{
			layout()->addWidget(_button);

			_button->setMinimumSize(400, 400);

			connect(_button, SIGNAL(clicked()), this, SLOT(_clicked()));
		}
};


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	Main_window *w = new Main_window;
	w->show();

	return app.exec();
}

#include "main.moc"
