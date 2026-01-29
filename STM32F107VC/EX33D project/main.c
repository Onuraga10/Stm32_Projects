#include "stm32f10x.h"

void enable_clock(){
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN |
                  RCC_APB2ENR_IOPDEN |
									RCC_APB2ENR_AFIOEN; //Enable Clock for all ports
}

void gpio_output_enable(GPIO_TypeDef * port,uint32_t bit){ //Configure as output push-pull 50mhz
			port->CRL &= ~(0xF << bit*4);
			port->CRL |= (0x3 << bit*4);
}
void gpio_input_enable(GPIO_TypeDef * port,uint32_t bit){ //Configure as input pull-up
		port->CRL &= ~(0xF <<bit*4);
		port->CRL |= (0x8 <<bit*4);
		port->ODR |= (1<<bit);
}

void config_systick(){
	SysTick->LOAD = (SystemCoreClock / 1000) - 1;
	SysTick->VAL = 0;
	SysTick->CTRL = (1 << 2) |  // CLKSOURCE
                  (1 << 1) |  // TICKINT
                  (1 << 0);   // ENABLE
}

volatile uint32_t msTicks = 0;

void SysTick_Handler(void) {
    msTicks++;  
}

void delay_ms(uint32_t ms){
	uint32_t start = msTicks;
	while((msTicks - start) < ms);
}

void blink_row(GPIO_TypeDef *port,uint32_t speed){
	for(int i=0;i<8;i++){
			port->ODR |= (1<<i);
			
			delay_ms(speed);
		}
	for(int i=0;i<8;i++){
			port->ODR &= ~(1<<i);
			delay_ms(speed);
		}
}

uint8_t gpio_readbit(GPIO_TypeDef *GPIOx,uint16_t bit){
	uint8_t bitstatus;
	
	if((GPIOx->IDR & (1<<bit)) != 0){
		bitstatus = 1;	
	}
	else{
		bitstatus = 0;
	}
	return bitstatus;
}

volatile uint32_t last_press_time = 0;

void EXTI3_IRQHandler(void){
	if(EXTI->PR & (1<<3)){
		EXTI->PR |= (1<<3);
	}
	uint32_t now = msTicks;
  if ((now - last_press_time) > 200) {  // 50 ms debounce
            last_press_time = now;
            GPIOC->ODR ^= (1 << 0);  // Toggle PC0
        }
}

int main(){
	enable_clock();
	config_systick();
	for(int i=0;i<8;i++){
		gpio_output_enable(GPIOD,i);
	}
	gpio_input_enable(GPIOC,3); 
	gpio_output_enable(GPIOC,0);
	
	//Map EXTI3 to PC3
	AFIO->EXTICR[0] &= ~(0xF <<12); 
	AFIO->EXTICR[0] |= (0x2 <<12);
	
	
	EXTI->IMR |= (1<<3);	
	EXTI->FTSR |= (1<<3);
	
	NVIC_EnableIRQ(EXTI3_IRQn);
	
	
	//Blinking rows will activate when button 73 is pressed'PC3'
	while (1) {
			if(gpio_readbit(GPIOC,0)){
				blink_row(GPIOD,50);
			}
		
	}

	
	return 0;
	
}