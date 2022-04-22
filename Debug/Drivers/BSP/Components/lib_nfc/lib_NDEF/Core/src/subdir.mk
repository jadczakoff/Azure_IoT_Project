################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_AAR.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Bluetooth.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Email.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Geo.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Handover.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_MyApp.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_SMS.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Text.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_URI.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Vcard.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Wifi.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_config.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_wrapper.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype3_wrapper.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype4_wrapper.c \
../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype5_wrapper.c 

OBJS += \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_AAR.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Bluetooth.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Email.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Geo.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Handover.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_MyApp.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_SMS.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Text.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_URI.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Vcard.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Wifi.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_config.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_wrapper.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype3_wrapper.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype4_wrapper.o \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype5_wrapper.o 

C_DEPS += \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_AAR.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Bluetooth.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Email.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Geo.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Handover.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_MyApp.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_SMS.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Text.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_URI.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Vcard.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Wifi.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_config.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_wrapper.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype3_wrapper.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype4_wrapper.d \
./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype5_wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/%.o: ../Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/%.c Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L4S5xx -DTX_INCLUDE_USER_DEFINE_FILE -DNX_INCLUDE_USER_DEFINE_FILE -c -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" @"Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF.c_includes.args"

clean: clean-Drivers-2f-BSP-2f-Components-2f-lib_nfc-2f-lib_NDEF-2f-Core-2f-src

clean-Drivers-2f-BSP-2f-Components-2f-lib_nfc-2f-lib_NDEF-2f-Core-2f-src:
	-$(RM) ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_AAR.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_AAR.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Bluetooth.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Bluetooth.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Email.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Email.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Geo.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Geo.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Handover.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Handover.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_MyApp.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_MyApp.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_SMS.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_SMS.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Text.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Text.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_URI.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_URI.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Vcard.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Vcard.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Wifi.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_Wifi.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_config.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_NDEF_config.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_wrapper.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/lib_wrapper.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype3_wrapper.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype3_wrapper.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype4_wrapper.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype4_wrapper.o ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype5_wrapper.d ./Drivers/BSP/Components/lib_nfc/lib_NDEF/Core/src/tagtype5_wrapper.o

.PHONY: clean-Drivers-2f-BSP-2f-Components-2f-lib_nfc-2f-lib_NDEF-2f-Core-2f-src

