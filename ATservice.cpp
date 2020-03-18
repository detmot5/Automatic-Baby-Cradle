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
extern bool isCradleTimActive;


static inline void reset(void){
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
	else if(inout == 1 && stopFlag) USART_PutStr_P(_deviceStopped);
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
		servoParams.speed = _SERVO_MIN_DELAY;
		servoParams.duration = _SERVO_MIN;
		eeprom_update_speed();
		eeprom_update_duration();
		if(inout == 1 && !strcmp("-a",params)){
			eeprom_save_actual_pos();
			_delay_ms(1500);
			reset();
		}
	}

	return SUCCESS;
}

atresult_t at_rst_service(uint8_t inout, char *params){
	eeprom_save_actual_pos();
	_delay_ms(1500);
	reset();
	return SUCCESS;
}

atresult_t at_tim_service(uint8_t inout, char *params){
	int32_t time = atoi(params); //in seconds
	static uint32_t timeRemaining;
	static uint8_t pauseFlag = 0;
	if(inout == 1){
		if(time > 0){
			stopFlag = false;
			Timers[cradleDownCnt] = time * 100; // conversion to 10ms ticks
			isCradleTimActive = true;
		}
		else{
			isCradleTimActive = false;
			stopFlag = false;
			pauseFlag = 0;
		}
	}
	else if(inout == 2 && isCradleTimActive){
		pauseFlag ^= 1;
		if(pauseFlag){
			timeRemaining = Timers[cradleDownCnt];
			stopFlag = 1;
			USART_PutStr_P(_deviceStopped);
		}
		else{
			Timers[cradleDownCnt] = timeRemaining;
			stopFlag = 0;
		}
	}

	return SUCCESS;
}
