
#include <stdio.h>

#include "cm1109.h"
//#include "sensirion_arch_config.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"



static const uint8_t CM1109_I2C_ADDRESS = 0x31;

int16_t cm1109_check_crc(uint8_t* pBuf, int16_t nSize)
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

int16_t cm1109_i2c_write_cmd(uint8_t address, uint8_t *pCmd, uint8_t len)
{
    return sensirion_i2c_write(address, pCmd, len);
}

int16_t cm1109_read_serial_number(uint16_t *pSerial)
{
	int16_t i = 0;
	int16_t ret = 0;
	uint8_t Cmd[1];
	uint8_t szBuf[CM1109_READ_THE_SERIAL_NUMBER_OF_SENSOR_SIZE+4];

	Cmd[0] = CM1109_READ_THE_SERIAL_NUMBER_OF_SENSOR;
	ret = cm1109_i2c_write_cmd(CM1109_I2C_ADDRESS, Cmd, CM1109_COMMAND_SIZE);
	ret = sensirion_i2c_read(CM1109_I2C_ADDRESS, szBuf, CM1109_READ_THE_SERIAL_NUMBER_OF_SENSOR_SIZE);

	sensirion_sleep_usec(1000000);
	
	ret = cm1109_i2c_write_cmd(CM1109_I2C_ADDRESS, Cmd, CM1109_COMMAND_SIZE);
	if (ret != NO_ERROR)
		return ret;

//	sensirion_sleep_usec(1000);

	ret = sensirion_i2c_read(CM1109_I2C_ADDRESS, szBuf, CM1109_READ_THE_SERIAL_NUMBER_OF_SENSOR_SIZE);
	if (ret != NO_ERROR)
		return ret;


	ret = cm1109_check_crc(szBuf, CM1109_READ_THE_SERIAL_NUMBER_OF_SENSOR_SIZE);
	if (ret != NO_ERROR)
		return ret;

	for (i = 1; i < CM1109_READ_THE_SERIAL_NUMBER_OF_SENSOR_SIZE-1; i+=2) {
		*pSerial++ = (szBuf[i]<<8) + (szBuf[i+1]);
//		printf("serial : %d, %x, %x, %x\r\n", i, szBuf[i], szBuf[i+1], (szBuf[i]<<8) + (szBuf[i+1]));
		}
	
	return NO_ERROR;


}


int16_t cm1109_get_software_version(uint8_t *pString)
{
	int16_t i = 0;
    int16_t ret = 0;
	uint8_t *pBuff = pString;
	uint8_t Cmd[1];
	uint8_t szBuf[CM1109_CHECK_SOFTWARE_VERSION_SIZE];

	Cmd[0] = CM1109_CHECK_SOFTWARE_VERSION;
	ret = cm1109_i2c_write_cmd(CM1109_I2C_ADDRESS, Cmd, CM1109_COMMAND_SIZE);
	ret = sensirion_i2c_read(CM1109_I2C_ADDRESS, szBuf, CM1109_CHECK_SOFTWARE_VERSION_SIZE);

	sensirion_sleep_usec(1000000);

	ret = cm1109_i2c_write_cmd(CM1109_I2C_ADDRESS, Cmd, CM1109_COMMAND_SIZE);
	if (ret != NO_ERROR)
        return ret;

//    sensirion_sleep_usec(1000);

	ret = sensirion_i2c_read(CM1109_I2C_ADDRESS, szBuf, CM1109_CHECK_SOFTWARE_VERSION_SIZE);
	if (ret != NO_ERROR)
		return ret;


	ret = cm1109_check_crc(szBuf, CM1109_CHECK_SOFTWARE_VERSION_SIZE);
	if (ret != NO_ERROR)
        return ret;

	for (i = 1; i < CM1109_CHECK_SOFTWARE_VERSION_SIZE-1; i++) {
        pBuff[i-1] = szBuf[i];
//		printf("swver: i=%d, %c, 0x%x\r\n", i, szBuf[i], szBuf[i]);
		}
	pBuff[i-1] = 0;
	
	return NO_ERROR;
}


