################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Global/Global_Definitions.c 

OBJS += \
./Core/Src/Global/Global_Definitions.o 

C_DEPS += \
./Core/Src/Global/Global_Definitions.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Global/%.o Core/Src/Global/%.su Core/Src/Global/%.cyclo: ../Core/Src/Global/%.c Core/Src/Global/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H755xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Global

clean-Core-2f-Src-2f-Global:
	-$(RM) ./Core/Src/Global/Global_Definitions.cyclo ./Core/Src/Global/Global_Definitions.d ./Core/Src/Global/Global_Definitions.o ./Core/Src/Global/Global_Definitions.su

.PHONY: clean-Core-2f-Src-2f-Global

