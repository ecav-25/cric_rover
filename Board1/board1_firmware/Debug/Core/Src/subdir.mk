################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Board1.c \
../Core/Src/adc.c \
../Core/Src/app_freertos.c \
../Core/Src/batt.c \
../Core/Src/board1_functions.c \
../Core/Src/crc.c \
../Core/Src/deadline_watchdog.c \
../Core/Src/dma.c \
../Core/Src/encoder.c \
../Core/Src/gpio.c \
../Core/Src/hw_config.c \
../Core/Src/led.c \
../Core/Src/led_stripes.c \
../Core/Src/main.c \
../Core/Src/motor.c \
../Core/Src/motor_diagnostic.c \
../Core/Src/pid_law.c \
../Core/Src/stm32g4xx_hal_msp.c \
../Core/Src/stm32g4xx_hal_timebase_tim.c \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c \
../Core/Src/temp.c \
../Core/Src/tim.c \
../Core/Src/usart.c 

OBJS += \
./Core/Src/Board1.o \
./Core/Src/adc.o \
./Core/Src/app_freertos.o \
./Core/Src/batt.o \
./Core/Src/board1_functions.o \
./Core/Src/crc.o \
./Core/Src/deadline_watchdog.o \
./Core/Src/dma.o \
./Core/Src/encoder.o \
./Core/Src/gpio.o \
./Core/Src/hw_config.o \
./Core/Src/led.o \
./Core/Src/led_stripes.o \
./Core/Src/main.o \
./Core/Src/motor.o \
./Core/Src/motor_diagnostic.o \
./Core/Src/pid_law.o \
./Core/Src/stm32g4xx_hal_msp.o \
./Core/Src/stm32g4xx_hal_timebase_tim.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o \
./Core/Src/temp.o \
./Core/Src/tim.o \
./Core/Src/usart.o 

C_DEPS += \
./Core/Src/Board1.d \
./Core/Src/adc.d \
./Core/Src/app_freertos.d \
./Core/Src/batt.d \
./Core/Src/board1_functions.d \
./Core/Src/crc.d \
./Core/Src/deadline_watchdog.d \
./Core/Src/dma.d \
./Core/Src/encoder.d \
./Core/Src/gpio.d \
./Core/Src/hw_config.d \
./Core/Src/led.d \
./Core/Src/led_stripes.d \
./Core/Src/main.d \
./Core/Src/motor.d \
./Core/Src/motor_diagnostic.d \
./Core/Src/pid_law.d \
./Core/Src/stm32g4xx_hal_msp.d \
./Core/Src/stm32g4xx_hal_timebase_tim.d \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d \
./Core/Src/temp.d \
./Core/Src/tim.d \
./Core/Src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G474xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/Board1.cyclo ./Core/Src/Board1.d ./Core/Src/Board1.o ./Core/Src/Board1.su ./Core/Src/adc.cyclo ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/app_freertos.cyclo ./Core/Src/app_freertos.d ./Core/Src/app_freertos.o ./Core/Src/app_freertos.su ./Core/Src/batt.cyclo ./Core/Src/batt.d ./Core/Src/batt.o ./Core/Src/batt.su ./Core/Src/board1_functions.cyclo ./Core/Src/board1_functions.d ./Core/Src/board1_functions.o ./Core/Src/board1_functions.su ./Core/Src/crc.cyclo ./Core/Src/crc.d ./Core/Src/crc.o ./Core/Src/crc.su ./Core/Src/deadline_watchdog.cyclo ./Core/Src/deadline_watchdog.d ./Core/Src/deadline_watchdog.o ./Core/Src/deadline_watchdog.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/encoder.cyclo ./Core/Src/encoder.d ./Core/Src/encoder.o ./Core/Src/encoder.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/hw_config.cyclo ./Core/Src/hw_config.d ./Core/Src/hw_config.o ./Core/Src/hw_config.su ./Core/Src/led.cyclo ./Core/Src/led.d ./Core/Src/led.o ./Core/Src/led.su ./Core/Src/led_stripes.cyclo ./Core/Src/led_stripes.d ./Core/Src/led_stripes.o ./Core/Src/led_stripes.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/motor.cyclo ./Core/Src/motor.d ./Core/Src/motor.o ./Core/Src/motor.su ./Core/Src/motor_diagnostic.cyclo ./Core/Src/motor_diagnostic.d ./Core/Src/motor_diagnostic.o ./Core/Src/motor_diagnostic.su ./Core/Src/pid_law.cyclo ./Core/Src/pid_law.d ./Core/Src/pid_law.o ./Core/Src/pid_law.su ./Core/Src/stm32g4xx_hal_msp.cyclo ./Core/Src/stm32g4xx_hal_msp.d ./Core/Src/stm32g4xx_hal_msp.o ./Core/Src/stm32g4xx_hal_msp.su ./Core/Src/stm32g4xx_hal_timebase_tim.cyclo ./Core/Src/stm32g4xx_hal_timebase_tim.d ./Core/Src/stm32g4xx_hal_timebase_tim.o ./Core/Src/stm32g4xx_hal_timebase_tim.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su ./Core/Src/temp.cyclo ./Core/Src/temp.d ./Core/Src/temp.o ./Core/Src/temp.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su

.PHONY: clean-Core-2f-Src

