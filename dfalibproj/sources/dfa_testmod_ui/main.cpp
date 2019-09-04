#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtDebug>
#include <QQmlContext>
#include "wrapper.h"
#include "authenticationmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::addLibraryPath(".");
    QCoreApplication::setOrganizationName("BroutonLab");
    QCoreApplication::setOrganizationDomain("broutonlab.com");
    QCoreApplication::setApplicationName("DAFNA");
    /*
     * theres a bug [https://bugreports.qt.io/browse/QTBUG-54355] that doesn't
     * show properly the Qtable Sort indicators used to order the data
     * in the calculations view, temporarely the solution is to use QApplication
     * instead of QGuiApplication and add +Widgets in the CMake file
     * https://forum.qt.io/topic/77569/tableview-sort-indicator-not-show/4
     */
    //    QGuiApplication app(argc, argv);

    QApplication app(argc,argv);
    QQmlApplicationEngine engine;
    QSettings settings;
    AuthenticationManager authentication_manager(&settings);
    Wrapper wrapper(&settings,&authentication_manager);
    engine.rootContext()->setContextProperty("Wrapper",&wrapper);
    engine.rootContext()->setContextProperty("AuthManager",&authentication_manager);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
