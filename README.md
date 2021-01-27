# Main loop watch dog timer for Qt & QML
When the main loop freezes, this component can kill or restart the current application, just by adding two lines of code.
You can also use the application restart function, for a complete restart of your application.

## See in action (video)
[![Demo in Windows 10](https://img.youtube.com/vi/kDTmXKUPyJU/0.jpg)](https://www.youtube.com/watch?v=kDTmXKUPyJU)

## HOW IT WORKS:
A timer activated by the main loop triggers watch function every 250 ms.
Another timer, triggered by a QThread, checks the status of whatchdog status every 250ms.
This function should not affect the performance of the application.
In case the app is suspended, the watchdog timer function restarts.
When the watchdog timer is activated, the application aborts its execution or restarts (configurable)

## USAGE:
create object in main function, just before main loop exec:

    mainLoopWdt wdt(true,2000, 5000,0, &app);
    wdt.startWdt();
    return app.exec();

## Tested OS:
 Ubuntu 20.04
 MacOS Big Sur
 Windows 10


## Tested main loop locks:
 QML js infinite loop
 C++ infinite loop
 C++ mutex double lock

## Restart estrategy
 Windows : QProcess::startDetached + std::exit (application arguments are kept)
 Linux & macOs : exec (application arguments are kept)

## Delayed restart estrategy
 Windows : cmd.exe
 Linux & MacOs : bash

## License
 Main loop WDT for Qt & QML is released under the terms of the **GNU LGPL v3 License**. Full details in `license.txt` file.
