#include <QCoreApplication>
#include <QtCore>
#include <QObject>

#include <QtSerialPort/QSerialPort>
#include <qt_ja_commander_thread.h>
#include <qt_ja_led_control.h>
#include <jathread.h>
QSerialPort serial;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QtJaCommanderThread *jaCmd = new QtJaCommanderThread((QObject*)&a);
    JaThread *t;
    QtJaLedControl *led;
    t = new JaThread(jaCmd);

    QObject::connect(t, SIGNAL(tiggerTransfer(ja_host_request_header_t*)), jaCmd, SLOT(performTransfer(ja_host_request_header_t*)));
    led = new QtJaLedControl(jaCmd);
    //QTimer::singleShot(0, jaCmd, SLOT(run()));
    t->start();
    return a.exec();
}

