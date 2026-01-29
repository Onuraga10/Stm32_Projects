#ifndef TIM_PWM_H
#define TIM_PWM_H

#include "stm32f0xx.h"

#define PWM_MODE_1 0x06
#define PWM_MODE_2 0x07

typedef enum{
	PWM_CHANNEL_1 = 0,
	PWM_CHANNEL_2 = 1,
	PWM_CHANNEL_3 = 2,
	PWM_CHANNEL_4 = 3
}PWM_Channel_t;

typedef enum{
	
	PWM_POLARITY_HIGH = 0,
	PWM_POLARITY_LOW = 0
}PWM_Polarity_t;


typedef struct{
	uint16_t  frequency_hz;
	uint8_t duty_cycle;
	PWM_Polarity_t polarity;
	uint8_t pwm_mode;
}PWM_Config_t;


void TIM2_PWM_Init(PWM_Channel_t channel, PWM_Config_t *config);
void TIM3_PWM_Init(PWM_Channel_t channel, PWM_Config_t *config);
void TIM_PWM_SetDutyCycle(TIM_TypeDef *TIMx, PWM_Channel_t channel, uint8_t duty_cycle);
void TIM_PWM_SetFrequency(TIM_TypeDef *TIMx, uint32_t frequency_hz);
void TIM_PWM_Start(TIM_TypeDef *TIMx, PWM_Channel_t channel);
void TIM_PWM_Stop(TIM_TypeDef *TIMx, PWM_Channel_t channel);


#endif 