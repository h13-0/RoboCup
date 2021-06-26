# CubeMX配置
System Core -> NVIC
    -> Code generation
        -> Time base: System tick timer
            -> Cancel "Call HAL handler" 

# 使用
1. 将 `void SysTickInterruptHandler(void)` 放到 `stm32f1xx_it.c` 中的 `void SysTick_Handler(void)` 中。
2. 调用 `ClockInit()` 。