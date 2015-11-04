#ifndef QTJACOMMANDERTHREAD_H
#define QTJACOMMANDERTHREAD_H
#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <ja_bus_protocol.h>
#include <ja_host.h>
#include <QMutex>


class QtJaCommanderThread: public QThread, QSerialPort
{
public:
    QtJaCommanderThread(void);
    ~QtJaCommanderThread(void);
    void addPeriodic(ja_host_request_header_t *);
    void addAsynchony(ja_host_request_header_t *);

private:
    void run();
    void performTransfer(QList<ja_host_request_header_t *>::iterator req);
    //QSerialPort serial;
    QList<ja_host_request_header_t *> periodicList;
    QList<ja_host_request_header_t *> asynchronyList;
    QMutex periodicLock;
    QMutex asynchronyLock;
    QMutex serialLock;
    QMutex releaseBusLock;

protected:


};


#endif // QTJACOMMANDERTHREAD_H
