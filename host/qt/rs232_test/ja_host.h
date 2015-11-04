#ifndef JA_HOST
#define JA_HOST
#include <stdint-gcc.h>
#include <ja_bus_protocol.h>

typedef void (*jaRequestCallBack)(void *);
typedef struct _ja_host_request_header {
    ja_host_command_header_t cmd_header;
    uint64_t periodic;
    uint64_t count;
    jaRequestCallBack call_back;

    uint16_t status;
    uint16_t receive_count;
    uint16_t send_count;
    uint8_t	*receive_buf;
    uint8_t	*send_buf;
}ja_host_request_header_t;

#define JA_REQUEST_TRIGGER      0x01
#define JA_REQUEST_COMPLETED    0x02
#define JA_REQUEST_TIMEOUT      0x04
#define JA_REQUEST_NACK         0x08


#endif // JA_HOST

