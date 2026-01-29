#include "stm32f0xx.h"


void TIM3_Init(){
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 7999;
	TIM3->ARR = 0xFFFF;
	TIM3->CR1 |= TIM_CR1_CEN;
}


void delay_ms(uint32_t ms){
	uint32_t start = TIM3->CNT;
	while((TIM3->CNT)-start < ms);
}

void GPIO_output_en(GPIO_TypeDef *GPIOx,uint8_t pin){
	GPIOx->MODER &= ~(0x3<<pin*2);
	GPIOx->MODER |= (1U<<pin*2);
	
	GPIOx->OSPEEDR &= ~(0x3<<pin*2);	//medium speed
	GPIOx->OSPEEDR |= (1U<<pin*2);
}



void GPIO_writePin(GPIO_TypeDef *GPIOx,uint8_t pin,uint8_t mode){
	GPIOx->ODR &= ~(1U<<pin);
	GPIOx->ODR |= (mode<<pin);
}


void GPIO_TogglePin(GPIO_TypeDef *GPIOx,uint8_t pin){
	GPIOx->ODR ^= (1U<<pin);
}

int main(){
		
	TIM3_Init();
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	
	
	for(int i=5;i<10;i++){
		GPIO_output_en(GPIOA,i);
	}
	
	GPIO_writePin(GPIOA,5,1);
	
	
	while(1){
		GPIO_TogglePin(GPIOA,5);
		delay_ms(100);
	}
	
	return 0;
	
}