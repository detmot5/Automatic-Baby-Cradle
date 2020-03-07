/*
 * main.cpp
 *
 *  Created on: 27 lut 2020
 *      Author: norbe
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "LCD/lcd44780.h"
#include "UARTlib/uart.h"
#include "AT_commands/AT_commands.h"
#include "cradle.h"

//ServoMvRes_t result;




int main(void) {
	char buff[20];

	timersInit();


	USART_BeginTransmission(__UBRR);

	usart_rx_str_register_event_callback(parse_uart_data);

	USART_PutStr_P(PSTR("Start\r\n"));

	DDRD |= (1<<PD7);
	PORTD |= (1<<PD7);

	while(1){
		USART_RX_STR_EVENT(buff);
		servo_event();
		if(!Timer1){
			PORTD ^= (1<<PD7);
			Timer1 = 500;
		}
	}
}








