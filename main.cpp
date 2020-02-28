/*
 * main.cpp
 *
 *  Created on: 27 lut 2020
 *      Author: norbe
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "LCD/lcd44780.h"
#include "cradle.h"

ServoMvRes_t result;


int main(void) {

	softTimInit();
	servo_speed = 1;
	servo_pos = 2500;
	//OCR1A = 800;
	DDRB |= (1<<PB0);
	PORTB |= (1<<PB0);
	while(1){
		result = servoEvent();
		if(result){
			servo_pos = 500;
			PORTB &= ~(1<<PB0);
		}
	//	_delay_us(5000);
	}
}
