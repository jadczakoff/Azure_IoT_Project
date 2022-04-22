################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/VL53L0X/vl53l0x_api.c \
../Drivers/BSP/Components/VL53L0X/vl53l0x_api_calibration.c \
../Drivers/BSP/Components/VL53L0X/vl53l0x_api_core.c \
../Drivers/BSP/Components/VL53L0X/vl53l0x_api_ranging.c \
../Drivers/BSP/Components/VL53L0X/vl53l0x_api_strings.c \
../Drivers/BSP/Components/VL53L0X/vl53l0x_platform.c \
../Drivers/BSP/Components/VL53L0X/vl53l0x_platform_log.c \
../Drivers/BSP/Components/VL53L0X/vl53l0x_proximity.c 

OBJS += \
./Drivers/BSP/Components/VL53L0X/vl53l0x_api.o \
./Drivers/BSP/Components/VL53L0X/vl53l0x_api_calibration.o \
./Drivers/BSP/Components/VL53L0X/vl53l0x_api_core.o \
./Drivers/BSP/Components/VL53L0X/vl53l0x_api_ranging.o \
./Drivers/BSP/Components/VL53L0X/vl53l0x_api_strings.o \
./Drivers/BSP/Components/VL53L0X/vl53l0x_platform.o \
./Drivers/BSP/Components/VL53L0X/vl53l0x_platform_log.o \
./Drivers/BSP/Components/VL53L0X/vl53l0x_proximity.o 

C_DEPS += \
./Drivers/BSP/Components/VL53L0X/vl53l0x_api.d \
./Drivers/BSP/Components/VL53L0X/vl53l0x_api_calibration.d \
./Drivers/BSP/Components/VL53L0X/vl53l0x_api_core.d \
./Drivers/BSP/Components/VL53L0X/vl53l0x_api_ranging.d \
./Drivers/BSP/Components/VL53L0X/vl53l0x_api_strings.d \
./Drivers/BSP/Components/VL53L0X/vl53l0x_platform.d \
./Drivers/BSP/Components/VL53L0X/vl53l0x_platform_log.d \
./Drivers/BSP/Components/VL53L0X/vl53l0x_proximity.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/VL53L0X/%.o: ../Drivers/BSP/Components/VL53L0X/%.c Drivers/BSP/Components/VL53L0X/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4S5xx -DTX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" @"Drivers/BSP/Components/VL53L0X/vl53l0x_api.c_includes.args"

clean: clean-Drivers-2f-BSP-2f-Components-2f-VL53L0X

clean-Drivers-2f-BSP-2f-Components-2f-VL53L0X:
	-$(RM) ./Drivers/BSP/Components/VL53L0X/vl53l0x_api.d ./Drivers/BSP/Components/VL53L0X/vl53l0x_api.o ./Drivers/BSP/Components/VL53L0X/vl53l0x_api_calibration.d ./Drivers/BSP/Components/VL53L0X/vl53l0x_api_calibration.o ./Drivers/BSP/Components/VL53L0X/vl53l0x_api_core.d ./Drivers/BSP/Components/VL53L0X/vl53l0x_api_core.o ./Drivers/BSP/Components/VL53L0X/vl53l0x_api_ranging.d ./Drivers/BSP/Components/VL53L0X/vl53l0x_api_ranging.o ./Drivers/BSP/Components/VL53L0X/vl53l0x_api_strings.d ./Drivers/BSP/Components/VL53L0X/vl53l0x_api_strings.o ./Drivers/BSP/Components/VL53L0X/vl53l0x_platform.d ./Drivers/BSP/Components/VL53L0X/vl53l0x_platform.o ./Drivers/BSP/Components/VL53L0X/vl53l0x_platform_log.d ./Drivers/BSP/Components/VL53L0X/vl53l0x_platform_log.o ./Drivers/BSP/Components/VL53L0X/vl53l0x_proximity.d ./Drivers/BSP/Components/VL53L0X/vl53l0x_proximity.o

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-VL53L0X

