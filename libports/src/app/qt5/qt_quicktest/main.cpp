#include <QGuiApplication>
#include <QQuickView>
extern "C" void wait_for_continue();

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
//wait_for_continue();
    QQuickView view;
    view.setSource(QUrl("qrc:/qt_quicktest.qml"));
    view.show();

    return app.exec();
}
