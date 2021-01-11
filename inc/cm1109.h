
#ifndef CM1109_H
#define CM1109_H
//#include "sensirion_arch_config.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"

#define CM1109_ERR_UNSUPPORTED_FEATURE_SET 	(-10)
#define CM1109_ERR_INVALID_PRODUCT_TYPE 	(-12)
#define CM1109_ERR_INVALID_CRC				(-13)

#ifdef __cplusplus
extern "C" {
#endif


#define	CM1109_COMMAND_SIZE											0x01

#define CM1109_MEASURE_RESULT                                       0x01
#define CM1109_MEASURE_RESULT_SIZE									5


#define CM1109_AUTO_ZERO_SETTING_SPECIFICATION_SETTING              0x10
#define CM1109_AUTO_ZERO_SETTING_SPECIFICATION_SETTING_SIZE         8

#define CM1109_CALIBRATION                                          0x03
#define CM1109_CALIBRATION_SIZE										4

#define CM1109_READ_THE_SERIAL_NUMBER_OF_SENSOR                     0x1F
#define CM1109_READ_THE_SERIAL_NUMBER_OF_SENSOR_SIZE				12

#define CM1109_CHECK_SOFTWARE_VERSION                               0x1E
#define CM1109_CHECK_SOFTWARE_VERSION_SIZE							12




int16_t cm1109_check_crc(uint8_t* pBuf, int16_t nSize);
int16_t cm1109_i2c_write_cmd(uint8_t address, uint8_t *pCmd, uint8_t len);

int16_t cm1109_read_serial_number(uint16_t *pSerial);
int16_t cm1109_get_software_version(uint8_t *pString);
int16_t cm1109_auto_zero_setting(uint8_t *pString);
int16_t cm1109_calibration(uint8_t *pString);
int16_t cm1109_measuring_result(uint16_t *pCo2, uint8_t *pStatus);


#ifdef __cplusplus
}
#endif

#endif /* CM1109_H */
