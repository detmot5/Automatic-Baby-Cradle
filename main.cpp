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
#include "buttonService.h"


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
	lcd_init();
	USART_BeginTransmission(__UBRR);
	usart_rx_str_register_event_callback(parse_uart_data);
	registerButtonsCallbacks();

		// ---------------------

	USART_PutStr_P(PSTR("Start\r\n"));
	DBG_LED_DIR_OUT();
	DBG_LED_ON();

	while(1){
		USART_RX_STR_EVENT(buff);
		CRADLE_EVENT();
		ButtonUp.Event();
		ButtonSwitch.Event();
		ButtonDown.Event();
		ButtonReset.Event();
		if(!Timers[testLed]){
			//DBG_LED_TOG();
			Timers[testLed] = 50;
		}
	}
}








