################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/RTOS\ Layer/Task_manager.c 

OBJS += \
./Core/Src/RTOS\ Layer/Task_manager.o 

C_DEPS += \
./Core/Src/RTOS\ Layer/Task_manager.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/RTOS\ Layer/Task_manager.o: ../Core/Src/RTOS\ Layer/Task_manager.c Core/Src/RTOS\ Layer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32H755xx -c -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Core/Src/RTOS Layer/Task_manager.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-RTOS-20-Layer

clean-Core-2f-Src-2f-RTOS-20-Layer:
	-$(RM) ./Core/Src/RTOS\ Layer/Task_manager.cyclo ./Core/Src/RTOS\ Layer/Task_manager.d ./Core/Src/RTOS\ Layer/Task_manager.o ./Core/Src/RTOS\ Layer/Task_manager.su

.PHONY: clean-Core-2f-Src-2f-RTOS-20-Layer

