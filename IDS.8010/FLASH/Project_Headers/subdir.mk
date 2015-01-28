################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Project_Headers/lcd.c" \

C_SRCS += \
../Project_Headers/lcd.c \

OBJS += \
./Project_Headers/lcd.o \

C_DEPS += \
./Project_Headers/lcd.d \

OBJS_QUOTED += \
"./Project_Headers/lcd.o" \

C_DEPS_QUOTED += \
"./Project_Headers/lcd.d" \

OBJS_OS_FORMAT += \
./Project_Headers/lcd.o \


# Each subdirectory must supply rules for building sources it contributes
Project_Headers/lcd.o: ../Project_Headers/lcd.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Project_Headers/lcd.args" -MMD -MP -MF"$(@:%.o=%.d)" -o"Project_Headers/lcd.o"
	@echo 'Finished building: $<'
	@echo ' '


