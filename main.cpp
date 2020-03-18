/*
 * main.cpp
 *
 *  Created on: 27 Feb 2020
 *      Author: Norbert Bielak
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
#include "common.h"
extern svParams_t servoParams;
void __init3(void) __attribute__ (( section( ".init3" ), naked, used ));
void __init3(void){
	//turn wdt off before program start
    MCUSR = 0;
    WDTCSR = (1<<WDCE) | (1<<WDE);
    WDTCSR = 0;
}


int main(void) {
	char buff[20];


		// Initializations
	cradleInit();
	softTimInit();
	USART_BeginTransmission(__UBRR);
	usart_rx_str_register_event_callback(parse_uart_data);

		// ---------------------

	USART_PutStr_P(PSTR("Start\r\n"));
	USART_PutInt(servoParams.duration,dec);
	DDRD |= (1<<PD7);
	PORTD |= (1<<PD7);

	while(1){
		USART_RX_STR_EVENT(buff);
		CRADLE_EVENT();
			//test led blinking
		if(!Timers[testLed]){
			PORTD ^= (1<<PD7);
			Timers[testLed] = 50;
		}
	}
}








