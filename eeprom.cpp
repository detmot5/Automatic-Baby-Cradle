/*
 * eeprom.cpp
 *
 *  Created on: 7 mar 2020
 *      Author: norbe
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include "cradle.h"
#include "eeprom.h"



extern svParams_t servoParams_eep EEMEM;
extern svParams_t servoParams;

void eeprom_update_speed(void){
	eeprom_write_byte(&servoParams_eep.speed, servoParams.speed);
}
void eeprom_update_duration(void){
	eeprom_write_word(&servoParams_eep.duration, servoParams.duration);
}

void eeprom_read_speed(void){
	register uint8_t temp = eeprom_read_byte(&servoParams_eep.speed);

	if(temp >= _SERVO_MIN_DELAY && temp <= _SERVO_MAX_DELAY) servoParams.speed = temp;
	else servoParams.speed = _SERVO_MAX_DELAY;
}
void eeprom_read_duration(void){
	register uint16_t temp = eeprom_read_word(&servoParams_eep.duration);

	if(temp >= _SERVO_MIN && temp <= _SERVO_MAX)servoParams.duration = temp;
	else servoParams.duration = _SERVO_MIN;
}



