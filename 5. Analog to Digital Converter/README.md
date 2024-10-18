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





