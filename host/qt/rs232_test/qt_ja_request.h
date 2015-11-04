#ifndef QT_JA_REQUEST
#define QT_JA_REQUEST
#include <QObject>
#include <ja_bus_protocol.h>
#include <ja_host.h>
class QtJaRequest:public QObject
{
    Q_OBJECT
private:
    ja_host_request_header_t requestHeader;
};

#endif // QT_JA_REQUEST

