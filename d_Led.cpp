/*
 * dLed.cpp
 *
 *  Created on: 4 kwi 2020
 *      Author: norbe
 */



#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "d_Led.h"
#include "common.h"


#define SEG_CNT  8
#define DP_INDEX SEG_CNT-1

namespace dLED{


		// -std=c++11
	uint8_t LedSegment::segmentsCnt = 0;
	static LedSegment seg[SEG_CNT] {
		{&SEG_A_PORT, SEG_A},
		{&SEG_B_PORT, SEG_B},
		{&SEG_C_PORT, SEG_C},
		{&SEG_D_PORT, SEG_D},
		{&SEG_E_PORT, SEG_E},
		{&SEG_F_PORT, SEG_F},
		{&SEG_G_PORT, SEG_G},
		{&SEG_DP_PORT, SEG_DP}
	};


	static uint8_t actualDisplay;
	static uint8_t beforeDisplayed;
	static bool isElapsed;
	static bool isBlinkOn;


	static const uint8_t numbers[] PROGMEM = {
		0x3F,	// 0
		0x06,	// 1
		0x5B,	// 2
		0x4F,	// 3
		0x66,	// 4
		0x6D,	// 5
		0x7D,	// 6
		0x27,	// 7
		0x7F,	// 8
		0x6F,	// 9
		0x77,	// A
		0x2D,	// S
		0x40,   // Sleep Mode
		0x00	// clear
	};



		// If user change parameters before parameter symbol disappear
	static inline bool changedBeforeElapse(void){
		if(actualDisplay != SPEED_SYMBOL && actualDisplay != RANGE_SYMBOL && actualDisplay != SLEEP_SYMBOL){
			return true;
		}
		return false;
	}


	void dP_blink(bool isOn){
		isBlinkOn = isOn;
		//if(!isBlinkOn) seg[DP_INDEX].set();
	}

	void clear(void){
		print(D_LED_CLEAR);
	}


	uint8_t print(uint8_t number){

		uint8_t cnt = 0x1;
		actualDisplay = number;

		for(uint8_t i = 0; i < 7; i++){

			if((cnt & pgm_read_byte(&numbers[number]))){
				seg[i].set();
			}
			else{
				seg[i].reset();
			}
			cnt <<= 1;
		}
		return actualDisplay;
	}


	void displayOnTop(uint8_t c, uint16_t timeMs){
			beforeDisplayed = actualDisplay;
			print(c);
			Timers[dLedTim] = timeMs/10;
			isElapsed = false;
	}

	void EVENT(void){
		if(!Timers[dLedTim] && !isElapsed){
			if(!changedBeforeElapse()) print(beforeDisplayed);
			isElapsed = true;
		}
		if(!Timers[dLedDP_BlinkTim] && isBlinkOn){

			seg[DP_INDEX].toggle();

			Timers[dLedDP_BlinkTim] = 50;
		}

	}

}









