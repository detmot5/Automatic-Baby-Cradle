/*
 * eeprom.h
 *
 *  Created on: 7 mar 2020
 *      Author: norbe
 */

#ifndef EEPROM_H_
#define EEPROM_H_

//--------------------------------------------------------------------------
//		Simple functions to read/write servo parameters from eeprom
//--------------------------------------------------------------------------

void eeprom_update_speed(void);
void eeprom_update_duration(void);
void eeprom_save_actual_pos(void);
void eeprom_save_stop_flag(void);
void eeprom_read_speed(void);
void eeprom_read_duration(void);
void eeprom_read_actual_pos(void);
void eeprom_read_stop_flag(void);

#endif /* EEPROM_H_ */
