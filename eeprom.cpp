/*
 * eeprom.cpp
 *
 *  Created on: 7 Mar 2020
 *      Author: Norbert Bielak
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include "cradle.h"
#include "eeprom.h"





void eeprom_update_speed(void){
	eeprom_write_byte(&servoParams_eep.speed, servoParams.speed);
}
void eeprom_update_duration(void){
	eeprom_write_word(&servoParams_eep.duration, servoParams.duration);
}
void eeprom_save_actual_pos(void){
	eeprom_write_word(&servoParams_eep.actualPos, servoParams.actualPos);
}

void eeprom_update_time_to_sleep(void){
	eeprom_write_word(&servoParams_eep.secondsToEnterSleep, servoParams.secondsToEnterSleep);

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
void eeprom_read_actual_pos(void){
	register uint16_t temp = eeprom_read_word(&servoParams_eep.actualPos);

	if(temp >= _SERVO_MIN && temp <= _SERVO_MAX)servoParams.actualPos = temp;
	else servoParams.actualPos = _SERVO_MIN;
}

void eeprom_read_time_to_sleep(void){
	register uint16_t temp = eeprom_read_word(&servoParams_eep.secondsToEnterSleep);

	if(temp != 0xFFFF && temp > 0) servoParams.secondsToEnterSleep = temp;
	else servoParams.secondsToEnterSleep = _DEFAULT_TIME_TO_SLEEP;
}

