/*
 * sensor.c
 *
 *  Created on: Jun 19, 2025
 *      Author: Randy
 */

#include "project.h"
#include <stdint.h>

uint8_t crc8_sensirion(uint16_t data);

HAL_StatusTypeDef I2cSendCommand(uint8_t addr, uint8_t * cmd);
HAL_StatusTypeDef I2cReadData(uint8_t addr, uint8_t * rxbuf, int numbytes);

//int sensor_read_period_ms = 360000 - 1;

COUNTDOWN sensor_read_timer;


int SensorInit()
{
	int errors = 0;

	printf("SensorInit()\r\n");

	//SetCountDown(&sensor_read_timer, sensor_read_period_ms);
	SetCountDown(&sensor_read_timer, GetPeriodSec() * 1000 - 1);

	return errors;
}


void DoSensorProcess()
{
	int dtemp_int = 0;
	int dtemp_tenths = 0;
	int dtemp = 0;
	int dtempx10;
	//static int loopcnt = 0;
	char txbuf1[32];
	char txbuf2[32];
	static char tmpbuf[6];


	//return; //debug remove for periodic readings

	if (GetCountDown(&sensor_read_timer))
		return;

	// the countdown timer expired so lets get to work

	// restart the timer
	//SetCountDown(&sensor_read_timer, sensor_read_period_ms);
	SetCountDown(&sensor_read_timer, GetPeriodSec() * 1000 - 1);

	// Check if autoSend is false
	if(autoSend == false)
	{
		return;
	}

	// trigger the measurement
	// S - i2c addr - command MSB - command LSB - P
	tmpbuf[0] = HIBYTE(HDC302X_TRIG_LPM0);
	tmpbuf[1] = LOBYTE(HDC302X_TRIG_LPM0);
	HAL_I2C_Master_Transmit(&hi2c1, HDC302X_I2C_ADDR, (uint8_t *)&tmpbuf[0], 2, 100);		// sends 0x2c

	HAL_Delay(20); //Wait for measurement to complete

	// Read the temperature/humidity sensor
	uint8_t rxbuf[6];
	HAL_I2C_Master_Receive(&hi2c1, HDC302X_I2C_ADDR, (uint8_t *)&rxbuf, 6, HDC302X_READ_DATA_TIMEOUT_MS);
#ifdef LATER
	for(int i=0; i < 6; i++)
	{
		printf("%x ", rxbuf[i]);
	}
#endif
	printf("\r\n");
	//construct a 16 bit value representing temperature
	dtemp = (rxbuf[0] << 8) + rxbuf[1];
	//printf("dtemp = %d\r\n", dtemp);
	// we want to calculate the temperature to the nearest tenth of a degree C
	dtempx10 = -450 + (1750 * dtemp) / 65535;			// integer math!
	dtemp_int = dtempx10 / 10;							// integer math!
	dtemp_tenths = dtempx10 - (10 * dtemp_int);			// integer math!

	// construct message to send to the gateway
	//sprintf(txbuf1, "TEMP=%d.%dC", dtemp_int, dtemp_tenths);
	sprintf(txbuf1, "$TEMP:%d.%dC", dtemp_int, dtemp_tenths);

	sprintf(txbuf2, "AT+SEND=1,%d,", strlen(txbuf1));
	strcat(txbuf2, txbuf1);

	// send the message to the gateway
	printf("%s\r\n", txbuf2);
	SendRadioMsg(txbuf2);


}

void ResetSensorTimer()
{

	// restart the timer
	//SetCountDown(&sensor_read_timer, sensor_read_period_ms);
	SetCountDown(&sensor_read_timer, GetPeriodSec() * 1000 - 1);

}























