# 2. UART and Intrupt Projects
 
	Kha4
	In this project, we learn how to use UART peripheral in polling mode.
	We learn how to use "HAL_UART_Transmit" function to transfer data from MCU to a PC.
	In this case, we send some options to user, so he can choose one of them.
	Option 1, is toggle LED2 on nucleo board.
	Option2, let us check if the blue push button(GPIOA, Pin13) is pressed or not.
	then print its status.
	Option3 is clear the terminal and start sending the options again.
	
	Data receivement is done using "HAL_UART_Receive" function.

	Since we using polling mode(blocking mode), we can wait for the data transfer completion.
	its because we set the delay on "HAL_MAX_DELAY" in both sending and receiving functions.
	

	Kha5
	This is the same project, but we are using UART in Intrupt mode.
	We can't wait for I/O data for too long, like polling mode, specialy if the core had more task to do.
	In intrupt mode, we let the core manage other tasks, and get the I/O data when an intrupt rises.
	
	So, We have to define the corresponding ISR and add the call to the "HAL_UART_IRQHandler()".
	Since we don't use "HAL_MAX_DELAY" anymore, we have to check that if the added data by user is received or not.
	This is done using "HAL_UART_RxCpltCallback".
	
	We should note that the functions used for transmit and receiving data in this mode are different from polling mode.
	"HAL_UART_Transmit_IT" and "HAL_UART_Receive_IT"
	
	The reason of changing the "printWelcomeMessage" function, is the use of "HAL_UART_Transmit_IT()".
	Since the function returns the control to the caller as soon as it finishes to setup the UART, 
	a subsequent call of the same function will fail, and it will return the HAL_BUSY value.
	It's because we use "HAL_UART_GetState()" function to check the UART status for avoiding possible errors.
	
	Also, it's good to mention that, the intrupt mode used, only when we exit the main cycle.
	for the function "processUserInput", since we are already got the user input, the polling mode uses to process it.

	
	Kha6
	A more elegant way to avoid errors, when calling "HAL_UART_Transmit_IT()" for multiple times, is the use of a circullar buffer.
	Here is a simple and fast implementation of circular buffer(Ring buffer).
	To use a  ringbuffer, add the "ringbuffer.c" file to "./Core/Src/".
	Also, need to add the "ringbuffer.h" file to ".Core/Inc/" directory.
	
	 





	