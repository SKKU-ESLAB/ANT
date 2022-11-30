################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/s5k5cag/s5k5cag.c 

C_DEPS += \
./Drivers/BSP/Components/s5k5cag/s5k5cag.d 

OBJS += \
./Drivers/BSP/Components/s5k5cag/s5k5cag.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/s5k5cag/s5k5cag.o: ../Drivers/BSP/Components/s5k5cag/s5k5cag.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -DSTM32F746xx -c -I../Src -I../Drivers/BSP/STM32746G-Discovery -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include" -I"D:/ANT/tinyengine/TinyEngine_vww_tutorial/Src/TinyEngine/include/arm_cmsis" -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/BSP/Components/s5k5cag/s5k5cag.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

