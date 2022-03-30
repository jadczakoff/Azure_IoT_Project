################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include NetXDuo/Target/subdir.mk
-include NetXDuo/App/subdir.mk
-include Middlewares/ST/threadx/ports/cortex_m4/gnu/src/subdir.mk
-include Middlewares/ST/threadx/common/src/subdir.mk
-include Middlewares/ST/netxduo/nx_secure/src/subdir.mk
-include Middlewares/ST/netxduo/crypto_libraries/src/subdir.mk
-include Middlewares/ST/netxduo/common/src/subdir.mk
-include Middlewares/ST/netxduo/addons/web/subdir.mk
-include Middlewares/ST/netxduo/addons/sntp/subdir.mk
-include Middlewares/ST/netxduo/addons/ppp/subdir.mk
-include Middlewares/ST/netxduo/addons/mqtt/subdir.mk
-include Middlewares/ST/netxduo/addons/dns/subdir.mk
-include Middlewares/ST/netxduo/addons/dhcp/subdir.mk
-include Middlewares/ST/netxduo/addons/cloud/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/utils/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/model/objects/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/src/collectors/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/collection/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/utils/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/serializer/extensions/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/serializer/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/model/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/collectors/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/src/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/iot-security-module-core/deps/flatcc/src/runtime/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure_iot_security_module/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/azure-sdk-for-c/sdk/src/azure/core/subdir.mk
-include Middlewares/ST/netxduo/addons/azure_iot/subdir.mk
-include Middlewares/ST/netxduo/addons/auto_ip/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/Components/st25dv/subdir.mk
-include Drivers/BSP/Components/lsm6dsl/subdir.mk
-include Drivers/BSP/Components/lps22hb/subdir.mk
-include Drivers/BSP/Components/lis3mdl/subdir.mk
-include Drivers/BSP/Components/hts221/subdir.mk
-include Drivers/BSP/B-L4S5I-IOT01/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include AZURE_RTOS/App/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Azure_IoT_Project_1
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Azure_IoT_Project_1.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Azure_IoT_Project_1.list \

OBJCOPY_BIN += \
Azure_IoT_Project_1.bin \


# All Target
all: main-build

# Main-build Target
main-build: Azure_IoT_Project_1.elf secondary-outputs

# Tool invocations
Azure_IoT_Project_1.elf: $(OBJS) $(USER_OBJS) C:\Users\domin\STM32CubeIDE\workspace_1.8.0\Azure_IoT_Project_1\STM32L4S5VITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Azure_IoT_Project_1.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"C:\Users\domin\STM32CubeIDE\workspace_1.8.0\Azure_IoT_Project_1\STM32L4S5VITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Azure_IoT_Project_1.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Azure_IoT_Project_1.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Azure_IoT_Project_1.list"
	@echo 'Finished building: $@'
	@echo ' '

Azure_IoT_Project_1.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "Azure_IoT_Project_1.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Azure_IoT_Project_1.bin Azure_IoT_Project_1.elf Azure_IoT_Project_1.list default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
