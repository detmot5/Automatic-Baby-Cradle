/*
 * servohw.h
 *
 *  Created on: 14 gru 2019
 *      Author: norbe
 */

#ifndef SERVOHW_H_
#define SERVOHW_H_

#ifdef __cplusplus
extern "C"{
#endif

//--------------------------------------------------------------------------
//						    LIBRARY SETTINGS
//--------------------------------------------------------------------------

#define _SERVO_MIN						800						// Min value of PWM duration
#define _SERVO_MAX						2200					// Max value of PWM duration

#define SERVO_FRONT_OUT DDRB |= (1<<PB1);						// OCR1 pins out (atmega32)
#define SERVO_REAR_OUT  DDRD |= (1<<PD4);


//--------------------------------------------------------------------------
//						 SERVO CONTROL DEFINES
//--------------------------------------------------------------------------

#define _SERVO_NEUTRAL 					(_SERVO_MIN + _SERVO_MAX)/2

#define _TICKS_PER_US					F_CPU/1000000UL
#define _SERVO_REFRESH_INTERVAL 		19999

#define usToTicks(_us)  ((_TICKS_PER_US * _us)/8)



//--------------------------------------------------------------------------
// 					 DECLARATIONS OF FUNCTIONS AND VARIABLES
//--------------------------------------------------------------------------




void servoHwInit(void);
void servoHwWrite(uint16_t duration);

#ifdef __cplusplus
}
#endif
#endif /* SERVOHW_H_ */
