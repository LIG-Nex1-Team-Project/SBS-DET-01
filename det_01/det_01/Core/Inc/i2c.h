/*
 * i2c.h
 *
 *  Created on: Mar 2, 2026
 *      Author: 82109
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "main.h"
#include "vl53l1_platform.h"

float Read_ToF_Sensor_Data();
float Apply_EMA_Filter( uint16_t raw_distance_mm );

void IrInit();


#endif /* INC_I2C_H_ */
