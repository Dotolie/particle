
#include <stdio.h>

#include "pm5000.h"
//#include "sensirion_arch_config.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"



static const uint8_t PM5000_I2C_ADDRESS = 0x28;

int16_t pm5000_check_crc(uint8_t* pBuf, int16_t nSize)
{
    int16_t i = 0;
	uint8_t crc = 0;
	
    /* check the CRC for each word */
    for (i = 0; i < (nSize-1); i++) {
        crc += pBuf[i];
	//	printf("crc: %d, %x, %x\r\n",i, pBuf[i], crc);
    	}

	//printf("-----calc=%x, crc=%x\r\n", (uint8_t)-crc, pBuf[nSize-1]);
	if ((uint8_t)-crc != pBuf[nSize-1])
    	return STATUS_FAIL;

	return NO_ERROR;

}

int16_t pm5000_i2c_write_cmd(uint8_t address, uint8_t *pCmd, uint8_t len)
{
    return sensirion_i2c_write(address, pCmd, len);
}

int16_t pm5000_read(uint8_t *pData)
{
	int16_t i = 0;
	int16_t ret = 0;
	uint8_t szBuf[32];
	uint32_t nP03, nP05, nP10, nP25, nP50,nP100;
	
	ret = sensirion_i2c_read(PM5000_I2C_ADDRESS, szBuf, 32);
	if (ret != NO_ERROR)
		return ret;

//	sensirion_sleep_usec(1000);


//	for (i = 0; i < 32; i++) {
//		printf("i[%d]=%x\r\n", i, szBuf[i]);
//		}
	nP03 = szBuf[13]*256*256 + szBuf[14]*256 + szBuf[15];
	nP05 = szBuf[16]*256*256 + szBuf[17]*256 + szBuf[18];
	nP10 = szBuf[19]*256*256 + szBuf[20]*256 + szBuf[21];
	nP25 = szBuf[22]*256*256 + szBuf[23]*256 + szBuf[24];
	nP50 = szBuf[25]*256*256 + szBuf[26]*256 + szBuf[27];
	nP100 = szBuf[28]*256*256 + szBuf[29]*256 + szBuf[30];
	
	sprintf(pData, "len=%d, sts=%x, p0.3=%d, p0.5=%d, p1.0=%d, p2.5=%d, p5.0-%d, p10.0=%d",
		szBuf[1], szBuf[2], nP03, nP05, nP10, nP25, nP50, nP100);
	return NO_ERROR;


}


int16_t pm5000_send(uint8_t cIsOpen)
{
	int16_t i = 0;
    int16_t ret = 0;
	uint8_t szBuf[PM5000_SEND_SIZE];

	szBuf[0] = 0x16;
	szBuf[1] = 7;
	szBuf[2] = cIsOpen;
	szBuf[3] = 0;
	szBuf[4] = 0;
	szBuf[5] = 0;
	szBuf[6] = szBuf[0]^szBuf[1]^szBuf[2]^szBuf[3]^szBuf[4]^szBuf[5];

	ret = pm5000_i2c_write_cmd(PM5000_I2C_ADDRESS, szBuf, 7);
	if (ret != NO_ERROR)
        return ret;
	
	return NO_ERROR;
}



