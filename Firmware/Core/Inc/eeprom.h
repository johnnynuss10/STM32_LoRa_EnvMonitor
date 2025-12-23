/*
 * eeprom.h
 *
 *  Created on: Jul 20, 2025
 *      Author: johnn
 */

#ifndef INC_EEPROM_H_
#define INC_EEPROM_H_

#define EE24FC256_I2C_ADDR (0x50 << 1)
#define HDC302X_I2C_ADDR (0x44 << 1)

#define EE_MAGIC_NUM 0x0304


int InitEEPROM();
int WriteEEPROM(uint32_t addr, unsigned char * pBuf, int numBytes );
int ReadEEPROM(uint32_t addr, unsigned char * pBuf, int numBytes);
int GetPeriodSec();
int SetPeriodSec(uint32_t period);

//EESTRUCT revision 1 holds the measurement period
typedef struct eestruct_tag
{
	uint16_t magicNum;
	uint16_t rev;
	uint32_t period;

} EESTRUCT_REV1;
#define EESTRUCT_CUR_REV 1




#endif /* INC_EEPROM_H_ */
