################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Inc/runtime/ai_runtime_api.c \
../Inc/runtime/runtime.c 

C_DEPS += \
./Inc/runtime/ai_runtime_api.d \
./Inc/runtime/runtime.d 

OBJS += \
./Inc/runtime/ai_runtime_api.o \
./Inc/runtime/runtime.o 


# Each subdirectory must supply rules for building sources it contributes
Inc/runtime/ai_runtime_api.o: ../Inc/runtime/ai_runtime_api.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F746xx -c -I../Src -I../Drivers/BSP/STM32746G-Discovery -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Inc/runtime/ai_runtime_api.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Inc/runtime/runtime.o: ../Inc/runtime/runtime.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F746xx -c -I../Src -I../Drivers/BSP/STM32746G-Discovery -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Inc/runtime/runtime.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

