#include <QApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include "wrapper.h"
#include <QtDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::addLibraryPath(".");
//    QGuiApplication app(argc, argv);
    QApplication app(argc,argv);
    QQmlApplicationEngine engine;
    Wrapper w;
    engine.rootContext()->setContextProperty("Wrapper",&w);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
