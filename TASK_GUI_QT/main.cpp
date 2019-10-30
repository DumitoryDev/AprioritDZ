#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "mylistmodel.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    MyListModel modelData;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("myModel", QVariant::fromValue(&modelData));
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
