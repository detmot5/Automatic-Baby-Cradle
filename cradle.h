/*
 *
 * cradle.h
 *
 *  Created on: 27 Feb 2020
 *      Author: Norbert Bielak
 */

#ifndef CRADLE_H_
#define CRADLE_H_

//--------------------------------------------------------------------------
//						    PROGRAMM SETTINGS
//--------------------------------------------------------------------------

#define _SERVO_MIN						800						// Min value of PWM duration
#define _SERVO_MAX						2200					// Max value of PWM duration

#define _SERVO_MIN_DELAY				1						// Delay between each servo step
#define _SERVO_MAX_DELAY				9

#define _DEFAULT_TIME_TO_SLEEP 			5

#define SERVO_OUT() DDRB |= (1<<PB1)								// OCR1 pin out (atmega88)

#define servoWrite(_dur)	OCR1A = usToTicks(_dur)

//--------------------------------------------------------------------------
//						 SERVO CONTROL DEFINES
//--------------------------------------------------------------------------

#define _SERVO_NEUTRAL 					(_SERVO_MIN + _SERVO_MAX)/2

#define _TICKS_PER_US					F_CPU/1000000UL
#define _SERVO_REFRESH_INTERVAL 		19999

#define usToTicks(_us)  ((_TICKS_PER_US * _us)/8)


	// "duration" is the value of servo angle
	// as a PWM period time
typedef struct {
	uint8_t speed;
	uint16_t duration;
	uint16_t actualPos;
	uint16_t secondsToEnterSleep;
}svParams_t;

	// "range" is value of duration mapped to 1 - 9 range
	// to easier control
typedef enum {speed,range} svParamsEnum_t;

//--------------------------------------------------------------------------
//					DECLARATIONS OF FUNCTIONS AND VARIABLES
//--------------------------------------------------------------------------


extern svParams_t servoParams_eep;
extern svParams_t servoParams;

extern bool stopFlag;		// 0 - run  1 - stop
extern bool isCradleTimActive;

void cradleInit(void);
int8_t CRADLE_EVENT(void);

int8_t cradleSetParams(svParamsEnum_t cradleParam, uint8_t value);
uint8_t cradleGetParams(svParamsEnum_t cradleParam);


void stop(bool isEnabled);
#endif /* CRADLE_H_ */
