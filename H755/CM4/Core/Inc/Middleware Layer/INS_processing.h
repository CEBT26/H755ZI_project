/*
 * @file: INS_processing.h
 *
 * @details: Estructuras, variables y funciones para el desarrrollo del INS.
 * @date: Apr 9, 2025
 * @author: cbogarin
 *
 */

#ifndef INC_MIDDLEWARE_LAYER_INS_PROCESSING_H_
#define INC_MIDDLEWARE_LAYER_INS_PROCESSING_H_

#include "stdint.h"
#include "Global/Global_Definitions.h"
#include <stdio.h>
#include <string.h>
#include "usart.h"

/**
 * @brief Defines para el uso de los bytes del INS
 */
#define PREAMBLE 				0xFA	/*!< Preamble, inicialización de trama */
#define BID						0xFF	/*!< BID, Identificador de bus */
#define MTData2					0x36	/*!< ID de mensaje, MTData2 */
#define INS_ID_MTData_Angles	0x34	/*!< Largo de bytes, en este caso en específico podemos usarlo como un identificador de trama de ángulos y aceleraciones */
#define INS_ID_AUX_ANGLES		0x55	/*!< Largo de bytes, en este caso en específico podemos usarlo como un identificador de trama de ángulos y aceleraciones */
#define INS_ID_MTData_GNSS		0x61	/*!< Largo de bytes, en este caso en específico podemos usarlo como un identificador de trama de ángulos y aceleraciones */
#define INS_ID_AUX				0x2D	/*!< ID auxiliar */
#define INS_Index_Sync1	 		0		/*!< Posición 0 */
#define INS_Index_Sync2	 		1		/*!< Posición 1 */
#define INS_Index_ID	 		2		/*!< Posición 2 */
#define INS_Index_Len	 		3		/*!< Posición 3 */
#define 	axis_x_invert  		1		/*!< Definición de ejes IMU para eje X */
#define 	axis_y_invert  		-1		/*!< Definición de ejes IMU para eje Y */
#define 	axis_z_invert  		1		/*!< Definición de ejes IMU para eje z */
/**
 * @}
 */

/**
 * @brief Enum para el filtrado de mensajes.
 */
typedef enum INS_RX_STATES_enum
{
	INS_STATE_GetSyncFirst,		/*!< Estado preamble */
	INS_STATE_GetSyncCommands,	/*!< Estado BID */
	INS_STATE_GetIDCommands,	/*!< Identificador de MTData2  = 0x36 */
	INS_STATE_GetLenCommands,	/*!< Longitud de trama */
	INS_STATE_GetDataCommands	/*!< Identificador para tipo de trama ángulos/GNSS, Toma los datos del payload */
} INS_RX_StateCurrent_t;

/**
 * @brief Enum para el filtrado del tipo de mensajes.
 */
typedef enum INS_Cpl_Data_Flag_enum
{
	INS_Cpl_Data_Flag_null,		/*!< Dato nulo. */
	INS_Cpl_Data_Flag_Angles,	/*!< Recpción del tipo ángulo */
	INS_Cpl_Data_Flag_GNSS		/*!< Recepción del tipo GNSS */
}INS_Cpl_Data_Flag_enum_t;

/**
 * @brief Estructura de los datos de ángulos.
 */
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

/**
 * @brief Estructura de los datos de GNSS.
 */
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

	s32_t north_velocoty;
	s32_t east_velocity;
	s32_t down_velocity;

	s32_t gSpeed;

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

extern INS_Angles_Data_Struct_t INS_Angles_Data_Struct;
extern INS_GNSS_Data_Struct_t INS_GNSS_Data_Struct;

void INS_Rx_Message(uint8_t INS_Rx_buff);
void INS_kindInf(uint8_t *INS_buffRx, INS_Cpl_Data_Flag_enum_t INS_dataFlag);
void INS_WatchDog(void);

#endif /* INC_MIDDLEWARE_LAYER_INS_PROCESSING_H_ */
