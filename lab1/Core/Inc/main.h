/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */



/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define BIAM_Pin GPIO_PIN_0
#define BIAM_GPIO_Port GPIOC
#define FIOM_Pin GPIO_PIN_1
#define FIOM_GPIO_Port GPIOC
#define ZIEM_Pin GPIO_PIN_2
#define ZIEM_GPIO_Port GPIOC
#define NIEM_Pin GPIO_PIN_3
#define NIEM_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define SZASO_Pin GPIO_PIN_6
#define SZASO_GPIO_Port GPIOA
#define CZASO_Pin GPIO_PIN_7
#define CZASO_GPIO_Port GPIOA
#define BIASO_Pin GPIO_PIN_4
#define BIASO_GPIO_Port GPIOC
#define ZOLM_Pin GPIO_PIN_0
#define ZOLM_GPIO_Port GPIOB
#define ZOL_Pin GPIO_PIN_1
#define ZOL_GPIO_Port GPIOB
#define CZE_Pin GPIO_PIN_2
#define CZE_GPIO_Port GPIOB
#define FIO_Pin GPIO_PIN_13
#define FIO_GPIO_Port GPIOB
#define NIE_Pin GPIO_PIN_14
#define NIE_GPIO_Port GPIOB
#define ZIE_Pin GPIO_PIN_15
#define ZIE_GPIO_Port GPIOB
#define BRO_Pin GPIO_PIN_6
#define BRO_GPIO_Port GPIOC
#define BIA_Pin GPIO_PIN_8
#define BIA_GPIO_Port GPIOC
#define SEC_Pin GPIO_PIN_8
#define SEC_GPIO_Port GPIOA
#define POMMO_Pin GPIO_PIN_10
#define POMMO_GPIO_Port GPIOA
#define BROMO_Pin GPIO_PIN_11
#define BROMO_GPIO_Port GPIOA
#define CZEMO_Pin GPIO_PIN_12
#define CZEMO_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define FIOSO_Pin GPIO_PIN_10
#define FIOSO_GPIO_Port GPIOC
#define ZIESO_Pin GPIO_PIN_11
#define ZIESO_GPIO_Port GPIOC
#define NIESO_Pin GPIO_PIN_12
#define NIESO_GPIO_Port GPIOC
#define ZOLSO_Pin GPIO_PIN_2
#define ZOLSO_GPIO_Port GPIOD
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
