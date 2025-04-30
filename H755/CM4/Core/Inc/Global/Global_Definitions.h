/*
 * @file: Global_Definitions.c
 *
 * @details Variables globales para el uso en el proyecto.
 * @date: Apr 11, 2025
 * @author: cbogarin
 *
 */

#ifndef SRC_HT_APP_GLOBAL_DEFINITIONS_H_
#define SRC_HT_APP_GLOBAL_DEFINITIONS_H_

#include <stdio.h>
#include <stdint.h>

/**
 * @union 	float_tu
 * @brief 	Tipo de dato flotante y su equivalente en 8 bits
 */
typedef union
{
	float sFloat;		/*!< Variable flotante */
	uint8_t byte[4];	/*!< Arreglo de 4 unsigned bytes */
}float_tu;

/**
 * @union	u32_t
 * @brief 	Tipo de dato no signado de 32 bits y sus equivalentes en 16 bits y en 8 bits.
 */
typedef union
{
	uint32_t u32;		/*!< Variable unsigned 32 bits */
	uint16_t u16 [2];	/*!< Arreglo de 2 unsigned 16 bits */
	uint8_t byte[4];	/*!< Arreglo de 4 unsigned bytes */
}u32_t;

/**
 * @union	u16_t
 * @brief 	Tipo de dato no signado de 16 bits y su equivalente en 8 bits.
 */
typedef union
{
	uint16_t u16;		/*!< Variable unsigned 16 bits */
	uint8_t byte[2];	/*!< Arreglo de 2 unsigned bytes */
}u16_t;

/**
 * @union	s32_t
 * @brief 	Tipo de dato signado de 32 bits y sus equivalentes en 16 bits y en 8 bits.
 */
typedef union
{
	int32_t s32;		/*!< Variable signed 32 bits */
	uint8_t byte[4];	/*!< Arreglo de 4 unsigned bytes */
}s32_t;

#endif /* SRC_HT_APP_GOLBAL_DEFINITIONS_H_ */
