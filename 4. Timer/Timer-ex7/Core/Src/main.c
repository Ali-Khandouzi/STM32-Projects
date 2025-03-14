/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32l4xx_hal.h"
#include <nucleo_hal_bsp.h>
#include <string.h>

TIM_HandleTypeDef htim2;

static void MX_TIM2_Init(void);

uint16_t computePulse(TIM_HandleTypeDef *htim, uint32_t chFrequency) {
	uint32_t timFrequency= HAL_RCC_GetHCLKFreq() / (htim->Instance->PSC + 1);
	return (uint16_t)(timFrequency / chFrequency);
}


volatile uint16_t CH1_FREQ = 0;
volatile uint16_t CH2_FREQ = 0;
int main(void)
{
  HAL_Init();
  Nucleo_BSP_Init();

  MX_TIM2_Init();
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_1);
  HAL_TIM_OC_Start_IT(&htim2, TIM_CHANNEL_2);

  while (1);
}

void MX_TIM2_Init(void) {

  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 10;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_OC_Init(&htim2);

  CH1_FREQ = computePulse(&htim2, 25000);
  CH2_FREQ = computePulse(&htim2, 50000);

  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = CH1_FREQ;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1);

  sConfigOC.Pulse = CH2_FREQ;
  HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2);
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {
  uint32_t pulse;
  uint16_t arr = __HAL_TIM_GET_AUTORELOAD(htim);

  /* TIMx_CH1 toggling with frequency = 50KHz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
    pulse = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    /* Set the Capture Compare Register value */
    if((pulse + CH1_FREQ) < arr)
      __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (pulse + CH1_FREQ));
    else
      __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (pulse + CH1_FREQ) - arr);
  }

  /* TIMx_CH2 toggling with frequency = 100KHz */
  if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2) {
  pulse = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
  /* Set the Capture Compare Register value */
  if((pulse + CH2_FREQ) < arr)
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, (pulse + CH2_FREQ));
  else
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, (pulse + CH2_FREQ) - arr);
  }
}

void HAL_TIM_OC_MspInit(TIM_HandleTypeDef* htim_base) {
  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim_base->Instance==TIM2) {
    __TIM2_CLK_ENABLE();

    /**TIM2 GPIO Configuration
    PA0     ------> TIM2_CH1
    PA1     ------> TIM2_CH2
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  }
}

void TIM2_IRQHandler(void) {
  HAL_TIM_IRQHandler(&htim2);
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
