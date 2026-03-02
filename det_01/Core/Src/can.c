/*
 * can.c
 *
 *  Created on: Feb 26, 2026
 *      Author: 82109
 */

#include "can.h"
#include <stdio.h>


extern CAN_HandleTypeDef hcan;
extern uint8_t g_SystemMode;

#define PI_CONV_FACTOR 3.141592/180;


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t RxData[8];

    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK) {

        if(RxHeader.ExtId == 0x00000100) { // ECS

        	// start cmd receive
        	g_SystemMode = RxData[0];
        }
    }
}

uint8_t Send_Target_Coordinate(float target_x , float target_y)
{
    CAN_TxHeaderTypeDef TxHeader;
    uint32_t TxMailbox;
    uint8_t txData[8] = {0,}; // 8바이트 0으로 초기화 (패딩)
    TargetPayload_t payload;

    TxHeader.ExtId = 0x00000200;    // 0x00000100
    TxHeader.IDE = CAN_ID_EXT;
    TxHeader.RTR = CAN_RTR_DATA;
    TxHeader.DLC = 8; // 고정 8바이트
    TxHeader.TransmitGlobalTime = DISABLE;

    payload.targetPos.x_mm = target_x;
    payload.targetPos.y_mm = target_y;
    if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, payload.buffer, &TxMailbox) != HAL_OK) {
        	return 0;
        }

    return 1;
}

float test_x;
float test_y;
void Calculate_Target_Position(float distance_mm, float current_angle_deg)
{
	// 1.5m = 1500mm
	if(distance_mm < 1500 && distance_mm > 0)
	{
		float thera_rad = current_angle_deg * PI_CONV_FACTOR;

		float x = distance_mm * cos(thera_rad);
		float y = distance_mm * sin(thera_rad);
		test_x = x;
		test_y = y;

		printf("pos: %d, %d\n", x, y);
		Send_Target_Coordinate(x,y);
	}
}



void canInit()
{

	// can 필터 설정.
	CAN_FilterTypeDef sFilterConfig;
	uint32_t filter_id = 0x00000100;      // 내가 받고 싶은 ID
	uint32_t filter_mask = 0x1FFFFFFF;    // 29비트 모든 자리를 다 검사하겠다 (정확히 일치해야 함)

	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;


	sFilterConfig.FilterIdHigh = (uint16_t)((filter_id << 3) >> 16);
	sFilterConfig.FilterIdLow  = (uint16_t)((filter_id << 3) | CAN_ID_EXT);

	// 마스크도 ID와 똑같은 위치 규격을 가져야 함
	sFilterConfig.FilterMaskIdHigh = (uint16_t)((filter_mask << 3) >> 16);
	sFilterConfig.FilterMaskIdLow  = (uint16_t)((filter_mask << 3) | CAN_ID_EXT);


	sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	sFilterConfig.FilterActivation = ENABLE;



	// can 필터 설정 적용.
	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK) {
		Error_Handler();
	}

	// can 시작
	HAL_CAN_Start(&hcan);

	// 인터럽트 활성화
	HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);

}
