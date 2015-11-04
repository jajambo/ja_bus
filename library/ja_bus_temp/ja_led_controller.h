

#ifndef _JA_LED_CONTROLLER_H
#define _JA_LED_CONTROLLER_H
#include <HardwareSerial.h>
#include <ja_device.h>
#include <ja_led_common.h>



#define SWITCH_SUPPORT	0x01
#define CURRENT_METER_SUPPORT	0x02


#define DEFAULT_SAMPLE_RATE   5*1000  // 1sec




class JALedControllerDevice
{
  protected:
    ja_device_command_t rx_cmd_buf;
    ja_device_command_t tx_cmd_buf;
    ja_dev_eeprom_t dev_eeprom;
		ja_slave_info_t slave_info;
		float currentValue;
		HardwareSerial *dev_serial;
    char device_rank = DEV_DEFAULT_RANK;
    unsigned char	device_state = DEV_WAIT_FOR_SYNC;
    unsigned char	host_sn[16];
    unsigned char temp_buf[BUFFER_SIZE];
    unsigned char	device_status = 0;
    unsigned char	reportId;
    unsigned char	reportData[4];
	    uint8_t ledSwitch;
    uint8_t currentPin;
    uint8_t powerPin;
    
    char  cmd_jitter;
    char  ack_jitter;
    char  sync_jitter;
    char  rx_jitter;
    char  temp_count;
    char  rest_count;
		bool	dev_sn_empty;
		bool	host_sn_empty;
		bool	dev_id_empty;


    void  syncFromEeprom(void);
    void  updateSn(char *, char *);
		void	syncJitter(void);
		void	waitForSync(void);
		void	rankDelay(unsigned int delay);
		void	tryRequestId(void);
		void	beforeSendCmd(void);
		void	afterSendCmd(void);
    void  programDevId(void);
    void  programDevSn(void);
    void  programHostSn(void);
    bool  waitForCmd(char, unsigned int);
    void  tryProcessCmd(void);
    void  tryRequest(void);
    void  dispatchCmd(void);
    void  forceStop(void);
    void  functionStop(void);
    void  tryClassProcessCmd(void);
    void  classProcess(void);
    void  waitForAck(unsigned int timeout);
    void  processClassCmd(void);
    void  processJADevCmd(void);
	    void  getCurrentReg(void);
    void  processSwitchReg(void);
    


  public:
      JALedControllerDevice(HardwareSerial *, uint8_t, uint8_t);

    virtual bool  checkHostMatched(void);
    virtual void  ISRHooking(void);
		virtual void	run(void);
    virtual void  start();
    virtual void  cmdService(void);
    virtual void	sendCmd(size_t, unsigned int);

};











#endif //_JA_LED_CONTROLLER_H
