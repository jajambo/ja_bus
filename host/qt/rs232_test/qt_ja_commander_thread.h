#ifndef QTJACOMMANDERTHREAD_H
#define QTJACOMMANDERTHREAD_H
#include <QObject>
#include <QSerialPort>
#include <QThread>
#include <ja_bus_protocol.h>
#include <ja_host.h>
#include <QMutex>
#include <QCoreApplication>


class QtJaCommanderThread: public QThread
{
    Q_OBJECT
public:
    QtJaCommanderThread(void);
    QtJaCommanderThread(QObject *);
    ~QtJaCommanderThread(void);
    void addPeriodic(ja_host_request_header_t *);
    void addAsynchony(ja_host_request_header_t *);


    void run();
protected:
    //void performTransfer(QList<ja_host_request_header_t *>::iterator req);
    //QSerialPort serial;
    QList<ja_host_request_header_t *> periodicList;
    QList<ja_host_request_header_t *> asynchronyList;
    QMutex periodicLock;
    QMutex asynchronyLock;
    QMutex serialLock;
    QMutex releaseBusLock;


    QSerialPort *serial;
signals:
    int requestTransfer(char *, int);
    int requestData(char *, int);

private slots:
    void performTransfer(ja_host_request_header_t *);
    int sendData(char *, int);
    int readData(char *, int);
    friend class JaThread;
};


#endif // QTJACOMMANDERTHREAD_H
