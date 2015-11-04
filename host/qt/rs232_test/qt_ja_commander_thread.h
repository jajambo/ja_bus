#ifndef QTJACOMMANDERTHREAD_H
#define QTJACOMMANDERTHREAD_H
#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QThread>
#include <ja_bus_protocol.h>
#include <ja_host.h>
#include <QMutex>


class QtJaCommanderThread : public QObject
{

public:
    QtJaCommanderThread(void);
    ~QtJaCommanderThread(void);
    void addPeriodic(ja_host_request_header_t *);
    void addAsynchony(ja_host_request_header_t *);
    QSerialPort *serial;


private:

    //void run();
    void performTransfer(QList<ja_host_request_header_t *>::iterator req);
    //QSerialPort serial;
    QList<ja_host_request_header_t *> periodicList;
    QList<ja_host_request_header_t *> asynchronyList;
    QMutex periodicLock;
    QMutex asynchronyLock;
    QMutex serialLock;
    QMutex releaseBusLock;

protected:

    void timerEvent(QTimerEvent *);
    int currentTimerId;
};


#endif // QTJACOMMANDERTHREAD_H
