/*
 * cradle.cpp
 *
 *  Created on: 27 lut 2020
 *      Author: norbe
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "cradle.h"




volatile uint16_t Timer1,Timer2,Timer3,Timer4;

uint16_t servo_pos;
uint8_t servo_speed = 2;
static uint16_t servo_actual_pos = _SERVO_MIN;

//--------------------------------------------------------------------------
//						  STATIC FUNCTIONS
//--------------------------------------------------------------------------

static int8_t servoDrive(void){
	static uint8_t isDone;

	if(servo_pos != servo_actual_pos){
		isDone = 0;
		if(!Timer4 && !isDone){
				// speed - delay between each step
			Timer4 = servo_speed;
			// going forward
			if(servo_actual_pos < servo_pos){
					//increment on every step
				servo_actual_pos++;
					//set this value to servo
				servoWrite(servo_actual_pos);
			}
			//going back
			else{
					//decrement on every step
				servo_actual_pos--;
				servoWrite(servo_actual_pos);
			}
		}
	}
	else{
			//if first move done, 1 signalize ready to go back
			//if back move done, 2 signalize ready to next cycle
		isDone++;
	}
	return isDone;
}

//--------------------------------------------------------------------------

void timersInit(void){

		// **INIT SERVO TIMER**
	TCCR1A |= (1<<WGM11);						// Fast PWM mode - TOP value - ICR1
	TCCR1B |= (1<<WGM12) | (1<<WGM13);
	TCCR1B |= (1<<CS11);						// Timer prescaler /8
	TCCR1A |= (1<<COM1A1);						// OCR1A active (non inverting mode)
	ICR1 = usToTicks(_SERVO_REFRESH_INTERVAL);	// Set top value to 20000us
	SERVO_OUT;

		// **INIT TIME BASE TIMER**
	TCCR0A |= (1<<WGM01);						// CTC mode
	TCCR0B |= (1<<CS02) | (1<<CS00);			// Timer prescaler /1024
	TIMSK0 |= (1<<OCIE0A);						// Output Compare Interrupt enable
	OCR0A = 2;									// Time base

	sei();
}



int8_t servo_event(uint16_t angle, uint8_t speed){
	int8_t result;


	if((result = servoDrive()) == 1){
		servo_pos = _SERVO_MIN; 	// move done, go back
	}
	else if(result == 2){
		servo_pos = angle;		//repeat
	}
	return result;
}





//ISR - time base for servo speed control
ISR(TIMER0_COMPA_vect){
	uint16_t n;

	n = Timer1;
	if (n) Timer1 = --n;
	n = Timer2;
	if (n) Timer2 = --n;
	n = Timer3;
	if (n) Timer3 = --n;
	n = Timer4;
	if (n) Timer4 = --n;
}
