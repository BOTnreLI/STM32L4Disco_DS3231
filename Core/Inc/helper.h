/*
 * helper.h
 *
 *  Created on: Aug 5, 2023
 *      Author: forma
 */

/*
 * I will try to implement a
 *
 */
#ifndef INC_HELPER_H_
#define INC_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "string.h"
#include "stdint.h"

uint8_t decToBcd(int val);
int     bcdToDec(uint8_t val);
void  send_uart(char *string);

typedef struct{
	void  (*SetTime)(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);
	void  (*RetriveTime)(void);
	float (*RetriveTemperature)(void);
	void  (*TemperatureConversion_foreced)(void);
}
DS3231DrvTypDef;

typedef struct{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hour;
	uint8_t dayofweek;
	uint8_t dayofmonth;
	uint8_t month;
	uint8_t year;
}TIME;

typedef enum{
	ds3231_ok = 0,
	ds3231_error = 1,
	ds3231_timeout = 2
}
DS3231_StatusTypDef;


/*
 * According to manual, the 7-bit addresses for RTC(real time clock) module are 0x68.
 * To read, it start from 0xD1
 * To write, it start from 0xD0
 * I'll uses 0xD0 to be my base for initializing the module, and subtracted by 1 to read the module
 */
#define DS3231_ADDRESS 0xD0
#endif /* INC_HELPER_H_ */
