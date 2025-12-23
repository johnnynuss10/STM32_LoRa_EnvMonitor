/*
 * util.c
 *
 *  Created on: Jun 13, 2025
 *      Author: Randy
 */
#include "stm32f1xx_hal.h"
#include <sys/param.h>
#include <stdint.h>
#include <ctype.h>
#include "project.h"

#ifdef OBSOLETE
void BlinkLed(int loopcnt)
{
	// Toggle the LED (active low)
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

    // Delay ~500 ms
    HAL_Delay(1000);

}
#endif
/////////////////  COUNTDOWN /////////////////

// initialize a countdown timer
void SetCountDown(COUNTDOWN * pCountDown, uint32_t time)
{
	pCountDown->remaining = time;
	pCountDown->timestamp = HAL_GetTick();
}

// Display remaining time of a countdown timer
DWORD GetCountDown( COUNTDOWN * pCountDown)
{
	// check for infinite count
	if (pCountDown->remaining == CD_MAXCOUNT)
		return CD_MAXCOUNT;

	DWORD now = HAL_GetTick();
	pCountDown->remaining -= MIN(pCountDown->remaining, now - pCountDown->timestamp);
	pCountDown->timestamp = now;

	return pCountDown->remaining;
}
#ifdef LATER
void DbgPrintBuf( char * bufname, unsigned char * buf, int numbytes)
{
	printf("%s[]: ", bufname);
	for (int i = 0; i < numbytes; i++)
	{
		printf("0x%02x ", (unsigned char)buf[i]);
	}
	printf("\r\n");
}
#endif


int atox(const char *hex)
{
    int result = 0;

    // Check for "0x" or "0X" prefix
    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        hex += 2; // Skip prefix
    }

    while (*hex) {
        char c = *hex++;

        if (isdigit(c)) {
            result = (result << 4) | (c - '0');
        } else if (c >= 'a' && c <= 'f') {
            result = (result << 4) | (c - 'a' + 10);
        } else if (c >= 'A' && c <= 'F') {
            result = (result << 4) | (c - 'A' + 10);
        } else {
            break; // Invalid character
        }
    }

    return result;
}
