/*
 * dLed.h
 *
 *  Created on: 4 kwi 2020
 *      Author: norbe
 */

#ifndef D_LED_H_
#define D_LED_H_

//--------------------------------------------------------------------------
//						    LIBRARY SETTINGS
//--------------------------------------------------------------------------

#define COMMON_ANODE true
#define USE_SHIFT_REG false

#if USE_SHIFT_REG
#define MOSI_PORT PORTC
#define MOSI_DDR  DDRC
#define MOSI_PIN  5

#define SCK_PORT  PORTC
#define SCK_DDR   DDRC
#define SCK_PIN   4

#define LT_PORT   PORTC
#define LT_DDR    DDRC
#define LT_PIN    3


#define CLR_MOSI MOSI_PORT &= ~(1<<MOSI_PIN)
#define SET_MOSI MOSI_PORT |= (1<<MOSI_PIN)

#define CLR_SCK SCK_PORT &= ~(1<<SCK_PIN)
#define SET_SCK SCK_PORT |= (1<<SCK_PIN)

#define CLR_LT LT_PORT &= ~(1<<LT_PIN)
#define SET_LT LT_PORT |= (1<<LT_PIN)


#else
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

#endif


#define RANGE_SYMBOL 	10
#define SPEED_SYMBOL	11
#define SLEEP_SYMBOL	12
#define D_LED_CLEAR		13
//--------------------------------------------------------------------------



//--------------------------------------------------------------------------
//						    DECLARATIONS
//--------------------------------------------------------------------------

namespace dLED{

#if !USE_SHIFT_REG
	class LedSegment{
	public:
		LedSegment(volatile uint8_t *portx, uint8_t pin){
			PORTx = portx;
			Pin = pin;
			segmentsCnt++;
			*(PORTx-1) |= (1<<pin);
		}
#if COMMON_ANODE
		inline void set(void) {*PORTx &= ~(1<<Pin);};
		inline void reset(void) {*PORTx |= (1<<Pin);};
#else
		inline void set(void) {*PORTx |= (1<<Pin);};
		inline void reset(void) {*PORTx &= ~(1<<Pin);};

#endif
		inline void toggle(void) {*PORTx ^= (1<<Pin);};



		static uint8_t segmentsCnt;
	private:
		volatile uint8_t *PORTx;
		uint8_t Pin;
	};
#endif

	uint8_t print(uint8_t number);
	void dP_blink(bool isOn);
	void clear(void);
	void displayOnTop(uint8_t c, uint16_t timeMs);
	void EVENT(void);
#if USE_SHIFT_REG
	void initSpi(void);
#endif
}

//--------------------------------------------------------------------------
#endif /* D_LED_H_ */
