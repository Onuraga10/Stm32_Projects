#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>

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


void usart1_init(void){
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;    
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	
	GPIOA->CRH    &= ( GPIO_CRH_MODE9 |
                       GPIO_CRH_CNF9 |
                       GPIO_CRH_MODE10 |
                       GPIO_CRH_CNF10 );
  GPIOA->CRH    |= ( ( 0x1 << 4 ) |   
                       ( 0x2 << 6 ) |
                       ( 0x0 << 8 ) |
                       ( 0x1 << 10 ) );
	
	USART1->BRR = (468<<4) | 12; //9600 baud rate
	
	USART1->CR1 |= (USART_CR1_TE | USART_CR1_UE | USART_CR1_RE);
	
}

void usart1_send_char(char c){
	while(!(USART1->SR & USART_SR_TXE));
	USART1->DR = c;
}

char usart1_receive_char(void) {
    while (!(USART1->SR & USART_SR_RXNE));  
    return (char) USART1->DR;                     
}

void usart1_send_string(char *s) {
    while (*s) {
        usart1_send_char(*s++);
    }
}
int main(){
	usart1_init();
	config_systick();

	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	
	GPIOD->CRL &= ~(0xF <<0);
	GPIOD->CRL |= (0x2<<0);
	
	usart1_send_string("OK\r\n");
		char rxb = '\0';
    while (1) {
			while(!(USART1->SR & USART_SR_RXNE));	 
			rxb = USART1->DR;
			
			while(!(USART1->SR & USART_SR_TXE));
			USART1->DR = rxb;
					
			
    }
		return 0;
}