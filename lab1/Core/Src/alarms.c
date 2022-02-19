/*
 * alarms.c
 *
 *  Created on: 15 lut 2022
 *      Author: NASA
 */
#include "main.h"

#define FLASHTX_BUFF_SIZE 128

extern RTC_HandleTypeDef* hrtc;
extern RTC_AlarmTypeDef sAlarm;
extern I2C_HandleTypeDef hi2c1;


RTC_AlarmTypeDef AlarmB;
RTC_DateTypeDef currentDate;



extern uint16_t FlashTx_buff[FLASHTX_BUFF_SIZE];
extern uint16_t  hours[16];
extern uint16_t  months[16];
extern uint16_t  days[16];
extern uint16_t  minutes[16];
extern uint16_t  seconds[16];


RTC_DateTypeDef getDate(){

	HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);

}

//save all the contents without 255 to flash
void reinitAlarms(){











}


/* alarm a callback */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc){

	getDate();

	HAL_GPIO_WritePin(BRO_GPIO_Port, BRO_Pin, 1);

	handleGetAlarms();


	uint8_t earliestAlarm = parseAlarms(FlashTx_buff);

	uint8_t day = days[earliestAlarm];
	uint8_t month = months[earliestAlarm];
	uint8_t hour = hours[earliestAlarm];
	uint8_t minute = minutes[earliestAlarm];
	uint8_t second = seconds[earliestAlarm];



	sAlarm.Alarm = RTC_ALARM_A;
	sAlarm.AlarmTime.Hours = hour;
	sAlarm.AlarmTime.Minutes = minute;
	sAlarm.AlarmTime.Seconds = second;
	sAlarm.AlarmDateWeekDay = day;

	Flash_delete(day, month, hour, minute, second);





	Send("current day == %d\r\n day readen: %d", currentDate.Date, day);
	if(day == currentDate.Date && month == currentDate.Month){
		Send("next alarm found: %d %d %d %d %d \r\n ", day, month, hour, minute, second);

		HAL_RTC_SetAlarm_IT(hrtc, &sAlarm, RTC_FORMAT_BIN);


	}


	//set the next earliest alarm if date is correct





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


	HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
	AlarmB.AlarmTime.Hours = 00;
	AlarmB.AlarmTime.Minutes = 00;
	AlarmB.AlarmTime.Seconds = 0;
	AlarmB.AlarmDateWeekDay = currentDate.Date;
	AlarmB.Alarm = RTC_ALARM_B;





}



