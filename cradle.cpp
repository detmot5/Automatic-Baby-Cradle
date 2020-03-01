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
#include "Servo/servohw.h"

volatile uint16_t Timer1,Timer2,Timer3,Timer4;

uint16_t servo_pos, servo_speed;
static uint16_t servo_actual_pos = _SERVO_MIN;

//--------------------------------------------------------------------------
//						  STATIC FUNCTIONS
//--------------------------------------------------------------------------

static ServoMvRes_t servoDrive(void){
	static uint8_t isDone;

	if(servo_pos != servo_actual_pos){
		isDone = 0;
		if(!Timer4 && !isDone){
			Timer4 = servo_speed;
			if(servo_actual_pos < servo_pos){
				servo_actual_pos++;
				OCR1A = usToTicks(servo_actual_pos);
			}
			else{
				servo_actual_pos--;
				OCR1A = usToTicks(servo_actual_pos);
			}
		}
	}
	else{
		isDone++;
	}
	return isDone;
}

//--------------------------------------------------------------------------

void softTimInit(void){
	servoHwInit();
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS02) | (1<<CS00);
	TIMSK0 |= (1<<OCIE0A);
	OCR0A = 2;
	sei();
}


ServoMvRes_t servo_event(uint16_t angle, uint8_t speed){
	ServoMvRes_t result;

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
