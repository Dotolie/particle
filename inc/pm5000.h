
#ifndef PM5000_H
#define PM5000_H

#include "sensirion_common.h"
#include "sensirion_i2c.h"



#ifdef __cplusplus
extern "C" {
#endif



#define PM5000_SEND 		        	                            0x01
#define PM5000_SEND_SIZE											5

#define PM5000_READ													0x10
#define PM5000_READ_SIZE     									    8


typedef struct _PM5000 {
	int nP03;
	int nP05;
	int nP10;
	int nP25;
	int nP50;
	int nP100;
} tPM5000;


int16_t pm5000_check_crc(uint8_t* pBuf, int16_t nSize);
int16_t pm5000_i2c_write_cmd(uint8_t address, uint8_t *pCmd, uint8_t len);

int16_t pm5000_read(int *, int *, int *, int *, int *, int *);
int16_t pm5000_send(uint8_t cIsOpen);

#ifdef __cplusplus
}
#endif

#endif /* PM5000_H */
