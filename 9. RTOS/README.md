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

	RTOS-4
	In this example instead of queue, we use a semaphore to control the access of threads. 
	This is again the classical blinking application, but this time the delay of the blinkThread() is established by another thread, delayThread(), which “unlock” the blinking thread by releasing a binary semaphore. 
	First, we create a binary semaphore with ID semiID. The semaphore is immediately acquired after creation by osSemaphoreAcquire(semID, osWaitForever) in the main code, causing its counter to become equal to zero.
	The blinkThread() and delayThread() are scheduled, but the blinkThread() is placed in blocked state as soon as it reaches the osSemaphoreAcquire() in its function.
	Being the semaphore already “acquired”, the thread will be swapped to the delayThread() until it release the semaphore by osSemaphoreRelease(semID), which performs this operation every 500ms.
	This will cause the LD2 LED to blink at a 2Hz rate.

	RTOS-5
	In this example, we're using Event Flags to trigger execution states between threads (also possible between ISR and threads).
	The thread blinkThread() is placed in blocked state until the flag FLAG_LED_BLINK is set by the delayThread(), which is by default every 500ms causing the LD2 LED to blink.
	But, when the USER BUTTON is pressed, the corresponding EXTI callback sets the flag FLAG_CHANGE_FREQUENCY, causing the delay variable to be decremented by determined steps and increasing the blinking frequency.
	The result is shown in "./Videos/RTOS-5.mp4".
	
	RTOS-6
	In this example, we made the famous blink application using a software timer.
	Software timers are the way an RTOS provides to schedule the execution of routines on a time-basis.
	There is no need to specific hardware support for software timers, except for the timer used as tick generator for the OS.

	RTOS-8
	When threads are spawned dynamically, it is hard to keep track of their lifecycle. FreeRTOS provides a way to retrieve both the complete list of live threads and some relevant information regarding their status.
	The uxTaskGetNumberOfTasks() function returns the number of live threads and the uxTaskGetSystemState() function returns the status information of every thread in the system.
	The uxTaskGetSystemState() for each thread, accepts  the maximum number of elements that the array can hold (uxArraySize) and a pointer to a variable (pulTotalRunTime) that will contain the total run-time since the kernel started.
	This means that, FreeRTOS can collect information on the amount of processing time that has been used by each thread.
	We need to enable the configGENERATE_RUN_TIME_STATS macro in the FreeRTOSConfig.h for this matter. 
	Moreover, since this feature requires that we use another timer different from the one used to feed the tick counter, we use TIM3 as tick generator.
	This because the run-time statistics timebase needs to have a higher resolution than the tick interrupt, otherwise the statistics may be too inaccurate to be truly useful.
	When the configGENERATE_RUN_TIME_STATS macro is set to 1, we have to provide two additional macros. The first one, portCONFIGURE_TIMER_FOR_RUN_TIME_STATS(), is used to setup the timer needed for run-time statistics. 
	The second one, portGET_RUN_TIME_COUNTER_VALUE(), is used by FreeRTOS to retrieve the cumulative value of the timer counter. These two macos are also defined in the FreeRTOSConfig.h.
	
	We have two main thread in the code. The first one is the typical blinkthread and the otherone called threadsDumpThread() is use to collect and print the information through the UART on Nucleo VCP.
	The threadsDumpThread() works this way: When the USER button is pressed, its allocates a buffer (pxTaskStatusArray) that will contain the TaskStatus_t structures for each thread in the system.
	The uxTaskGetSystemState() populates this array, and for each thread contained in it some statistics are get printed.
	The information inculeds the threads name and IDs (even the idle threads are shown), status(blocked, running, ready), Priority, Runtime and Runtime in percentage. 
	As it shown in the "./Videos/RTOS-8.mp4".









