/*
 * helper.c
 *
 *  Created on: Aug 5, 2023
 *      Author: forma
 */

#include "helper.h"


extern TIME time;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;

void  Set_Time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year);
void  Get_Time (void);
float Get_Temp (void);
void  force_temp_conv (void);

uint8_t decToBcd(int val);
int bcdToDec(uint8_t val);

void send_uart(char *string);

DS3231DrvTypDef DS3231 = {
    .SetTime = Set_Time,
    .RetriveTime = Get_Time,
    .RetriveTemperature = Get_Temp,
    .TemperatureConversion_foreced = force_temp_conv
};

void Set_Time(uint8_t sec, uint8_t min, uint8_t hour, uint8_t dow, uint8_t dom, uint8_t month, uint8_t year)
{
	uint8_t set_time[7];
	set_time[0] = decToBcd(sec);
	set_time[1] = decToBcd(min);
	set_time[2] = decToBcd(hour);
	set_time[3] = decToBcd(dow);
	set_time[4] = decToBcd(dom);
	set_time[5] = decToBcd(month);
	set_time[6] = decToBcd(year);

	HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x00, 1, set_time, 7, 1000);
}

// A function to read the time from the DS3231
void Get_Time (void)
{
	uint8_t get_time[7];
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x00, 1, get_time, 7, 1000);
	time.seconds = bcdToDec(get_time[0]);
	time.minutes = bcdToDec(get_time[1]);
	time.hour = bcdToDec(get_time[2]);
	time.dayofweek = bcdToDec(get_time[3]);
	time.dayofmonth = bcdToDec(get_time[4]);
	time.month = bcdToDec(get_time[5]);
	time.year = bcdToDec(get_time[6]);
}

// A function to read the temperature from the DS3231
float Get_Temp (void)
{
	uint8_t temp[2];
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x11, 1, temp, 2, 1000);
	return ((temp[0])+(temp[1]>>6)/4.0);
}
void force_temp_conv (void)
{
	uint8_t status=0;
	uint8_t control=0;
	HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0F, 1, &status, 1, 100);  // read status register
	if (!(status&0x04))  // if the BSY bit is not set
	{
		HAL_I2C_Mem_Read(&hi2c1, DS3231_ADDRESS, 0x0E, 1, &control, 1, 100);  // read control register
		HAL_I2C_Mem_Write(&hi2c1, DS3231_ADDRESS, 0x0E, 1, (uint8_t *)(control|(0x20)), 1, 100);  // write modified control register with CONV bit as'1'
	}
}


uint8_t decToBcd(int val)
{
  return (uint8_t)( (val/10*16) + (val%10) );
}
int bcdToDec(uint8_t val)
{
  return (int)( (val/16*10) + (val%16) );
}

// A function to send a string over UART
void send_uart(char *string)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), HAL_MAX_DELAY);  // send message via UART
}
