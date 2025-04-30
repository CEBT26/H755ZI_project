/*
 * @file: INS_processing.c
 *
 * @brief: Implementación de las funcionalidades de la IRQ de UARTs.
 * @date: Apr 9, 2025
 * @author: cbogarin
 *
 */

#include "Middleware Layer/INS_processing.h"

INS_Angles_Data_Struct_t INS_Angles_Data_Struct;            /*!< Estructura de datos de INS Angulos y aceleraciones */
INS_GNSS_Data_Struct_t INS_GNSS_Data_Struct;				/*!< Estructura de datos para GNSS */

INS_RX_StateCurrent_t INS_RX_StateCurrent = INS_STATE_GetSyncFirst;	/*!< Estados de la maquina de recepción del INS */

/**
 * @brief Recepción y filtración del sensor XSENS.
 *
 * @details Filtración de mensajes del sensor XSENS.
 * @param [in] INS_Rx_buff -> Byte de recepción del USART6.
 *
 */
void INS_Rx_Message(uint8_t INS_Rx_buff)
{
	static uint8_t INS_gaucBufferRX[102];										/*!< Buffer a guardar datos */
	static uint8_t INS_aux_Len = 0;												/*!< Longitud de trama */
	static uint8_t countRxData = 1;												/*!< Contador de datos recividos */
	//static INS_RX_StateCurrent_t INS_RX_StateCurrent = INS_STATE_GetSyncFirst;	/*!< Estados de la maquina de recepción del INS */
	static INS_Cpl_Data_Flag_enum_t INS_Cpl_Data_Flag = INS_Cpl_Data_Flag_null;	/*!< Bandera para la asignación tipo de ID */

	switch(INS_RX_StateCurrent)
	{
		case INS_STATE_GetSyncFirst:

			if(INS_Rx_buff == PREAMBLE)								/*!< Revisión de 0xFA */
			{
				INS_RX_StateCurrent = INS_STATE_GetSyncCommands;
				INS_gaucBufferRX[INS_Index_Sync1] = INS_Rx_buff;	/*!< Guarda 0xFA */

			}else{
				INS_RX_StateCurrent = INS_STATE_GetSyncFirst;
			}
			break;

		case INS_STATE_GetSyncCommands:

			if(INS_Rx_buff == BID)									/*!< Revisión de 0xFF */
			{
				INS_RX_StateCurrent = INS_STATE_GetIDCommands;
				INS_gaucBufferRX[INS_Index_Sync2] = INS_Rx_buff;	/*!< Guarda 0xFF */

			}else{
				INS_RX_StateCurrent = INS_STATE_GetSyncFirst;
			}
			break;

		case INS_STATE_GetIDCommands:

			//if(INS_Rx_buff == MTData2)							/*!< Revisión de 0x36 */
			//{
				INS_RX_StateCurrent = INS_STATE_GetLenCommands;
				INS_gaucBufferRX[INS_Index_ID] = INS_Rx_buff;		/*!< Guarda 0x36 */

			//}else{
			//	INS_RX_StateCurrent = INS_STATE_GetSyncFirst;
			//}
			break;

		case INS_STATE_GetLenCommands:

			INS_RX_StateCurrent = INS_STATE_GetDataCommands;
			INS_aux_Len = INS_Rx_buff;
			INS_gaucBufferRX[INS_Index_Len] = INS_Rx_buff;			/*!< Guarda longitud de trama */

			if((INS_Rx_buff == INS_ID_MTData_Angles) || (INS_Rx_buff == INS_ID_AUX_ANGLES) || (INS_Rx_buff == INS_ID_AUX))	/*!< Datos de Ángulos, Bandera */
			{
				INS_Cpl_Data_Flag = INS_Cpl_Data_Flag_Angles;

			}else if(INS_Rx_buff == INS_ID_MTData_GNSS)				/*!< Datos del GNSS, Bandera */
			{
				INS_Cpl_Data_Flag = INS_Cpl_Data_Flag_GNSS;

			}else{
				INS_RX_StateCurrent = INS_STATE_GetSyncFirst;
				INS_Cpl_Data_Flag = INS_Cpl_Data_Flag_null;
			}
			break;

		case INS_STATE_GetDataCommands:

			if( countRxData <= INS_aux_Len )									/*!< Guarda los datos del payload de la trama */
			{
				INS_gaucBufferRX[countRxData + INS_Index_Len] = INS_Rx_buff;
				countRxData++;

			}else if (countRxData == INS_aux_Len + 1){
				INS_gaucBufferRX[countRxData + INS_Index_Len] = INS_Rx_buff;    /*!< Checksum */
				INS_RX_StateCurrent = INS_STATE_GetSyncFirst;
				countRxData = 1;
				INS_kindInf(INS_gaucBufferRX, INS_Cpl_Data_Flag);				/*!< Guardamos datos de INS o INS-GPS */

			}else{
				INS_RX_StateCurrent = INS_STATE_GetSyncFirst;					/*!< Regresamos al estado de espera de trama de sincronía */
				countRxData = 1;												/*!< Regresamos el contador de datos a 1 */
			}
			break;

		default:
			INS_RX_StateCurrent = INS_STATE_GetSyncFirst;
			countRxData = 1;													/*!< Regresamos el contador de datos a 1 */
			break;
	}
}

