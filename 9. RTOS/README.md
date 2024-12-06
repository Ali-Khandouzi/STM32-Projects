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
	As it shown in the "./Videos/RTOS-2.mp4".
	
	RTOS-3
	In this example, we use a queue to transfer data between two threads. The UARTThread() in the last example is considered producer and the blinkThread() is considered consumer.
	We can send the delay value from UARTThread() to blinkThread() using a queue. The UARTThread uses the I/O retargeting technique, to use the classical printf()/scanf() routines.
	The blinkThread() takes these values from the queue and uses them as delay values for the osDelay() function. It's shown in the "./Videos/RTOS-3.mp4".
	We can also check if the queue works if we pass several delay. For this purpose, if we specify a large delay value like 10000, the consumer runs slower than the producer.
	We can then immediately put another delay value inside the queue. The frequency changes after 10 sec and we can see the new LED starts blinking at new rate, since blinkThread() is blocked by the osDelay() function.
	"./Videos/RTOS-3-2.mp4".