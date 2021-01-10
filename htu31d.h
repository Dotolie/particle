
#ifndef HTU31D_H
#define HTU31D_H
//#include "sensirion_arch_config.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"



#ifdef __cplusplus
extern "C" {
#endif



#define HTU31D_RESET	0x1e
#define HTU31D_HEATER_ON											0x04
#define HTU31D_HEATER_OFF											0x02
#define HTU31D_READ_TnRH											0x00
#define HTU31D_READ_TnRH_SIZE										6
#define HTU31D_READ_RH												0x10
#define HTU31D_READ_RH_SIZE											4
#define HTU31D_CONVERSION     									    0x40
#define HTU31D_READ_DIAGNOSTIC										0x08
#define HTU31D_READ_DIAGNOSTIC_SIZE									1
#define HTU31D_READ_SERIAL_NO										0x0a
#define HTU31D_READ_SERIAL_NO_SIZE									6




int16_t htu31d_check_crc(uint8_t* pBuf, int16_t nSize);
uint8_t htu31d_crc8 (const uint8_t *data, uint8_t len);

int16_t htu31d_reset();
int16_t htu31d_heaterOn();
int16_t htu31d_heaterOff();
int16_t htu31d_readTnRH(uint8_t *pData);
int16_t htu31d_Convertion(uint8_t cOsrrh, uint8_t cOsrt);
int16_t htu31d_readDiagnostic();
int16_t htu31d_readSerialNum();
int16_t htu31d_send(uint8_t cIsOpen);

#ifdef __cplusplus
}
#endif

#endif /* HTU31D_H */
