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
	servo_speed = 2;

	DDRD |= (1<<PD7);


	//int i = 0;

	while(1){


		servo_event(2500,servo_speed);
		if(!Timer1){
			PORTD ^= (1<<PD7);
			Timer1 = 100;

		}

		_delay_us(1000);
	}
}








