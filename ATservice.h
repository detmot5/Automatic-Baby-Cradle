/*
 * ATservice.h
 *
 *  Created on: 7 Mar 2020
 *      Author: Norbert Bielak
 */

#ifndef ATSERVICE_H_
#define ATSERVICE_H_

#define SUCCESS 0
#define ERROR   -1

typedef int8_t atresult_t;

atresult_t at_service(uint8_t inout, char *params);
atresult_t ati_service(uint8_t inout, char *params);
atresult_t at_spd_service(uint8_t inout, char *params);			// Cradle speed control
atresult_t at_dur_service(uint8_t inout, char *params);			// Cradle angle control
atresult_t at_stop_service(uint8_t inout, char* params);		// Stop cradle in any time
atresult_t at_fac_service(uint8_t inout, char *params);			// Restore factory settings
atresult_t at_rst_service(uint8_t inout, char *params);			// Reset device
atresult_t at_tim_service(uint8_t inout, char *params);			// Turn off device after a specified time

#endif /* ATSERVICE_H_ */
