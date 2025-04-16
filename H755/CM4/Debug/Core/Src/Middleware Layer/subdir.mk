################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Middleware\ Layer/INS_processing.c 

OBJS += \
./Core/Src/Middleware\ Layer/INS_processing.o 

C_DEPS += \
./Core/Src/Middleware\ Layer/INS_processing.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Middleware\ Layer/INS_processing.o: ../Core/Src/Middleware\ Layer/INS_processing.c Core/Src/Middleware\ Layer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H755xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Core/Src/Middleware Layer/INS_processing.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Middleware-20-Layer

clean-Core-2f-Src-2f-Middleware-20-Layer:
	-$(RM) ./Core/Src/Middleware\ Layer/INS_processing.cyclo ./Core/Src/Middleware\ Layer/INS_processing.d ./Core/Src/Middleware\ Layer/INS_processing.o ./Core/Src/Middleware\ Layer/INS_processing.su

.PHONY: clean-Core-2f-Src-2f-Middleware-20-Layer

