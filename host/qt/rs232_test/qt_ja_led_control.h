#ifndef QT_JA_LED_CONTROL
#define QT_JA_LED_CONTROL
#include <QObject>
#include <qt_ja_commander_thread.h>
#include <qt_ja_device.h>
#include <ja_host.h>
#include <QList>
class QtJaLedControl : public QtJaDevice
{
private:
    QtJaCommanderThread * cmdThread;
    float currentValue;
    uint8_t ledSwitch;
    QList<ja_host_request_header_t *> dynamicList;
    ja_host_request_header_t reqCurrent;
protected:
    void timerEvent(QTimerEvent *);
    int currentTimerId;
public:
    QtJaLedControl(QtJaCommanderThread * thread);
    ~QtJaLedControl();
};
#endif // QT_JA_LED_CONTROL
