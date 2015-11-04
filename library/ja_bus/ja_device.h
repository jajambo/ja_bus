

#ifndef _JA_DEVICE_H
#define _JA_DEVICE_H

#include <inttypes.h>
#include <HardwareSerial.h>
#include <ja_bus_protocol.h>


#define byte_ascii(c,b) \
  b = (c/16 <=9)? c/16+0x30:c/16+0x40-9; \
  Serial.write(b);\
  b = (c%16 <=9)? c%16+0x30:c%16+0x40-9; \
  Serial.write(b)

#define JASERIAL_PORT Serial1
#define BAUD_RATE 9600
#define BUFFER_SIZE 32

#define JA_CMD_SERVICE_INTERVAL   2000

#define DEV_WAIT_FOR_SYNC	0x00
#define DEV_TRY_REQUEST_ID	0x01
#define DEV_READY		0x02
#define DEV_TRY_REQUEST		0x03
#define DEV_TRY_PROCESS		0x04
#define DEV_DEAD		0x05

#define DEV_RESPOND_TIMEOUT		0x10	//10s
#define DEV_RANK		0x03
#define DEV_DEFAULT_RANK	DEV_RANK
#define DEV_REQUEST_ID_RANK	0x0
#define DEV_RANK_INTERVAL	RELEASE_BUS_TIME	//1s
#define DEV_RANK_MAX_RANDOM	0x100	// DEVICE_RANK_INTERVAL/256

//device status bit field
#define	DEV_HOST_MATCHED		0x01
#define DEV_INTERRUPT_OCCUR		0x02
#define DEV_RECEIVE_RELEASE_BUS		0x04
#define DEV_RECEIVE_CMD			0x08
#define DEV_REQUEST_REPORT		0x10

//report
#define DEV_HOST_NOT_MATCHED		0x00



#define DEV_START_FLAG		0x55AA



#define DEV_SN_EMPTY	0x01
#define HOST_SN_EMPTY	0x02
#define DEV_ID_EMPTY	0x03

//reportId
#define DEV_REPORT_HOST_NOT_MATCHED 0x01
#define DEV_REPORT_FORCE_STOP       0x02
#define DEV_REPORT_FUNCTION_CRASH   0x03
 
#define DEV_DEFAULT_TIMEOUT 1000





class JADevice
{
  protected:
    ja_device_command_t rx_cmd_buf;
    ja_device_command_t tx_cmd_buf;
    ja_dev_eeprom_t dev_eeprom;
		ja_slave_info_t slave_info;
    char device_rank = DEV_DEFAULT_RANK;
    unsigned char	device_state = DEV_WAIT_FOR_SYNC;
    unsigned char	host_sn[16];
    unsigned char temp_buf[BUFFER_SIZE];
    unsigned char	device_status = 0;
    unsigned char	reportId;
    unsigned char	reportData[4];
    HardwareSerial *dev_serial;
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
    virtual void  processClassCmd(void);
    void  processJADevCmd(void);
    


  public:
    JADevice(HardwareSerial *serial);
    JADevice();
    virtual bool  checkHostMatched(void);
    virtual void  ISRHooking(void);
		virtual void	run(void);
    virtual void  start();
    virtual void  cmdService(void);
    void	sendCmd(size_t, unsigned int);

};


#endif //_JA_DEVICE_H