#ifdef LATER
	char txbuf1[32];
	char txbuf2[32];
	int dtemp = 0;
	int dtempx10;
	int dtemp_int = 0;
	int dtemp_tenths = 0;


	static char tmpbuf[6];

	if (0 == GetCountDown(&sensor_read_timer))
	{
		printf("Trig %d\r\n", (int)HAL_GetTick());

		// restart the timer
		SetCountDown(&sensor_read_timer, sensor_read_period_ms);


		// debug stuff - read the status register
		tmpbuf[0] = HIBYTE(SHT_CLR_STATUS_REGISTER);
		tmpbuf[1] = LOBYTE(SHT_CLR_STATUS_REGISTER);
		HAL_I2C_Master_Transmit(&hi2c1, SHT_I2C_ADDR, (uint8_t *)&tmpbuf[0], 2, 100);
		uint8_t rxbuf[6];
		HAL_I2C_Master_Receive(&hi2c1, SHT_I2C_ADDR, (uint8_t *)&rxbuf, 3, SHT_READ_DATA_TIMEOUT_MS);

		HAL_Delay(100);

		// debug stuff - read the status register
		tmpbuf[0] = HIBYTE(SHT_GET_STATUS_REGISTER);
		tmpbuf[1] = LOBYTE(SHT_GET_STATUS_REGISTER);
		HAL_I2C_Master_Transmit(&hi2c1, SHT_I2C_ADDR, (uint8_t *)&tmpbuf[0], 2, 100);
		uint8_t rxbuf[6];
		HAL_I2C_Master_Receive(&hi2c1, SHT_I2C_ADDR, (uint8_t *)&rxbuf, 3, SHT_READ_DATA_TIMEOUT_MS);

		printf("SHT31 status: 0x%02x 0x%02x 0x%02x\r\n", rxbuf[0], rxbuf[1], rxbuf[2]);

		// trigger the measurement
		// S - i2c addr - command MSB - command LSB - P
		tmpbuf[0] = HIBYTE(HDC302X_TRIG_LPM0);
		tmpbuf[1] = LOBYTE(HDC302X_TRIG_LPM0);

		HAL_I2C_Master_Transmit(&hi2c1, SHT_I2C_ADDR, (uint8_t *)&tmpbuf[0], 2, 100);		// sends 0x2c


		// we trigger the measurement with clock stretching so should be able to read
		// the values as soon as the measurement is completed

		HAL_Delay(5);

		// Read the SHT3x temperature/humidity sensor
		uint8_t rxbuf[6];
		HAL_I2C_Master_Receive(&hi2c1, SHT_I2C_ADDR, (uint8_t *)&rxbuf, 6, SHT_READ_DATA_TIMEOUT_MS);

		dtemp = rxbuf[0] << 8 + rxbuf[1];

		// we want to calculate the temperature to the nearest tenth of a degree C
		dtempx10 = -450 + (1750 * dtemp) / 65535;			// integer math!

		dtemp_int = dtempx10 / 10;							// integer math!
		dtemp_tenths = dtempx10 - (10 * dtemp_int);			// integer math!

		// construct message to send to the gateway
		sprintf(txbuf1, "TEMP=%d.%dC", dtemp_int, dtemp_tenths);
		sprintf(txbuf2, "AT+SEND=1,%d,%s", strlen(txbuf1),txbuf1);

		// send the message to the gateway
		int SendRadioMsg(txbuf2);

	}
#endif


#ifdef OBSOLETE
HAL_StatusTypeDef DoSensorWriteCommand(char * buf)
{
	unsigned cmd;
	HAL_StatusTypeDef status;
	char cmdbuf[2];

	// extract the command WORD from the input buffer
	cmd = atox(buf);

//	printf("Sending to sensor command: 0x%04x\r\n", cmd);

	cmdbuf[0] = HIBYTE(cmd);
	cmdbuf[1] = LOBYTE(cmd);
	status = I2cSendCommand(SHT31_I2C_ADDR, (uint8_t *)cmdbuf); 		// does a retry if necessary
	if (status != HAL_OK)
		printf("I2cSendCommand() failed\r\n");

	return status;
//	HAL_I2C_Master_Transmit(&hi2c1, SHT_I2C_ADDR, (uint8_t *)&tmpbuf[0], 2, 100);

}

HAL_StatusTypeDef DoSensorReadCommand(char * buf)
{
	HAL_StatusTypeDef status;
	unsigned cmd;
	char cmdbuf[2];


	// arg1 is command
	cmd = atox(buf);

	cmdbuf[0] = HIBYTE(cmd);
	cmdbuf[1] = LOBYTE(cmd);


	printf("DoSensorReadCommand: 0x%02x 0x%02x\r\n", cmdbuf[0], cmdbuf[1]);

	status = I2cSendCommand(SHT31_I2C_ADDR, (uint8_t *)cmdbuf); 		// does retries if necessary
	if (status != HAL_OK) {
		printf("DoSensorReadCommand: I2CSendCommand failed. Aborting DoSensorReadCommand()\r\n");
		return status;
	}

	HAL_Delay(15);

	uint8_t rxbuf[6];
	memset(rxbuf, 0xcd, 6);			// set to invalid
//	status = HAL_I2C_Master_Receive(&hi2c1, SHT31_I2C_ADDR << 1, rxbuf, 6, 100);
	status =  I2cReadData(SHT31_I2C_ADDR, rxbuf, 6);
	if (status != HAL_OK) {
		printf("DoSensorReadCommand: I2cReadData failed\r\n");
		return status;
	}

	printf("read2: 0x%02x 0x%02x 0x%02x\r\n", rxbuf[0], rxbuf[1], rxbuf[2]);
	return status;

}


