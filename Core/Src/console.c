/*
 * console.c
 *
 *  Created on: Jun 18, 2025
 *      Author: Randy
 */

#include "project.h"
#include <stdlib.h>

// private static variables
uint8_t rx1_buffer[64]; // Buffer to store received data
volatile uint8_t rx1_index = 0; // Index for received data
volatile uint8_t rx1_complete = 0; // Flag to indicate reception complete
bool autoSend = true;

// private static functions
static int ProcessConsoleMsg(char * buf);

int ConsoleInit()
{
	// clear the message rx buffer
	memset(rx1_buffer, 0, sizeof(rx1_buffer));
	rx1_index = 0;
	rx1_complete = 0;

	// Enable UART receive interrupts
	return HAL_UART_Receive_IT(&huart1, &rx1_buffer[rx1_index], 1);

}

// This is called by the ISR when a new character has been received
void ConsoleRxCpltCallback()
{
    // Check for end of data (e.g., newline character)
	// putty, for example appends only \r so we use \r to detect the end of the message
    if (rx1_buffer[rx1_index] == '\r' || rx1_index >= sizeof(rx1_buffer) - 1)
    {
        printf("\n");
    	rx1_complete = 1; // Set flag to process data in main loop

		// warning: message be truncated if it overflows the buffer
    }
    else
    {
        rx1_index++;
        // Continue receiving next byte
        HAL_UART_Receive_IT(&huart1, &rx1_buffer[rx1_index], 1);
    }

}

// this is called constantly in main loop
// check the message received flag and deal with it
void DoConsoleProcess()
{
    // are there messages from the console?
	if (rx1_complete)
	{
		// strip off the trailing \r and process the command
		if (rx1_buffer[rx1_index] == '\r')
			rx1_buffer[rx1_index] = 0;

		ProcessConsoleMsg((char *)rx1_buffer);

		ConsoleInit();		// prep the buffer, index and flag  for the next incoming message
	}
}

// we have a sanitized incoming message from the console. Take action if required.
static int ProcessConsoleMsg(char * buf)			// NULL terminated string
{

	//static char lastcmd[40];

	//printf("Processing console message: %s\r\n", rx1_buffer);

	if (strlen(buf) == 0)
		return 0;

	//////////////////////////////////////
	//
	// parse the command
	//
	//////////////////////////////////////
	if (strnicmp(buf, "help", 4) == 0)
	{
		printf("Console Commands:\r\n");
		//printf("R       Repeat last command\r\n");
		printf("P       Pause automatic measurement\r\n");
		printf("U       Unpause automatic measurement\r\n");
		printf("T <sec> Set measurement period in seconds (default 360)\r\n");
		printf("AT*     Send AT commands to radio\r\n");

		return 0;
	}

#ifdef LATER
	if (strnicmp(buf, "R", 1) == 0)
	{
		strcpy(buf, lastcmd);
		printf("Repeating last command %s\r\n", buf);
		return 0;
	}
#endif

	// Pause automatic temperature sending
	if (strnicmp(buf, "P", 1) == 0)
	{
		autoSend = false;
		return 0;
	}

	// Un-pause automatic temperature sending
	if (strnicmp(buf, "U", 1) == 0)
	{
		autoSend = true;
		return 0;
	}



	if (strnicmp(buf, "T", 1) == 0)
	{
		char* numPointer = buf + 2;
		int tempInt = atoi(numPointer);
		//sensor_read_period_ms = tempInt * 1000 - 1;
		SetPeriodSec(tempInt);
		ResetSensorTimer();

		return 0;
	}

	// is it an AT command to the radio?
	if (strnicmp(buf, "AT", 2) == 0)
	{
		SendRadioMsg(buf);

	}

	//strcpy(lastcmd, buf);	// save for subsequent use

#ifdef OBSOLETE
	else if (strnicmp(buf, "SW", 2) == 0)
	{
		DoSensorWriteCommand(buf+3);
	}
	else if (strnicmp(buf, "SR", 2) == 0)
	{
		DoSensorReadCommand(buf+3);
	}
	else if (strnicmp(buf, "SM", 2) == 0)
	{
		DoSensorReadCommand(buf+3);
	}
#endif
	else
	{
		printf("%s is not a valid command\r\n", buf);
	}

	return 0;
}


// sends a string to the console. can also use printf()
int SendConsoleMsg(char * buf)
{
	const char * lineterm = "\r\n";

	// send it to the radio
	HAL_UART_Transmit(&huart1, (unsigned char *)buf, strlen(buf), HAL_MAX_DELAY);

	// append "/n"
	HAL_UART_Transmit(&huart1, (unsigned char *)lineterm, strlen(lineterm), HAL_MAX_DELAY);

	return 0;
}
