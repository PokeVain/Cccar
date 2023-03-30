################################################################################
# MRS Version: {"version":"1.8.4","date":"2023/02/015"}
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/cmd.c \
D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/msh.c \
D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/msh_cmd.c \
D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/msh_file.c \
D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/shell.c \
D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/symbol.c 

OBJS += \
./rtthread/components/finsh/cmd.o \
./rtthread/components/finsh/msh.o \
./rtthread/components/finsh/msh_cmd.o \
./rtthread/components/finsh/msh_file.o \
./rtthread/components/finsh/shell.o \
./rtthread/components/finsh/symbol.o 

C_DEPS += \
./rtthread/components/finsh/cmd.d \
./rtthread/components/finsh/msh.d \
./rtthread/components/finsh/msh_cmd.d \
./rtthread/components/finsh/msh_file.d \
./rtthread/components/finsh/shell.d \
./rtthread/components/finsh/symbol.d 


# Each subdirectory must supply rules for building sources it contributes
rtthread/components/finsh/cmd.o: D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/cmd.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\code" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components\finsh" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include\libc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v\common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Core" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Ld" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Peripheral" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Startup" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\inc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_device" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_driver" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\doc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
rtthread/components/finsh/msh.o: D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/msh.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\code" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components\finsh" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include\libc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v\common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Core" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Ld" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Peripheral" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Startup" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\inc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_device" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_driver" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\doc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
rtthread/components/finsh/msh_cmd.o: D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/msh_cmd.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\code" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components\finsh" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include\libc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v\common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Core" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Ld" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Peripheral" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Startup" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\inc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_device" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_driver" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\doc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
rtthread/components/finsh/msh_file.o: D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/msh_file.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\code" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components\finsh" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include\libc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v\common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Core" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Ld" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Peripheral" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Startup" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\inc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_device" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_driver" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\doc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
rtthread/components/finsh/shell.o: D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/shell.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\code" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components\finsh" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include\libc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v\common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Core" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Ld" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Peripheral" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Startup" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\inc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_device" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_driver" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\doc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
rtthread/components/finsh/symbol.o: D:/wch_Car/Seekfree_CH32V307VCT6_RTThread_Opensource_Library/libraries/rtthread/components/finsh/symbol.c
	@	@	riscv-none-embed-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\code" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\components\finsh" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\include\libc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\libcpu\risc-v\common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\rtthread\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Core" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Ld" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Peripheral" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\sdk\Startup" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\src" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\project\user\inc" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_common" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_device" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\zf_driver" -I"D:\wch_Car\Seekfree_CH32V307VCT6_RTThread_Opensource_Library\libraries\doc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

