#include "qt_ja_led_control.h"
#include <ja_led_common.h>
#include <math.h>
#include <QDebug>

void QtJaLedControl::timerEvent(QTimerEvent * tEvent)
{
    qDebug()<<"timerEvent";
    if(!(reqCurrent.status & JA_REQUEST_TRIGGER))
        reqCurrent.status |= JA_REQUEST_TRIGGER;
}

QtJaLedControl::QtJaLedControl(QtJaCommanderThread * thread)
{
    cmdThread = thread;

    reqCurrent.cmd_header.cmd = CMD_READ_4_BYTE;
    reqCurrent.cmd_header.slave_info.slave_class = LIGHT_CONTROLLER;
    reqCurrent.cmd_header.slave_info.slave_id = 0x01;
    reqCurrent.cmd_header.reg_index = offsetof(ja_led_controller_register_t, currentValue);
    reqCurrent.cmd_header.start_flag = HOST_START_FLAG;
    reqCurrent.call_back = NULL;
    reqCurrent.status = 0;
    reqCurrent.receive_count = sizeof(currentValue);
    reqCurrent.receive_buf = (uint8_t *)&currentValue;
    cmdThread->addPeriodic(&reqCurrent);
    currentTimerId = startTimer(1000);
    reqCurrent.status |= JA_REQUEST_TRIGGER;

}


QtJaLedControl::~QtJaLedControl()
{
    if(currentTimerId != 0)
            killTimer(currentTimerId);
}

