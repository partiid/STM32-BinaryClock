/*
 * flash.c
 *
 *  Created on: 6 sty 2022
 *      Author: NASA
 */
//read from flash memory
//sectors are
#include "main.h"
#define COUNTER_ADDR 0x01
#define FLASHTX_BUFF_SIZE 128

int counter = 0x00; //counter of the current top address
extern I2C_HandleTypeDef hi2c1;

uint16_t FlashTx_buff[FLASHTX_BUFF_SIZE];


uint8_t FlashTx_empty = 0; //tail //read
uint8_t FlashTx_busy = 0; //head //write


/* ===== FLASH INIT ===== */
uint16_t search_word[12];


void FLASH_init(){


	FlashTx_empty = 0;
	FlashTx_busy = 0;
	memset(FlashTx_buff, 255, FLASHTX_BUFF_SIZE);
}

void FLASH_reinit(){

}



void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef *hi2c) {

//	if(FlashTx_busy != FlashTx_empty){
//		++counter;
//			uint8_t temp = FlashTx_buff[FlashTx_empty];
//
//			FlashTx_empty++;
//			Send("Temp: %d", temp);
//			Send("COunter at: %d\r\n", counter);
//			if(FlashTx_empty >= FLASHTX_BUFF_SIZE){
//				FlashTx_empty = 0;
//			}
// 	HAL_I2C_Mem_Write_IT(&hi2c1, 0xa0, counter, 1, (uint8_t*)&temp, sizeof(temp));
//
//			HAL_Delay(5);
//
//		}

}





void Flash_flush(){
	uint8_t zero = 0xff;
	for(int i = 0; i <= FLASHTX_BUFF_SIZE; i++){
		HAL_I2C_Mem_Write_IT(&hi2c1, 0xa0, i, 1 , (uint8_t*)&zero, sizeof(zero));
		HAL_Delay(5);
	}
}



uint8_t bt = 0;

void Flash_write(uint8_t data[], int start_idx){

		counter = start_idx;



		uint8_t arr_size = 6;

		//control memory
		if(counter >= FLASHTX_BUFF_SIZE - arr_size - 1){
			counter = 0;
		}


		Send("Counter at: %d\r\n", counter);

		for(int i = 0; i < arr_size ; i++){

			HAL_I2C_Mem_Write_IT(&hi2c1, 0xa0, counter, 1 , (uint8_t*)&data[i], sizeof(data[i]));
			//FlashTx_buff[FlashTx_busy++] = data[i];

			counter++;
			HAL_Delay(5);
		}

}


void Flash_read(){
	FLASH_init();

	uint8_t byte = 0x00;
	uint8_t data_found = 0;


	uint8_t page = 0;
	for(page = 0; page <= 128; page++){
		HAL_I2C_Mem_Read(&hi2c1, 0xa1, page, 1, (uint8_t*)&byte, sizeof(byte), HAL_MAX_DELAY);

		//if byte is found then start downloading bytes to buffer
		if(byte != 0x00 ){
			//FlashTx_buff[Tx_busy++] = byte;
			data_found = 1;

		}

		if(data_found == 1){
				FlashTx_buff[FlashTx_busy++] = byte;
				if(FlashTx_busy >= FLASHTX_BUFF_SIZE){
					FlashTx_busy = 0;
				}
				//if too many zeros found stop downlaoding to save only data
				if(byte == 0x00 ){
					data_found = 0;
				}
		}

}



}


void Flash_delete(uint8_t day, uint8_t month, uint8_t hour, uint8_t minute, uint8_t second){


	uint8_t byte = 0x00;
	uint8_t data_found = 0;



	for(int i = 0; i <= 128; i++){


		if(FlashTx_buff[i] == second && FlashTx_buff[i - 1] == minute && FlashTx_buff[i - 2] == hour && FlashTx_buff[i - 3] == 22 && FlashTx_buff[i - 4] == month && FlashTx_buff[i - 5 ] == day){

			FlashTx_buff[i] = 255;
			FlashTx_buff[i - 1] = 255;
			FlashTx_buff[i - 2] = 255;
			FlashTx_buff[i - 3] = 255;
			FlashTx_buff[i - 4] = 255;
			FlashTx_buff[i - 5] = 255;


		}


	}




}


// function to get first free address to write to
int Flash_getFreeSpace(){
	uint8_t byte = 0x00;
	int i = 0;

	while(byte != 0xff){
		HAL_I2C_Mem_Read(&hi2c1, 0xa1, i, 1, (uint8_t*)&byte, sizeof(byte), 1000);
		i++;

	}
	return i;


}

