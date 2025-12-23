/*
 * sensor.h
 *
 *  Created on: Jun 19, 2025
 *      Author: Randy
 */

#ifndef INC_SENSOR_H_
#define INC_SENSOR_H_

#ifdef SHT31
#define SHT31_I2C_ADDR 0x44
// CS - clock stretching enabled
#define SHT_MEAS_CMD_CS_HI_RPT		0x2c06
#define SHT_MEAS_CMD_CS_MED_RPT		0x2c0d
#define SHT_MEAS_CMD_CS_LO_RPT		0x2c10
#define SHT_MEAS_CMD_NCS_HI_RPT		0x2400
#define SHT_MEAS_CMD_NCS_MED_RPT	0x240b
#define SHT_MEAS_CMD_NCS_LO_RPT		0x2416

#define SHT_GET_STATUS_REGISTER		0xf32d
#define SHT_CLR_STATUS_REGISTER		0x3041
#define SHT_CMD_SOFT_RESET			0x30A2

#define SHT_READ_DATA_TIMEOUT_MS	20
#endif
#define HDC302X_I2C_ADDR (0x44 << 1)
#define HDC302X_TRIG_LPM0 0x2400
#define HDC302X_READ_DATA_TIMEOUT_MS	20
#define DEFAULT_SENSOR_READ_PERIOD_SEC 360

//continue here

void DoSensorProcess();
int SensorInit();
void ResetSensorTimer();
HAL_StatusTypeDef DoSensorWriteCommand(char * buf);
HAL_StatusTypeDef DoSensorReadCommand(char * buf);
//HAL_StatusTypeDef GetSensorValues(int timeout_ms);


#endif /* INC_SENSOR_H_ */
