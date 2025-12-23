/*
 * eeprom.c
 *
 *  Created on: Jul 20, 2025
 *      Author: johnn
 */

#include "project.h"

EESTRUCT_REV1 eestruct;

int InitEEPROM()
{
	uint32_t testaddr = 0x1000;
	uint8_t txBuf[8] = {0xaa, 0x55, 0xdd, 0xbb, 0xab, 0x12, 0x34, 0x56};
	uint8_t rxBuf[8];
	memset(rxBuf, 0, sizeof(rxBuf));
	int numBytes = 8;

	// Test EEPROM
	WriteEEPROM(testaddr, txBuf, numBytes);
	ReadEEPROM(testaddr, rxBuf, numBytes);

	for (int i = 0; i < numBytes; i++)
	{
		if (rxBuf[i] != txBuf[i])
		{
			printf("InitEEPROM: EEPROM data mismatch, addr: %d, expect: 0x%02x, actual: 0x%02x\r\n", i, txBuf[i], rxBuf[i]);
		}
	}

	// Read EEPROM content into data struct
	int status = ReadEEPROM(0, (uint8_t *)&eestruct.magicNum, sizeof(eestruct));
	if (status != 0)
	{
		printf("InitEEPROM: ReadEEPROM failed\r\n");
		return status;
	}

	// Check for blank/corrupt EEPROM image
	if ( (eestruct.magicNum != EE_MAGIC_NUM) || (eestruct.rev != EESTRUCT_CUR_REV) )
	{
		printf("InitEEPROM: Blank or invalid EEPROM image found, initializing EEPROM image to rev %d\r\n", EESTRUCT_CUR_REV);
		eestruct.magicNum = EE_MAGIC_NUM;
		eestruct.rev = EESTRUCT_CUR_REV;
		eestruct.period = DEFAULT_SENSOR_READ_PERIOD_SEC;

		// End of revision 1 data structure

		// Initialize the contents of the EEPROM
		status = WriteEEPROM(0, (uint8_t *)&eestruct.magicNum, sizeof(eestruct));
		if (status != 0)
		{
			printf("InitEEPROM: WriteEEPROM failed\r\n");
			return status;
		}
	}
	else
	{
		printf("InitEEPROM: Found valid image at rev %d\r\n", EESTRUCT_CUR_REV);
	}
	printf("eestruct.period: %d\r\n", GetPeriodSec());

	return 0;
}

int WriteEEPROM(uint32_t addr, uint8_t * pBuf, int numBytes )
{
	uint8_t tmpbuf[3];

	//printf("WriteEEPROM: addr: 0x%x, numBytes: %d\r\n", (unsigned int)addr, numBytes);

	// Write Bytes one at a time
	for (int i = 0; i < numBytes; i++)
	{
		tmpbuf[0] = HIBYTE(addr + i);
		tmpbuf[1] = LOBYTE(addr + i);
		tmpbuf[2] = *(pBuf + i);
		HAL_I2C_Master_Transmit(&hi2c1, EE24FC256_I2C_ADDR, (uint8_t *)&tmpbuf[0], 3, 100);

		HAL_Delay(6); // Wait for write to complete
	}

	return 0;
}

int ReadEEPROM(uint32_t addr, uint8_t * pBuf, int numBytes)
{

	//printf("ReadEEPROM: addr: 0x%x, numBytes: %d\r\n", (unsigned int)addr, numBytes);

	uint8_t tmpbuf[3];

	tmpbuf[0] = HIBYTE(addr);
	tmpbuf[1] = LOBYTE(addr);
	HAL_I2C_Master_Transmit(&hi2c1, EE24FC256_I2C_ADDR, &tmpbuf[0], 2, 100);

	// Read Bytes one at a time
	for (int i = 0; i < numBytes; i++)
	{
		//uint8_t rxbuf[6];
		HAL_I2C_Master_Receive(&hi2c1, EE24FC256_I2C_ADDR, pBuf + i, 1, 100);
	}

	return 0;
}

int GetPeriodSec()
{
	return eestruct.period;
}

int SetPeriodSec(uint32_t period)
{
	eestruct.period = period;
	int status = WriteEEPROM((uint32_t)&eestruct.period - (uint32_t)&eestruct.magicNum, (uint8_t *)&eestruct.period, sizeof(uint32_t));
	if (status != 0)
	{
		printf("InitEEPROM: WriteEEPROM failed\r\n");
		return status;
	}

	return 0;
}



