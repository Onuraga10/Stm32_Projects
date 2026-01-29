#include "stm32f0xx.h"
#include "tim_pwm.h"



void GPIO_TIM3_Config(void){
	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	GPIOA->MODER &= ~GPIO_MODER_MODER6;
	GPIOA->MODER |= GPIO_MODER_MODER6_1;
	
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL6;
	GPIOA->AFR[0] |= (1<<24);

	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6;
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR6;
}

void delay_ms(volatile uint32_t count) {
    count *= 4800; // Approx delay for 48MHz clock
    while(count--);
}
int main(void) {
    // 1. Initialize System (Clock, etc.)
    // SystemInit(); // Usually called automatically by startup file

    // 2. Configure GPIOs for TIM3
    GPIO_TIM3_Config();

    // 3. Configure PWM Driver
    PWM_Config_t pwm_conf;
    pwm_conf.frequency_hz = 1000;       // 1 kHz is good for LEDs and Motors
    pwm_conf.duty_cycle   = 0;          // Start off
    pwm_conf.pwm_mode     = 6;          // PWM Mode 1 (Standard)
    pwm_conf.polarity     = PWM_POLARITY_HIGH;

    // Initialize TIM3 Channel 1 (PA6)
    TIM3_PWM_Init(PWM_CHANNEL_1, &pwm_conf);
    TIM_PWM_Start(TIM3, PWM_CHANNEL_1);

    int duty = 0;
    int direction = 1;

    while (1) {
        // Update Duty Cycle
        TIM_PWM_SetDutyCycle(TIM3, PWM_CHANNEL_1, duty);

        // Calculate next step
        duty += direction;

        // Reverse direction at edges
        if (duty >= 100) direction = -1;
        if (duty <= 0)   direction = 1;

        // Small delay to make the fading visible to human eye
        delay_ms(20);
    }
}