/*
 * radio.h
 *
 *  Created on: Jun 18, 2025
 *      Author: Randy
 */

#ifndef INC_RADIO_H_
#define INC_RADIO_H_

#define RADIO_RX_TIMEOUT 500

int RadioInit();
int RadioPrintStatus();
void RadioRxCpltCallback();
void DoRadioProcess();
int ProcessRadioMsg(char * buf);
int SendRadioMsg(char * buf);
int RecvRadioMsg(char * rxbuf, int maxlen, int timeout_ms);

#endif /* INC_RADIO_H_ */
