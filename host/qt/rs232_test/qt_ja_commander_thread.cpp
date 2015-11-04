#include "qt_ja_commander_thread.h"
#include <QDebug>
#include <QtSerialPort/QSerialPort>


void QtJaCommanderThread::performTransfer(QList<ja_host_request_header_t *>::iterator req)
{
    uint8_t direct = (*req)->cmd_header.cmd & JA_READ;
    uint8_t transBuf[64];
    uint8_t receiveBuf[64];
    uint8_t *buf, *dest;
    uint16_t crc;
    uint i = 0;
    uint16_t  count = 0;

    qDebug()<<"performTransfer" << sizeof(ja_host_command_header_t);
    buf = (uint8_t *) (&(*req)->cmd_header);
    for(i = 0; i < sizeof(ja_host_command_header_t); i++) {
        transBuf[i] = *(buf + i);
    }

    if (direct) {

        i += sizeof(crc);
        qDebug()<<"performRead"<<i;
        for(int tindex=0; tindex < i; tindex++)
            printf("%x ", transBuf[tindex]);
        printf("\n");
        //for(count = 0; count < i; count ++)
            qDebug()<<"output "<<serial->write((const char *)transBuf, i);
        //wait writeData completed
        qDebug()<<"beforeWriteCmd";
        while (serial->bytesToWrite()) {
            //msleep(2);
        }
        qDebug()<<"afterWriteCmd";
        //get response
        count = (*req)->receive_count + sizeof(crc) + sizeof(ja_host_command_header_t);
        i = 0;
        while (count)  {
            if(serial->bytesAvailable()) {
                i= serial->read((char *)((&receiveBuf) + i), (qint64)count);
                count -= i ;
            }
        }
        dest = (*req)->receive_buf;
        buf = (uint8_t *)(&receiveBuf + sizeof(ja_host_command_header_t));
        for (i=0; i < (*req)->receive_count; i++) {
            *(dest + i) = *(buf + i);
        }

        //TODO send ack;


    } else {
        qDebug()<<"performWrite";
        while (count < (*req)->send_count) {
            transBuf[i] = (*req)->send_buf[count];
            count ++;
        }
        i += sizeof(crc);
        QByteArray a;
        a.append((char)0x22);
        serial->write(a);
        //wait writeData completed
        while (serial->bytesToWrite()) {
            //msleep(2);
        }

        //TODO wait ack
    }
    //after transfer completed call callback.
    if ((*req)->call_back)
        (*req)->call_back((void*)&req);


}

void QtJaCommanderThread::timerEvent(QTimerEvent *)
{
    QList<ja_host_request_header_t *>::iterator i;
    qDebug() << "onTimerEvent(): ";

   // while(1){
    //    qDebug() << "Thread::stop called from main thread: "<<currentThreadId();
        //assume asynchrony request first priority

        asynchronyLock.lock();
        if (!asynchronyList.isEmpty())
            qDebug()<<"asynchronyList is not empty";
        else
            qDebug()<<"asynchronyList is empty";
        while(!asynchronyList.isEmpty()) {

            i = asynchronyList.begin();
            qDebug() << "asynchronyList(): "<<&i;
            releaseBusLock.lock();
            performTransfer(i);
            //do asynchrony
            releaseBusLock.unlock();
            //remove it from asynchronyList.
            asynchronyList.removeFirst();

        }
        asynchronyLock.unlock();

        periodicLock.lock();
        if (!periodicList.isEmpty()) {
            qDebug()<<"periodicList is not empty";
            i = periodicList.begin();
            while (i != periodicList.end()) {
                qDebug() << "periodicList(): "<<&i;
                releaseBusLock.lock();
                //do periodic
                //1. check count = 0, means need perform a transfer.
                if (((*i)->status) && JA_REQUEST_TRIGGER){
                    performTransfer(i);
                    (*i)->status &= ~JA_REQUEST_TRIGGER;
                }
                ++i;
                releaseBusLock.unlock();
            }
        } else {
            qDebug()<<"periodicList is empty";
        }
        periodicLock.unlock();
    //}
}

void QtJaCommanderThread::addAsynchony(ja_host_request_header_t *request)
{
    qDebug() << "addAsynchony(): ";
    asynchronyLock.lock();
    asynchronyList.append(request);
    asynchronyLock.unlock();
}

void QtJaCommanderThread::addPeriodic(ja_host_request_header_t *request)
{
    qDebug() << "addPeriodic(): ";
    periodicLock.lock();
    periodicList.append(request);
    if(periodicList.isEmpty())
        qDebug()<<"periodicList is empty";
    else
        qDebug()<<"periodicList is not empty";
    periodicLock.unlock();
}

QtJaCommanderThread::QtJaCommanderThread()
{
    serial = new QSerialPort(this);
    serial->setPortName("COM4");
    serial->setBaudRate(9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if(!serial->open(QIODevice::ReadWrite)) {
        printf("open serial port fail\n");
    }
    currentTimerId = startTimer(1000);
}


QtJaCommanderThread::~QtJaCommanderThread()
{
    serial->close();
}
