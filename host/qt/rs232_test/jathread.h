#ifndef JATHREAD_H
#define JATHREAD_H
#include <QThread>
#include <ja_host.h>
#include <qt_ja_commander_thread.h>
#include <QList>

class JaThread : public QThread
{
    Q_OBJECT
public:
    JaThread(QtJaCommanderThread *);
private:
        QtJaCommanderThread *cmd;
        void run();
signals:
        void tiggerTransfer(ja_host_request_header_t *);
};

#endif // JATHREAD_H
