# 5. Analog to Digital Converter

	ADC1 $ ADC2
	In ADC1 and ADC2 examples, we learned how to use ADC in polling and interrupt mode respectivley.
	In both examples, we use ADC temperture sensor. The output signal of sensor convert to celesius temp using formula.
	After temp calculation, the data transmit through the UART2 continuously.
	Since the temp sensor works every 10μs, we need to set te ADC sampling time on this value.
	The ADC works at 64MHz,performs 64 cycles every 1μs. If we want it to work every 10μs, the ADC_SAMPLETIME must set on 640CYCLES.
	In these examples, ADC works in Single channel and Continuous convertion mode.
	
	ADC3
	In this example, we use DMA to send ADC data. Using the DMA, we can choose multiple channel to convert without intervention of CPU.
	ADC works in Single convertion mode in this example, but we can also choose to convert continuously.
	All the settings can be done, using DMA_CIRCULAR mode, Enabling DMAContinuousRequests and Enabling ContinuousConvMode field in the hadc.Init structure.

	ADC4
	The ADC can also trigger by a timer through the TRGO trigger line. In this example, we use timer and DMA to send the temp data every 1 seconds.
	
	ADC5
	I just wanted to check if its possible to get the temp, when I press an external button like BP1 (The blue button on the board).
	And as you can guess, It works...
	
	DAC1
	The DAC peripheral can be driven manually or using the DMA and a trigger source like a Timer. In this example we drive it manualy.
	In nucleo L476RG, The Channel 2 of DAC1 is connected to LED2(PA5). This allows us to fade ON/OFF the LD2 using the DAC and infinit loops.
	The fading is shown in video "./Videos/DAC1.mp4". 

	DAC2
	A DAC normally use to genetarte an analog waveform with a given frequency. So, the best way to drive the DAC is by using the DMA and a timer to trigger the conversions.
	We already generate a sine wave using a timer PDM. Using a DAC, this can be carried out more easily.
	Like we seen before, to generate a sine wave, we need to divide it into several steps. We choose 200 steps in this example to generate a 50Hz sine wave.
	We also need to generate an initialization vector to feed the DAC in DMA mode. 
	The DAC will perform a convertion, anytime the TIM6 TRGO line is generated. Moreover, the DMA is configured accordingly, setting it in circular mode so that it transfers the content of the initialization vector in the DAC data register continuously.
	The result is shown in video "DAC2.mp4", but since our logic analyzer doesn't show analog wave, its the digital form of it.





