/*
 * ATservice.cpp
 *
 *  Created on: 7 Mar 2020
 *      Author: Norbert Bielak
 */


#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>



#include "ATservice.h"
#include "common.h"
#include "UARTlib/uart.h"
#include "texts.h"
#include "cradle.h"
#include "eeprom.h"

#include <stdio.h>


enum inout {deviceAsk = 0, param = 1, noParam = 2};



static inline bool isNumber(char *str){
	while(*(str++))	if(*str && (*str < '0' || *str > '9')) return false;
	return true;
}


atresult_t at_service(uint8_t inout, char *params){
	USART_PutStr_P(_OK);
	return 0;
}


atresult_t ati_service(uint8_t inout, char *params){
	USART_PutStr_P(_version);
	USART_PutStr_P(_endl);
	// send all parameters to controller
	Cradle::USART_deviceAnswer(PSTR("AT_SPD="), Cradle::GetParams(speed));
	Cradle::USART_deviceAnswer(PSTR("AT_RANGE="), Cradle::GetParams(range));
	Cradle::USART_deviceAnswer(PSTR("AT_STOP="), Cradle::stopFlag);

	return 0;
}

atresult_t at_spd_service(uint8_t inout, char *params){
	uint8_t spd;

	if(inout == param){
		if(Cradle::stopFlag){
			USART_PutStr_P(_deviceStopped);
			return ERROR;
		}

		if(!strlen(params) || !isNumber(params)) return ERROR;

		spd = atoi(params);
		if(Cradle::SetParams(speed,spd) < 0) return ERROR;
	}
	else if(inout == deviceAsk){

	}
	else if(inout == noParam){
		return ERROR;
	}


	return SUCCESS;
}
atresult_t at_range_service(uint8_t inout, char *params){
	uint16_t dur;

	if(inout == param){
		if(Cradle::stopFlag){
			USART_PutStr_P(_deviceStopped);
			return ERROR;
		}
		if(!strlen(params) || !isNumber(params)) return ERROR;


		dur = atoi(params);
		if(Cradle::SetParams(range,dur) < 0) return ERROR;
	}
	else if(inout == deviceAsk){
	}

	else if(inout == noParam){
		return ERROR;
	}

	return SUCCESS;
}
atresult_t at_stop_service(uint8_t inout, char *params){
		static bool stopFlagstate = false;
	if(inout == noParam){
		Cradle::Stop(stopFlagstate ^= 1);
	}
	return SUCCESS;
}

atresult_t at_fac_service(uint8_t inout, char *params){
	if(inout == noParam || inout == param){
		Cradle::SetParams(speed, 1);
		Cradle::SetParams(range, 1);
		if(inout == param && !strcmp("-a",params)){
			reset();
		}
	}

	return SUCCESS;
}

atresult_t at_rst_service(uint8_t inout, char *params){
	reset();
	return SUCCESS;
}

atresult_t at_tim_service(uint8_t inout, char *params){
	int32_t time; //in seconds
	static uint32_t timeRemaining;
	static bool pauseFlag = 0;
	if(inout == param){
		if(!strlen(params) || !isNumber(params)) return ERROR;
		time = atoi(params);
		if(time >= 0){
			Cradle::Stop(false);
			Timers[cradleDownCnt] = time * 100; // conversion to 10ms ticks
			Cradle::isTimActive = true;
		}
		else{
			Cradle::isTimActive = false;
			Cradle::Stop(false);
			pauseFlag = false;
		}
	}
	else if(inout == noParam && Cradle::isTimActive){
		pauseFlag ^= 1;
		if(pauseFlag){
			timeRemaining = Timers[cradleDownCnt];
			Cradle::Stop(true);
		}
		else{
			Timers[cradleDownCnt] = timeRemaining;
			Cradle::Stop(false);
		}
	}


	return SUCCESS;
}

atresult_t at_slptim_service(uint8_t inout, char *params){

	uint32_t time;
	if(inout == param){
		if(!strlen(params) || !isNumber(params)) return ERROR;
		time = atoi(params);
		Cradle::Params.secondsToEnterSleep = time;
	}

	else if(inout == noParam){
		return ERROR;
	}
	else if(inout == deviceAsk){
		return ERROR;
	}

	return SUCCESS;
}








