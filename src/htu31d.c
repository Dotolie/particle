
#include <stdio.h>

#include "htu31d.h"
//#include "sensirion_arch_config.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"


#define HTU31D_CRC8_INIT           0x00
#define HTU31D_CRC8_POLYNOMIAL     0x31

static const uint8_t HTU31D_I2C_ADDRESS = 0x40;


float cal_humidity(uint16_t hum)
{
    return (float)(100.0f * hum / 65535);
}

float cal_temperature(uint16_t temp)
{
    return (float)(-40.0f + (float)(165.0f * temp / 65535));
}

uint8_t htu31d_crc8 (const uint8_t *data, uint8_t len)
{
	uint8_t crc = HTU31D_CRC8_INIT;
	uint8_t byteCtr;

	for (byteCtr=0; byteCtr<len; byteCtr++)
    {
		crc ^= (data[byteCtr]);

		for (uint8_t bit=8; bit>0; --bit)
        {
			if (crc & 0x80)
            {
				crc = (crc << 1) ^ HTU31D_CRC8_POLYNOMIAL;
			}
			else
            {
				crc = (crc << 1);
			}
		}
	}

	return crc;
}


int16_t htu31d_reset()
{
	int16_t ret = 0;
	uint8_t szBuf[1] = {HTU31D_RESET};

	ret = sensirion_i2c_write(HTU31D_I2C_ADDRESS, szBuf, 1);
	if (ret != NO_ERROR)
        return ret;

	return NO_ERROR;
}

int16_t htu31d_heaterOn()
{
	int16_t ret = 0;
	uint8_t szBuf[1] = {HTU31D_HEATER_ON};

	ret = sensirion_i2c_write(HTU31D_I2C_ADDRESS, szBuf, 1);
	if (ret != NO_ERROR)
        return ret;

	return NO_ERROR;
}

int16_t htu31d_heaterOff()
{
	int16_t ret = 0;
	uint8_t szBuf[1] = {HTU31D_HEATER_OFF};

	ret = sensirion_i2c_write(HTU31D_I2C_ADDRESS, szBuf, 1);
	if (ret != NO_ERROR)
        return ret;

	return NO_ERROR;
}

int16_t htu31d_readTnRH(float *pfTemp, float *pfHum)
{
	int16_t i = 0;
	int16_t ret = 0;
	uint16_t hum = 0;
	uint16_t temp = 0;
	float fHum = 0;
	float fTemp = 0;
	uint8_t szBuf[HTU31D_READ_TnRH_SIZE] = {HTU31D_READ_TnRH,};
	uint8_t szCrc[2];
	
	ret = sensirion_i2c_write(HTU31D_I2C_ADDRESS, szBuf, 1);
	if (ret != NO_ERROR)
		return ret;

	ret = sensirion_i2c_read(HTU31D_I2C_ADDRESS, szBuf, HTU31D_READ_TnRH_SIZE);
	if (ret != NO_ERROR)
		return ret;

	szCrc[0] = htu31d_crc8(&szBuf[0],2);
	szCrc[1] = htu31d_crc8(&szBuf[3],2);

	if( szCrc[0] != szBuf[2] || szCrc[1] != szBuf[5])
		return -1;
	
//	for(i=0;i<HTU31D_READ_TnRH_SIZE;i++)
//		printf("TnRH i[%d]=%x\r\n",i, szBuf[i]);

//	for(i=0;i<2;i++)
//		printf("crc[%d]=%x\r\n",i, szCrc[i]);

	temp = szBuf[0]*256 + szBuf[1];
	hum = szBuf[3]*256 + szBuf[4];

	fTemp = cal_temperature(temp);
	fHum = cal_humidity(hum);

	*pfTemp = fTemp;
	*pfHum = fHum;
	
	printf("t=%5.1f'C, rh=%5.1f%\r\n", fTemp, fHum);
	
	
	return NO_ERROR;
}

int16_t htu31d_readRH(uint8_t *pData)
{
	int16_t i = 0;
	int16_t ret = 0;
	uint16_t hum = 0;
	float fHum = 0;
	uint8_t crc;
	uint8_t szBuf[HTU31D_READ_RH_SIZE] = {HTU31D_READ_RH,};

	ret = sensirion_i2c_write(HTU31D_I2C_ADDRESS, szBuf, 1);
	if (ret != NO_ERROR)
		return ret;

	ret = sensirion_i2c_read(HTU31D_I2C_ADDRESS, szBuf, HTU31D_READ_RH_SIZE);
	if (ret != NO_ERROR)
		return ret;

	crc = htu31d_crc8(&szBuf[0],2);

	if( crc != szBuf[2])
		return -1;

	hum = szBuf[0]*256 + szBuf[1];
	fHum = cal_humidity(hum);

	for(i=0;i<HTU31D_READ_RH_SIZE;i++)
		printf("i[%d]=%x\r\n",i, szBuf[i]);

	printf("crc=0x%x\r\n", crc);

	printf("hum=%d, %5.1f\r\n", hum, fHum);
	
	return NO_ERROR;
}

int16_t htu31d_Convertion(uint8_t cOsrrh, uint8_t cOsrt)
{
	int16_t ret = 0;
	uint8_t szBuf[1];

	szBuf[0] = HTU31D_CONVERSION | ((cOsrrh << 3 ) & 0x18) | (cOsrrh & 0x06 );
	ret = sensirion_i2c_write(HTU31D_I2C_ADDRESS, szBuf, 1);
//	if (ret != NO_ERROR)
//       return ret;

	return NO_ERROR;

}

int16_t htu31d_readDiagnostic()
{
	int16_t i = 0;
	int16_t ret = 0;
	uint8_t szBuf[HTU31D_READ_DIAGNOSTIC_SIZE] = {HTU31D_READ_DIAGNOSTIC};

	ret = sensirion_i2c_write(HTU31D_I2C_ADDRESS, szBuf, 1);
//	if (ret != NO_ERROR)
//		return ret;

	ret = sensirion_i2c_read(HTU31D_I2C_ADDRESS, szBuf, HTU31D_READ_DIAGNOSTIC_SIZE);
//	if (ret != NO_ERROR)
//		return ret;

	printf("diag=0x%x\r\n", szBuf[0]);

	return NO_ERROR;
}

int16_t htu31d_readSerialNum(uint8_t *pData)
{
	int16_t i = 0;
	int16_t ret = 0;
	uint8_t szBuf[HTU31D_READ_SERIAL_NO_SIZE] = {HTU31D_READ_SERIAL_NO,};
	uint8_t szCrc[3];
	uint8_t crc = 0;
	
	ret = sensirion_i2c_write(HTU31D_I2C_ADDRESS, szBuf, 1);
	if (ret != NO_ERROR)
		return ret;

	ret = sensirion_i2c_read(HTU31D_I2C_ADDRESS, szBuf, HTU31D_READ_SERIAL_NO_SIZE);
	if (ret != NO_ERROR)
		return ret;
	
	crc = htu31d_crc8(&szBuf,3);

	if( szBuf[3] != crc ) 
		return -1;
	
	for(i=0;i<4;i++) {
		printf("i[%d]=%x, (%x)\r\n",i, szBuf[i], crc);
		pData[i]=szBuf[i+1];
		}
	
	return NO_ERROR;
}






