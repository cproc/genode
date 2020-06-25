/*
 * \brief   QPluginWidget test
 * \author  Christian Prochaska
 * \date    2012-04-23
 */

/* libc includes */
#include <dlfcn.h>  /* 'dlopen'  */
#include <stdio.h>  /* 'fprintf' */
#include <unistd.h> /* 'access'  */

/* Qt includes */
#include <QtGui>
#include <QApplication>
#include <qpluginwidget.h>

/* Qoost includes */
#include <qoost/compound_widget.h>

void Libc::Component::construct(Libc::Env &env)
{
	Libc::with_libc([&] {

		/*
		 * initialize the QPA plugin
		 *
		 * when Qt loads the plugin again, it will get the same handle
		 */

		char const *qpa_plugin = "/qt/plugins/platforms/libqnitpicker.lib.so";

		void *qpa_plugin_handle = nullptr;

		/* check existence with 'access()' first to avoid ld error messages */
		if (access(qpa_plugin, F_OK) == 0)
			qpa_plugin_handle = dlopen(qpa_plugin, RTLD_LAZY | RTLD_GLOBAL);
		
		if (qpa_plugin_handle) {

			typedef void (*initialize_qpa_plugin_t)(Genode::Env &);

			initialize_qpa_plugin_t initialize_qpa_plugin = 
				(initialize_qpa_plugin_t) dlsym(qpa_plugin_handle,
				                                "initialize_qpa_plugin");

			if (!initialize_qpa_plugin) {
				fprintf(stderr, "Could not find 'initialize_qpa_plugin' \
				                 function in QPA plugin\n");
				dlclose(qpa_plugin_handle);
				exit(1);
			}

			initialize_qpa_plugin(env);
		}

		int argc = 1;
		char const *argv[] = { "test-qpluginwidget", 0 };

		QApplication app(argc, (char**)argv);

		Compound_widget<QWidget, QHBoxLayout> w;

		QPluginLoader plugin_loader("/qt/plugins/qpluginwidget/libqpluginwidget.lib.so");

		QObject *plugin = plugin_loader.instance();

		if (!plugin)
			qFatal("Error: Could not load QPluginWidget Qt plugin");

		QPluginWidgetInterface *plugin_widget_interface = qobject_cast<QPluginWidgetInterface*>(plugin);

		plugin_widget_interface->env(env);

		QString plugin_args("ram_quota=4M, caps=500");

		QPluginWidget *plugin_widget = 
			plugin_widget_interface->createWidget(&w,
			                                      QUrl("rom:///test-plugin.tar"),
			                                      plugin_args, 100, 100);

		w.layout()->addWidget(plugin_widget);

		w.resize(150, 150);

		w.show();

		return app.exec();
	});
}
