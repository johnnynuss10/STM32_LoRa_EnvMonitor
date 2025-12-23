/*
 * project.h
 *
 *  Created on: Jun 18, 2025
 *      Author: Randy
 */

#ifndef INC_PROJECT_H_
#define INC_PROJECT_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"

//typedef unsigned long DWORD;
//typedef unsigned long UINT32;
#define DWORD uint32_t
#define BYTE uint8_t


#include "util.h"
#include "radio.h"

#include "console.h"
#include "sensor.h"
#include "eeprom.h"
#include "rgbled.h"
#include "pushbutton.h"

void DbgPrintBuf( char * bufname, unsigned char * buf, int numbytes);
extern I2C_HandleTypeDef hi2c1;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

extern bool autoSend;

extern int sensor_read_period_ms;

extern TIM_HandleTypeDef htim2;

extern int ledResetFlag;
extern uint32_t ledTimestamp;




#endif /* INC_PROJECT_H_ */
