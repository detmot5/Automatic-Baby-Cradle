/*
 * common.cpp
 *
 *  Created on: 13 Mar 2020
 *  	Author: Norbert Bielak
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "common.h"



volatile uint32_t Timers[TIMERS_CNT];

timNames_t timNames;

void softTimInit(void){
	// **TIMER FOR 10ms TIME BASE**

	TCCR2A |= (1<<WGM21);						 // CTC mode
	TCCR2B |= (1<<CS20) | (1<<CS21) | (1<<CS22); // Prescaler 1024
	TIMSK2 |= (1<<OCIE2A);						 // OC2 irq enable
	OCR2A = 78;									 // 10ms time base for F_CPU = 8MHz
}


// **ISR - 10ms TIME BASE**
ISR(TIMER2_COMPA_vect){
	uint16_t n;
	for(uint8_t i = 0; i < TIMERS_CNT; i++){
		n = Timers[i];
		if(n) Timers[i] = --n;
	}

}
