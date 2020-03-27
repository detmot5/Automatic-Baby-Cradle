/*
 * buttonService.cpp
 *
 *  Created on: 27 Mar 2020
 *      Author: Norbert Bielak
 */


#include <avr/io.h>

#include "MicroSwitch/MicroSwitch.h"
#include "common.h"
#include "cradle.h"




#define SPEED 0
#define DURATION 1


enum value {speed,duration};
static uint8_t changingValue;

//static uint8_t changingValue;





	// from Arduino
static inline uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


static void buttonSwitchHandler(void){

	changingValue ^= 1;
}

static void buttonResetHandler(void){

}

static void buttonUpHandler(void){
	switch(changingValue){
	case speed:

		break;
	case duration:

		break;
	}
}

static void buttonDownHandler(void){

}



void registerButtonsCallbacks(void){
	ButtonSwitch.registerPressEventCallback(buttonSwitchHandler);
	ButtonReset.registerPressEventCallback(buttonResetHandler);

	ButtonUp.registerPressEventCallback(buttonUpHandler);
	ButtonDown.registerPressEventCallback(buttonDownHandler);

}


