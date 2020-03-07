/*
 * ATservice.cpp
 *
 *  Created on: 7 mar 2020
 *      Author: norbe
 */


#include <avr/io.h>
#include <string.h>
#include <stdlib.h>


#include "ATservice.h"
#include "UARTlib/uart.h"
#include "texts.h"
#include "cradle.h"

extern svParams_t servoParams;


atresult_t at_service(uint8_t inout, char *params){
	USART_PutStr_P(_OK);
	return 0;
}
atresult_t ati_service(uint8_t inout, char *params){
	USART_PutStr_P(_version);
	return 0;
}

atresult_t at_spd_service(uint8_t inout, char *params){
	uint8_t spd;

	if(inout == 1){
		if(!strlen(params)) return ERROR;
		spd = atoi(params);

		if(spd >= _SERVO_MAX_SPEED && spd <= _SERVO_MIN_SPEED) servoParams.speed = spd;
	}
	else if(inout == 2){
		USART_PutStr_P(_atSpd);
	}
	else if(!inout){
		return ERROR;
	}


	return SUCCESS;
}
atresult_t at_dur_service(uint8_t inout, char *params){
	uint16_t dur;

	if(inout == 1){
		if(!strlen(params)) return ERROR;
		dur = atoi(params);
		if(dur >= _SERVO_MIN && dur <= _SERVO_MAX) servoParams.duration = dur;
	}
	else if(inout == 2){
		USART_PutStr_P(_atDur);
	}
	else if(!inout){
		return ERROR;
	}


	return SUCCESS;
}

