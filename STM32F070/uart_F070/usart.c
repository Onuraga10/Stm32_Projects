//This is a driver for USART2 configuration on STM32F070RB microcontroller
//UART is used with 9600 baud rate and can send 8 data bits


#include "stm32f0xx.h"
#include "usart.h"
#include <string.h>
#define RX_BUFFER_SIZE 128

volatile char rx_buffer[RX_BUFFER_SIZE];
volatile uint16_t rx_head = 0;
volatile uint16_t rx_tail = 0;

//Interrupt operations
int usart2_data_available(void){
	return (rx_head != rx_tail);
}

char usart2_read_buffer(void){
	if(rx_head == rx_tail) return 0;
	char c = rx_buffer[rx_tail];
	rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
  return c;
}

void USART2_IRQHandler(void){
		if ((USART2->ISR & USART_ISR_RXNE)){
		char c = (char)(USART2->RDR); /* Receive data, clear flag */
		uint16_t next_head = (rx_head + 1) % RX_BUFFER_SIZE;
		if(next_head != rx_tail){
			rx_buffer[rx_head] = c;
			rx_head = next_head;
		}
	}
}
//-------------------------------------------------------------------------

//Polling operations
void usart2_send_char(char c){
	while(!(USART2->ISR & USART_ISR_TXE));
	USART2->TDR = c;
}

char usart2_receive_char(){
	while(!(USART2->ISR & USART_ISR_RXNE));
	return (char) USART2->RDR;
}

void usart2_send_string(char *s) {
    while (*s) {
        usart2_send_char(*s++);
    }
}
//---------------------------------------------------------------------------

//DMA
void usart2_send_string_DMA(const char *s){
	
	while (DMA1_Channel4->CNDTR > 0);
	uint16_t len = (uint16_t) strlen(s);
	DMA1_Channel4->CCR &= ~(DMA_CCR_EN); 
	DMA1_Channel4->CPAR = (uint32_t) &USART2->TDR; //(1)
	DMA1_Channel4->CMAR = (uint32_t) s; //(2)
	DMA1_Channel4->CNDTR = len; //(3)
	//(4)
	DMA1_Channel4->CCR |= DMA_CCR_DIR | DMA_CCR_MINC | DMA_CCR_EN;
}
//-----------------------------------------------------------------------------

void usart2_Init(void){
	unsigned int interrupt_enabled = 1;
	unsigned int dma_enabled=1;
	
	
	
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
	 //PA5 output
	
	GPIOA->MODER &= ~((0x3 << 4) | (0x3 << 6)); 
  GPIOA->MODER |= (0x2 << 4) | (0x2 << 6);
	GPIOA->OSPEEDR |= (0x3 << 4) | (0x3 << 6);
	GPIOA->AFR[0] &= ~((0xF << 8) | (0xF << 12)); 
  GPIOA->AFR[0] |= (0x1 << 8) | (0x1 << 12);
	
	//Assuming oversampling by 16
 //Formula is given in reference manual pg.716
	uint32_t baudrate= 9600;
	USART2->BRR = SystemCoreClock / baudrate;  
	USART2->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
	
	if(interrupt_enabled){
		USART2->CR1 |= USART_CR1_RXNEIE;
		NVIC_EnableIRQ(USART2_IRQn);
		NVIC_SetPriority(USART2_IRQn, 1);
	}
	if(dma_enabled){
		RCC->AHBENR |= RCC_AHBENR_DMA1EN;
		USART2->CR3 |= USART_CR3_DMAT;
	}
}
