# 6. I2C
	
	I2C-1
	In this project, we use I2C protocol to write/read data on/from a 64Kbits EEPROM. We also check if the written data is correct.
	To do this, we store a string inside the EEPROM, and then reading it back.
	If the original string is equal to the one read from the EEPROM, then the Nucleo LD2 LED starts blinking.
	We configure PB8 and PB9 pins as SCL and SDA lines of I2C.
	The string stores at the 0x1AAA memory location.
	The "./Videos/I2C-1.mp4" shows the result and also shows how we suppose to wire a 64Kbits EEPROM to MCU.