/**
 * @brief Revisión del tipo de mensaje del XSENS.
 *
 * @details Verificación del mensaje GNSS o ángulos.
 * @param [in] INS_buffRx -> Puntero al payload recivido.
 * @param [in] INS_dataFlag -> Bandera para ver el tipo de datop recivido.
 *
 */
void INS_kindInf(uint8_t *INS_buffRx, INS_Cpl_Data_Flag_enum_t INS_dataFlag)
{
	if(INS_dataFlag == INS_Cpl_Data_Flag_Angles)			/*!< Guardado de datos en estructura si es mensaje de ángulos */
	{
		INS_Angles_Data_Struct.Sync[INS_Index_Sync1] = INS_buffRx[INS_Index_Sync1];	/*!< 0xFA */
		INS_Angles_Data_Struct.Sync[INS_Index_Sync2] = INS_buffRx[INS_Index_Sync2];	/*!< 0xFF */
		INS_Angles_Data_Struct.ID = INS_buffRx[INS_Index_ID];						/*!< 0x36, index ID */
		INS_Angles_Data_Struct.Len = INS_buffRx[INS_Index_Len];						/*!< Longitud de la trama */

		INS_Angles_Data_Struct.Ang_ID[0] = INS_buffRx[4];							/*!< Byte 1 de ángulos de Euler */
		INS_Angles_Data_Struct.Ang_ID[1] = INS_buffRx[5];							/*!< Byte 2 de ángulos de Euler */
		INS_Angles_Data_Struct.Ang_Len = INS_buffRx[6];								/*!< Longitud del payload de ángulos de Euler */

		if((INS_Angles_Data_Struct.Ang_ID[0] == 0x20) && (INS_Angles_Data_Struct.Ang_ID[1] == 0x30) && (INS_Angles_Data_Struct.Ang_Len == 0x0C))	/*!< Verificamos ID de ángulos */
		{
			INS_Angles_Data_Struct.Ang_Data_Roll.byte[3] = INS_buffRx[7];			/*!< Byte1 ROLL */
			INS_Angles_Data_Struct.Ang_Data_Roll.byte[2] = INS_buffRx[8];			/*!< Byte2 ROLL */
			INS_Angles_Data_Struct.Ang_Data_Roll.byte[1] = INS_buffRx[9];			/*!< Byte3 ROLL */
			INS_Angles_Data_Struct.Ang_Data_Roll.byte[0] = INS_buffRx[10];			/*!< Byte4 ROLL */
			INS_Angles_Data_Struct.Ang_Data_Roll.sFloat = INS_Angles_Data_Struct.Ang_Data_Roll.sFloat * axis_x_invert;

			INS_Angles_Data_Struct.Ang_Data_pitch.byte[3] = INS_buffRx[11];			/*!< Byte1 PITCH */
			INS_Angles_Data_Struct.Ang_Data_pitch.byte[2] = INS_buffRx[12];			/*!< Byte2 PITCH */
			INS_Angles_Data_Struct.Ang_Data_pitch.byte[1] = INS_buffRx[13];			/*!< Byte3 PITCH */
			INS_Angles_Data_Struct.Ang_Data_pitch.byte[0] = INS_buffRx[14];			/*!< Byte4 PITCH */
			INS_Angles_Data_Struct.Ang_Data_pitch.sFloat = INS_Angles_Data_Struct.Ang_Data_pitch.sFloat * axis_y_invert;

			INS_Angles_Data_Struct.Ang_Data_Yaw.byte[3] = INS_buffRx[15];			/*!< Byte1 YAW */
			INS_Angles_Data_Struct.Ang_Data_Yaw.byte[2] = INS_buffRx[16];			/*!< Byte2 YAW */
			INS_Angles_Data_Struct.Ang_Data_Yaw.byte[1] = INS_buffRx[17];			/*!< Byte3 YAW */
			INS_Angles_Data_Struct.Ang_Data_Yaw.byte[0] = INS_buffRx[18];			/*!< Byte4 YAW */
			INS_Angles_Data_Struct.Ang_Data_Yaw.sFloat = INS_Angles_Data_Struct.Ang_Data_Yaw.sFloat * axis_z_invert;
		}

		INS_Angles_Data_Struct.Acc_ID[0] = INS_buffRx[19];							/*!< Byte1 ID acelerómetro */
		INS_Angles_Data_Struct.Acc_ID[1] = INS_buffRx[20];							/*!< Byte2 ID acelerómetro */
		INS_Angles_Data_Struct.Acc_Len 	 = INS_buffRx[21];							/*!< Longitud del payload del acelerómetro */

		if((INS_Angles_Data_Struct.Acc_ID[0] == 0x40) && (INS_Angles_Data_Struct.Acc_ID[1] == 0x30))	/*!< Verificamos ID de acelerómetro */
		{
			INS_Angles_Data_Struct.Acc_AccX.byte[3] = INS_buffRx[22];				/*!< Byte1 de datos de aceleración X */
			INS_Angles_Data_Struct.Acc_AccX.byte[2] = INS_buffRx[23];				/*!< Byte2 de datos de aceleración X */
			INS_Angles_Data_Struct.Acc_AccX.byte[1] = INS_buffRx[24];				/*!< Byte3 de datos de aceleración X */
			INS_Angles_Data_Struct.Acc_AccX.byte[0] = INS_buffRx[25];				/*!< Byte4 de datos de aceleración X */

			INS_Angles_Data_Struct.Acc_AccY.byte[3] = INS_buffRx[26];				/*!< Byte1 de datos de aceleración Y */
			INS_Angles_Data_Struct.Acc_AccY.byte[2] = INS_buffRx[27];				/*!< Byte2 de datos de aceleración Y */
			INS_Angles_Data_Struct.Acc_AccY.byte[1] = INS_buffRx[28];				/*!< Byte3 de datos de aceleración Y */
			INS_Angles_Data_Struct.Acc_AccY.byte[0] = INS_buffRx[29];				/*!< Byte4 de datos de aceleración Y */

			INS_Angles_Data_Struct.Acc_AccZ.byte[3] = INS_buffRx[30];				/*!< Byte1 de datos de aceleración Z */
			INS_Angles_Data_Struct.Acc_AccZ.byte[2] = INS_buffRx[31];				/*!< Byte2 de datos de aceleración Z */
			INS_Angles_Data_Struct.Acc_AccZ.byte[1] = INS_buffRx[32];				/*!< Byte3 de datos de aceleración Z */
			INS_Angles_Data_Struct.Acc_AccZ.byte[0] = INS_buffRx[33];				/*!< Byte4 de datos de aceleración Z */
		}

		INS_Angles_Data_Struct.Gyr_ID[0] = INS_buffRx[34];							/*!< Byte1 ID GIROSCOPIO */
		INS_Angles_Data_Struct.Gyr_ID[1] = INS_buffRx[35];							/*!< Byte2 ID GIROSCOPIO */
		INS_Angles_Data_Struct.Gyr_Len 	 = INS_buffRx[36];							/*!< Longitud del payload del GIROSCOPIO */

		if((INS_Angles_Data_Struct.Gyr_ID[0] == 0x80) && (INS_Angles_Data_Struct.Gyr_ID[1] == 0x20))	/*!< Verificamos ID de GIROSCOPIO */
		{
			INS_Angles_Data_Struct.Gyr_GyrX.byte[3] = INS_buffRx[37];				/*!< Byte1 de datos de giroscopio X */
			INS_Angles_Data_Struct.Gyr_GyrX.byte[2] = INS_buffRx[38];				/*!< Byte2 de datos de giroscopio X */
			INS_Angles_Data_Struct.Gyr_GyrX.byte[1] = INS_buffRx[39];				/*!< Byte3 de datos de giroscopio X */
			INS_Angles_Data_Struct.Gyr_GyrX.byte[0] = INS_buffRx[40];				/*!< Byte4 de datos de giroscopio X */

			INS_Angles_Data_Struct.Gyr_GyrY.byte[3] = INS_buffRx[41];				/*!< Byte1 de datos de giroscopio y */
			INS_Angles_Data_Struct.Gyr_GyrY.byte[2] = INS_buffRx[42];				/*!< Byte2 de datos de giroscopio y */
			INS_Angles_Data_Struct.Gyr_GyrY.byte[1] = INS_buffRx[43];				/*!< Byte3 de datos de giroscopio y */
			INS_Angles_Data_Struct.Gyr_GyrY.byte[0] = INS_buffRx[44];				/*!< Byte4 de datos de giroscopio y */

			INS_Angles_Data_Struct.Gyr_GyrZ.byte[3] = INS_buffRx[45];				/*!< Byte1 de datos de giroscopio Z */
			INS_Angles_Data_Struct.Gyr_GyrZ.byte[2] = INS_buffRx[46];				/*!< Byte2 de datos de giroscopio Z */
			INS_Angles_Data_Struct.Gyr_GyrZ.byte[1] = INS_buffRx[47];				/*!< Byte3 de datos de giroscopio Z */
			INS_Angles_Data_Struct.Gyr_GyrZ.byte[0] = INS_buffRx[48];				/*!< Byte4 de datos de giroscopio Z */
		}

		INS_Angles_Data_Struct.Baro_ID[0] = INS_buffRx[49];							/*!< Byte1 ID Barómetro */
		INS_Angles_Data_Struct.Baro_ID[1] = INS_buffRx[50];							/*!< Byte2 ID Barómetro */
		INS_Angles_Data_Struct.Baro_Len   = INS_buffRx[51];							/*!< Longitud del payload del Barómetro */

		if((INS_Angles_Data_Struct.Baro_ID[0] == 0x30) && (INS_Angles_Data_Struct.Baro_ID[1] == 0x10))	/*!< Verificamos ID de Barómetro */
		{
			INS_Angles_Data_Struct.Baro_Data.byte[3] = INS_buffRx[52];				/*!< Byte1 de datos de Barómetro */
			INS_Angles_Data_Struct.Baro_Data.byte[2] = INS_buffRx[53];				/*!< Byte2 de datos de Barómetro */
			INS_Angles_Data_Struct.Baro_Data.byte[1] = INS_buffRx[54];				/*!< Byte3 de datos de Barómetro */
			INS_Angles_Data_Struct.Baro_Data.byte[0] = INS_buffRx[55];				/*!< Byte4 de datos de Barómetro */
		}

		INS_Angles_Data_Struct.Check_sum = INS_buffRx[56];							/*!< Checksum */

		//DTLNK_Telemetry_Slow_Data_Struct.Status_INS = 0b00000110;					/*!< Bandera INS */
	}else if(INS_dataFlag == INS_Cpl_Data_Flag_GNSS){

		INS_GNSS_Data_Struct.Sync[0] = 		INS_buffRx[INS_Index_Sync1];			/*!< 0xFA */
		INS_GNSS_Data_Struct.Sync[1] = 		INS_buffRx[INS_Index_Sync2];			/*!< 0xFF */

		INS_GNSS_Data_Struct.ID = 			INS_buffRx[INS_Index_ID];				/*!< 0x36, index ID */
		INS_GNSS_Data_Struct.Len = 			INS_buffRx[INS_Index_Len];				/*!< Longitud de la trama */

		INS_GNSS_Data_Struct.GNSS_ID[0] = 	INS_buffRx[4];							/*!< Primer byte de ID GNSS */
		INS_GNSS_Data_Struct.GNSS_ID[1] = 	INS_buffRx[5];							/*!< Segundo byte de ID GNSS */
		INS_GNSS_Data_Struct.Len = 			INS_buffRx[6];							/*!< Longitud de payload */

		INS_GNSS_Data_Struct.Itow.byte[3] = INS_buffRx[7];							/*!< Byte1 GPS time of week-itow */
		INS_GNSS_Data_Struct.Itow.byte[2] = INS_buffRx[8];							/*!< Byte2 GPS time of week-itow */
		INS_GNSS_Data_Struct.Itow.byte[1] = INS_buffRx[9];							/*!< Byte3 GPS time of week-itow */
		INS_GNSS_Data_Struct.Itow.byte[0] = INS_buffRx[10];							/*!< Byte4 GPS time of week-itow */
		INS_GNSS_Data_Struct.Year.byte[1] = INS_buffRx[11];							/*!< Byte1 del año */
		INS_GNSS_Data_Struct.Year.byte[0] = INS_buffRx[12];							/*!< Byte2 del año */
		INS_GNSS_Data_Struct.month = 		INS_buffRx[13];							/*!< Byte de mes */
		INS_GNSS_Data_Struct.day = 			INS_buffRx[14];							/*!< Byte de día */
		INS_GNSS_Data_Struct.hour = 		INS_buffRx[15];							/*!< Byte de horas */
		INS_GNSS_Data_Struct.min = 			INS_buffRx[16];							/*!< Byte de minutos */
		INS_GNSS_Data_Struct.sec = 			INS_buffRx[17];							/*!< Byte de segundos */

		INS_GNSS_Data_Struct.valid_flag = 	INS_buffRx[18];							/*!< Bandera de validación */

		INS_GNSS_Data_Struct.tAcc.byte[3] = INS_buffRx[19];							/*!< Byte1 Time accuracy estimate (UTC) */
		INS_GNSS_Data_Struct.tAcc.byte[2] = INS_buffRx[20];							/*!< Byte2 Time accuracy estimate (UTC) */
		INS_GNSS_Data_Struct.tAcc.byte[1] = INS_buffRx[21];							/*!< Byte3 Time accuracy estimate (UTC) */
		INS_GNSS_Data_Struct.tAcc.byte[0] = INS_buffRx[22];							/*!< Byte4 Time accuracy estimate (UTC) */

		INS_GNSS_Data_Struct.nano.byte[3] = INS_buffRx[23];							/*!< Byte1 Fraction of second */
		INS_GNSS_Data_Struct.nano.byte[2] = INS_buffRx[24];							/*!< Byte2 Fraction of second */
		INS_GNSS_Data_Struct.nano.byte[1] = INS_buffRx[25];							/*!< Byte3 Fraction of second */
		INS_GNSS_Data_Struct.nano.byte[0] = INS_buffRx[26];							/*!< Byte4 Fraction of second */

		INS_GNSS_Data_Struct.fix_type = 	INS_buffRx[27];							/*!< GNSS fix type */
		INS_GNSS_Data_Struct.fix_flag = 	INS_buffRx[28];							/*!< Bandera de estatus Fix */

		INS_GNSS_Data_Struct.Num_Sat = 		INS_buffRx[29];							/*!< Número de satelites */

		INS_GNSS_Data_Struct.Reserv1 = 		INS_buffRx[30];							/*!< Reservado */

		INS_GNSS_Data_Struct.longitud.byte[3] = INS_buffRx[31];						/*!< Byte1 Longitud */
		INS_GNSS_Data_Struct.longitud.byte[2] = INS_buffRx[32];						/*!< Byte2 Longitud */
		INS_GNSS_Data_Struct.longitud.byte[1] = INS_buffRx[33];						/*!< Byte3 Longitud */
		INS_GNSS_Data_Struct.longitud.byte[0] = INS_buffRx[34];						/*!< Byte4 Longitud */
		INS_GNSS_Data_Struct.latitude.byte[3] = INS_buffRx[35];						/*!< Byte1 Latitud */
		INS_GNSS_Data_Struct.latitude.byte[2] = INS_buffRx[36];						/*!< Byte2 Latitud */
		INS_GNSS_Data_Struct.latitude.byte[1] = INS_buffRx[37];						/*!< Byte3 Latitud */
		INS_GNSS_Data_Struct.latitude.byte[0] = INS_buffRx[38];						/*!< Byte4 Latitud */
		INS_GNSS_Data_Struct.altura.byte[3] = 	INS_buffRx[39];						/*!< Byte1 Altura */
		INS_GNSS_Data_Struct.altura.byte[2] = 	INS_buffRx[40];						/*!< Byte2 Altura */
		INS_GNSS_Data_Struct.altura.byte[1] = 	INS_buffRx[41];						/*!< Byte3 Altura */
		INS_GNSS_Data_Struct.altura.byte[0] = 	INS_buffRx[42];						/*!< Byte4 Altura */
		INS_GNSS_Data_Struct.altitude.byte[3] = INS_buffRx[43];						/*!< Byte1 Altitud */
		INS_GNSS_Data_Struct.altitude.byte[2] = INS_buffRx[44];						/*!< Byte2 Altitud */
		INS_GNSS_Data_Struct.altitude.byte[1] = INS_buffRx[45];						/*!< Byte3 Altitud */
		INS_GNSS_Data_Struct.altitude.byte[0] = INS_buffRx[46];						/*!< Byte4 Altitud */

		//Si el GPS ublox M10 cuenta con mayor numero de satelites, le da prioridad y reemplaza la info
		//GPS_CheckAccuracyFromNumSats();	//Todo por definir

		//Transmisión de altitud para cálculos de airspeed en tarjeta pitot
		//uint16_t uiAuxAltitude = (uint16_t) INS_GNSS_Data_Struct.altitude.s32; //Todo por definir
		//CAN_GPSExtraBuf[5] = (uint8_t) uiAuxAltitude;	//Todo por definir
		//CAN_GPSExtraBuf[6] = (uiAuxAltitude >> 8);

		INS_GNSS_Data_Struct.horizon_accuracy.byte[3] = INS_buffRx[47];				/*!< Byte1 Horizontal accuracy estimate */
		INS_GNSS_Data_Struct.horizon_accuracy.byte[2] = INS_buffRx[48];				/*!< Byte2 Horizontal accuracy estimate */
		INS_GNSS_Data_Struct.horizon_accuracy.byte[1] = INS_buffRx[49];				/*!< Byte3 Horizontal accuracy estimate */
		INS_GNSS_Data_Struct.horizon_accuracy.byte[0] = INS_buffRx[50];				/*!< Byte4 Horizontal accuracy estimate */
		INS_GNSS_Data_Struct.vertical_accuracy.byte[3] = INS_buffRx[51];			/*!< Byte1 Vertical accuracy estimate */
		INS_GNSS_Data_Struct.vertical_accuracy.byte[2] = INS_buffRx[52];			/*!< Byte2 Vertical accuracy estimate */
		INS_GNSS_Data_Struct.vertical_accuracy.byte[1] = INS_buffRx[53];			/*!< Byte3 Vertical accuracy estimate */
		INS_GNSS_Data_Struct.vertical_accuracy.byte[0] = INS_buffRx[54];			/*!< Byte4 Vertical accuracy estimate */

		INS_GNSS_Data_Struct.north_velocoty.byte[3] = INS_buffRx[55];				/*!< Byte1  NED north velocity */
		INS_GNSS_Data_Struct.north_velocoty.byte[2] = INS_buffRx[56];				/*!< Byte2  NED north velocity */
		INS_GNSS_Data_Struct.north_velocoty.byte[1] = INS_buffRx[57];				/*!< Byte3  NED north velocity */
		INS_GNSS_Data_Struct.north_velocoty.byte[0] = INS_buffRx[58];				/*!< Byte4  NED north velocity */
		INS_GNSS_Data_Struct.east_velocity.byte[3] = INS_buffRx[59];				/*!< Byte1 NED east velocity */
		INS_GNSS_Data_Struct.east_velocity.byte[2] = INS_buffRx[60];				/*!< Byte2 NED east velocity */
		INS_GNSS_Data_Struct.east_velocity.byte[1] = INS_buffRx[61];				/*!< Byte3 NED east velocity */
		INS_GNSS_Data_Struct.east_velocity.byte[0] = INS_buffRx[62];				/*!< Byte4 NED east velocity */

		INS_GNSS_Data_Struct.down_velocity.byte[3] = INS_buffRx[63];				/*!< Byte1 NED down velocity */
		INS_GNSS_Data_Struct.down_velocity.byte[2] = INS_buffRx[64];				/*!< Byte2 NED down velocity */
		INS_GNSS_Data_Struct.down_velocity.byte[1] = INS_buffRx[65];				/*!< Byte3 NED down velocity */
		INS_GNSS_Data_Struct.down_velocity.byte[0] = INS_buffRx[66];				/*!< Byte4 NED down velocity */

		INS_GNSS_Data_Struct.gSpeed.byte[3] = INS_buffRx[67];						/*!< Byte1 2D ground speed */
		INS_GNSS_Data_Struct.gSpeed.byte[2] = INS_buffRx[68];						/*!< Byte2 2D ground speed */
		INS_GNSS_Data_Struct.gSpeed.byte[1] = INS_buffRx[69];						/*!< Byte3 2D ground speed */
		INS_GNSS_Data_Struct.gSpeed.byte[0] = INS_buffRx[70];						/*!< Byte4 2D ground speed */

		//GPS_CheckAccuracyFromNumSats_gspeed();	//Todo Por definir

		INS_GNSS_Data_Struct.head_Motion.byte[3] = INS_buffRx[71];					/*!< Byte1 2D heading of motion */
		INS_GNSS_Data_Struct.head_Motion.byte[2] = INS_buffRx[72];					/*!< Byte2 2D heading of motion */
		INS_GNSS_Data_Struct.head_Motion.byte[1] = INS_buffRx[73];					/*!< Byte3 2D heading of motion */
		INS_GNSS_Data_Struct.head_Motion.byte[0] = INS_buffRx[74];					/*!< Byte4 2D heading of motion */

		INS_GNSS_Data_Struct.speed_accuracy.byte[3] = INS_buffRx[75];				/*!< Byte1 Speed accuracy estimate */
		INS_GNSS_Data_Struct.speed_accuracy.byte[2] = INS_buffRx[76];				/*!< Byte2 Speed accuracy estimate */
		INS_GNSS_Data_Struct.speed_accuracy.byte[1] = INS_buffRx[77];				/*!< Byte3 Speed accuracy estimate */
		INS_GNSS_Data_Struct.speed_accuracy.byte[0] = INS_buffRx[78];				/*!< Byte4 Speed accuracy estimate */
		INS_GNSS_Data_Struct.heading_accuracy.byte[3] = INS_buffRx[79];				/*!< Byte1 Heading accuracy estimate */
		INS_GNSS_Data_Struct.heading_accuracy.byte[2] = INS_buffRx[80];				/*!< Byte2 Heading accuracy estimate */
		INS_GNSS_Data_Struct.heading_accuracy.byte[1] = INS_buffRx[81];				/*!< Byte3 Heading accuracy estimate */
		INS_GNSS_Data_Struct.heading_accuracy.byte[0] = INS_buffRx[82];				/*!< Byte4 Heading accuracy estimate */

		INS_GNSS_Data_Struct.heading_vehicle.byte[3] = INS_buffRx[83];				/*!< Byte1 2D heading of vehicle */
		INS_GNSS_Data_Struct.heading_vehicle.byte[2] = INS_buffRx[84];				/*!< Byte2 2D heading of vehicle */
		INS_GNSS_Data_Struct.heading_vehicle.byte[1] = INS_buffRx[85];				/*!< Byte3 2D heading of vehicle */
		INS_GNSS_Data_Struct.heading_vehicle.byte[0] = INS_buffRx[86];				/*!< Byte4 2D heading of vehicle */

		INS_GNSS_Data_Struct.gdop.byte[1] = INS_buffRx[87];							/*!< Byte1 Geometric DOP */
		INS_GNSS_Data_Struct.gdop.byte[0] = INS_buffRx[88];							/*!< Byte2 Geometric DOP */
		INS_GNSS_Data_Struct.pdop.byte[1] = INS_buffRx[89];							/*!< Byte3 Geometric DOP */
		INS_GNSS_Data_Struct.pdop.byte[0] = INS_buffRx[90];							/*!< Byte4 Geometric DOP */
		INS_GNSS_Data_Struct.tdop.byte[1] = INS_buffRx[91];							/*!< Byte1 Time DOP */
		INS_GNSS_Data_Struct.tdop.byte[0] = INS_buffRx[92];							/*!< Byte2 Time DOP */
		INS_GNSS_Data_Struct.vdop.byte[1] = INS_buffRx[93];							/*!< Byte3 Time DOP */
		INS_GNSS_Data_Struct.vdop.byte[0] = INS_buffRx[94];							/*!< Byte4 Time DOP */
		INS_GNSS_Data_Struct.hdop.byte[1] = INS_buffRx[95];							/*!< Byte1 Horizontal DOP */
		INS_GNSS_Data_Struct.hdop.byte[0] = INS_buffRx[96];							/*!< Byte2 Horizontal DOP */
		INS_GNSS_Data_Struct.ndop.byte[1] = INS_buffRx[97];							/*!< Byte1 Northing DOP */
		INS_GNSS_Data_Struct.ndop.byte[0] = INS_buffRx[98];							/*!< Byte2 Northing DOP */
		INS_GNSS_Data_Struct.edop.byte[1] = INS_buffRx[99];							/*!< Byte1 Easting DOP */
		INS_GNSS_Data_Struct.edop.byte[0] = INS_buffRx[100];						/*!< Byte1 Easting DOP */

		INS_GNSS_Data_Struct.Check_sum = INS_buffRx[101];							/*!< Checksum */
		//DTLNK_Telemetry_Fast_Data_Struct.Status_CTRLCOM1 &= 0b11111110;						//Todo Quitar números mágicos
	}
}

