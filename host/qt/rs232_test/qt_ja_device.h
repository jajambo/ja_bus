#ifndef QT_JA_DEVICE
#define QT_JA_DEVICE
#include <QObject>
#include <ja_bus_protocol.h>
#include <ja_host.h>

class QtJaDevice:public QObject
{
private:
    ja_host_request_header_t reportHeader;
    QList<ja_host_request_header_t> headerList;
protected:

public:
    QtJaDevice();
    ~QtJaDevice();
    void onCommandCompleted(ja_host_request_header_t *);
    void onDevReport(ja_host_request_header_t *);
};

#endif // QT_JA_DEVICE
