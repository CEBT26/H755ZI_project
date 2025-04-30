/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <stdio.h>
#include <string.h>

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
#define LD1_G_Pin GPIO_PIN_0
#define LD1_G_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/*!
 * @union 	float_tu
 * @brief 	Tipo de dato flotante y su equivalente en 8 bits
 */
typedef union
{
	float sFloat;		/*!< Variable flotante */
	uint8_t byte[4];	/*!< Arreglo de 4 unsigned bytes */
}float_tu;

/*!
 * @union	u32_t
 * @brief 	Tipo de dato no signado de 32 bits y sus equivalentes en 16 bits y en 8 bits.
 */
typedef union
{
	uint32_t u32;		/*!< Variable unsigned 32 bits */
	uint16_t u16 [2];	/*!< Arreglo de 2 unsigned 16 bits */
	uint8_t byte[4];	/*!< Arreglo de 4 unsigned bytes */
}u32_t;

/*!
 * @union	u16_t
 * @brief 	Tipo de dato no signado de 16 bits y su equivalente en 8 bits.
 */
typedef union
{
	uint16_t u16;		/*!< Variable unsigned 16 bits */
	uint8_t byte[2];	/*!< Arreglo de 2 unsigned bytes */
}u16_t;

/*!
 * @union	s32_t
 * @brief 	Tipo de dato signado de 32 bits y sus equivalentes en 16 bits y en 8 bits.
 */
typedef union
{
	int32_t s32;		/*!< Variable signed 32 bits */
	uint8_t byte[4];	/*!< Arreglo de 4 unsigned bytes */
}s32_t;

typedef struct INS_Angles_Data_Struct_Def
{
	uint8_t Sync[2];
	uint8_t ID;
	uint8_t Len;

	uint8_t Ang_ID[2];
	uint8_t Ang_Len;
	float_tu Ang_Data_Roll;
	float_tu Ang_Data_pitch;
	float_tu Ang_Data_Yaw;

	uint8_t Acc_ID[2];
	uint8_t Acc_Len;
	float_tu Acc_AccX;
	float_tu Acc_AccY;
	float_tu Acc_AccZ;

	uint8_t Gyr_ID[2];
	uint8_t Gyr_Len;
	float_tu Gyr_GyrX;
	float_tu Gyr_GyrY;
	float_tu Gyr_GyrZ;

	uint8_t Baro_ID [2];
	uint8_t Baro_Len;
	u32_t Baro_Data;

	uint8_t Check_sum;

} INS_Angles_Data_Struct_t;

typedef struct INS_GNSS_Data_Struct_Def
{
	uint8_t Sync [2];
	uint8_t ID;
	uint8_t Len;

	uint8_t GNSS_ID [2];
	uint8_t GNSS_Len;

	u32_t   Itow;
	u16_t   Year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;

	uint8_t valid_flag;

	u32_t tAcc;
	u32_t nano;

	uint8_t fix_type;
	uint8_t fix_flag;

	uint8_t Num_Sat;

	uint8_t Reserv1;

	s32_t longitud;
	s32_t latitude;
	s32_t altura;
	s32_t altitude;

	u32_t horizon_accuracy;
	u32_t vertical_accuracy;

	s32_t north_velocoty; //54
	s32_t east_velocity;
	s32_t down_velocity;

	s32_t gSpeed; //66

	s32_t head_Motion;

	u32_t speed_accuracy;
	u32_t heading_accuracy;

	s32_t heading_vehicle;

	u16_t gdop;

	u16_t pdop;
	u16_t tdop;
	u16_t vdop;
	u16_t hdop;
	u16_t ndop;
	u16_t edop;

	uint8_t Check_sum;

} INS_GNSS_Data_Struct_t;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
