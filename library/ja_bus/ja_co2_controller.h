

#ifndef _JA_CO2_CONTROLLER_H
#define _JA_CO2_CONTROLLER_H
#include <HardwareSerial.h>
#include <ja_device.h>


#define SWITCH_SUPPORT	0x01
#define CURRENT_METER_SUPPORT	0x02
#define FLOW_METER_SUPPORT	0x04


#define DEFAULT_SAMPLE_RATE   5*1000  // 1sec
#pragma pack(push)
#pragma pack(1)
typedef struct _ja_co2_controller_register {
  ja_dev_register_t commonReg;
  unsigned int  co2Swtich;
  unsigned int  currentValue;
  unsigned int  flowValue;
  
}ja_co2_controller_register_t;

#pragma pack(pop)






class JACo2ControllerDevice: public JADevice
{
  private:
    unsigned int co2Switch;
    unsigned int currentValue;
    unsigned int flowValue;


  protected:
    void  processClassCmd(void);
  public:
    JACo2ControllerDevice(HardwareSerial *);
    virtual void	run(void);
};



#endif //_JA_CO2_CONTROLLER_H
