/*
 * handlers.c
 *
 *  Created on: 26 sty 2022
 *      Author: NASA
 */
#include "main.h"
#include "clock.h"
#include <math.h>



extern  uint8_t clock_mode;
extern uint8_t hour_to_show, minute_to_show, second_to_show;
extern uint8_t hour_displayed;

extern RTC_AlarmTypeDef sAlarm;
extern RTC_DateTypeDef sDate;
extern RTC_HandleTypeDef* hrtc;
extern uint16_t FlashTx_buff[128];
/* ===== HANDLERS FOR SPECIFIC COMMANDS ====== */



//handler for setMode

void handleSetClockMode(){

		int mode = parseIntData();

		if(mode == 1 || mode == 2){

			clock_mode = mode;

			if(mode == 2){
				resetPins();
				displayHour(hour_to_show, minute_to_show, second_to_show);
			}

			Send("$Success=1#\r\n");

		}else {
				sendFail(4);
		}


}


//handler for showHour
void handleShowHour(uint8_t  hour, uint8_t  minute, uint8_t second){
	resetPins();

		if((hour > 0 && hour < 23) && (minute >= 0 && minute <= 59) && (second >= 0 && second <= 59)){

			 hour_displayed = 0;

			} else {
				sendFail(4);
	        }


}



/* ===== alrams ==== */

//compare alarms that are set
/**
 * @returns earlier alarm in form of 1/2
 */
uint8_t compareAlarms(RTC_AlarmTypeDef alarmA, RTC_AlarmTypeDef alarmB){
			//todo
			//add month checking
			uint8_t alarmA_day = alarmA.AlarmDateWeekDay;
			uint8_t alarmA_hours = alarmA.AlarmTime.Hours;
			uint8_t alarmA_minutes = alarmA.AlarmTime.Minutes;
			uint8_t alarmA_seconds = alarmA.AlarmTime.Seconds;

			uint8_t alarmB_day = alarmB.AlarmDateWeekDay;
			uint8_t alarmB_hours = alarmB.AlarmTime.Hours;
			uint8_t alarmB_minutes = alarmB.AlarmTime.Minutes;
			uint8_t alarmB_seconds = alarmB.AlarmTime.Seconds;

			if(alarmA_day == alarmB_day){ //check if day is same if yes - check if hour is the same

				if(alarmA_hours == alarmB_hours){

					if(alarmA_minutes == alarmB_minutes){


						if(alarmA_seconds == alarmB_seconds){
							return 0;
						} else {
							if(alarmA_seconds < alarmB_seconds){
								return 1;
							} else {
								return 2;
							}
						}



					}else {

						if(alarmA_minutes < alarmB_minutes){
							return 1;
						} else {
							return 2;
						}
					}

				} else {

					if(alarmA_hours < alarmB_hours){

						return 1;
					} else {
						return 2;
					}

				}
			} else {
				if(alarmA_day < alarmB_day){
					return 1;
				} else {
					return 2;
				}

			}


}


/**
 * @param INT {alarm} 1 for alarm A 2 for alarmB
 */
RTC_AlarmTypeDef getAlarm(uint8_t whichAlarm){
	RTC_AlarmTypeDef alarm;

	switch(whichAlarm){
	case 1:
		HAL_RTC_GetAlarm(&hrtc, &alarm, RTC_ALARM_A, RTC_FORMAT_BIN);
		break;
	case 2:
		HAL_RTC_GetAlarm(&hrtc, &alarm, RTC_ALARM_B, RTC_FORMAT_BIN);
		break;
	}



	return alarm;




}
RTC_DateTypeDef getDate(){
	RTC_DateTypeDef date;
	HAL_RTC_GetDate(&hrtc, &date, RTC_FORMAT_BIN);
	return date;
}

uint8_t getDay(){
	return  getDate().Date;

}
uint8_t getWeekDay(){
	return getDate().WeekDay;
}

