#include "stm32f407_min.h"

uint32_t SystemCoreClock = 16000000UL;
static volatile uint32_t g_ms_ticks;

void SystemInit(void)
{
    SystemCoreClock = 16000000UL;
}

void SysTick_Handler(void)
{
    g_ms_ticks++;
}

uint32_t millis(void)
{
    return g_ms_ticks;
}

void delay_init(void)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    (void)RCC->APB1ENR;

    TIM2->CR1 = 0;
    TIM2->PSC = (SystemCoreClock / 1000000UL) - 1UL;
    TIM2->ARR = 0xFFFFFFFFUL;
    TIM2->CNT = 0;
    TIM2->CR1 = TIM_CR1_CEN;

    SysTick->LOAD = (SystemCoreClock / 1000UL) - 1UL;
    SysTick->VAL = 0;
    SysTick->CTRL = SYSTICK_CTRL_CLKSRC | SYSTICK_CTRL_TICKINT | SYSTICK_CTRL_ENABLE;
}

void delay_us(uint32_t us)
{
    uint32_t start = TIM2->CNT;
    while ((uint32_t)(TIM2->CNT - start) < us) {
    }
}

void delay_ms(uint32_t ms)
{
    uint32_t start = millis();
    while ((uint32_t)(millis() - start) < ms) {
    }
}
