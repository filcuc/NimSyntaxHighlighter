#include "applicationlogic.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ApplicationLogic logic;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("applicationLogic", &logic);
    engine.load(QUrl("qrc:///main.qml"));

    return app.exec();
}
