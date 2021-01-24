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
    //argument test
    auto argList =QCoreApplication::arguments();
    qDebug()<<"APP ARGUMENTS:"<<argList;
    argList.pop_front();
    QString env_wdt_test=qgetenv("WDT_TEST");
    qDebug()<<"ENV WDT_TEST="<<env_wdt_test;
    qputenv("WDT_TEST","test ok"); //set magic word to WDT_TEST

    QQmlApplicationEngine engine;

    //WDT for QT mainLoop.
    //@param restartApp  true: Blocking the  main event loop restarts the application  , false:  Blocking the  main event loop kills the application
    //@param warningTimeut_ms  timeout (ms) since locking the main event loop, once fulfilled, a warning message is displayed
    //@param restartTimeut_ms  timeout (ms) since locking the main event loop, once fulfilled, the application is killed or restarted
    //@param appRestart_ms timeout (ms) to restart application. Maybe some software may need time after shutdown before restarting
    //@param parent  QObject parent
    mainLoopWdt wdt(true,2000, 5000,0,&app); //Declare WWDT Instance

    Utils util;//utility for C++ infinite loop
    engine.rootContext()->setContextProperty("Utils", &util);//test: C++ main loop block
    engine.rootContext()->setContextProperty("WDT", &wdt);//test: force restart
    engine.rootContext()->setContextProperty("ENV",env_wdt_test); //test: keep environment variables between reboots
    engine.rootContext()->setContextProperty("ARGLIST",argList); //test: keep process arguments
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    wdt.startWdt();//start WDT
    return app.exec();
}
