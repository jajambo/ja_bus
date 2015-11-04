#include "jathread.h"

void JaThread::run()
{
    QList<ja_host_request_header_t *>::iterator i;

    ja_host_request_header_t *temp;
    while(1){
        //qDebug() << "Thread::stop called from main thread: "<<currentThreadId();
        //assume asynchrony request first priority

        cmd->asynchronyLock.lock();
        while(!cmd->asynchronyList.isEmpty()) {
            i = cmd->asynchronyList.begin();
            cmd->releaseBusLock.lock();
            //cmd->performTransfer(i);
            temp = (*i);
            emit tiggerTransfer(temp);
            //do asynchrony
            cmd->releaseBusLock.unlock();
            //remove it from asynchronyList.
            cmd->asynchronyList.removeFirst();

        }
        cmd->asynchronyLock.unlock();

        cmd->periodicLock.lock();
        i = cmd->periodicList.begin();
        while (i != cmd->periodicList.end()) {
            cmd->releaseBusLock.lock();
            //do periodic
            //1. check count = 0, means need perform a transfer.
            if (((*i)->status) && JA_REQUEST_TRIGGER){
                //performTransfer(i);
                temp = *i;
                emit tiggerTransfer(temp);
                (*i)->status &= ~JA_REQUEST_TRIGGER;
            }
            ++i;
            cmd->releaseBusLock.unlock();
        }
        cmd->periodicLock.unlock();
        //exec();
    }
}
JaThread::JaThread(QtJaCommanderThread *t)
{
    cmd = t;
}

