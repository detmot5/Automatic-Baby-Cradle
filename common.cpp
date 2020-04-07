/*
 * common.cpp
 *
 *  Created on: 13 Mar 2020
 *  	Author: Norbert Bielak
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <util/delay.h>


#include "MicroSwitch/MicroSwitch.h"
#include "common.h"
#include "eeprom.h"
#include "d_Led.h"
#include "cradle.h"




volatile uint32_t Timers[TIMERS_CNT];


	// buttons objects, only in first one we have to pass time base variable
MicroSwitch ButtonSwitch(&SWITCH_BUTTON_INPUT, SWITCH_BUTTON_PIN, DEFAULT_HOLD_TIME, 255, &Timers[buttonsTimeBase]);

MicroSwitch ButtonUp(&UP_BUTTON_INPUT, UP_BUTTON_PIN,50);
MicroSwitch ButtonDown(&DOWN_BUTTON_INPUT, DOWN_BUTTON_PIN,50);

MicroSwitch ButtonReset(&RESET_BUTTON_INPUT, RESET_BUTTON_PIN);




void periphInit(void){
	// **TIMER FOR 10ms TIME BASE**

	TCCR2A |= (1<<WGM21);						 // CTC mode
	TCCR2B |= (1<<CS20) | (1<<CS21) | (1<<CS22); // Prescaler 1024
	TIMSK2 |= (1<<OCIE2A);						 // OC2 irq enable
	OCR2A = 78;									 // 10ms time base for F_CPU = 8MHz

		// disable analog comparator
	ADCSRB |= (1<<ACME);
		//enable sleep mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);

	dLED::print(cradleGetParams(speed));
	dLED::displayOnTop(SPEED_SYMBOL,1000);

}


void reset(void){
		//Led blinking to singnalize start of reset procedure
	for(uint8_t i = 0; i < 10; i++){
		DBG_LED_TOG();
		_delay_ms(100);
	}
	eeprom_save_actual_pos();
	cli();
	wdt_enable(0);
	while(1);
}



void sleep(void){
		// enable PCINT2 to wake up uC by any button
	PCICR |= (1<<PCIE2);
	PCMSK2 |= (1<<PCINT19) | (1<<PCINT20) | (1<<PCINT21);
	dLED::print(SLEEP_SYMBOL);
	DBG_LED_OFF();
	sleep_mode();
}


// **ISR - 10ms TIME BASE**
ISR(TIMER2_COMPA_vect){
	uint32_t n;
	for(uint8_t i = 0; i < TIMERS_CNT; i++){
		n = Timers[i];
		if(n) Timers[i] = --n;
	}

}

ISR(PCINT2_vect){
		//disable PCINT
	PCICR = 0;
	PCMSK2 = 0;

}




