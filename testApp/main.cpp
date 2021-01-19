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
    qDebug()<<"ARGUMENTS START "<<list;
    QQmlApplicationEngine engine;

    //utility for C++ infinite loop
    Utils util;
    engine.rootContext()->setContextProperty("Utils", &util);
    //end utility for C++ infinite loop

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    engine.load(url);

    //WDT for QT mainLoop.
    //@param restartApp  true: Blocking the  main event loop restarts the application  , false:  Blocking the  main event loop kills the application
    //@param warningTimeut_ms  timeout (ms) since locking the main event loop, once fulfilled, a warning message is displayed
    //@param restartTimeut_ms  timeout (ms) since locking the main event loop, once fulfilled, the application is killed or restarted
    //@param parent  QObject parent
    mainLoopWdt wdt(true,2000, 5000,&app);
    wdt.startWdt();//you can connect this slot to another signal for a delayed start of WDT

    return app.exec();
}
