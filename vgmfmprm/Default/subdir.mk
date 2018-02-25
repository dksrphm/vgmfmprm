################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vgmfmprm.c \
../vgmfmprm_main.c \
../vgmfmprm_ym2151.c \
../vgmfmprm_ym2203.c \
../vgmfmprm_ym2608.c \
../vgmfmprm_ym2610b.c \
../vgmfmprm_ym2612.c \
../vgmutil.c 

OBJS += \
./vgmfmprm.o \
./vgmfmprm_main.o \
./vgmfmprm_ym2151.o \
./vgmfmprm_ym2203.o \
./vgmfmprm_ym2608.o \
./vgmfmprm_ym2610b.o \
./vgmfmprm_ym2612.o \
./vgmutil.o 

C_DEPS += \
./vgmfmprm.d \
./vgmfmprm_main.d \
./vgmfmprm_ym2151.d \
./vgmfmprm_ym2203.d \
./vgmfmprm_ym2608.d \
./vgmfmprm_ym2610b.d \
./vgmfmprm_ym2612.d \
./vgmutil.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


