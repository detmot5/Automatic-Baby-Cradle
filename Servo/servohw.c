/*
 * servohw.c
 *
 *  Created on: 14 gru 2019
 *      Author: norbe
 */

#include <avr/io.h>

#include "servohw.h"


void servoHwInit(void){

	TCCR1A |= (1<<WGM11);						//Fast PWM mode - TOP value - ICR1
	TCCR1B |= (1<<WGM12) | (1<<WGM13);
	TCCR1B |= (1<<CS11);						//Timer prescaler /8
	TCCR1A |= (1<<COM1A1) | (1<<COM1B1);		//OCR1A and OCR1B active (non inverting mode)
	ICR1 = usToTicks(_SERVO_REFRESH_INTERVAL);	//Set top value to 20000us

	SERVO_FRONT_OUT;

	servoHwWrite(_SERVO_NEUTRAL);


}

void servoHwWrite(uint16_t duration){
		OCR1A = usToTicks(duration);
}


