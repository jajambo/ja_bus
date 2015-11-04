

#ifndef _JA_AIR_METER_H
#define _JA_AIR_METER_H
#include <HardwareSerial.h>
#include <ja_device.h>
#define MH_Z14_SUPPORT  1
#if MH_Z14_SUPPORT
#include <mh_z14.h>
#endif

#define CO2_METER_SUPPORT	0x01
#define TEMPTURE_METER_SUPPORT	0x02
#define HUMIDITY_METER_SUPPORT	0x04

#define DEFAULT_SAMPLE_RATE   5*1000  // 1sec
#pragma pack(push)
#pragma pack(1)
typedef struct _ja_air_meter_register {
  ja_dev_register_t commonReg;
  unsigned int  co2Value;
  unsigned int  tempValue;
  unsigned int  humidityValue;
	unsigned char	supportFunction;
	unsigned char sampleRage;
}ja_air_meter_register_t;
#pragma pack(pop)








class JAAirMeterDevice: public JADevice
{
  private:
    HardwareSerial *co2_serial;
    mhz14Co2Meter *mhz14;

  protected:
    void  processClassCmd(void);
  public:
    JAAirMeterDevice(HardwareSerial *);
    virtual void  co2SerialIrq(void);
    virtual void	run(void);
};



#endif //_JA_AIR_METER_H
