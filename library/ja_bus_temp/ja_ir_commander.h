

#ifndef _JA_IR_COMMANDER_H
#define _JA_IR_COMMANDER_H
#include <HardwareSerial.h>
#include <ja_device.h>


#define IR_SUPPORT	0x01



#define DEFAULT_SAMPLE_RATE   5*1000  // 1sec



#define JA_IR_TEMP_25           0x2D09097B
#define JA_IR_TEMP_25_OFF       0x2E002833
#define JA_IR_TEMP_24           0x84FCC86D
#define JA_IR_TEMP_23           0x91F211B9
#define JA_IR_TEMP_22           0x25D13269
#define JA_IR_TEMP_21           0xDE0981CD
#define JA_IR_TEMP_20           0x4BC49A03
#define JA_IR_TEMP_19           0xF3292079
#define JA_IR_TEMP_18           0x38C9F56F
#define JA_IR_TEMP_17           0xD78FBDA1
#define JA_IR_TEMP_16           0x6B6EDE51
#define JA_IR_TEMP_15           0xF7C3E05F
#define JA_IR_TEMP_14           0x867E9949


#pragma pack(push)
#pragma pack(1)

typedef struct _ja_ir_commander_register {
  ja_dev_register_t commonReg;
  unsigned int  irCommand;
  unsigned int  start;
  
}ja_ir_commander_register_t;


#pragma pack(pop)





class JAIrCommanderDevice: public JADevice
{
  private:



  protected:
    void  processClassCmd(void);
  public:
    JAIrCommanderDevice(HardwareSerial *);
    virtual void	run(void);
};



#endif //_JA_IR_COMMANDER_H
