/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

RTC_HandleTypeDef hrtc;




/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RX_BUFF_SIZE 128


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	uint8_t znak, kom[20];
	uint16_t dl_kom;



	/* ===== RX RING BUFFER ===== */

	volatile uint8_t Rx_buff[RX_BUFF_SIZE];
	volatile uint8_t Rx_empty = 0;
	volatile uint8_t Rx_busy = 0;

	/* ===== RX RING BUFFER END ===== */


	uint8_t Rx_byte;
	uint8_t Rx_data[RX_BUFF_SIZE];
	uint8_t Rx_idx = 0;
	char command[16];
	uint8_t last_index = 0;

	uint8_t Tx_byte;
	uint8_t Tx_data[128];
	uint16_t Tx_data_size;






	//project
	uint16_t *hr;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */




int splitNumber(int num){
	      int arr[2];

	      while(num != 0){
	          for(int i = 0; i < 2; i++){
	              arr[i] = num % 10;
	          }
	          num = num / 10;
	      }
	      return arr[1];

}

void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc){
	static RTC_TimeTypeDef sTime;
	static char mess[30];
	 size_t size;

	HAL_RTC_GetTime(hrtc, &sTime, RTC_FORMAT_BIN);

	size = sprintf(mess, "%2.2u:%2.2u:%2.2u\n", sTime.Hours, sTime.Minutes, sTime.Seconds);
	//HAL_UART_Transmit_IT(&huart2, (uint8_t*)mess, size);

	 uint8_t hours = sTime.Hours;
	 uint8_t seconds = sTime.Seconds;


	 int hour = splitNumber(seconds);
	 hr = &hour;

	 //size = sprintf(mess, "Split number: %d\n", *hr);
	 //HAL_UART_Transmit_IT(&huart2, (uint8_t*)mess, size);



}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

uint16_t time_passed = 0;
uint8_t delay_done = 0;
uint8_t led_blink_mode = 0;



void checkDelay(){
	time_passed++;
	if(time_passed > 500){
		time_passed = 0;
		delay_done = 1;

	}
}


void find_command(){
	char *led_on;
	char *led_off;
	char *led_blink;

		//strcpy(command, Rx_data);

		if(Rx_busy != Rx_empty){


			Rx_byte = Rx_buff[Rx_busy]; //get single char from array

			Rx_busy++;

			if(Rx_busy >= RX_BUFF_SIZE){
				Rx_busy = 0;
			}

			if(Rx_byte == 0x3B /* ; */){



							led_on = strstr(Rx_buff, "LED[ON]");
							led_off = strstr(Rx_buff, "LED[OFF]");
							led_blink = strstr(Rx_buff, "LED[BLINK");

						if(led_on != NULL){

								HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
								resetBuffers();


							}

						  if(led_off != NULL){
							HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
							resetBuffers();


							}
						  if(led_blink != NULL){
							  led_blink_mode = !led_blink_mode;





							  resetBuffers();
						  }

							if(led_off == NULL && led_on == NULL && led_blink == NULL){
								uint8_t siz = sprintf(Tx_data, "Niepoprawna komenda\r\n");
							    HAL_UART_Transmit_IT(&huart2, Tx_data, siz);

							    //memset(command, 0, sizeof command);
							    //resetBuffers();
							}

						}


		}











}

void resetBuffers(){


	//memset(Rx_buff, 0, RX_BUFF_SIZE);
	Rx_busy = 0;
	Rx_empty = 0;
	Rx_byte = 0;



}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart ){

	if(huart->Instance == USART2)
	{

		//uint8_t data[RX_BUFF_SIZE];
		//uint16_t Rx_data_size = 0;
	      // HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
			//sprintf(kom, "Date: %d:%d:%d", sTime.Hours, sTime.Minutes, sTime.Seconds);

			//HAL_RTCEx_WakeUpTimerEventCallback(&hrtc);

			//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);

		//Rx_data[Rx_idx++] = Rx_byte;
		//Rx_data_size = sprintf(data, Rx_data);
		Rx_empty++;

		if(Rx_empty >= RX_BUFF_SIZE){
			Rx_empty = 0;
		}


		/*
		if(Rx_idx == 0){
		   memset(&Rx_data, 0, sizeof(Rx_data));
		 } */


		//HAL_UART_Transmit_IT(&huart2, kom, dl_kom);
		HAL_UART_Receive_IT(&huart2, &Rx_buff[Rx_empty], 1);



	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */


  HAL_UART_Transmit_IT(&huart2, "Hello, i am stm32!!!", 20);

  //rx_buff

  HAL_UART_Receive_IT(&huart2, &Rx_buff[Rx_empty], 1);




  static GPIO_TypeDef* leds0[2] = {{BIA_GPIO_Port}, {BRO_GPIO_Port}};
  static uint16_t leds0_pins[2] = {{BIA_Pin}, {BRO_Pin}};

  RTC_TimeTypeDef sTime;

  char mess[50];
  uint8_t size;



  HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

  sTime.Hours = 20;
  sTime.Minutes = 25;
  sTime.Seconds = 15;

  HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

  HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 2047, RTC_WAKEUPCLOCK_RTCCLK_DIV16);



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);


	  if(*hr == 2 || 6){
		  HAL_GPIO_TogglePin(leds0[0], BIA_Pin);
		  //

	  }
	  //TURN On led blinkin mode
	  if(led_blink_mode == 1){
		  if(delay_done == 1){
			  HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			  delay_done = 0;
		  }

	  }

	  find_command();


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */











/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
