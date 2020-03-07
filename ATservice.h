/*
 * ATservice.h
 *
 *  Created on: 7 mar 2020
 *      Author: norbe
 */

#ifndef ATSERVICE_H_
#define ATSERVICE_H_

#define SUCCESS 0
#define ERROR   -1

typedef int8_t atresult_t;

atresult_t at_service(uint8_t inout, char *params);
atresult_t ati_service(uint8_t inout, char *params);
atresult_t at_spd_service(uint8_t inout, char *params);
atresult_t at_dur_service(uint8_t inout, char *params);

#endif /* ATSERVICE_H_ */
