/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId Task_1Handle;
osThreadId Task_2Handle;
osThreadId Task_3Handle;
osThreadId Task_4Handle;
osThreadId Task_5Handle;
osThreadId Task_6Handle;
osThreadId Task_7Handle;
osThreadId Task_8Handle;
osThreadId Task_9Handle;
osThreadId Task_10Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Task_1ms(void const * argument);
void Task_5ms(void const * argument);
void Task_10ms(void const * argument);
void Task_15ms(void const * argument);
void Task_20ms(void const * argument);
void Task_30ms(void const * argument);
void Task_50ms(void const * argument);
void Task_100ms(void const * argument);
void Task_200ms(void const * argument);
void Task_500ms(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Task_1 */
  osThreadDef(Task_1, Task_1ms, osPriorityNormal, 0, 256);
  Task_1Handle = osThreadCreate(osThread(Task_1), NULL);

  /* definition and creation of Task_2 */
  osThreadDef(Task_2, Task_5ms, osPriorityIdle, 0, 256);
  Task_2Handle = osThreadCreate(osThread(Task_2), NULL);

  /* definition and creation of Task_3 */
  osThreadDef(Task_3, Task_10ms, osPriorityIdle, 0, 256);
  Task_3Handle = osThreadCreate(osThread(Task_3), NULL);

  /* definition and creation of Task_4 */
  osThreadDef(Task_4, Task_15ms, osPriorityIdle, 0, 256);
  Task_4Handle = osThreadCreate(osThread(Task_4), NULL);

  /* definition and creation of Task_5 */
  osThreadDef(Task_5, Task_20ms, osPriorityIdle, 0, 256);
  Task_5Handle = osThreadCreate(osThread(Task_5), NULL);

  /* definition and creation of Task_6 */
  osThreadDef(Task_6, Task_30ms, osPriorityIdle, 0, 256);
  Task_6Handle = osThreadCreate(osThread(Task_6), NULL);

  /* definition and creation of Task_7 */
  osThreadDef(Task_7, Task_50ms, osPriorityIdle, 0, 256);
  Task_7Handle = osThreadCreate(osThread(Task_7), NULL);

  /* definition and creation of Task_8 */
  osThreadDef(Task_8, Task_100ms, osPriorityIdle, 0, 256);
  Task_8Handle = osThreadCreate(osThread(Task_8), NULL);

  /* definition and creation of Task_9 */
  osThreadDef(Task_9, Task_200ms, osPriorityIdle, 0, 256);
  Task_9Handle = osThreadCreate(osThread(Task_9), NULL);

  /* definition and creation of Task_10 */
  osThreadDef(Task_10, Task_500ms, osPriorityIdle, 0, 256);
  Task_10Handle = osThreadCreate(osThread(Task_10), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Task_1ms */
/**
  * @brief  Function implementing the Task_1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Task_1ms */
void Task_1ms(void const * argument)
{
  /* USER CODE BEGIN Task_1ms */

	task_init_1ms();

  /* Infinite loop */
  for(;;)
  {
	  task_manager_1ms();
	  osDelay(1);
  }
  /* USER CODE END Task_1ms */
}

/* USER CODE BEGIN Header_Task_5ms */
/**
* @brief Function implementing the Task_2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_5ms */
void Task_5ms(void const * argument)
{
  /* USER CODE BEGIN Task_5ms */

	task_init_5ms();

  /* Infinite loop */
  for(;;)
  {
	  task_manager_5ms();
	  osDelay(5);
  }
  /* USER CODE END Task_5ms */
}

/* USER CODE BEGIN Header_Task_10ms */
/**
* @brief Function implementing the Task_3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_10ms */
void Task_10ms(void const * argument)
{
  /* USER CODE BEGIN Task_10ms */

	task_init_10ms();

  /* Infinite loop */
  for(;;)
  {
	  task_manager_10ms();
	  osDelay(10);
  }
  /* USER CODE END Task_10ms */
}

/* USER CODE BEGIN Header_Task_15ms */
/**
* @brief Function implementing the Task_4 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_15ms */
void Task_15ms(void const * argument)
{
  /* USER CODE BEGIN Task_15ms */

	task_init_15ms();

  /* Infinite loop */
  for(;;)
  {
	  task_manager_15ms();
	  osDelay(15);
  }
  /* USER CODE END Task_15ms */
}

/* USER CODE BEGIN Header_Task_20ms */
/**
* @brief Function implementing the Task_5 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_20ms */
void Task_20ms(void const * argument)
{
  /* USER CODE BEGIN Task_20ms */

	task_init_20ms();

  /* Infinite loop */
  for(;;)
  {
	  task_manager_20ms();
	  osDelay(20);
  }
  /* USER CODE END Task_20ms */
}

/* USER CODE BEGIN Header_Task_30ms */
/**
* @brief Function implementing the Task_6 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_30ms */
void Task_30ms(void const * argument)
{
  /* USER CODE BEGIN Task_30ms */

	task_init_30ms();

  /* Infinite loop */
  for(;;)
  {
	  task_manager_30ms();
	  osDelay(30);
  }
  /* USER CODE END Task_30ms */
}

/* USER CODE BEGIN Header_Task_50ms */
/**
* @brief Function implementing the Task_7 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_50ms */
void Task_50ms(void const * argument)
{
  /* USER CODE BEGIN Task_50ms */

	task_init_50ms();

  /* Infinite loop */
  for(;;)
  {
	  task_manager_50ms();
	  osDelay(50);
  }
  /* USER CODE END Task_50ms */
}

/* USER CODE BEGIN Header_Task_100ms */
/**
* @brief Function implementing the Task_8 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_100ms */
void Task_100ms(void const * argument)
{
  /* USER CODE BEGIN Task_100ms */

	task_init_100ms();

  /* Infinite loop */
  for(;;)
  {
	  task_manager_100ms();
	  osDelay(100);
  }
  /* USER CODE END Task_100ms */
}

/* USER CODE BEGIN Header_Task_200ms */
/**
* @brief Function implementing the Task_9 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_200ms */
void Task_200ms(void const * argument)
{
  /* USER CODE BEGIN Task_200ms */

	task_init_200ms();

  /* Infinite loop */
  for(;;)
  {
	  task_manager_200ms();
	  osDelay(200);
  }
  /* USER CODE END Task_200ms */
}

/* USER CODE BEGIN Header_Task_500ms */
/**
* @brief Function implementing the Task_10 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Task_500ms */
void Task_500ms(void const * argument)
{
  /* USER CODE BEGIN Task_500ms */

	task_init_500ms();

  /* Infinite loop */
  for(;;)
  {
	  task_manager_500ms();
	  osDelay(500);
  }
  /* USER CODE END Task_500ms */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
