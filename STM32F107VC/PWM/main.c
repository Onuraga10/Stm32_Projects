#include "stm32f10x.h"


int main(void){
	
	
	//Enable GPIOC,TIM2 and ADC1 clocks
	RCC->APB2ENR |= (1<<4);
	RCC->APB2ENR |= (1<<9);
	RCC->APB1ENR |= (1<<0);
	RCC->APB2ENR |= (1<<2);
	
	
	// TIM2 PWM config
	TIM2->PSC = 71;        // Prescaler ? 72MHz / (71+1) = 1MHz
	TIM2->ARR = 1000;      // Auto-reload ? 1kHz PWM
	TIM2->CCR1 = 500;      // Initial duty = 50%

	// PWM mode 1
	TIM2->CCMR1 &= ~(7 << 4);
	TIM2->CCMR1 |=  (6 << 4);  // OC1M = 110 (PWM mode 1)
	TIM2->CCMR1 |=  (1 << 3);  // OC1PE = 1 (Preload enable)

	// Enable channel output
	TIM2->CCER |= (1 << 0);    // CC1E = 1

	// Enable auto-reload preload + generate update
	TIM2->CR1  |= (1 << 7);    // ARPE = 1
	TIM2->EGR  |= (1 << 0);    // UG = 1
	TIM2->CR1  |= (1 << 0);    // CEN = 1 (start timer)
	//PC0 as input push pull
	GPIOC->CRL &= ~(0xF<<0);
	
	//ADC prescaler divide by 6
	RCC->CFGR |= (0b10 << 14);
	
	// Set channel 10 as the first in the sequence (SQ1)
	ADC1->SQR3 = 10;
	
	//Sample time 55.5 cycle
	ADC1->SMPR1 |= (0b101<<0);
	//Start conv
	ADC1->CR2 |= (1<<0);
	for(int i=0;i<1000;i++);
	ADC1->CR2 |= (1<<2);
	while (ADC1->CR2 & (1 << 2));
	
	
	
	
	uint16_t adc= ADC1->DR;
	
	while (1) {
    ADC1->CR2 |= (1 << 22);                // Start conversion
    while (!(ADC1->SR & (1 << 1)));        // Wait EOC
    adc = ADC1->DR;               // Read ADC result
    TIM2->CCR1 = (adc * 1000) / 4095;      // Update PWM duty
}
	
	
	
	while(1);
	
	return 0;
}