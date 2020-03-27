/*
 * common.cpp
 *
 *  Created on: 13 Mar 2020
 *  	Author: Norbert Bielak
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "MicroSwitch/MicroSwitch.h"
#include "common.h"






volatile uint32_t Timers[TIMERS_CNT];


	// buttons objects, only in first one we have to pass time base variable
MicroSwitch ButtonUp(&UP_BUTTON_INPUT, UP_BUTTON_PIN, &Timers[buttonsTimeBase]);
MicroSwitch ButtonDown(&DOWN_BUTTON_INPUT, DOWN_BUTTON_PIN);

MicroSwitch ButtonSwitch(&SWITCH_BUTTON_INPUT, SWITCH_BUTTON_PIN);
MicroSwitch ButtonReset(&RESET_BUTTON_INPUT, RESET_BUTTON_PIN);




void softTimInit(void){
	// **TIMER FOR 10ms TIME BASE**

	TCCR2A |= (1<<WGM21);						 // CTC mode
	TCCR2B |= (1<<CS20) | (1<<CS21) | (1<<CS22); // Prescaler 1024
	TIMSK2 |= (1<<OCIE2A);						 // OC2 irq enable
	OCR2A = 78;									 // 10ms time base for F_CPU = 8MHz
}


// **ISR - 10ms TIME BASE**
ISR(TIMER2_COMPA_vect){
	uint32_t n;
	for(uint8_t i = 0; i < TIMERS_CNT; i++){
		n = Timers[i];
		if(n) Timers[i] = --n;
	}

}
