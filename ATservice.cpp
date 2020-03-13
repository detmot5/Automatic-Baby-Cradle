/*
 * ATservice.cpp
 *
 *  Created on: 7 mar 2020
 *      Author: norbe
 */


#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <string.h>
#include <stdlib.h>



#include "ATservice.h"
#include "common.h"
#include "UARTlib/uart.h"
#include "texts.h"
#include "cradle.h"
#include "eeprom.h"

extern svParams_t servoParams;
extern uint8_t stopFlag;		// 0 - run  1 - stop

static inline void reset(){
	cli();
	wdt_enable(0);
	while(1);
}


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

		if(spd >= _SERVO_MIN_DELAY && spd <= _SERVO_MAX_DELAY){
			servoParams.speed = spd;
			eeprom_update_speed();
		}
	}
	else if(inout == 0){
		USART_PutStr_P(_atSpd);
		USART_PutStr_P(_endl);
		USART_PutInt(servoParams.speed,dec);

	}
	else if(inout == 2){
		return ERROR;
	}


	return SUCCESS;
}
atresult_t at_dur_service(uint8_t inout, char *params){
	uint16_t dur;

	if(inout == 1 && !stopFlag){
		if(!strlen(params)) return ERROR;
		dur = atoi(params);
		if(dur >= _SERVO_MIN && dur <= _SERVO_MAX){
			servoParams.duration = dur;
			eeprom_update_duration();
		}
	}
	else if(inout == 1 && stopFlag) USART_PutStr_P(_deviceStopped);
	else if(inout == 0){
		USART_PutStr_P(_atDur);
		USART_PutStr_P(_endl);
		USART_PutInt(servoParams.duration,dec);

	}

	else if(inout == 2){
		return ERROR;
	}

	return SUCCESS;
}
atresult_t at_stop_service(uint8_t inout, char *params){
	if(inout == 2){
		stopFlag ^= 1;
	}
	else if(inout == 0){
		USART_PutStr_P(_endl);
		USART_PutInt(stopFlag,dec);
	}

	return SUCCESS;
}

atresult_t at_fac_service(uint8_t inout, char *params){
	if(inout == 2 || inout == 1){
		if(inout == 1){
			if(!strcmp("-a",params)){
				_delay_ms(1500);
				reset();
			}
		}
	}

	return SUCCESS;
}

atresult_t at_rst_service(uint8_t inout, char *params){
	_delay_ms(1500);
	reset();
	return SUCCESS;
}

atresult_t at_tim_service(uint8_t inout, char *params){
	if(inout == 1){

	}


	return SUCCESS;
}


