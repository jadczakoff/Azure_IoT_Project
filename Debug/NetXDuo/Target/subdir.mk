################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../NetXDuo/Target/nx_driver_stm32l4.c 

OBJS += \
./NetXDuo/Target/nx_driver_stm32l4.o 

C_DEPS += \
./NetXDuo/Target/nx_driver_stm32l4.d 


# Each subdirectory must supply rules for building sources it contributes
NetXDuo/Target/%.o: ../NetXDuo/Target/%.c NetXDuo/Target/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4S5xx -DTX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" @"NetXDuo/Target/nx_driver_stm32l4.c_includes.args"

clean: clean-NetXDuo-2f-Target

clean-NetXDuo-2f-Target:
	-$(RM) ./NetXDuo/Target/nx_driver_stm32l4.d ./NetXDuo/Target/nx_driver_stm32l4.o

.PHONY: clean-NetXDuo-2f-Target

