#include <QCoreApplication>
#include <QtCore>
#include <QObject>

#include <QtSerialPort/QSerialPort>
#include <qt_ja_commander_thread.h>
#include <qt_ja_led_control.h>

QSerialPort serial;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QtJaCommanderThread t;
    QtJaLedControl *led;
    //QObject::connect(&t, SIGNAL(finished()), &a, SLOT(quit()));
    led = new QtJaLedControl(&t);
    t.start();
    return a.exec();
}

