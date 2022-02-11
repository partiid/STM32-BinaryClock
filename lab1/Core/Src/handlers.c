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
//function to parse alarms downloaded from flash

void compareAlarm(int alarms[], int alarm_to_set[]){

//					alarm_to_set[0] = day;
//					alarm_to_set[1] = month;
//					alarm_to_set[2] = year;
//					alarm_to_set[3] = hour;
//					alarm_to_set[4] = minute;


	uint8_t day = alarm_to_set[0];
	uint8_t month = alarm_to_set[1];



	uint8_t alarms_size = sizeof(alarms) / sizeof(alarms[0]);

	uint8_t day_start_idx = 0;
	uint8_t month_start_idx = 0;
	uint8_t year_start_idx = 0;
	uint8_t hour_start_idx = 0;



	for(int i = 0; i < alarms_size; i++){
		//
		if(i == 0 && alarms[i] != 0){
			day_start_idx = i;

		}




		if(alarms[i] != 0){

		}
	}

	//check day - if day is eq or gt check next
	//inc by 5 to check only days
	for(int i = day_start_idx; i < alarms_size; i+=5){
		if(day >= alarms[i] && alarms[i] != 255){

		}
	}






}

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

//handler for setAlarm
void handleSetAlarm(RTC_DateTypeDef sDate, uint8_t day, uint8_t month, uint16_t year, uint8_t hour, uint8_t minute, uint8_t second){

	uint8_t max_days = getNumOfDaysInMonth(month); //get maximum days in a month

	//fill array to encode it and save to flash
	int alarm_to_set[6];




	//int encoded = encodeDateTime(to_encode, 5);
	//Send("Encoded: %d\r\n", encoded);




	if(year >= 22 && (day > 0 && day <= max_days) && (month > 0 && month <= 12)){

		if((hour > 0 && hour < 23) && (minute >= 0 && minute <= 59) && (second >= 0 && second <= 59)){
				alarm_to_set[0] = day;
				alarm_to_set[1] = month;
				alarm_to_set[2] = year;
				alarm_to_set[3] = hour;
				alarm_to_set[4] = minute;
				alarm_to_set[5] = second;

			Send("$Success=1#\r\n");



			sAlarm.AlarmTime.Hours = hour;
			sAlarm.AlarmTime.Minutes = minute;
			sAlarm.AlarmTime.Seconds = second;
			sAlarm.AlarmDateWeekDay = day;


			int start_idx = Flash_getFreeSpace() - 1;

			Send("Start idx: %d\r\n", start_idx);

			//Flash_write(alarm_to_set, start_idx);

			//=========get alarms from eeprom to check whether to set new alarm or not===========

			int *alarms = Flash_read();


			//get two already defined alarms
			RTC_AlarmTypeDef alarmA = getAlarm((uint8_t)1);
			RTC_AlarmTypeDef alarmB = getAlarm((uint8_t)2);

//			Send("AlarmA: %d/%d:%d\r\n AlarmB: %d/%d:%d\r\n", alarmA.AlarmDateWeekDay, alarmA.AlarmTime.Hours, alarmA.AlarmTime.Minutes,
//					alarmB.AlarmDateWeekDay, alarmB.AlarmTime.Hours, alarmB.AlarmTime.Minutes);


			//check which of the two is earlier
			uint8_t earlierAlarm = compareAlarms(alarmA, alarmB);

			//if alarm is earlier set that alarm instead and set the other alarm to oposite of next alarm


			//check alarm to set with earlier alarm
			uint8_t alarmToRecheck = compareAlarms(getAlarm(earlierAlarm)/* wczesniejszy z dwoch zdefiniowanych*/ , sAlarm); //will return 1 if earlier is earlier



			//if alarm to set is earlier than earlier alarm - zastąp
			if(alarmToRecheck == 2){

				//set earlier alarm to alarm b - SET ALARM A TO BE B
				RTC_AlarmTypeDef swapAlarmAtoB = getAlarm(earlierAlarm);
				swapAlarmAtoB.Alarm = RTC_ALARM_B;

				HAL_RTC_SetAlarm_IT(&hrtc, &swapAlarmAtoB, RTC_FORMAT_BIN);

				//set alarm to set to primary ALARM
				sAlarm.Alarm = RTC_ALARM_A;


			} else {	//else check second  // later alarm
				if(compareAlarms(getAlarm(1), sAlarm) == 2){
					sAlarm.Alarm = RTC_ALARM_A;



				} else {
					//compare if alarm to set is later than alarm B
					if(compareAlarms(getAlarm(2), sAlarm) == 1){
						//if alarm is later than alarm B, store it in flash
						Flash_write(alarm_to_set, start_idx);
					}

					sAlarm.Alarm = RTC_ALARM_B;				// jesli jest wczesniejszy to 1 przechodzi na 2 a on jest 1
					                                        //sprawdzam z najwczesniejszym - jesli nie jest to sprawdzam z drugim

				}


			}



			while(HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK){
				Error_Handler();
				sendFail(4);
			};

			HAL_Delay(5);


//			alarmA = getAlarm((uint8_t)1);
//			alarmB = getAlarm((uint8_t)2);
//
//			Send("\r\nAlarmA: %d/%d:%d\r\n AlarmB: %d/%d:%d\r\n", alarmA.AlarmDateWeekDay, alarmA.AlarmTime.Hours, alarmA.AlarmTime.Minutes,
//				alarmB.AlarmDateWeekDay, alarmB.AlarmTime.Hours, alarmB.AlarmTime.Minutes);

			//get alarms to display it
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
	Send("$Success=1#\r\n");
}



void handleGetAlarms(){

	RTC_AlarmTypeDef alarmA = getAlarm((uint8_t)1);
	RTC_AlarmTypeDef alarmB = getAlarm((uint8_t)2);


		Send("$Success={AlarmA: %d/%d:%d:%d  AlarmB: %d/%d:%d:%d}#\r\n", alarmA.AlarmDateWeekDay, alarmA.AlarmTime.Hours, alarmA.AlarmTime.Minutes, alarmA.AlarmTime.Seconds,
							alarmB.AlarmDateWeekDay, alarmB.AlarmTime.Hours, alarmB.AlarmTime.Minutes, alarmA.AlarmTime.Seconds);




}
