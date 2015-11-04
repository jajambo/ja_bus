#include "qt_ja_commander_thread.h"
#include <QDebug>



void QtJaCommanderThread::performTransfer(QList<ja_host_request_header_t *>::iterator req)
{
    uint8_t direct = (*req)->cmd_header.cmd & JA_READ;
    uint8_t transBuf[64];
    uint8_t receiveBuf[64];
    uint8_t *buf, *dest;
    uint16_t crc;
    uint i = 0;
    uint16_t  count = 0;
    qDebug()<<"performTransfer";
    buf = (uint8_t *) (&(*req)->cmd_header);
    for(i = 0; i < sizeof(ja_host_command_header_t); i++) {
        transBuf[i] = *(buf + i);
    }
    if (direct) {
        i += sizeof(crc);
        writeData((const char *)&transBuf, i);
        //wait writeData completed
        while (bytesToWrite()) {
            msleep(2);
        }
        //get response
        count = (*req)->receive_count + sizeof(crc) + sizeof(ja_host_command_header_t);
        i = 0;
        while (count)  {
            if(bytesAvailable()) {
                i= readData((char *)((&receiveBuf) + i), (qint64)count);
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

        while (count < (*req)->send_count) {
            transBuf[i] = (*req)->send_buf[count];
            count ++;
        }
        i += sizeof(crc);
        writeData((const char *) &transBuf, i);
        //wait writeData completed
        while (bytesToWrite()) {
            msleep(2);
        }

        //TODO wait ack
    }
    //after transfer completed call callback.
    if ((*req)->call_back)
        (*req)->call_back((void*)&req);


}

void QtJaCommanderThread::run()
{
    QList<ja_host_request_header_t *>::iterator i;

    while(1){
        qDebug() << "Thread::stop called from main thread: "<<currentThreadId();
        //assume asynchrony request first priority

        asynchronyLock.lock();
        while(!asynchronyList.isEmpty()) {
            i = asynchronyList.begin();
            releaseBusLock.lock();
            performTransfer(i);
            //do asynchrony
            releaseBusLock.unlock();
            //remove it from asynchronyList.
            asynchronyList.removeFirst();

        }
        asynchronyLock.unlock();

        periodicLock.lock();
        i = periodicList.begin();
        while (i != periodicList.end()) {
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
        periodicLock.unlock();
    }
}

void QtJaCommanderThread::addAsynchony(ja_host_request_header_t *request)
{
    asynchronyLock.lock();
    asynchronyList.append(request);
    asynchronyLock.unlock();
}

void QtJaCommanderThread::addPeriodic(ja_host_request_header_t *request)
{
    periodicLock.lock();
    periodicList.append(request);
    periodicLock.unlock();
}

QtJaCommanderThread::QtJaCommanderThread()
{
    setPortName("COM4");
    setBaudRate(9600);
    if(open(QIODevice::ReadWrite)) {
        printf("open serial port fail\n");
    }
}


QtJaCommanderThread::~QtJaCommanderThread()
{
    close();
}
