/*
 * mSwitch.cpp
 *
 *  Created on: 27 mar 2020
 *      Author: norbe
 */




#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include "MicroSwitch.h"




volatile uint32_t *MicroSwitch::timeBase10Ms = 0;

MicroSwitch::MicroSwitch(volatile uint8_t *_PINx, uint8_t _pin, volatile uint32_t *timeBase,
		uint8_t _holdTime, uint8_t _holdRepeatTime){
	PINx = _PINx;
	pin = _pin;
	pinMask = (1<<_pin);
	holdTime = _holdTime;
	holdRepeatTime = _holdRepeatTime;

		// Set pointers at 0 address
	PressEventCallback = 0;
	HoldEventCallback = 0;
		// Enable pullup
	*(_PINx+2) |= (1<<_pin);

	if(timeBase){
		timeBase10Ms = timeBase;
	}
}


void MicroSwitch::registerPressEventCallback(void (*callback)()){
	PressEventCallback = callback;
}
void MicroSwitch::registerHoldEventCallback(void (*callback)()){
	HoldEventCallback = callback;
}


