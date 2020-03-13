/*
 * common.h
 *
 *  Created on: 13 mar 2020
 *      Author: norbe
 */

#ifndef COMMON_H_
#define COMMON_H_

#define TIMERS_CNT 4


//--------------------------------------------------------------------------
//					DECLARATIONS OF FUNCTIONS AND VARIABLES
//--------------------------------------------------------------------------
typedef enum {testLed = 0, cradleDownCnt = 1}timNames_t;
extern volatile uint16_t Timers[TIMERS_CNT];
void softTimInit(void);

#endif /* COMMON_H_ */
