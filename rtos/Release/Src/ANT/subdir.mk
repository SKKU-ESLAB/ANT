################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/ANT/network.c \
../Src/ANT/network_data.c \
../Src/ANT/network_lib0.c 

C_DEPS += \
./Src/ANT/network.d \
./Src/ANT/network_data.d \
./Src/ANT/network_lib0.d 

OBJS += \
./Src/ANT/network.o \
./Src/ANT/network_data.o \
./Src/ANT/network_lib0.o 


# Each subdirectory must supply rules for building sources it contributes
Src/ANT/network.o: ../Src/ANT/network.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F746xx -c -I../Src -I../Drivers/BSP/STM32746G-Discovery -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ANT/network.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ANT/network_data.o: ../Src/ANT/network_data.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F746xx -c -I../Src -I../Drivers/BSP/STM32746G-Discovery -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ANT/network_data.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Src/ANT/network_lib0.o: ../Src/ANT/network_lib0.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F746xx -c -I../Src -I../Drivers/BSP/STM32746G-Discovery -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/ANT/network_lib0.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

