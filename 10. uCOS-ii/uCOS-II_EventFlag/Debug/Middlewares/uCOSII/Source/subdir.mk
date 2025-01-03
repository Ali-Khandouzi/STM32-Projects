################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/uCOSII/Source/os_core.c \
../Middlewares/uCOSII/Source/os_flag.c \
../Middlewares/uCOSII/Source/os_mbox.c \
../Middlewares/uCOSII/Source/os_mem.c \
../Middlewares/uCOSII/Source/os_mutex.c \
../Middlewares/uCOSII/Source/os_q.c \
../Middlewares/uCOSII/Source/os_sem.c \
../Middlewares/uCOSII/Source/os_task.c \
../Middlewares/uCOSII/Source/os_time.c \
../Middlewares/uCOSII/Source/os_tmr.c \
../Middlewares/uCOSII/Source/ucos_ii.c 

OBJS += \
./Middlewares/uCOSII/Source/os_core.o \
./Middlewares/uCOSII/Source/os_flag.o \
./Middlewares/uCOSII/Source/os_mbox.o \
./Middlewares/uCOSII/Source/os_mem.o \
./Middlewares/uCOSII/Source/os_mutex.o \
./Middlewares/uCOSII/Source/os_q.o \
./Middlewares/uCOSII/Source/os_sem.o \
./Middlewares/uCOSII/Source/os_task.o \
./Middlewares/uCOSII/Source/os_time.o \
./Middlewares/uCOSII/Source/os_tmr.o \
./Middlewares/uCOSII/Source/ucos_ii.o 

C_DEPS += \
./Middlewares/uCOSII/Source/os_core.d \
./Middlewares/uCOSII/Source/os_flag.d \
./Middlewares/uCOSII/Source/os_mbox.d \
./Middlewares/uCOSII/Source/os_mem.d \
./Middlewares/uCOSII/Source/os_mutex.d \
./Middlewares/uCOSII/Source/os_q.d \
./Middlewares/uCOSII/Source/os_sem.d \
./Middlewares/uCOSII/Source/os_task.d \
./Middlewares/uCOSII/Source/os_time.d \
./Middlewares/uCOSII/Source/os_tmr.d \
./Middlewares/uCOSII/Source/ucos_ii.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/uCOSII/Source/%.o Middlewares/uCOSII/Source/%.su Middlewares/uCOSII/Source/%.cyclo: ../Middlewares/uCOSII/Source/%.c Middlewares/uCOSII/Source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU -I../Middlewares/uCOSII/Source -I../Middlewares/uCOSII/Cfg/Template -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-uCOSII-2f-Source

clean-Middlewares-2f-uCOSII-2f-Source:
	-$(RM) ./Middlewares/uCOSII/Source/os_core.cyclo ./Middlewares/uCOSII/Source/os_core.d ./Middlewares/uCOSII/Source/os_core.o ./Middlewares/uCOSII/Source/os_core.su ./Middlewares/uCOSII/Source/os_flag.cyclo ./Middlewares/uCOSII/Source/os_flag.d ./Middlewares/uCOSII/Source/os_flag.o ./Middlewares/uCOSII/Source/os_flag.su ./Middlewares/uCOSII/Source/os_mbox.cyclo ./Middlewares/uCOSII/Source/os_mbox.d ./Middlewares/uCOSII/Source/os_mbox.o ./Middlewares/uCOSII/Source/os_mbox.su ./Middlewares/uCOSII/Source/os_mem.cyclo ./Middlewares/uCOSII/Source/os_mem.d ./Middlewares/uCOSII/Source/os_mem.o ./Middlewares/uCOSII/Source/os_mem.su ./Middlewares/uCOSII/Source/os_mutex.cyclo ./Middlewares/uCOSII/Source/os_mutex.d ./Middlewares/uCOSII/Source/os_mutex.o ./Middlewares/uCOSII/Source/os_mutex.su ./Middlewares/uCOSII/Source/os_q.cyclo ./Middlewares/uCOSII/Source/os_q.d ./Middlewares/uCOSII/Source/os_q.o ./Middlewares/uCOSII/Source/os_q.su ./Middlewares/uCOSII/Source/os_sem.cyclo ./Middlewares/uCOSII/Source/os_sem.d ./Middlewares/uCOSII/Source/os_sem.o ./Middlewares/uCOSII/Source/os_sem.su ./Middlewares/uCOSII/Source/os_task.cyclo ./Middlewares/uCOSII/Source/os_task.d ./Middlewares/uCOSII/Source/os_task.o ./Middlewares/uCOSII/Source/os_task.su ./Middlewares/uCOSII/Source/os_time.cyclo ./Middlewares/uCOSII/Source/os_time.d ./Middlewares/uCOSII/Source/os_time.o ./Middlewares/uCOSII/Source/os_time.su ./Middlewares/uCOSII/Source/os_tmr.cyclo ./Middlewares/uCOSII/Source/os_tmr.d ./Middlewares/uCOSII/Source/os_tmr.o ./Middlewares/uCOSII/Source/os_tmr.su ./Middlewares/uCOSII/Source/ucos_ii.cyclo ./Middlewares/uCOSII/Source/ucos_ii.d ./Middlewares/uCOSII/Source/ucos_ii.o ./Middlewares/uCOSII/Source/ucos_ii.su

.PHONY: clean-Middlewares-2f-uCOSII-2f-Source

