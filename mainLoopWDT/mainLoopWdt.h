// Copyright (C) 2021 Dianiux Innova SL
// mapons@dianiuxinnova.com

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU Lesser General Public
//License as published by the Free Software Foundation; either
//version 3 of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//Lesser General Public License for more details.

//You should have received a copy of the GNU Lesser General Public License
//along with this program; If not, see <http://www.gnu.org/licenses/>.

#ifndef mainLoopWdt_H
#define mainLoopWdt_H

#include <QThread>
#include <QTimer>
#include <QElapsedTimer>
#include <QMutex>
class mainLoopWdt : public QThread
{
    Q_OBJECT
public:
    /**
        WDT for QT mainLoop.
        @param AppRestart  true: Blocking the  main event loop restarts the application  , false:  Blocking the  main event loop kills the application
        @param warningTimeut_ms  timeout (ms) since locking the main event loop, once fulfilled, a warning message is displayed
        @param restartTimeut_ms  timeout (ms) since locking the main event loop, once fulfilled, the application is killed or restarted
        @param appRestart_ms timeout (ms) to restart application. Maybe some software may need time after shutdown before restarting
        @param parent  QObject parent
    */
    explicit mainLoopWdt(bool AppRestart,unsigned int warningTimeut_ms=2000, unsigned int restartTimeut_ms=10000,unsigned int appRestart_ms=0, QObject *parent = nullptr);
    ~mainLoopWdt() override;
signals:
    //emited on WDT warning
    void mainLoopWarning();
       //emited on WDT error (main loop blocked)
    void mainLoopError();
public slots:
    //start WDT monitor
    void startWdt();
    //stop WDT monitor
    void stopWdt();
    //Force app restart
    void forceAppRestart();
private slots:
    //main loop reset WDT
    void resetWdtMainloop();
    //thread check WDT
    void checkWdtThread();
private:

    void forceExit[[ noreturn ]] ();
    void forceRestart[[ noreturn ]] ();

    QMutex m_mutex;
    QElapsedTimer m_tcpp;
    QTimer m_timer_mainloop;//mainloop timer
    QTimer m_timer_thread;//thread timer
    unsigned int m_timerWarning=0;
    unsigned int m_timerRestart=0;
    unsigned int m_timerMainloopCheck=250;
    unsigned int m_appRestart_ms=0;
    bool m_exit=false;
    bool m_restart=false;
    bool m_half=false;
};

#endif // mainLoopWdt_H
