#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "wrapper.h"
#include <QtDebug>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::addLibraryPath(".");
    QGuiApplication app(argc, argv);
    for(QString s: app.libraryPaths()){
        qDebug() << s << endl;
    }
    QQmlApplicationEngine engine;

    Wrapper w;
    engine.rootContext()->setContextProperty("Wrapper",&w);
//    ResultModel* model = new ResultModel();
//    engine.rootContext()->setContextProperty("ResultModel", model);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
