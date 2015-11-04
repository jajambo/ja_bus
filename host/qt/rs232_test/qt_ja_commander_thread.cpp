#include "qt_ja_commander_thread.h"
#include <QDebug>

int QtJaCommanderThread::sendData(char *buf, int i)
{
    return serial->write((const char *)buf, i);
}

int QtJaCommanderThread::readData(char *buf, int i)
{
        qDebug()<<"readData";
    if(serial->bytesAvailable()) {
        qDebug()<<"bytesAvailable";
        return serial->read((char *)(buf + i), (qint64)i);

    }
    return 0;
}


void QtJaCommanderThread::performTransfer(ja_host_request_header_t * req)
{
    uint8_t direct;
    uint8_t transBuf[64];
    uint8_t receiveBuf[64];
    uint8_t *buf, *dest;
    uint16_t crc;
    uint i = 0, sendBytes, bytes;
    uint16_t  count = 0;

    qDebug()<<"performTransfer";

direct = req->cmd_header.cmd & JA_READ;
    buf = (uint8_t *) (&(req->cmd_header));
    for(sendBytes = 0; sendBytes < sizeof(ja_host_command_header_t); sendBytes++) {
        transBuf[sendBytes] = *(buf + sendBytes);
    }
    if (direct) {
        sendBytes += sizeof(crc);

        //serial->write((const char *)&transBuf, i);
        emit requestTransfer((char *)&transBuf,sendBytes);

        //wait writeData completed

        serial->flush();
       while (serial->bytesToWrite()) {
           sleep(100);
        }
        qDebug()<<"after write";
        //get response

        count = req->receive_count + sizeof(crc) + sizeof(ja_host_command_header_t);

        i = 0;


        while (count)  {
            qDebug()<<"count"<<count;

                if(serial->waitForReadyRead(500)){
                bytes= serial->read((char *)((&receiveBuf) + i), count);
                qDebug()<<"received bytes"<<bytes;
                i += bytes;
                count -= bytes ;

                }
                else{
                    qDebug()<<"read timeout";
                    emit requestTransfer((char *)&transBuf,sendBytes);
                    while (serial->bytesToWrite()) {
                        sleep(100);
                    }
                }
                //sleep(100);


        }

        dest = req->receive_buf;
        buf = (uint8_t *)(&receiveBuf + sizeof(ja_host_command_header_t));
        for (i=0; i < req->receive_count; i++) {
            *(dest + i) = *(buf + i);
        }

        //TODO send ack;


    } else {

        while (count < req->send_count) {
            transBuf[i] = req->send_buf[count];
            count ++;
        }
        i += sizeof(crc);
        //serial->write((const char *) &transBuf, i);
        emit requestTransfer((char *)&transBuf,i);
        //wait writeData completed
        while (serial->bytesToWrite()) {

        }

        //TODO wait ack
    }
    //after transfer completed call callback.
    if (req->call_back)
        req->call_back((void*)req);


}

void QtJaCommanderThread::run()
{
    QList<ja_host_request_header_t *>::iterator i;

    ja_host_request_header_t *temp;
    while(1){
        //qDebug() << "Thread::stop called from main thread: "<<currentThreadId();
        //assume asynchrony request first priority

        asynchronyLock.lock();
        while(!asynchronyList.isEmpty()) {
            i = asynchronyList.begin();
            releaseBusLock.lock();
            //performTransfer(i);
            temp = (*i);
            performTransfer(temp);
            //emit tiggerTransfer(temp);
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
                //performTransfer(i);
                temp = *i;
                performTransfer(temp);
                //emit tiggerTransfer(temp);
                (*i)->status &= ~JA_REQUEST_TRIGGER;
            }
            ++i;
            releaseBusLock.unlock();
        }
        periodicLock.unlock();
        //exec();
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
    serial = new QSerialPort();
    serial->setPortName("COM3");
    serial->setBaudRate(9600);
    if(!serial->open(QIODevice::ReadWrite)) {
        printf("open serial port fail\n");
    }
}


QtJaCommanderThread::~QtJaCommanderThread()
{
    serial->close();
}


