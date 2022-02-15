/*
 * alarms.c
 *
 *  Created on: 15 lut 2022
 *      Author: NASA
 */
#include "main.h"




/* alarm a callback */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
	HAL_GPIO_WritePin(BRO_GPIO_Port, BRO_Pin, 1);
	//
}




