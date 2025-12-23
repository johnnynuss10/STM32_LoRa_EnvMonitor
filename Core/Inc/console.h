/*
 * console.h
 *
 *  Created on: Jun 18, 2025
 *      Author: Randy
 */

#ifndef INC_CONSOLE_H_
#define INC_CONSOLE_H_

int ConsoleInit();
void ConsoleRxCpltCallback();
void DoConsoleProcess();
int SendConsoleMsg(char * buf);



#endif /* INC_CONSOLE_H_ */
