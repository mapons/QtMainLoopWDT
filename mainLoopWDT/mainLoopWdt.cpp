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


#include "mainLoopWdt.h"

#include <QMutexLocker>
#include <QProcess>
#include <QDebug>
#include <QCoreApplication>

#include <cstdlib>

#ifndef Q_OS_WINDOWS
#include <unistd.h>
#else
#include "Windows.h"
#endif

mainLoopWdt::mainLoopWdt(bool restartApp,unsigned int warningTimeut_ms, unsigned int restartTimeut_ms,unsigned int appRestart_ms, QObject *parent) :
    QThread(parent),m_timerWarning(warningTimeut_ms),m_timerRestart(restartTimeut_ms),m_appRestart_ms(appRestart_ms),m_restart(restartApp)
{
    //coherent input data
    if(m_timerWarning<100) m_timerWarning=100;
    if(m_timerRestart<=m_timerWarning)m_timerRestart=m_timerWarning+1;
    if(m_timerWarning<m_timerMainloopCheck)m_timerMainloopCheck=m_timerWarning/2;

    setObjectName("WDT MainLoop thread");
    //timer mainloop
    m_timer_mainloop.setInterval(m_timerMainloopCheck);
    m_timer_mainloop.setSingleShot(false);
    connect(&m_timer_mainloop,&QTimer::timeout,this,&mainLoopWdt::resetWdtMainloop,Qt::DirectConnection);
    //timer thread
    m_timer_thread.moveToThread(this);
    m_timer_thread.setInterval(m_timerMainloopCheck);
    m_timer_thread.setSingleShot(false);
    connect(&m_timer_thread,&QTimer::timeout,this,&mainLoopWdt::checkWdtThread,Qt::DirectConnection);//Qt::DirectConnection mandatory
    //start and stop thread timer
    connect(this,&QThread::started,&m_timer_thread,static_cast<void (QTimer::*)()>(&QTimer::start),Qt::DirectConnection);
    connect(this,&QThread::finished,&m_timer_thread,&QTimer::stop,Qt::DirectConnection);
}

mainLoopWdt::~mainLoopWdt(){
    m_timer_mainloop.stop();
    quit();
    wait();
    qDebug()<<"WDT: destructor";
}

void mainLoopWdt::startWdt(){
    if(!isRunning()){
        qInfo()<<"WDT: start";
        m_tcpp.restart();
        m_timer_mainloop.start();//mainloop timer
        start(QThread::HighestPriority);//thread timer
    }
}

void mainLoopWdt::stopWdt(){
    qInfo()<<"WDT: stop";
    m_timer_mainloop.stop();//mainloop timer
    quit();
    wait();
}


void  mainLoopWdt::checkWdtThread() {
    {
        QMutexLocker m(&m_mutex);
        if(m_tcpp.isValid()){
            if(m_tcpp.elapsed()>(4*m_timerRestart )){
                //AFTER OS SUSPENSION, elapsed timer can be very big
                qDebug()<<"WDT: APP BLOCKED TO LONG"<< m_tcpp.elapsed();
                m_tcpp.restart();
                m_half=false;
            }
            if(!m_half && m_tcpp.elapsed()>m_timerWarning) {
                m_half=true;
                qWarning()<<"WDT: WARNING";
                emit mainLoopWarning();
            }
            if(m_half && m_tcpp.elapsed()>m_timerRestart) {
                m_exit=true;
                qWarning()<<"WDT: APP BLOCKED";
                emit mainLoopError();
            }
        }
    }

    //coment this if you want to manage kill/restart using external slots
    if(m_exit){
        if(m_restart) forceRestart();
        else forceExit();
    }
}

void mainLoopWdt::resetWdtMainloop(){
    QMutexLocker m(&m_mutex);
    m_tcpp.restart();
    m_half=false;
}

void mainLoopWdt::forceExit[[ noreturn ]](){
    qCritical()<<"WDT: FORCE EXIT APP";
    //MSDN: When the application is linked with a debug version of the run-time libraries, abort creates a message box with three buttons: Abort, Retry, and Ignore
    //std::abort(); //Abort https://en.cppreference.com/w/cpp/utility/program/abort
    //std::quick_exit(EXIT_FAILURE);//FORCE EXIT https://en.cppreference.com/w/cpp/utility/program/quick_exit
    std::exit(EXIT_FAILURE);// EXIT https://en.cppreference.com/w/cpp/utility/program/exit
}

void mainLoopWdt::forceRestart(){
    qCritical()<<"WDT: FORCE RESTART APP";
    auto list =QCoreApplication::arguments();
    //DELAYED RESTART
    if(m_appRestart_ms>0){
        QString progcmd;
        for(auto &arg:list){
            if(progcmd.size()>0)progcmd+=" ";
            progcmd+=arg;
        }

#ifdef Q_OS_WINDOWS
        if(m_appRestart_ms<1000)m_appRestart_ms=1000;
        progcmd="cmd.exe /C \"timeout /T "+QString::number((m_appRestart_ms+500)/1000,'g',0)+" & " + progcmd    +"\"";
        WinExec(progcmd.toStdString().c_str(),11);//SW_HIDE
#else
        progcmd="sleep " + QString::number(m_appRestart_ms/1000.0,'g',3)+"s;"+progcmd;
        QProcess::startDetached("bash",{"-c",progcmd} ); //start a new software instance
#endif
        //EXIT
        //MSDN: When the application is linked with a debug version of the run-time libraries, abort creates a message box with three buttons: Abort, Retry, and Ignore
        //std::abort(); //Abort https://en.cppreference.com/w/cpp/utility/program/abort
        //std::quick_exit(EXIT_FAILURE);//FORCE EXIT https://en.cppreference.com/w/cpp/utility/program/quick_exit
        std::exit(EXIT_FAILURE);
    }
#ifdef Q_OS_WINDOWS
    list.pop_front();
    QProcess::startDetached(QCoreApplication::arguments().at(0),list ); //start a new software instance
    //EXIT
    //MSDN: When the application is linked with a debug version of the run-time libraries, abort creates a message box with three buttons: Abort, Retry, and Ignore
    //std::abort(); //Abort https://en.cppreference.com/w/cpp/utility/program/abort
    //std::quick_exit(EXIT_FAILURE);//FORCE EXIT https://en.cppreference.com/w/cpp/utility/program/quick_exit
    std::exit(EXIT_SUCCESS);// EXIT https://en.cppreference.com/w/cpp/utility/program/exit

#else
    //ON UNIX BASED SYSTEMS:EXEC-> SAME PID ,NO ABORT
    std::vector<std::string> argsCopy;
    for(auto &val:list)argsCopy.push_back(val.toStdString());
    std::vector<const char *> argv2(argsCopy.size());
    std::transform(argsCopy.begin(), argsCopy.end(), argv2.begin(),[](std::string& str){return str.c_str();});
    argv2.push_back(0); //null termination for exec
    execvp(QCoreApplication::arguments().at(0).toLocal8Bit().data(),const_cast<char**>(argv2.data()));
#endif
    //DEATH CODE
    std::exit(EXIT_FAILURE);
}
void mainLoopWdt::forceAppRestart(){
    forceRestart();
}
