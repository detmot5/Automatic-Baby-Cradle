/*
 * dLed.h
 *
 *  Created on: 4 kwi 2020
 *      Author: norbe
 */

#ifndef D_LED_H_
#define D_LED_H_

#define SEG_A_PORT 	PORTB
#define SEG_A		0

#define SEG_B_PORT 	PORTB
#define SEG_B		2

#define SEG_C_PORT 	PORTC
#define SEG_C		4

#define SEG_D_PORT 	PORTC
#define SEG_D		3

#define SEG_E_PORT 	PORTC
#define SEG_E		2

#define SEG_F_PORT 	PORTC
#define SEG_F		1

#define SEG_G_PORT 	PORTC
#define SEG_G		0


#define SEG_DP_PORT PORTC
#define SEG_DP		5


#define RANGE_SYMBOL 	10
#define SPEED_SYMBOL	11
#define SLEEP_SYMBOL	12
#define D_LED_CLEAR		13

namespace dLED{


	class LedSegment{
	public:
		LedSegment(volatile uint8_t *portx, uint8_t pin){
			PORTx = portx;
			Pin = pin;
			segmentsCnt++;
			*(PORTx-1) |= (1<<pin);
		}
		inline void set(void) {*PORTx |= (1<<Pin);};
		inline void reset(void) {*PORTx &= ~(1<<Pin);};
		inline void toggle(void) {*PORTx ^= (1<<Pin);};


		enum segment{A=0,B=1,C=2,D=3,E=4,F=5,G=6};
		static uint8_t segmentsCnt;
	private:
		volatile uint8_t *PORTx;
		uint8_t Pin;
	};

	void print(uint8_t number);
	void displayOnTop(uint8_t c, uint16_t timeMs);
	void EVENT(void);
	void dP_blink(bool isOn);
}
#endif /* D_LED_H_ */
