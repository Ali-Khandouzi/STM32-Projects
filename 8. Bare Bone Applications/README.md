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
	This is almost the same as the previous example. But this time we use a global initialized variable, dataVar to start the blinking loop. If we only define the dataVar and use it in a while loop, the board doesn't blink.
	This is because the dataVar is a global variable and needs to be initialized by a specific initialization function.
	Local variables are initialized during the calling process of their functions. But for the global variables, we need to copy the content of the flash region containing the initialization values to the SRAM region dedicated to global initialized variables.
	Before we can do that, we need to instruct LD to store the initialization values inside a specific region of the flash memory(LMA). Then we should pass the start and the end of the .data section in SRAM to the __initialize_data() function.
	Also, we need to pass the starting location where initialization values are stored in the flash memory to the same function.
	
	BareBone3
	The same goes for the .bss section, which is reserved for uninitialized variables. the content of .bss must be initialized to 0.
	The difference with the .data section is that the .bss does not have a corresponding flash region containing all zeros.
	Besides the linker script, the code should contain the bss initialization routine.

	BareBone4
	In case we wanted to use constant data such as strings and numeric constants or arrays, these datas should placed in flash memory(internal or external through the Quad-SPI) to save SRAM space.
	This can be achieved defining the .rodata section inside the linker script.

	BareBone5
	In case we need to allocate dynamically some portions of the memory, one way is the use of malloc() routine.
	We define a pointer to a memory region that dynamically allocated by malloc(). Then, we simply copy the content of the msg string and check if both strings are equal. If so, the LD2 LED starts blinking.
	Since the malloc() relies on the _sbrk() routine, we shoulf implement that too.
	The _sbrk() is a routine that accepts the amount of bytes to allocate inside the heap memory and returns the pointer to the start of this contiguous “chunk” of memory.
	We should define the end of .bss section, the dimension of heap and the final address of heap memory inside the linker script.
	Since MCUs have limited memory resources and it is easy to exceed the maximum SRAM memory, we should use the linker script also to add a sort of “static” checking about the maximum memory usage.
	This is done by defining a minimum size of stack inside the linker script. Also, using the location counter operator (“.”) we increment the size of this section so that it has a dimension equal
	to the maximum heap size and the “estimated” minimum stack size. 







