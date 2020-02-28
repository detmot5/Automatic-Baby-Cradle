/*
 * main.cpp
 *
 *  Created on: 27 lut 2020
 *      Author: norbe
 */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "LCD/lcd44780.h"
#include "cradle.h"

int main(void) {
	lcd_init();
	lcd_str_P(PSTR("start.."));
	softTimInit();
	servo_speed = 2;
	servo_pos = 500;
	//OCR1A = 800;
	while(1){
		servoEvent();
	}
}