//handler for setAlarm
void handleSetAlarm(RTC_DateTypeDef sDate, uint8_t day, uint8_t month, uint16_t year, uint8_t hour, uint8_t minute, uint8_t second){

	//w backupie przy programowaniu ustawiać wartość zmiennej ustawić w obsłudze RTC
	//Nie ma mozliwosci ustawiania alarmow na kolejny dzień więc trzeba ustawiać ALARM B na północ alarm
	//ktory sprawdza czy juz jest ten dzień
	//




	//i sprawdzac czy juz jest ten dzien i zaprogramowac alarm na najblizszy dzien

	uint8_t max_days = getNumOfDaysInMonth(month); //get maximum days in a month

	//fill array to encode it and save to flash
	uint8_t alarm_to_set[7];
	uint8_t alarm_to_backup[7];

	RTC_DateTypeDef currentDate ;
	HAL_RTC_GetDate(&hrtc, &currentDate, RTC_FORMAT_BIN);
Send("Current date: %d/%d", currentDate.Month, currentDate.Date);


	if(year >= 22 && (day > 0 && day <= max_days) && (month > 0 && month <= 12)){
		if(currentDate.Month <= month && currentDate.Date <= day){
			Send("Past date given");
		}


			if((hour > 0 && hour <= 23) && (minute >= 0 && minute <= 59) && (second >= 0 && second <= 59)){

				alarm_to_set[0] = day;
				alarm_to_set[1] = month;
				alarm_to_set[2] = year;
				alarm_to_set[3] = hour;
				alarm_to_set[4] = minute;
				alarm_to_set[5] = second;
				alarm_to_set[6] = 0;

			Send("$Success=1 %d#\r\n");



			sAlarm.AlarmTime.Hours = hour;
			sAlarm.AlarmTime.Minutes = minute;
			sAlarm.AlarmTime.Seconds = second;
			sAlarm.AlarmDateWeekDay = day;



			int start_idx = Flash_getFreeSpace() - 1;


			Send("Start idx: %d\r\n", start_idx);

			//Flash_write(alarm_to_set, start_idx);

			//=========get alarms from eeprom to check whether to set new alarm or not===========




			//get two already defined alarms
			RTC_AlarmTypeDef alarmA = getAlarm((uint8_t)1);

					alarm_to_backup[0] = alarmA.AlarmDateWeekDay;
					alarm_to_backup[1] = 02;
					alarm_to_backup[2] = year;
					alarm_to_backup[3] = alarmA.AlarmTime.Hours;
					alarm_to_backup[4] = alarmA.AlarmTime.Minutes;
					alarm_to_backup[5] = alarmA.AlarmTime.Seconds;
					alarm_to_backup[6] = 0;


//			Send("AlarmA: %d/%d:%d\r\n AlarmB: %d/%d:%d\r\n", alarmA.AlarmDateWeekDay, alarmA.AlarmTime.Hours, alarmA.AlarmTime.Minutes,
//					alarmB.AlarmDateWeekDay, alarmB.AlarmTime.Hours, alarmB.AlarmTime.Minutes);


			//check which of the two is earlier
			uint8_t earlierAlarm = compareAlarms(alarmA, sAlarm);

			//if alarm is earlier set that alarm set that alarm
			if(earlierAlarm == 2){
				 sAlarm.Alarm = RTC_ALARM_A;
				 Flash_write(alarm_to_backup, start_idx);

				 while(HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK){
						Error_Handler();
						sendFail(4);
				 };

				 HAL_Delay(5);

			} else {

				Flash_write(alarm_to_set, start_idx);

			}


//			sAlarm.Alarm = RTC_ALARM_A;
//			while(HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK){
//							 		Error_Handler();
//							 		sendFail(4);
//							};






			handleGetAlarms();

			} else { //send fail if data not correct
				sendFail(2);
			}


		} else {
			sendFail(2);
		}
}


void handleResetAlarms(){

	Flash_flush();
	FLASH_init();
	Alarms_init();
	Send("$Success=1#\r\n");



}



void handleGetAlarms(){

	Flash_read();


	RTC_AlarmTypeDef alarmA = getAlarm((uint8_t)1);
	RTC_AlarmTypeDef alarmB = getAlarm((uint8_t)2);


		Send("$Success={AlarmA: %d/%d:%d:%d  AlarmB: %d/%d:%d:%d}#\r\n", alarmA.AlarmDateWeekDay, alarmA.AlarmTime.Hours, alarmA.AlarmTime.Minutes, alarmA.AlarmTime.Seconds,
							alarmB.AlarmDateWeekDay, alarmB.AlarmTime.Hours, alarmB.AlarmTime.Minutes, alarmA.AlarmTime.Seconds);

		parseAlarms(FlashTx_buff);


}
