################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/lists/fsl_component_generic_list.c 

C_DEPS += \
./component/lists/fsl_component_generic_list.d 

OBJS += \
./component/lists/fsl_component_generic_list.o 


# Each subdirectory must supply rules for building sources it contributes
component/lists/%.o: ../component/lists/%.c component/lists/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_LPC55S69JBD100 -DCPU_LPC55S69JBD100_cm33 -DCPU_LPC55S69JBD100_cm33_core0 -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -DSERIAL_PORT_TYPE_UART=1 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__NEWLIB__ -I"C:\Users\benzh\Desktop\LPC55S69-OS\drivers" -I"C:\Users\benzh\Desktop\LPC55S69-OS\LPC55S69\drivers" -I"C:\Users\benzh\Desktop\LPC55S69-OS\utilities" -I"C:\Users\benzh\Desktop\LPC55S69-OS\component\uart" -I"C:\Users\benzh\Desktop\LPC55S69-OS\component\serial_manager" -I"C:\Users\benzh\Desktop\LPC55S69-OS\component\lists" -I"C:\Users\benzh\Desktop\LPC55S69-OS\CMSIS" -I"C:\Users\benzh\Desktop\LPC55S69-OS\device" -I"C:\Users\benzh\Desktop\LPC55S69-OS\board" -I"C:\Users\benzh\Desktop\LPC55S69-OS\source" -I"C:\Users\benzh\Desktop\LPC55S69-OS\drivers" -I"C:\Users\benzh\Desktop\LPC55S69-OS\LPC55S69\drivers" -I"C:\Users\benzh\Desktop\LPC55S69-OS\utilities" -I"C:\Users\benzh\Desktop\LPC55S69-OS\component\uart" -I"C:\Users\benzh\Desktop\LPC55S69-OS\component\serial_manager" -I"C:\Users\benzh\Desktop\LPC55S69-OS\component\lists" -I"C:\Users\benzh\Desktop\LPC55S69-OS\startup" -I"C:\Users\benzh\Desktop\LPC55S69-OS\CMSIS" -I"C:\Users\benzh\Desktop\LPC55S69-OS\device" -O0 -fno-common -g3 -gdwarf-4 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33 -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__NEWLIB__ -fstack-usage -specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-lists

clean-component-2f-lists:
	-$(RM) ./component/lists/fsl_component_generic_list.d ./component/lists/fsl_component_generic_list.o

.PHONY: clean-component-2f-lists