int16_t cm1109_auto_zero_setting(uint8_t *pString)
{
	int16_t i = 0;
	int16_t ret = 0;
	uint8_t *pBuff = pString;
	uint8_t Cmd[CM1109_AUTO_ZERO_SETTING_SPECIFICATION_SETTING_SIZE];
	uint8_t szBuf[CM1109_AUTO_ZERO_SETTING_SPECIFICATION_SETTING_SIZE];

	/*
		0 : reserved (default : 0x64)
		1 : open/close auto calibration (0 : Open, 2 : Close)
		2 : calibration cycle (1~30 days optional, default : 7)
		3 : calibration value (high byte)
		4 : calibration value (low byte)
		5 : reserved (default : 0x64)
	*/
	Cmd[0] = CM1109_AUTO_ZERO_SETTING_SPECIFICATION_SETTING;
	Cmd[1] = 0x64;
	Cmd[2] = 0;
	Cmd[3] = 7;
	Cmd[4] = 0x01;
	Cmd[5] = 0x90;
	Cmd[6] = 0x64;
	
	ret = cm1109_i2c_write_cmd(CM1109_I2C_ADDRESS, Cmd, 7 );
	ret = sensirion_i2c_read(CM1109_I2C_ADDRESS, szBuf, CM1109_AUTO_ZERO_SETTING_SPECIFICATION_SETTING_SIZE);

	sensirion_sleep_usec(1000000);

	ret = cm1109_i2c_write_cmd(CM1109_I2C_ADDRESS, Cmd, 7 );
	if (ret != NO_ERROR)
		return ret;

//	  sensirion_sleep_usec(1000);

	ret = sensirion_i2c_read(CM1109_I2C_ADDRESS, szBuf, CM1109_AUTO_ZERO_SETTING_SPECIFICATION_SETTING_SIZE);
	if (ret != NO_ERROR)
		return ret;


	ret = cm1109_check_crc(szBuf, CM1109_AUTO_ZERO_SETTING_SPECIFICATION_SETTING_SIZE);
	if (ret != NO_ERROR)
		return ret;

	for (i = 1; i < CM1109_AUTO_ZERO_SETTING_SPECIFICATION_SETTING_SIZE-1; i++) {
		*pBuff++ = szBuf[i];
		printf("auto zero: i[%d]= 0x%x\r\n", i, szBuf[i]);
		}
	
	return NO_ERROR;
}


int16_t cm1109_calibration(uint8_t *pString)
{
	int16_t i = 0;
	int16_t ret = 0;
	uint8_t *pBuff = pString;
	uint8_t Cmd[CM1109_CALIBRATION_SIZE];
	uint8_t szBuf[CM1109_CALIBRATION_SIZE];

	Cmd[0] = CM1109_CALIBRATION;
	// Adjust value : 400~1,500
	Cmd[1] = 0x01;
	Cmd[2] = 0x90;

	ret = cm1109_i2c_write_cmd(CM1109_I2C_ADDRESS, Cmd, 3 );
	ret = sensirion_i2c_read(CM1109_I2C_ADDRESS, szBuf, CM1109_CALIBRATION_SIZE);

	sensirion_sleep_usec(1000000);
	
	ret = cm1109_i2c_write_cmd(CM1109_I2C_ADDRESS, Cmd, 3 );
	if (ret != NO_ERROR)
		return ret;

//	  sensirion_sleep_usec(1000);

	ret = sensirion_i2c_read(CM1109_I2C_ADDRESS, szBuf, CM1109_CALIBRATION_SIZE);
	if (ret != NO_ERROR)
		return ret;

	ret = cm1109_check_crc(szBuf, CM1109_CALIBRATION_SIZE);
	if (ret != NO_ERROR)
		return ret;

	for (i = 1; i < CM1109_CALIBRATION_SIZE-1; i++) {
		*pBuff++ = szBuf[i];
		printf("calibration: i[%d]= 0x%x\r\n", i, szBuf[i]);
		}
	
	return NO_ERROR;
}

int16_t cm1109_measuring_result(uint16_t *pCo2, uint8_t *pStatus)
{
		int16_t i = 0;
		int16_t ret = 0;
		uint8_t Cmd[CM1109_MEASURE_RESULT_SIZE];
		uint8_t szBuf[CM1109_MEASURE_RESULT_SIZE];
	
		Cmd[0] = CM1109_MEASURE_RESULT;

			
		ret = cm1109_i2c_write_cmd(CM1109_I2C_ADDRESS, Cmd, 1 );
		if (ret != NO_ERROR)
			return ret;
	
	//	  sensirion_sleep_usec(1000);
	
		ret = sensirion_i2c_read(CM1109_I2C_ADDRESS, szBuf, CM1109_MEASURE_RESULT_SIZE);
		if (ret != NO_ERROR)
			return ret;
	
	
		ret = cm1109_check_crc(szBuf, CM1109_MEASURE_RESULT_SIZE);
		if (ret != NO_ERROR)
			return ret;
	
//		for (i = 1; i < CM1109_MEASURE_RESULT_SIZE-1; i++) {
//			printf("calibration: i[%d]= 0x%x\r\n", i, szBuf[i]);
//			}

		*pCo2 = (szBuf[1]*256 + szBuf[2]);
		*pStatus = szBuf[3];
		
		return NO_ERROR;

}

