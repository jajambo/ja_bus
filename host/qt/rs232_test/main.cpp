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

    QtJaCommanderThread *jaCmd = new QtJaCommanderThread();

    QtJaLedControl *led;


    QObject::connect(jaCmd, SIGNAL(requestTransfer(char *, int)), jaCmd, SLOT(sendData(char*,int)));
    QObject::connect(jaCmd, SIGNAL(requestData(char *, int)), jaCmd, SLOT(readData(char*,int)));
    led = new QtJaLedControl(jaCmd);
    //QTimer::singleShot(0, jaCmd, SLOT(run()));
    jaCmd->start();
    return a.exec();
}

