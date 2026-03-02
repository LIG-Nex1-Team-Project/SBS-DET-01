/*
 * can.c
 *
 *  Created on: Feb 26, 2026
 *      Author: 82109
 */

#ifndef INC_CAN_C_
#define INC_CAN_C_

#include "main.h"


void canInit();
uint8_t Send_Target_Coordinate(float target_x , float target_y);
void Calculate_Target_Position(float distance_mm,float current_angle_deg);

typedef union {
    uint8_t buffer[8];
    struct {
        float x_mm; // 4 Byte
        float y_mm; // 4 Byte
    } targetPos;
} TargetPayload_t;



#endif /* INC_CAN_C_ */
