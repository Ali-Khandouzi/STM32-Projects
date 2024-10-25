# 4. Timers

	Timer ex1
	In this example, we use a Basic Timer (Timer 6) to blink an LED.
	We understood the relation between Prescaler, Period and the Timer Counter speed.
	Since the Internal Clock Source frequency of a Nucleo L476RG is 80MHz,
	By setting the Prescaler on 39999, we reach the 2000Hz frequency for the timer counter.
	(80 MHz / 39999 + 1) = 2000Hz
	Also, by setting the Period on 999, we reach the 2Hz frequency, which means LED2 blinks 
	every half a second.
	(2000Hz / 999 + 1) = 2Hz
	This means that every half a second, the timer overflows, and the Update Event is generated. 
	Since we already activated the Timer 6 Interrupt, every 0.5 seconds, an interrupt fires, which causes Toggling pin 2 in port A.
	
	Timer ex2 
	In this example, we use Timers in DMA mode. Timer 5 is a general purpose timer
	and DMA2 in our board can access it using channel 2.
	If we use DMA in circular mode, we can send the data[] = {0xFF, 0x0}, multiple times
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
	We connect them using a jumper, shown in the video "Ex3.mp4".

	Timer ex4
	There are other ways to use external clock sources for timers. With the ETR2 pin, there is no need to set the timer to work in slave mode.
	But if it does so, it can be clocked by a signal connected to the ETR1 pin, by any trigger source connected to the internal lines ITR0…ITR2 (hence, the clock source
	can be only another timer), or by an input signal connected to the timer channels TI1 and TI2,
	which becomes TI1FP1 and TI2FP2 if the input filtering stage is activated.
	In this example, we route the Master Clock Output (MCO) pin to the TI2FP2 pin to use an external clock, as shown in the video "Ex4.mp4".
	
	Timer ex5
	This example shows how to use timers in Master/Slave mode. 
	We use Timer2 as the master of Timer3. Timer2 is connected to Timer3 through the TRGO and ITR1. Also, Timer2 is activated with an external trigger. 
	So anytime we trigger Timer2, it orders Timer3 to start counting at 2Hz. You can see that LED2 starts blinking every 500ms. 
	The result is shown in the video "Ex5.mp4."
	
	Timer ex6
	General-purpose and advanced timers allow us to compute the frequency of external signals.
	In this example, we set the LED2 frequency at 50Khz with the help of TIM6(). The input signal is demonstrated in "./Videos/Ex6-TIM6-signal.jpg"
	Then, We configure the Channel 1 of Timer2 as input capture. We have to configure it in DMA mode, because it can triggers the TIM_DMA_ID_CC1 request to fill a temporary buffer that stores the value of
	the Timer counter register(TIM2_CNT) when the rising edge of input signal is detected.
	To measure the frequency, we need to connect the PA5 pin (LED2) to the PA0 (TIM2_CH1) as it shown in video "Ex6.mp4".

	Timer ex7
	This example is about output compare mode. We control Timer2 CH1 and CH2 outputs in "Output compare toggle" mode.
	This will done by comparing the value of the channel compare register (TIMx_CCRx) and the timer counter register (TIMx_CNT).
	When these two value matches, a specific interrupt is generated (it's enabled), This allows to control the switching frequency of each channel independently, 
	and eventually perform phase shift between channel.
	In this example we show how to generate two output square wave signals, one running at 25kHz and one at 12.5kHz. Shown in video "EX7.mp4."
	
	Timer ex8
	Timers can also be used for Pulse-width modulation (PWM). The square waves generated until now, all have a 50% duty cycle.
	Using PWM technique, we can generate several pulses with different duty cycles at a given frequency.
	In this example, we use PWM to dimming LED2. We configure the first channel of timer TIM2 to work in PWM Mode 1.
	There are two main loop in the code. The first one increments the value of the Pulse(Capture Compare Register 1 (CCR1)) up to the Period value (Auto Reload Register (ARR)) every 1ms.
	And the second one, in the same way, decrements the Pulse field unless it reaches zero.
	The result is shown in video "EX8.mp4."

	Timer ex9
	In this example, we make a sine wave using PWM. This can be done by changing the output voltage(duty cycle).
	We can generate an output waveform with a frequency, that is a fraction of the PWM frequency. We divide the sine wave into 200 of divisions and each division have a single PWM cycle.
	A PWM pulse with 0% duty cycle will represent the min amplitude (0V), the one with 100% duty cycle will represent max amplitude(3.3V).
	To generate a 50Hz sine wave, we have to set the prescaler on 40 based on this formula:
	Prescaler = (80MHZ *Clock freq.)/ 50HZ * 200 divisions * 200 Pusle period = 40.
	The DMA1_Channel4 is configured to work in circular mode, so that it automatically sets the value of the TIMx_CCRx register according to the Pulse values contained in IV vector.
	The result shown in video "Ex9.mp4". Note that, our logic analyzer does not show the analog signals. But the PWM signal is demonstrated.
	
	Timer ex10


	




