#include "utils.h"
#include <QMutex>
#include <QElapsedTimer>
Utils::Utils(QObject *parent) : QObject(parent)
{
}

void Utils::infiniteLoop (){
    while(true){};
}

void Utils::temporalBlockMainLoop(unsigned int miliseconds){
    QElapsedTimer etimer;
    etimer.start();
    while(true){
        if(etimer.elapsed()>miliseconds)return;
    };
}

void Utils::mutexDoubleLock(){
    QMutex mutex;
    mutex.lock();
    mutex.lock();//double lock
}
