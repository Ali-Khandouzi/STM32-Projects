# 9. RTOS
	
	RTOS-1
	In this project, we want to blink a LED using a Thread. It starts by defining the Thread name "blinkThread" and specifying a stack size equal to 512 bytes and assigning a normal priority.
	In the main() routine, the OS scheduler is initialize by calling the "osKernelInitialize()" and the "osThreadNew()" creates the new thread and asks to the kernel to schedule its execution.
	Finally, we start the kernel scheduler by using the function osKernelStart(). The function blinkThread() is nothing more than the usuall blinking application. But instead of the classical HAL_Delay(),
	we have to use the osDelay() function.
	
	RTOS-2
	In this project we have two threads, one that blinks the LD2 LED and one that constantly prints on the UART2 a message.
	The UARTThread() is created with a priority higher than the blinkThread() one. Running this example, you can see that the LD2 LED never blinks.
	But when both priorities are equal, the CPU keep switching between two threads, and its like theyre running at the same time.
	As it shown in the "./Videos/RTOS-2.mp4"
	