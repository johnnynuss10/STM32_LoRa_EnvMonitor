/*
 * radio.c
 *
 *  Created on: Jun 18, 2025
 *      Author: Randy
 */
#include "project.h"
#include <ctype.h>

 uint8_t rx2_buffer[64]; // Buffer to store received data
 volatile uint8_t rx2_index = 0; // Index for received data
 volatile uint8_t rx2_complete = 0; // Flag to indicate reception complete
 static int RadioBufferInit();
 static char txbuf[64];
 static char rxbuf[64];

static char * radio_vars[] =
{
		"ADDRESS",
		"NETWORKID",
		"MODE",
		"IPR",
		"BAND",
		"PARAMETER",
		"CPIN",
		"CRFOP",
		"UID",
		"VER",
};

int RadioInit()
{
	int errors = 0;

	RadioBufferInit();		// set up for receiving from the radio

	// TBD do other stuff

	return errors;
}

int RadioPrintStatus()
{
	int errors = 0;

	// iterate through the radio vars table and query the radio
	for (int i = 0; i < sizeof(radio_vars)/sizeof(radio_vars[0]); i++)
	{

		// query each variable
		sprintf(txbuf, "AT+%s?", radio_vars[i]);
		SendRadioMsg(txbuf);

		if (RecvRadioMsg(rxbuf, sizeof(rxbuf), RADIO_RX_TIMEOUT))
		{
			printf("RadioStatus() Error: TX: %s, RX: <timeout>\r\n", txbuf);
			errors++;
		}
		else if (0 == strncmp(rxbuf, "+ERR", 4))
		{
			printf("RadioStatus() Error: TX: %s, RX: %s\r\n", txbuf, rxbuf);
			errors++;
		}
		else
		{
			printf("%s\r\n", rxbuf+1);
		}
	}

	return errors;
}

int RecvRadioMsg(char * rxbuf, int rxbuf_maxlen, int timeout_ms)
{
	// start a timeout timer
	COUNTDOWN to_timer;
	SetCountDown(&to_timer, timeout_ms);

	while (GetCountDown(&to_timer))
	{
		// are there messages from the radio
		if (rx2_complete)
		{
			// strip off the trailing \r and \n and null terminate the string
			if (rx2_buffer[rx2_index] == '\n')
			{
				rx2_buffer[rx2_index-1] = 0;
				rx2_buffer[rx2_index] = 0;
			}

			// copy the buffer to callers string and clear the receive buffer
			strcpy(rxbuf, (char *)rx2_buffer);
			RadioBufferInit();

			return 0;
		}
	}

	// oops we timed out, return non-zero
	return -1;
}


static int RadioBufferInit()
{
	// clear the message rx buffer
	memset(rx2_buffer, 0, sizeof(rx2_buffer));
	rx2_index = 0;
	rx2_complete = 0;

	// Enable UART receive interrupts
	return HAL_UART_Receive_IT(&huart1, &rx2_buffer[rx2_index], 1);

}


// the RYLR998 appends \r\n to all messages so we use \n as the last character
// This is called by the ISR when a new character has been received
void RadioRxCpltCallback()
{
	if (rx2_buffer[rx2_index] == '\n' || rx2_index >= sizeof(rx2_buffer) - 1)
	{
		rx2_complete = 1; // Set flag to process data in main loop

		// warning: message be truncated if it overflows the buffer
	}
	else
	{
		rx2_index++;
		// Continue receiving next byte
		HAL_UART_Receive_IT(&huart1, &rx2_buffer[rx2_index], 1);
	}
}


// this is called constantly in main loop
// check the message received flag and deal with it
void DoRadioProcess()
{
	// are there messages from the radio
	if (rx2_complete)
	{

		// strip off the trailing \r and \n and null terminate the string
		if (rx2_buffer[rx2_index] == '\n')
		{
			rx2_buffer[rx2_index-1] = 0;
			rx2_buffer[rx2_index] = 0;
		}

		ProcessRadioMsg((char *)rx2_buffer);			// NULL terminated string

		RadioBufferInit();
	}
}

int ProcessRadioMsg(char * buf)			// NULL terminated string
{

	printf("LoRa Rx: %s\r\n", buf);

	return 0;

}

int SendRadioMsg(char * buf)
{
	//printf("srm:%s\r\n", buf);

	const char * lineterm = "\r\n";

	// convert all characters to upper case to make the radio happy
	for (int i = 0; i < strlen(buf); i++)
		buf[i] = toupper(buf[i]);

	// send it to the radio
	HAL_UART_Transmit(&huart1, (unsigned char *)buf, strlen(buf), HAL_MAX_DELAY);

	// append "/r/n"
	HAL_UART_Transmit(&huart1, (unsigned char *)lineterm, strlen(lineterm), HAL_MAX_DELAY);

	return 0;

}


