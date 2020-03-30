/*
 * common.h
 *
 *  Created on: 13 mar 2020
 *      Author: norbe
 */

#ifndef COMMON_H_
#define COMMON_H_


#include "MicroSwitch/MicroSwitch.h"

#define DBG_LED_DIR_OUT() DDRD |= (1<<PD7)
#define DBG_LED_ON() PORTD |= (1<<PD7)
#define DBG_LED_OFF() PORTD &= ~(1<<PD7)
#define DBG_LED_TOG() PORTD ^= (1<<PD7)


#define UP_BUTTON_INPUT		PIND
#define UP_BUTTON_PIN		3

#define DOWN_BUTTON_INPUT 	PIND
#define DOWN_BUTTON_PIN		4

#define SWITCH_BUTTON_INPUT PIND
#define SWITCH_BUTTON_PIN	5

#define RESET_BUTTON_INPUT  PIND
#define RESET_BUTTON_PIN	6



#define TIMERS_CNT 4


//--------------------------------------------------------------------------
//					DECLARATIONS OF FUNCTIONS AND VARIABLES
//--------------------------------------------------------------------------

extern MicroSwitch ButtonUp;
extern MicroSwitch ButtonDown;

extern MicroSwitch ButtonSwitch;
extern MicroSwitch ButtonReset;


typedef enum {testLed = 0, cradleDownCnt = 1, buttonsTimeBase = 2, timeToSleep = 3}timNames_t;
extern volatile uint32_t Timers[TIMERS_CNT];
void periphInit(void);
void reset(void);
void sleep(void);
#endif /* COMMON_H_ */
