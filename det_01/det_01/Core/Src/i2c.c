/*
 * i2c.c
 *
 *  Created on: Mar 2, 2026
 *      Author: 82109
 */


#include "i2c.h"

#define ALPHA_WEIGHT 0.7
float prev_filtered_distance;
uint16_t distance = 0;
float Read_ToF_Sensor_Data()
{



	VL53L1__Init();
	VL53L1X_StartRanging(VL53L1__ADDR);
	VL53L1X_GetDistance(VL53L1__ADDR, &distance);

	//uint16_t ret = distance;
	//printf("%d\n",ret);
	float ret = Apply_EMA_Filter(distance);
    //printf("%0.2f\n",ret);
	//fflush(stdout);  // 표준 출력 버퍼를 즉시 비워서 UART로 보냄
	return ret;
}

float Apply_EMA_Filter( uint16_t raw_distance_mm )
{

	float Xn = (float)raw_distance_mm;
	float Yn = ALPHA_WEIGHT * Xn + (1-ALPHA_WEIGHT)* prev_filtered_distance;

	prev_filtered_distance = Yn;

	float ret = Yn;

	return ret;
}


void IrInit()
{
	VL53L1X_SetI2CAddress(VL53L1__ADDR, VL53L1__ADDR);
	VL53L1__Init();
	VL53L1X_StartRanging(VL53L1__ADDR);
	HAL_Delay(100);

}
