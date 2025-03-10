/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "includes.h"
#include <string.h>
#include "retarget.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void task1(void* pdata);
void task2(void* pdata);
void task3(void* pdata);

/* Definition of Task Stacks ------------------------------------------------ */
#define TASK_STACKSIZE 		128 * 4
OS_STK	task1_stk[TASK_STACKSIZE];
OS_STK	task2_stk[TASK_STACKSIZE];
OS_STK	task3_stk[TASK_STACKSIZE];

/* Definition of Task Priorities -------------------------------------------- */
#define TASK1_PRIORITY		6
#define TASK2_PRIORITY		7
#define TASK3_PRIORITY		8

/* Semaphore to protect UART ------------------------------------------------ */
OS_EVENT *Shared_Semaphore;

/* Message Queue EVENT structure -------------------------------------------- */
OS_EVENT *Queue1;
#define QSIZE 				10

/* Storage for message pointers --------------------------------------------- */
void *queue_data[QSIZE];

typedef struct msg_data {
	int time;
	int taskID;
	int msgNo;
}MSG_DATA;

MSG_DATA msg_t1;
MSG_DATA msg_t2;

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  RetargetInit(&huart2);

  OSInit();

  /* Create semaphore to protect UART */
  Shared_Semaphore = OSSemCreate(1);

  Queue1 = OSQCreate(&queue_data[0], 10);

  OSTaskCreateExt(task1,
  					  NULL,
  					  &task1_stk[TASK_STACKSIZE-1],
  					  TASK1_PRIORITY,
  					  TASK1_PRIORITY,
  					  &task1_stk[0],
  					  TASK_STACKSIZE,
  					  NULL,
  					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
  					  );

    OSTaskCreateExt(task2,
      		  	  	  NULL,
      				  &task2_stk[TASK_STACKSIZE-1],
      				  TASK2_PRIORITY,
      				  TASK2_PRIORITY,
      				  &task2_stk[0],
      				  TASK_STACKSIZE,
      				  NULL,
  					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
      				  );

    OSTaskCreateExt(task3,
					  NULL,
					  &task3_stk[TASK_STACKSIZE-1],
					  TASK3_PRIORITY,
					  TASK3_PRIORITY,
					  &task3_stk[0],
					  TASK_STACKSIZE,
					  NULL,
					  OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR
					  );

    OSStart();
    return 0;
}

/* Definition of Task1 ----------------------------------------------------------- */
void task1(void* pdata) {

	INT8U errorcode;
	int t1;
	int cnt = 1;

	while (1) {
		t1 = OSTimeGet();
		OSSemPend(Shared_Semaphore, 0, &errorcode);
		printf("Task1 sending message: time=%d ms, id=%d, msg_no=%d\r\n", t1, 1, cnt);
		OSSemPost(Shared_Semaphore);

		msg_t1.time = t1;
		msg_t1.taskID = 1;
		msg_t1.msgNo = cnt;
		errorcode = OSQPost(Queue1, (void *)&msg_t1);
		cnt = cnt+1;
		OSTimeDlyHMSM(0, 0, 10, 0);
	}
}

/* Definition of Task2 ----------------------------------------------------------- */
void task2(void* pdata) {

	INT8U errorcode;
	int t1;
	int cnt = 1;

	while (1) {
		t1 = OSTimeGet();
		OSSemPend(Shared_Semaphore, 0, &errorcode);
		printf("Task2 sending message: time=%d ms, id=%d, msg_no=%d\r\n", t1, 2, cnt);
		OSSemPost(Shared_Semaphore);

		msg_t2.time = t1;
		msg_t2.taskID = 2;
		msg_t2.msgNo = cnt;
		errorcode = OSQPost(Queue1, (void *)&msg_t2);
		cnt = cnt+1;
		OSTimeDlyHMSM(0, 0, 10, 0);
	}
}

/* Definition of Task3 ----------------------------------------------------------- */
void task3(void* pdata) {

	INT8U errorcode;
	int t1;
	MSG_DATA *msg_rx;
	MSG_DATA msg_local;
	while (1) {
		msg_rx = (MSG_DATA*)OSQPend(Queue1, 0, &errorcode);
		msg_local = *msg_rx;
		t1 = OSTimeGet();

		OSSemPend(Shared_Semaphore, 0, &errorcode);
		printf("Task3 received message: time=%d ms, id=%d, msg_no=%d (at %d ms)\r\n",
				msg_local.time, msg_local.taskID, msg_local.msgNo, t1);
		OSSemPost(Shared_Semaphore);
		OSTimeDlyHMSM(0, 0, 5, 0);
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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
