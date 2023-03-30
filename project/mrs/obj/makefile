################################################################################
# MRS Version: {"version":"1.8.4","date":"2023/02/015"}
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include zf_driver/subdir.mk
-include zf_device/subdir.mk
-include zf_common/subdir.mk
-include user_c/subdir.mk
-include sdk/Startup/subdir.mk
-include sdk/Peripheral/subdir.mk
-include sdk/Core/subdir.mk
-include rtthread/src/subdir.mk
-include rtthread/libcpu/risc-v/common/subdir.mk
-include rtthread/components/finsh/subdir.mk
-include rtthread/subdir.mk
-include code/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
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

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
Seekfree_CH32V307VCT6_RTThread_Opensource_Library.hex \

SECONDARY_LIST += \
Seekfree_CH32V307VCT6_RTThread_Opensource_Library.lst \

SECONDARY_SIZE += \
Seekfree_CH32V307VCT6_RTThread_Opensource_Library.siz \


# All Target
all: Seekfree_CH32V307VCT6_RTThread_Opensource_Library.elf secondary-outputs

# Tool invocations
Seekfree_CH32V307VCT6_RTThread_Opensource_Library.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -L"../" -Wl,-Map,"Seekfree_CH32V307VCT6_RTThread_Opensource_Library.map" --specs=nano.specs --specs=nosys.specs  -o "Seekfree_CH32V307VCT6_RTThread_Opensource_Library.elf" $(OBJS) $(USER_OBJS) $(LIBS) -lprintfloat
	@	@
Seekfree_CH32V307VCT6_RTThread_Opensource_Library.hex: Seekfree_CH32V307VCT6_RTThread_Opensource_Library.elf
	@	riscv-none-embed-objcopy -O ihex "Seekfree_CH32V307VCT6_RTThread_Opensource_Library.elf"  "Seekfree_CH32V307VCT6_RTThread_Opensource_Library.hex"
	@	@
Seekfree_CH32V307VCT6_RTThread_Opensource_Library.lst: Seekfree_CH32V307VCT6_RTThread_Opensource_Library.elf
	@	riscv-none-embed-objdump --all-headers --demangle --disassemble -M xw "Seekfree_CH32V307VCT6_RTThread_Opensource_Library.elf" > "Seekfree_CH32V307VCT6_RTThread_Opensource_Library.lst"
	@	@
Seekfree_CH32V307VCT6_RTThread_Opensource_Library.siz: Seekfree_CH32V307VCT6_RTThread_Opensource_Library.elf
	@	riscv-none-embed-size --format=berkeley "Seekfree_CH32V307VCT6_RTThread_Opensource_Library.elf"
	@	@
# Other Targets
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) Seekfree_CH32V307VCT6_RTThread_Opensource_Library.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
