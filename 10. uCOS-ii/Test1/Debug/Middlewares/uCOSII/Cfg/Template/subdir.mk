################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/uCOSII/Cfg/Template/app_hooks.c 

OBJS += \
./Middlewares/uCOSII/Cfg/Template/app_hooks.o 

C_DEPS += \
./Middlewares/uCOSII/Cfg/Template/app_hooks.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/uCOSII/Cfg/Template/%.o Middlewares/uCOSII/Cfg/Template/%.su Middlewares/uCOSII/Cfg/Template/%.cyclo: ../Middlewares/uCOSII/Cfg/Template/%.c Middlewares/uCOSII/Cfg/Template/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Middlewares/uCOSII/Ports/ARM-Cortex-M/ARMv7-M/GNU -I../Middlewares/uCOSII/Source -I../Middlewares/uCOSII/Cfg/Template -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-uCOSII-2f-Cfg-2f-Template

clean-Middlewares-2f-uCOSII-2f-Cfg-2f-Template:
	-$(RM) ./Middlewares/uCOSII/Cfg/Template/app_hooks.cyclo ./Middlewares/uCOSII/Cfg/Template/app_hooks.d ./Middlewares/uCOSII/Cfg/Template/app_hooks.o ./Middlewares/uCOSII/Cfg/Template/app_hooks.su

.PHONY: clean-Middlewares-2f-uCOSII-2f-Cfg-2f-Template

