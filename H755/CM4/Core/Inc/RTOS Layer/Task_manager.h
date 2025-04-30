/*
 * Task_manager.h
 *
 *  Created on: Apr 29, 2025
 *      Author: cbogarin
 */

#ifndef INC_RTOS_LAYER_TASK_MANAGER_H_
#define INC_RTOS_LAYER_TASK_MANAGER_H_

#include "usart.h"
#include "Middleware Layer/INS_processing.h"

void task_init_1ms(void);
void task_init_5ms(void);
void task_init_10ms(void);
void task_init_15ms(void);
void task_init_20ms(void);
void task_init_30ms(void);
void task_init_50ms(void);
void task_init_100ms(void);
void task_init_200ms(void);
void task_init_500ms(void);

void task_manager_1ms(void);
void task_manager_5ms(void);
void task_manager_10ms(void);
void task_manager_15ms(void);
void task_manager_20ms(void);
void task_manager_30ms(void);
void task_manager_50ms(void);
void task_manager_100ms(void);
void task_manager_200ms(void);
void task_manager_500ms(void);

#endif /* INC_RTOS_LAYER_TASK_MANAGER_H_ */
