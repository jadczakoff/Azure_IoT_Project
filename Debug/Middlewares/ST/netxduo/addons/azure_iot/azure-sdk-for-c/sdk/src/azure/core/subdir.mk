################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_base64.c \
../Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_context.c \
../Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_reader.c \
../Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_token.c \
../Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_writer.c \
../Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_log.c \
../Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_precondition.c \
../Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_span.c 

OBJS += \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_base64.o \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_context.o \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_reader.o \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_token.o \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_writer.o \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_log.o \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_precondition.o \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_span.o 

C_DEPS += \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_base64.d \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_context.d \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_reader.d \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_token.d \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_writer.d \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_log.d \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_precondition.d \
./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_span.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/%.o: ../Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/%.c Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4S5xx -DTX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" @"Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_base64.c_includes.args"

clean: clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-azure_iot-2f-azure-2d-sdk-2d-for-2d-c-2f-sdk-2f-src-2f-azure-2f-core

clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-azure_iot-2f-azure-2d-sdk-2d-for-2d-c-2f-sdk-2f-src-2f-azure-2f-core:
	-$(RM) ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_base64.d ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_base64.o ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_context.d ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_context.o ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_reader.d ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_reader.o ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_token.d ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_token.o ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_writer.d ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_json_writer.o ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_log.d ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_log.o ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_precondition.d ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_precondition.o ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_span.d ./Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/az_span.o

.PHONY: clean-Middlewares-2f-ST-2f-netxduo-2f-addons-2f-azure_iot-2f-azure-2d-sdk-2d-for-2d-c-2f-sdk-2f-src-2f-azure-2f-core