/**
 * @brief WatchDog de INS.
 *
 * @details Desarollo del WDG del INS XSENS, Si los datos no se están actualizando, se reinicia toda la comunicación.
 *
 */
void INS_WatchDog(void)
{
	static uint8_t INS_WD_Counter = 0;
	static uint8_t timer_WD_INS_t0 = 0;
	static uint8_t timer_WD_INS_t1 = 1;
	static uint8_t timer_WD_INS_t2 = 2;
	static uint8_t timer_WD_INS_t3 = 3;

	if (INS_WD_Counter >= 5)
	{
		if ((timer_WD_INS_t0 == timer_WD_INS_t1) && (timer_WD_INS_t0 == timer_WD_INS_t2) && (timer_WD_INS_t0 == timer_WD_INS_t3))
		{
			//MX_USART6_UART_Init;		//Todo verificar si es necesario
			UART_DMA_Init_function();
			timer_WD_INS_t0 = 0;
			timer_WD_INS_t1 = 1;
			timer_WD_INS_t2 = 2;
			timer_WD_INS_t3 = 3;
			INS_RX_StateCurrent = INS_STATE_GetSyncFirst;
			//DTLNK_Telemetry_Fast_Data_Struct.Status_CTRLCOM1 |= 0b00000001;	//Todo esperar DTLNK
			//DTLNK_Telemetry_Slow_Data_Struct.Status_INS = 0b01111000;
		}
	timer_WD_INS_t3 = timer_WD_INS_t2;
	timer_WD_INS_t2 = timer_WD_INS_t1;
	timer_WD_INS_t1 = timer_WD_INS_t0;
	timer_WD_INS_t0 = (uint8_t)INS_Angles_Data_Struct.Ang_Data_Roll.sFloat;
	INS_WD_Counter = 0;
	}

	INS_WD_Counter ++;
}
