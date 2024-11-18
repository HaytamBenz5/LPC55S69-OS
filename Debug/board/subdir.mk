################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/peripherals.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/peripherals.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/peripherals.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_LPC55S69JBD100 -DCPU_LPC55S69JBD100_cm33 -DCPU_LPC55S69JBD100_cm33_core0 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\drivers" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\LPC55S69\drivers" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\utilities" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\component\uart" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\component\serial_manager" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\component\lists" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\CMSIS" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\device" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\board" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\source" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\drivers" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\LPC55S69\drivers" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\utilities" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\component\uart" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\component\serial_manager" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\component\lists" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\startup" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\CMSIS" -I"C:\Users\BENZINANE\Desktop\LPC55S69_OS.src\device" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/peripherals.d ./board/peripherals.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

