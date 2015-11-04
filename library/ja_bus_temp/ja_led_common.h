
#ifndef _JA_LED_COMMON_H
#define _JA_LED_COMMON_H
#include <ja_bus_protocol.h>
#pragma pack(push)
#pragma pack(1)
typedef struct _ja_led_controller_register {
  ja_dev_register_t commonReg;
  float  currentValue;
  uint8_t  ledSwtich;  
}ja_led_controller_register_t;
#pragma pack(pop)
#endif //_JA_LED_COMMON_H