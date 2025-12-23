/*
 * rgbled.h
 *
 *  Created on: Jul 25, 2025
 *      Author: johnn
 */

#ifndef INC_RGBLED_H_
#define INC_RGBLED_H_

int InitRGBLED();
int ledSetColor(int redVal, int grnVal, int bluVal);
int ResetRGBLED();
int ModulateRGBLED();
int DoRGBLEDProcess();





#endif /* INC_RGBLED_H_ */
