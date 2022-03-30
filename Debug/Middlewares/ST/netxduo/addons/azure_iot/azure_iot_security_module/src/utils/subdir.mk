################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/ievent_loop.c \
../Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/irand.c \
../Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/itime.c \
../Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/os_utils.c 

OBJS += \
./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/ievent_loop.o \
./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/irand.o \
./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/itime.o \
./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/os_utils.o 

C_DEPS += \
./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/ievent_loop.d \
./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/irand.d \
./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/itime.d \
./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/os_utils.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/%.o: ../Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/%.c Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4S5xx -DTX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" @"Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/ievent_loop.c_includes.args"

clean: clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-azure_iot-2f-azure_iot_security_module-2f-src-2f-utils

clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-azure_iot-2f-azure_iot_security_module-2f-src-2f-utils:
	-$(RM) ./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/ievent_loop.d ./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/ievent_loop.o ./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/irand.d ./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/irand.o ./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/itime.d ./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/itime.o ./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/os_utils.d ./Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/os_utils.o

.PHONY: clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-azure_iot-2f-azure_iot_security_module-2f-src-2f-utils

