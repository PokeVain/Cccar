################################################################################
# MRS Version: {"version":"1.8.4","date":"2023/02/015"}
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/sdk/Startup/system_ch32v30x.c 

S_UPPER_SRCS += \
D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/sdk/Startup/startup_ch32v30x.S 

OBJS += \
./sdk/Startup/startup_ch32v30x.o \
./sdk/Startup/system_ch32v30x.o 

S_UPPER_DEPS += \
./sdk/Startup/startup_ch32v30x.d 

C_DEPS += \
./sdk/Startup/system_ch32v30x.d 


# Each subdirectory must supply rules for building sources it contributes
sdk/Startup/startup_ch32v30x.o: D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/sdk/Startup/startup_ch32v30x.S
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -x assembler-with-cpp -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components\finsh" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include\libc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v\common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Startup" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_device" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
sdk/Startup/system_ch32v30x.o: D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/sdk/Startup/system_ch32v30x.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\code" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components\finsh" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include\libc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v\common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Core" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Ld" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Peripheral" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Startup" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\inc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_device" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_driver" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\doc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

