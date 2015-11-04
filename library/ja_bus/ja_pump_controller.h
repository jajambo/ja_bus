

#ifndef _JA_PUMP_CONTROLLER_H
#define _JA_PUMP_CONTROLLER_H
#include <HardwareSerial.h>
#include <ja_device.h>


#define PUMP_SUPPORT	0x01
#define CURRENT_METER_SUPPORT	0x02
#define FLOW_METER_SUPPORT	0x04



#define DEFAULT_SAMPLE_RATE   5*1000  // 1sec
#pragma pack(push)
#pragma pack(1)
typedef struct _ja_pump_controller_register {
  ja_dev_register_t commonReg;
  unsigned int  flowValue;
  unsigned int  currentValue;
  unsigned int  flowMeterValue;
  unsigned int  startPwm;
}ja_pump_controller_register_t;

#pragma pack(pop)






class JAPumpControllerDevice: public JADevice
{
  private:



  protected:
    void  processClassCmd(void);
  public:
    JAPumpControllerDevice(HardwareSerial *);
    virtual void	run(void);
};



#endif //_JA_PUMP_CONTROLLER_H
