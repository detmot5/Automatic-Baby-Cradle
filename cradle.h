/*
 * cradle.h
 *
 *  Created on: 27 lut 2020
 *      Author: norbe
 */

#ifndef CRADLE_H_
#define CRADLE_H_

//--------------------------------------------------------------------------
//						    PROGRAMM SETTINGS
//--------------------------------------------------------------------------

#define _SERVO_MIN						500						// Min value of PWM duration
#define _SERVO_MAX						2500					// Max value of PWM duration

#define _SERVO_MIN_DELAY				1						// Delay between each servo step
#define _SERVO_MAX_DELAY				7

#define SERVO_OUT DDRB |= (1<<PB1);								// OCR1 pin out (atmega88)

#define servoWrite(_dur)	OCR1A = usToTicks(_dur)

//--------------------------------------------------------------------------
//						 SERVO CONTROL DEFINES
//--------------------------------------------------------------------------

#define _SERVO_NEUTRAL 					(_SERVO_MIN + _SERVO_MAX)/2

#define _TICKS_PER_US					F_CPU/1000000UL
#define _SERVO_REFRESH_INTERVAL 		19999

#define usToTicks(_us)  ((_TICKS_PER_US * _us)/8)



typedef struct {
	uint8_t speed;
	uint16_t duration;
}svParams_t;



//--------------------------------------------------------------------------
//					DECLARATIONS OF FUNCTIONS AND VARIABLES
//--------------------------------------------------------------------------

extern volatile uint16_t Timer1,Timer2,Timer3;

void cradleInit(void);
int8_t CRADLE_EVENT(void);


#endif /* CRADLE_H_ */
