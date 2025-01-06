# 10. uCOS-II

	************** ATTENTION ********************
	There are three steps you need to take, before using following codes.
	1. You should add the "Middlewares" folder to the resources of your project. In Cube IDE: "Project tab - Properties for your project - C/C++ General - Path and Symbols - Source Location"
	2. Also add the "Include paths". In Cube IDE: "Project tab - Properties - C/C++ Build - Settings - MCU/MPU GCC Compiler - Include paths".
	3. Open the "./Core/Startup/startup_stm32xxxxxx.s". In Line 148 and 149: Change the "PendSV_Handler" and "SysTick_Handler" to "OS_CPU_PendSVHandler" and "OS_CPU_SysTickHandler" respectively.
	   Samething goes for Line 263 to 267.

	Test-1
	In this project, we want to use uCOS_II RTOS to make Threads.
	We defined two tasks and specified a stack size equal to 512 bytes and assigning their priorities.
	It's a simple project, a task just blink a LED and the other sending message using UART.
	
	uCOS-II_Semaphore
	In this example, we learn how to create a semaphore using uCOS-II.
	There are two tasks in this example and both want to use UART to send their messages. UART is a shared resource then and must be protected.
	If the higher priority task preempts the lower priority task in the middle of its job, this preemption is more likely to lead to data corroption.
	To avoid that, we can use semaphores. A semaphore can act like a key. Each task that acquires it can be executed and a preemption not going to ruin data, since the higher priority task must acquire the key first.
	And they must be released from the previous task. That's how it works! Hope I explain it well!

	uCOS-II_Mutex