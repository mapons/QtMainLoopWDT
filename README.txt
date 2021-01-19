Main loop WDT for Qt & QML
Just adding just two lines of code.
When the main loop freezes, this component can kill or restart the current application.

DEMO:
https://www.youtube.com/watch?v=kDTmXKUPyJU

USAGE:
create object in main function, just before main loop exec:

    //WDT for QT mainLoop.
    //@param restartApp  true: Blocking the  main event loop restarts the application  , false:  Blocking the  main event loop kills the application
    //@param warningTimeut_ms  timeout (ms) since locking the main event loop, once fulfilled, a warning message is displayed
    //@param restartTimeut_ms  timeout (ms) since locking the main event loop, once fulfilled, the application is killed or restarted
    //@param parent  QObject parent
    mainLoopWdt wdt(true,2000, 5000,&app);
    wdt.startWdt(); //you can connect this slot to another signal for a delayed start of WDT

    return app.exec();
    }
    
Tested OS: 
 -Ubuntu 20.04
 -MacOS Big Sur
 -Windows 10


Tested main loop locks:
 -QML js infinite loop
 -C++ infinite loop
 -C++ mutex double lock

Restart estrategy (application arguments are kept)
 -Windows : QProcess::startDetached + std::exit
 -Linux & macOs : exec
