#include "applicationlogic.h"
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "textareakeyhandler.h"
#include "textcursor.h"
#include "nimsyntaxhighlighter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ApplicationLogic logic;

    qmlRegisterType<TextAreaKeyHandler>("AporiaQt", 1, 0, "TextAreaKeyHandler");
    qmlRegisterType<TextCursor>("AporiaQt", 1, 0, "TextCursor");
    qmlRegisterType<NimSyntaxHighlighter>("AporiaQt", 1, 0, "NimSyntaxHighlighter");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("applicationLogic", &logic);
    engine.load(QUrl("qrc:///main.qml"));

    return app.exec();
}
