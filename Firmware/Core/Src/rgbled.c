/*
 * rgbled.c
 *
 *  Created on: Jul 25, 2025
 *      Author: johnn
 */

#include "project.h"

uint32_t colorModTickCount;
uint32_t colorChangeCount = 0;
int red = 10;
int grn = 0;
int blu = 0;
int speed = 1;
bool increasing = true;

int InitRGBLED()
{
	// TBD Convert LEDs to gpio
	//HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	//HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	//HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
	//colorModTickCount = HAL_GetTick();

	return 0;
}

int ledSetColor(int redVal, int grnVal, int bluVal)
{
	/*
	//int redVal2 = (255 - redVal);
	int redVal2 = 255 - ( redVal * 0.6);

	int grnVal2 = 255 - grnVal;
	int bluVal2 = 255 - bluVal;
	*/
	//__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, redVal2);
	//__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, grnVal2);
	//__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, bluVal2);


	return 0;
}

int ResetRGBLED()
{
	if (ledResetFlag && (HAL_GetTick() - ledTimestamp >= 1000))
	{
		ledSetColor(10, 10, 10);
		ledResetFlag = 0;
	}

	return 0;
}

int ModulateRGBLED()
{

	if ((HAL_GetTick() - colorModTickCount >= 50) && ledResetFlag == 0)
	{
		if (increasing)
		{
			red = red + colorChangeCount * speed;
			ledSetColor(red, grn, blu);
			colorModTickCount = HAL_GetTick();
			colorChangeCount = colorChangeCount + 1;

			if (red > 255)
			{
				increasing = false;
				colorChangeCount = 0;
			}
		}

		else if(!increasing)
		{
			red = red - colorChangeCount * speed;
			ledSetColor(red, grn, blu);
			colorModTickCount = HAL_GetTick();
			colorChangeCount = colorChangeCount + 1;


			if (red < 30)
			{
				increasing = true;
				colorChangeCount = 0;
			}
		}

//		red = red + colorChangeCount * speed;
//		if (red > 255)
//		{
//			red = 10;
//			colorChangeCount = 0;
//		}
//		ledSetColor(red, grn, blu);
//		colorModTickCount = HAL_GetTick();
//		colorChangeCount = colorChangeCount + 1;
	}

	return 0;
}

int DoRGBLEDProcess()
{
	ResetRGBLED();
	ModulateRGBLED();

	return 0;
}



