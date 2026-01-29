#include "tim_pwm.h"

#define SYSTEM_CLOCK_HZ  48000000


/**
 * @brief  Calculate timer prescaler and auto-reload values for desired frequency
 * @param  frequency_hz: Desired PWM frequency in Hz
 * @param  prescaler: Pointer to store calculated prescaler value
 * @param  arr: Pointer to store calculated auto-reload value
 */
static void TIM_Calculate_PSC_ARR(uint32_t frequency_hz, uint16_t *prescaler, uint16_t *arr)
{
    uint32_t timer_clock = SYSTEM_CLOCK_HZ;
    
    // Start with prescaler = 0 (divide by 1)
    *prescaler = 0;
    
    // Calculate ARR: ARR = (Timer_Clock / (Prescaler + 1) / Frequency) - 1
    *arr = (timer_clock / frequency_hz) - 1;
    
    // If ARR is too large (> 65535), increase prescaler
    while (*arr > 65535) {
        (*prescaler)++;
        *arr = (timer_clock / ((*prescaler) + 1) / frequency_hz) - 1;
    }
}


/**
 * @brief  Initialize TIM3 for PWM operation on specified channel
 * @param  channel: PWM channel to configure (PWM_CHANNEL_1 to PWM_CHANNEL_4)
 * @param  config: Pointer to PWM configuration structure
 */