HAL_StatusTypeDef I2cSendCommand(uint8_t addr, uint8_t * cmd)
{
	int i;
	const int retries = 5;
	HAL_StatusTypeDef status;
	for (i = 0; i < retries; i++)
	{
		status = HAL_I2C_Master_Transmit(&hi2c1, addr << 1, cmd, 2, 100);

		if (status == HAL_OK)
			break;

		HAL_Delay(1); // brief delay before retry
	}

	if (status != HAL_OK)
	{
		printf("I2cSendCommand failed after %d attempts\r\n", i);
	}

	else if (i)
	{
		printf("I2cSendCommand passed on attempt %d\r\n", i+1);
	}

	return (int)status;
}

HAL_StatusTypeDef I2cReadData(uint8_t addr, uint8_t * rxbuf, int numbytes)
{
	int i;
	const int retries = 10;
	HAL_StatusTypeDef status;
	status = HAL_I2C_Master_Receive(&hi2c1, addr << 1, (uint8_t *)&rxbuf[0], numbytes, SHT_READ_DATA_TIMEOUT_MS);
	for (i = 0; i < retries; i++)
	{
		status = HAL_I2C_Master_Receive(&hi2c1, addr << 1, (uint8_t *)&rxbuf[0], numbytes, SHT_READ_DATA_TIMEOUT_MS);

		if (status == HAL_OK)
			break;

		HAL_Delay(1); // brief delay before retry
	}

	if (status != HAL_OK)
	{
		printf("I2cReadData failed after %d attempts\r\n", i);
		return status;
	}
	else if (i)
	{
		printf("I2cReadData passed on attempt %d\r\n", i+1);
	}

	// check the CRC

	uint8_t crc = crc8_sensirion((rxbuf[0] << 8) + rxbuf[1]);
	if (crc != rxbuf[2])
	{
		printf("CRC failed: expect: 0x%02x, actual: 0x%02x\r\n", crc, rxbuf[2]);
	}



	return (int)status;
}


uint8_t crc8_sensirion(uint16_t data)
{
    uint8_t crc = 0xFF; // Initialization
    uint8_t bytes[2];

    // Split 16-bit data into two bytes (MSB first)
    bytes[0] = (data >> 8) & 0xFF;
    bytes[1] = data & 0xFF;

    for (int i = 0; i < 2; i++) {
        crc ^= bytes[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80)
                crc = (crc << 1) ^ 0x31;
            else
                crc <<= 1;
        }
    }

    return crc; // No final XOR
}

HAL_StatusTypeDef sht31_read_measurement(I2C_HandleTypeDef *hi2c, float *temperature, float *humidity) {
    uint8_t cmd[2] = {0x24, 0x16};  // low repeatability, no clock stretching
    uint8_t buf[6];

    // Send measurement command
    if (HAL_I2C_Master_Transmit(hi2c, SHT31_ADDR << 1, cmd, 2, 100) != HAL_OK)
        return HAL_ERROR;

    HAL_Delay(5); // Wait for measurement to complete (low rep. = ~4 ms)

    // Read 6 bytes of data
    if (HAL_I2C_Master_Receive(hi2c, SHT31_ADDR << 1, buf, 6, 100) != HAL_OK)
        return HAL_ERROR;

    // Validate CRCs
    if (sht31_crc8(buf, 2) != buf[2] || sht31_crc8(buf + 3, 2) != buf[5])
        return HAL_ERROR;

    // Convert raw values to float
    uint16_t raw_temp = (buf[0] << 8) | buf[1];
    uint16_t raw_rh   = (buf[3] << 8) | buf[4];

    *temperature = -45.0f + 175.0f * (float)raw_temp / 65535.0f;
    *humidity    = 100.0f * (float)raw_rh / 65535.0f;

    return HAL_OK;
}
#endif
