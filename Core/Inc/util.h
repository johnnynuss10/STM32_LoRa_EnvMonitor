/*
 * util.h
 *
 *  Created on: Jun 13, 2025
 *      Author: Randy
 */

#ifndef SRC_UTIL_H_
#define SRC_UTIL_H_

//void BlinkLed(int loopcnt);

typedef struct {
	DWORD	remaining;
	DWORD	timestamp;
} COUNTDOWN;


#define CD_MAXCOUNT     0xffffffff
#define HIBYTE(x) ((0xff00 & (x)) >> 8)
#define LOBYTE(x) (0xff & (x))


//int Crc16(char * ptr, int count);
int atox(const char *hex);

// initialize a countdown timer
void SetCountDown(COUNTDOWN * pCountDown, uint32_t time);

// Display remaining time of a countdown timer
DWORD GetCountDown(COUNTDOWN * pCountDown);

void DbgPrintBuf( char * bufname, unsigned char * buf, int numbytes);


#endif /* SRC_UTIL_H_ */
