# 4. Timers

	Timer ex1
	In this example, we use a Basic Timer (Timer 6) to blink an LED.
	We understood the relation between Prescaler, Period and the Timer Counter speed.
	Since the Internal Clock Source frequency of a Nucleo L476RG is 80MHz,
	By setting the Prescaler on 39999, we reach the 2000Hz frequency for timer counter.
	(80 MHz / 39999 + 1) = 2000Hz
	Also, by setting the Period on 999, we reach the 2Hz frequency, which means LED2 blinks 
	every half a second.
	(2000Hz / 999 + 1) = 2Hz
	This means, every half a second the timer overflow and Update Event generated. 
	Since we already activate the Timer 6 Interrupt, every 0.5s an interrupt fires, which causes Toggle pin 2 in port A.
	
	
	Timer ex2 
	In this example, we use Timers in DMA mode. Timer 5 is a general purpose timer
	and DMA2 in our board can access it using channel 2.
	If we use DMA on circullar mode, we can send the data[] = {0xFF, 0x0}, multiple times
	to the GPIOA pin 2. 
	By setting the Prescaler and Period, we determine how often the timer overflows and a TIM6_UP request is generated.
	The data buffer is transferred inside the GPIOA->ODR register.
	This causes the LD2 LED to blink.

	
	Timer ex3
	This example is about how to use an External Clock Source for Timers.
	Timer 3 on our board can access an external clock source in mode 2.
	It's a general-purpose timer that has an ETR2 pin (TIM3_ETR2). 
	By connecting TIM3_ETR2 to the Master Clock Output(MCO) pin, we can give it an external clock. 
	MCO pin in Nucleo L476RG corresponds to the PA8 pin, and ETR2 in all Nucleos corresponds to the PD2 pin.
	We connect them using a jumper, shown in the Video Ex3.mp4.





