#include <QCoreApplication>
#include <QtCore>
#include <QObject>

#include <QtSerialPort/QSerialPort>
#include <qt_ja_commander_thread.h>
#include <qt_ja_led_control.h>

QSerialPort serial;



class Thread : public QThread
{


private:
    void run()
    {
        qDebug()<<"From work thread: "<<currentThreadId();
        QTimer timer;
        QtJaCommanderThread t1;
        QtJaLedControl *led;
        led = new QtJaLedControl(&t1);


        exec();
    }
};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Thread t;
    t.start();
    //QtJaCommanderThread t;
    //QtJaLedControl *led;
    //QObject::connect(&t, SIGNAL(finished()), &a, SLOT(quit()));
    //led = new QtJaLedControl(&t);
    //t.start();
    return a.exec();
}

