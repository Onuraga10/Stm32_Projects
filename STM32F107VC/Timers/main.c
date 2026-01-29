#include "stm32f10x.h"
volatile uint32_t msTicks = 0;

void SysTick_Handler(void)
{
    msTicks++;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = msTicks;
    while ((msTicks - start) < ms)
        ;
}

int main(void)
{
    // Enable clocks
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

    // Make sure TIM2_CH1 is NOT remapped
    AFIO->MAPR &= ~(1 << 8); // TIM2_REMAP = 0 ? default: CH1 on PA0

    // PA0 = Alternate Function Push-Pull (50 MHz)
    GPIOA->CRL &= ~(0xF << 0); // Clear CNF0 + MODE0
    GPIOA->CRL |= (0xB << 0);  // MODE0 = 11 (50 MHz), CNF0 = 10 (AF_PP)

    // Timer config: 1 kHz PWM, 50% duty
    TIM2->PSC = 71;   // 72MHz / (71+1) = 1MHz
    TIM2->ARR = 999;  // 1000 ticks = 1ms period
    TIM2->CCR1 = 500; // 50% duty cycle

    // PWM Mode 1 on CH1
    TIM2->CCMR1 &= ~(7 << 4);
    TIM2->CCMR1 |= (6 << 4); // PWM mode 1
    TIM2->CCMR1 |= (1 << 3); // OC1PE

    TIM2->CCER |= (1 << 0); // CC1E: Enable output on CH1

    TIM2->CR1 |= (1 << 7); // ARPE
    TIM2->EGR |= (1 << 0); // UG
    TIM2->CR1 |= (1 << 0); // CEN

    // In setup:
    SysTick->LOAD = (SystemCoreClock / 1000) - 1; // 1ms tick
    SysTick->VAL = 0;
    SysTick->CTRL = 0x7;

    while (1)
    {
        for (int i = 0; i < 1000; i++)
        {
            TIM2->CCR1 = i;
            delay_ms(1);
        }
        for (int i = 1000; i > 0; i--)
        {
            TIM2->CCR1 = i;
            delay_ms(1);
        }
    };
}
