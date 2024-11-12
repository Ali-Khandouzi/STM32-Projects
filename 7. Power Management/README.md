# 7. Power Management
	
	PowerManagement1
	In this project, we show how to switch between different power modes in stm32 MCUs.
	We use an STM32L476RGTx MCU and we switch between Run, Sleep, Stop and Standby mode.
	By default, and after power-on or a system reset, STM32L MCUs are placed in run mode and MSI is the main clock source.
	
	Before we can perform any operation related to power management, we need to enable the PWR peripheral using the macro __HAL_RCC_PWR_CLK_ENABLE().
	
	To enter sleep mode we execute the HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI).
	All GPIOs are configured as analog, to reduce current consumption on non-used IOs.
	Any peripheral interrupt from the NVIC can wake up the device from sleep mode (We use the USER button).
	
	The corresponding peripheral clock is turned OFF, except for the GPIOC peripheral: the PC13 GPIO is used to resume from low-power modes. The same applies to the
	UART2 interface and the SysTick timer.
	The stop mode is almost the same, except that we call the function HAL_PWR_EnterSTOPMode(). We also need to call the SystemClock_Config() function to
	restore the clock at the original settings to allow the USART2 working properly.
	
	Entering standby mode needs a little more configuration. we have to disable the PWR_WAKEUP_PIN1 pin first, then clear the
	wake-up flag in the PWR->CSR peripheral and then, re-enable the wake-up pin again. After that, we can enter standby mode using HAL_PWR_EnterSTANDBYMode() function.
	