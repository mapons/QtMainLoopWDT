#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "utils.h"

//MAIN LOOP WDT.
#include "mainLoopWdt.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    auto list =QCoreApplication::arguments();
    qDebug()<<"APP ARGUMENTS:"<<list;
    QQmlApplicationEngine engine;

    //WDT for QT mainLoop.
    //@param restartApp  true: Blocking the  main event loop restarts the application  , false:  Blocking the  main event loop kills the application
    //@param warningTimeut_ms  timeout (ms) since locking the main event loop, once fulfilled, a warning message is displayed
    //@param restartTimeut_ms  timeout (ms) since locking the main event loop, once fulfilled, the application is killed or restarted
    //@param parent  QObject parent
    mainLoopWdt wdt(true,2000, 5000,&app);


    //utility for C++ infinite loop
    Utils util;
    engine.rootContext()->setContextProperty("Utils", &util);

    engine.rootContext()->setContextProperty("WDT", &wdt);//force restart test

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    wdt.startWdt();//you can connect this slot to another signal for a delayed start of WDT
    return app.exec();
}
