################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32f103rctx.s 

OBJS += \
./Startup/startup_stm32f103rctx.o 

S_DEPS += \
./Startup/startup_stm32f103rctx.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/startup_stm32f103rctx.o: ../Startup/startup_stm32f103rctx.s Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -c -I"E:/project/RoboCup/STM32/Src/Platform" -I"E:/project/RoboCup/STM32/Src/Platform/AppLog" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/mllite" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/driver" -I"E:/project/RoboCup/STM32/Src/Platform/SPI" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/tests/lv_test_fonts" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/tests/lv_test_widgets" -I"E:/project/RoboCup/STM32/Src/Platform/ServoPorts" -I"E:/project/RoboCup/STM32/Src/SSD1283" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/driver/stm32L" -I"E:/project/RoboCup/STM32/Src/Platform/JustFloat" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/tests/lv_test_ref_imgs" -I"E:/project/RoboCup/STM32/Src/App/BitOperation" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/driver/eMPL" -I"E:/project/RoboCup/STM32/Src/mpu9250/msp430" -I"E:/project/RoboCup/STM32/Src/App/Timer" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/driver/include" -I"E:/project/RoboCup/STM32/Src/Platform/SerialPrintf" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty" -I"E:/project/RoboCup/STM32/Src/mpu9250/msp430/eMD-6.0" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/examples" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/mpl" -I"E:/project/RoboCup/STM32/Src/mpu9250" -I"E:/project/RoboCup/STM32/Src/Servo" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/tests" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/tests/lv_test_core" -I"E:/project/RoboCup/STM32/Src/App/PID" -I"E:/project/RoboCup/STM32/Src/Motor" -I"E:/project/RoboCup/STM32/Src/Platform/MotorPorts" -I"E:/project/RoboCup/STM32/Src/Platform/Clock" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/docs" -I"E:/project/RoboCup/STM32/Src/App/MotionControl" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/examples/porting" -I"E:/project/RoboCup/STM32/Src/App/PIDManagement" -I"E:/project/RoboCup/STM32/Src/Platform/lvglPorts" -I"E:/project/RoboCup/STM32/Src/mpu9250/dmp/eMPL-hal" -I"E:/project/RoboCup/STM32/Src/App" -I"E:/project/RoboCup/STM32/Src/App/ThirdParty/lvgl/src" -I"E:/project/RoboCup/STM32/Src/Drivers" -x assembler-with-cpp -MMD -MP -MF"Startup/startup_stm32f103rctx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

