/*
 * INS_processing.h
 *
 *  Created on: Apr 9, 2025
 *      Author: cbogarin
 */

#ifndef INC_MIDDLEWARE_LAYER_INS_PROCESSING_H_
#define INC_MIDDLEWARE_LAYER_INS_PROCESSING_H_

#include "stdint.h"
#include "Global/Global_Definitions.h"
#include <stdio.h>
#include <string.h>

#define PREAMBLE 				0xFA
#define BID						0xFF
#define MTData2					0x36
#define INS_ID_MTData_Angles	0x34	/*!< Largo de bytes, en este caso en específico podemos usarlo como un identificador de trama de ángulos y aceleraciones */
#define INS_ID_AUX_ANGLES		0x55
#define INS_ID_MTData_GNSS		0x61
#define INS_Index_Sync1	 		0
#define INS_Index_Sync2	 		1
#define INS_Index_ID	 		2
#define INS_Index_Len	 		3

//Para tramas principal, secundaria y comandos
typedef enum INS_RX_STATES_enum
{
	INS_STATE_GetSyncFirst,		/*!< Estado preamble */
	INS_STATE_GetSyncCommands,	/*!< Estado BID */
	INS_STATE_GetIDCommands,	/*!< Identificador de MTData2  = 0x36 */
	INS_STATE_GetLenCommands,	/*!< Longitud de trama */
	INS_STATE_GetDataCommands	/*!< Identificador para tipo de trama ángulos/GNSS, Toma los datos del payload */
} INS_RX_StateCurrent_t;

typedef enum INS_Cpl_Data_Flag_enum
{
	INS_Cpl_Data_Flag_null,
	INS_Cpl_Data_Flag_Angles,
	INS_Cpl_Data_Flag_GNSS
}INS_Cpl_Data_Flag_enum_t;

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

extern INS_Angles_Data_Struct_t INS_Angles_Data_Struct;
extern INS_GNSS_Data_Struct_t INS_GNSS_Data_Struct;

void INS_Rx_Message(uint8_t INS_Rx_buff);
void INS_kindInf(uint8_t *INS_buffRx, INS_Cpl_Data_Flag_enum_t INS_dataFlag);

#endif /* INC_MIDDLEWARE_LAYER_INS_PROCESSING_H_ */
