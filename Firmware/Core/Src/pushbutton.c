/*
 * pushbutton.c
 *
 *  Created on: Jul 28, 2025
 *      Author: johnn
 */

#include "project.h"

uint32_t ledTimestamp = 0;
int ledResetFlag = 0;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_8)
	{
		ledSetColor(255, 0, 255);
		ledTimestamp = HAL_GetTick();
		ledResetFlag = 1;
	}

	return;
}



