/*
 * alarms.c
 *
 *  Created on: 15 lut 2022
 *      Author: NASA
 */
#include "main.h"

extern RTC_HandleTypeDef hrtc;
extern RTC_AlarmTypeDef sAlarm;
RTC_AlarmTypeDef AlarmB;
extern uint16_t FlashTx_buff[128];

/* alarm a callback */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){
	HAL_GPIO_WritePin(BRO_GPIO_Port, BRO_Pin, 1);
	Flash_read();


	for(int i = 0 ; i < 128; i++){
		uint8_t byte = FlashTx_buff[i];

	}




	//
}
void HAL_RTC_AlarmBEventCallback(RTC_HandleTypeDef *hrtc){
	//check for existing alarm


}

void Alarms_init(){
	RTC_DateTypeDef currentDate;
	if(HAL_RTCEx_BKUPRead(&hrtc, RTC_BKP_DR2) != 0x32f3){
		sAlarm.AlarmTime.Hours = 23;
		sAlarm.AlarmTime.Minutes = 59;
		sAlarm.AlarmTime.Seconds = 0;
		sAlarm.AlarmDateWeekDay = 30;
		sAlarm.Alarm = RTC_ALARM_A;

		 if(HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK){
						 		Error_Handler();
						 		sendFail(4);
		};
		HAL_RTCEx_BKUPWrite(&hrtc, RTC_BKP_DR2, 0x32f3);
	}

	HAL_RTC_GetDate(&hrtc, currentDate, RTC_FORMAT_BIN);
	AlarmB.AlarmTime.Hours = 00;
	AlarmB.AlarmTime.Minutes = 00;
	AlarmB.AlarmTime.Seconds = 0;
	AlarmB.AlarmDateWeekDay = currentDate.Date;
	AlarmB.Alarm = RTC_ALARM_B;





}



