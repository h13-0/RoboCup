################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/App/Timer/Timer.c 

OBJS += \
./Src/App/Timer/Timer.o 

C_DEPS += \
./Src/App/Timer/Timer.d 


# Each subdirectory must supply rules for building sources it contributes
Src/App/Timer/Timer.o: ../Src/App/Timer/Timer.c Src/App/Timer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DSTM32F103xE '-DMPL_LOG_NDEBUG=1' -DEMPL -DMPU9250 -DEMPL_TARGET_STM32F4 -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER -c -I"E:/project/RoboCup/STM32/Src/Platform" -I"E:/project/RoboCup/STM32/Src/Platform/AppLog" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/mllite" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/driver" -I"E:/project/RoboCup/STM32/Src/Platform/SPI" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/tests/lv_test_fonts" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/tests/lv_test_widgets" -I"E:/project/RoboCup/STM32/Src/Platform/ServoPorts" -I"E:/project/RoboCup/STM32/Src/SSD1283" -IDrivers/STM32F1xx_HAL_Driver/Inc -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/driver/stm32L" -I"E:/project/RoboCup/STM32/Src/Platform/JustFloat" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/tests/lv_test_ref_imgs" -I"E:/project/RoboCup/STM32/Src/App/BitOperation" -IDrivers/CMSIS/Device/ST/STM32F1xx/Include -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/driver/eMPL" -I"E:/project/RoboCup/STM32/Src/mpu9250/msp430" -I"E:/project/RoboCup/STM32/Src/App/Timer" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/driver/include" -I../Drivers/CMSIS/Include -I"E:/project/RoboCup/STM32/Src/Platform/SerialPrintf" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"E:/project/RoboCup/STM32/Src/App/ThirdParty" -I"E:/project/RoboCup/STM32/Src/mpu9250/msp430/eMD-6.0" -IInc -IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/examples" -IDrivers/CMSIS/Include -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/mpl" -I"E:/project/RoboCup/STM32/Src/mpu9250" -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I"E:/project/RoboCup/STM32/Src/Servo" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/tests" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/tests/lv_test_core" -I"E:/project/RoboCup/STM32/Src/App/PID" -I"E:/project/RoboCup/STM32/Src/Motor" -I"E:/project/RoboCup/STM32/Src/Platform/MotorPorts" -I"E:/project/RoboCup/STM32/Src/Platform/Clock" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/docs" -I../Inc -I"E:/project/RoboCup/STM32/Src/App/MotionControl" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/examples/porting" -I"E:/project/RoboCup/STM32/Src/App/PIDManagement" -I"E:/project/RoboCup/STM32/Src/Platform/lvglPorts" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/eMPL-hal" -I../Drivers/STM32F1xx_HAL_Driver/Inc -I"E:/project/RoboCup/STM32/Src/App" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/src" -I"E:/project/RoboCup/STM32/Src/Drivers" -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Src/App/Timer/Timer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

