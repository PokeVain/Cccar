################################################################################
# MRS Version: {"version":"1.8.4","date":"2023/02/015"}
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/project/user/src/isr.c \
D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/project/user/src/main.c 

OBJS += \
./user_c/isr.o \
./user_c/main.o 

C_DEPS += \
./user_c/isr.d \
./user_c/main.d 


# Each subdirectory must supply rules for building sources it contributes
user_c/isr.o: D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/project/user/src/isr.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\code" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components\finsh" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include\libc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v\common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Core" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Ld" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Peripheral" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Startup" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\inc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_device" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_driver" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\doc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
user_c/main.o: D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/project/user/src/main.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\code" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components\finsh" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include\libc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v\common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Core" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Ld" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Peripheral" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Startup" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\inc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_device" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_driver" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\doc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

