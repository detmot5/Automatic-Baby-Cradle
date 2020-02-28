/*
 * cradle.cpp
 *
 *  Created on: 27 lut 2020
 *      Author: norbe
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#include "cradle.h"
#include "Servo/servohw.h"

volatile uint16_t Timer1,Timer2,Timer3,Timer4;

uint16_t servo_pos, servo_speed;
static uint16_t servo_actual_pos = 500;


void softTimInit(void){
	servoHwInit();
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<CS02) | (1<<CS00);
	TIMSK0 |= (1<<OCIE0A);
	OCR0A = 5;
	sei();
}



ServoMvRes_t servoEvent(void){
	static uint8_t isDone = 0;
	//static uint8_t handledPos = 0;
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
		isDone = 1;
		}
	return isDone;
}

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





