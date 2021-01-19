#ifndef UTILS_H
#define UTILS_H

#include <QObject>

class  Utils: public QObject
{
    Q_OBJECT
public:
    explicit Utils(QObject *parent = nullptr);
    //C++ main loop lock tests
    Q_INVOKABLE void infiniteLoop ();
    Q_INVOKABLE void temporalBlockMainLoop(unsigned int miliseconds);
    Q_INVOKABLE void mutexDoubleLock();
};

#endif // UTILS_H