void TIM3_PWM_Init(PWM_Channel_t channel, PWM_Config_t *config){
	
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	
	uint16_t prescaler, arr;
	TIM_Calculate_PSC_ARR(config->frequency_hz, &prescaler, &arr);
	
	TIM3->CR1 &= ~TIM_CR1_CEN;
	
	TIM3->PSC = prescaler;
	TIM3->ARR = arr;
	
	
	TIM3->CR1 &= ~TIM_CR1_DIR; //upcounting
	TIM3->CR1 &= ~TIM_CR1_CMS; //edge-aligned

	TIM3->CR1 |= TIM_CR1_ARPE;
	
	
	//Channel configurations
	switch(channel) {
        case PWM_CHANNEL_1:
            TIM3->CCER &= ~TIM_CCER_CC1E;
            TIM3->CCMR1 &= ~TIM_CCMR1_OC1M;
            TIM3->CCMR1 |= (config->pwm_mode << 4);
            TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
            if (config->polarity == PWM_POLARITY_LOW) {
                TIM3->CCER |= TIM_CCER_CC1P;
            } else {
                TIM3->CCER &= ~TIM_CCER_CC1P;
            }
            TIM3->CCR1 = (arr * config->duty_cycle) / 100;
            break;
            
        case PWM_CHANNEL_2:
            TIM3->CCER &= ~TIM_CCER_CC2E;
            TIM3->CCMR1 &= ~TIM_CCMR1_OC2M;
            TIM3->CCMR1 |= (config->pwm_mode << 12);
            TIM3->CCMR1 |= TIM_CCMR1_OC2PE;
            if (config->polarity == PWM_POLARITY_LOW) {
                TIM3->CCER |= TIM_CCER_CC2P;
            } else {
                TIM3->CCER &= ~TIM_CCER_CC2P;
            }
            TIM3->CCR2 = (arr * config->duty_cycle) / 100;
            break;
            
        case PWM_CHANNEL_3:
            TIM3->CCER &= ~TIM_CCER_CC3E;
            TIM3->CCMR2 &= ~TIM_CCMR2_OC3M;
            TIM3->CCMR2 |= (config->pwm_mode << 4);
            TIM3->CCMR2 |= TIM_CCMR2_OC3PE;
            if (config->polarity == PWM_POLARITY_LOW) {
                TIM3->CCER |= TIM_CCER_CC3P;
            } else {
                TIM3->CCER &= ~TIM_CCER_CC3P;
            }
            TIM3->CCR3 = (arr * config->duty_cycle) / 100;
            break;
            
        case PWM_CHANNEL_4:
            TIM3->CCER &= ~TIM_CCER_CC4E;
            TIM3->CCMR2 &= ~TIM_CCMR2_OC4M;
            TIM3->CCMR2 |= (config->pwm_mode << 12);
            TIM3->CCMR2 |= TIM_CCMR2_OC4PE;
            if (config->polarity == PWM_POLARITY_LOW) {
                TIM3->CCER |= TIM_CCER_CC4P;
            } else {
                TIM3->CCER &= ~TIM_CCER_CC4P;
            }
            TIM3->CCR4 = (arr * config->duty_cycle) / 100;
            break;
    }
	
	//Force Update Generation
	TIM3->EGR |= TIM_EGR_UG;

	//Enable counter
	TIM3->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief  Set the duty cycle of PWM output
 * @param  TIMx: Timer peripheral (TIM2 or TIM3)
 * @param  channel: PWM channel
 * @param  duty_cycle: Duty cycle in percentage (0-100)
 */
void TIM_PWM_SetDutyCycle(TIM_TypeDef *TIMx, PWM_Channel_t channel, uint8_t duty_cycle)
{
    uint16_t arr = TIMx->ARR;
    uint16_t ccr_value = (arr * duty_cycle) / 100;
    
    // Clamp duty cycle to 0-100%
    if (duty_cycle > 100) {
        ccr_value = arr;
    }
    
    switch(channel) {
        case PWM_CHANNEL_1:
            TIMx->CCR1 = ccr_value;
            break;
        case PWM_CHANNEL_2:
            TIMx->CCR2 = ccr_value;
            break;
        case PWM_CHANNEL_3:
            TIMx->CCR3 = ccr_value;
            break;
        case PWM_CHANNEL_4:
            TIMx->CCR4 = ccr_value;
            break;
    }
}

/**
 * @brief  Change the PWM frequency
 * @param  TIMx: Timer peripheral (TIM2 or TIM3)
 * @param  frequency_hz: New frequency in Hz
 */
void TIM_PWM_SetFrequency(TIM_TypeDef *TIMx, uint32_t frequency_hz)
{
    uint16_t prescaler, arr;
    
    // Calculate new values
    TIM_Calculate_PSC_ARR(frequency_hz, &prescaler, &arr);
    
    // Update timer registers
    TIMx->PSC = prescaler;
    TIMx->ARR = arr;
    
    // Force update to load new values
    TIMx->EGR |= TIM_EGR_UG;
}


/**
 * @brief  Start PWM output on specified channel
 * @param  TIMx: Timer peripheral (TIM2 or TIM3)
 * @param  channel: PWM channel to enable
 */
void TIM_PWM_Start(TIM_TypeDef *TIMx, PWM_Channel_t channel)
{
    switch(channel) {
        case PWM_CHANNEL_1:
            TIMx->CCER |= TIM_CCER_CC1E;
            break;
        case PWM_CHANNEL_2:
            TIMx->CCER |= TIM_CCER_CC2E;
            break;
        case PWM_CHANNEL_3:
            TIMx->CCER |= TIM_CCER_CC3E;
            break;
        case PWM_CHANNEL_4:
            TIMx->CCER |= TIM_CCER_CC4E;
            break;
    }
}

/**
 * @brief  Stop PWM output on specified channel
 * @param  TIMx: Timer peripheral (TIM2 or TIM3)
 * @param  channel: PWM channel to disable
 */
void TIM_PWM_Stop(TIM_TypeDef *TIMx, PWM_Channel_t channel)
{
    switch(channel) {
        case PWM_CHANNEL_1:
            TIMx->CCER &= ~TIM_CCER_CC1E;
            break;
        case PWM_CHANNEL_2:
            TIMx->CCER &= ~TIM_CCER_CC2E;
            break;
        case PWM_CHANNEL_3:
            TIMx->CCER &= ~TIM_CCER_CC3E;
            break;
        case PWM_CHANNEL_4:
            TIMx->CCER &= ~TIM_CCER_CC4E;
            break;
    }
}
