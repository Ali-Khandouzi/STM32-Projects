# 8. Bare Bone Applications
	
	BareBone1
	At the begining, we define Flash, SRAM and peripheral start addresses. These addresses are normally common between most of stm32 MCUs.
	We also define the SRAM size which is 96 KB in STM32L476RG (96 * 1024).
	RCC peripheral addresses also must be set in this minimal applications.
	Then, we define GPIOA peripheral address. GPIOA base address is 0x8000000 after Peripheral base. We also should define the MODER(General Purpose Output Mode) and ODR(Output Data Register).
	After that, we define the vector table. it just contains two things: the address in SRAM of the MSP and the pointer to the handler of the Reset exception.
	In the main function, we enable clock of the GPIOA using *RCC_AHB2ENR and set the GPIOA_MODER(MODER[11:10] = 0x1).
	The while loop is the same blinking LED program.

	After the source file, we should prepare a GNU LD linker script. The linker script contains the definition of the flash and SRAM memories.
	We set the attributes (w=writable, r=readable, x=executable), starting address and length for each region of memory.
	The main() function specifies the entry point of our application. Next, we define the .text section that composed by the vector table and the program code and placed inside the Flash memory.
	.data section also defined and placed inside the SRAM memory.
	
	BareBone2
	This is almost the same as previous example. But this time we use a global initialized variable, dataVar to start the blinking loop. If we only define the dataVar and use it in while loop, the board doesn't blinking.
	This is because of the dataVar is a global variable and needs to initialize it by a specific initialization function.
	Local variables are initialize during the calling process of their functions. But for the global variables, we need to copy the content of the flash region containing the initialization values to the SRAM region dedicated to global initialized variables.
	Before we can do that, we need to instruct LD to store the initialization values nside a specific region of the flash memory(LMA). Then we should pass the start and the end of .data section in SRAM to the __initialize_data() function.
	Also, we need to pass the starting location where initialization values are stored in the flash memory to the same function.
	
	BareBone3
	Samething goes for the .bss section, which is reserved to uninitialized variables. the content of .bss must be initialized to 0.
	The difference with .data section is that the .bss does not have a corresponding flash region containing all zeros.
	Besides of the linker script, The code also should contain the bss initialization routine.

	BareBone4
		
