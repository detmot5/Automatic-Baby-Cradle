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
	eeprom_write_byte(&Cradle::Params_eep.speed, Cradle::Params.speed);
}
void eeprom_update_duration(void){
	eeprom_write_word(&Cradle::Params_eep.duration, Cradle::Params.duration);
}
void eeprom_save_actual_pos(void){
	eeprom_write_word(&Cradle::Params_eep.actualPos, Cradle::Params.actualPos);
}

void eeprom_update_time_to_sleep(void){
	eeprom_write_word(&Cradle::Params_eep.secondsToEnterSleep, Cradle::Params.secondsToEnterSleep);
}



void eeprom_read_speed(void){
	register uint8_t temp = eeprom_read_byte(&Cradle::Params_eep.speed);

	if(temp >= _SERVO_MIN_DELAY && temp <= _SERVO_MAX_DELAY) Cradle::Params.speed = temp;
	else Cradle::Params.speed = _SERVO_MAX_DELAY;
}
void eeprom_read_duration(void){
	register uint16_t temp = eeprom_read_word(&Cradle::Params_eep.duration);

	if(temp >= _SERVO_MIN && temp <= _SERVO_MAX) Cradle::Params.duration = temp;
	else Cradle::Params.duration = _SERVO_MIN;
}
void eeprom_read_actual_pos(void){
	register uint16_t temp = eeprom_read_word(&Cradle::Params_eep.actualPos);

	if(temp >= _SERVO_MIN && temp <= _SERVO_MAX)Cradle::Params.actualPos = temp;
	else Cradle::Params.actualPos = _SERVO_MIN;
}

void eeprom_read_time_to_sleep(void){
	register uint16_t temp = eeprom_read_word(&Cradle::Params_eep.secondsToEnterSleep);

	if(temp != 0xFFFF && temp > 0) Cradle::Params.secondsToEnterSleep = temp;
	else Cradle::Params.secondsToEnterSleep = _DEFAULT_TIME_TO_SLEEP;
}


