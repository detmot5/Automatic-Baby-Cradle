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
#include "cradle.h"
#include "AT_commands.h"
ServoMvRes_t result;




int main(void) {

	cli();
	wdt_disable();
	softTimInit();
	USART_BeginTransmission(__UBRR);
	servo_speed = 2;
	usart_rx_str_register_event_callback(parse_uart_data);
	DDRD |= (1<<PD7);
	char buff[20];
	USART_PutStr_P(PSTR("Start\r\n"));
	PORTD |= (1<<PD7);
	//int i = 0;

	while(1){
		USART_RX_STR_EVENT(buff);
		if(!Timer1){
			PORTD ^= (1<<PD7);
			Timer1 = 500;
		}
	}
}








