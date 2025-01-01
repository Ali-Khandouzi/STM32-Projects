################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_c.c \
../Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_dbg.c 

S_UPPER_SRCS += \
../Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.S 

OBJS += \
./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.o \
./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_c.o \
./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_dbg.o 

S_UPPER_DEPS += \
./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.d 

C_DEPS += \
./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_c.d \
./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_dbg.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/%.o: ../Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/%.S Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/%.o Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/%.su Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/%.cyclo: ../Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/%.c Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU -I../Middlewares/uCOSII/Source -I../Middlewares/uCOSII/Cfg/Template -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-uCOSII-2f-Ports-2f-ARM-2d-Cortex-2d-M-2f-ARMv7-2d-M-2f-GNU

clean-Middlewares-2f-uCOSII-2f-Ports-2f-ARM-2d-Cortex-2d-M-2f-ARMv7-2d-M-2f-GNU:
	-$(RM) ./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.d ./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_a.o ./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_c.cyclo ./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_c.d ./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_c.o ./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_cpu_c.su ./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_dbg.cyclo ./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_dbg.d ./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_dbg.o ./Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU/os_dbg.su

.PHONY: clean-Middlewares-2f-uCOSII-2f-Ports-2f-ARM-2d-Cortex-2d-M-2f-ARMv7-2d-M-2f-GNU

