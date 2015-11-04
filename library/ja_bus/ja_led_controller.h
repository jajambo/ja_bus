

#ifndef _JA_LED_CONTROLLER_H
#define _JA_LED_CONTROLLER_H
#include <HardwareSerial.h>
#include <ja_device.h>
#include <ja_led_common.h>



#define SWITCH_SUPPORT	0x01
#define CURRENT_METER_SUPPORT	0x02


#define DEFAULT_SAMPLE_RATE   5*1000  // 1sec








class JALedControllerDevice: public JADevice
{
  private:
    uint8_t ledSwitch;
    float currentValue;
    uint8_t currentPin;
    uint8_t powerPin;


  protected:
    void  processClassCmd(void);
    void  getCurrentReg(void);
    void  processSwitchReg(void);
  public:
    JALedControllerDevice(HardwareSerial *, uint8_t, uint8_t);
    virtual void	run(void);
};



#endif //_JA_LED_CONTROLLER_H
