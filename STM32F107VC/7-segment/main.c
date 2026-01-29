#include "stm32f10x.h"

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



void gpio_output_enable(GPIO_TypeDef * port,uint32_t bit){ //Configure as output push-pull 50mhz
			if(bit < 8){
				port->CRL &= ~(0xF << bit*4);
				port->CRL |= (0x3 << bit*4);
			}
			else if(bit>=7 && bit<16){
				port->CRH &= ~(0xF << (bit-8)*4);
				port->CRH |= (0x3 << (bit-8)*4);
			}
			
}



void blink_row(GPIO_TypeDef *port,uint32_t speed){
	for(int i=8;i<16;i++){
			port->ODR |= (1<<i);
			
			delay_ms(speed);
		}
	for(int i=8;i<16;i++){
			port->ODR &= ~(1<<i);
			delay_ms(speed);
		}
}

void segment_display(uint8_t hex){
	GPIOD->ODR &= ~(0xFF<<8);
	GPIOD->ODR |= (hex<<8);
}

void animation(){
	GPIOD->ODR &= ~(0xFF<<8);
	GPIOD->ODR |= (1<<8);
	for(int i=1;i<7;i++){
		GPIOD->ODR |= (1<<(7+i));
		delay_ms(200);
		GPIOD->ODR &= ~(1<<(7+i));
	}
}


uint8_t numbers[]={
0x3F,
0x06,
0x5B,
0x4F,
0x66,
0x6D,
0x7D,
0x07,
0x7F,
0x67,
0x77,
0x7C,
0x39,
0x5E,
0x79,
0x71,
};

#define	A	0x77
#define	a	0x5F
#define	b	0x7C
#define	C	0x39
#define	c	0x58
#define	d	0x5E
#define	E	0x79
#define	F	0x71
#define	G	0x3D
#define	H	0x76
#define	h	0x74
#define	I	0x30
#define	J	0x1E
#define	L	0x38
#define	n	0x54
#define	O	0x3F
#define	o	0x5C
#define	P	0x73
#define	q	0x67
#define	r	0x50
#define	S	0x6D
#define	t	0x78
#define	U	0x3E
#define	u	0x1C
#define	y	0x6E


uint8_t sentence[]={
	b,E,r,q,E,o,r,o,S,P,u
};

int main(){
	
	RCC->APB2ENR |= RCC_APB2ENR_IOPDEN |
									RCC_APB2ENR_IOPEEN;
	config_systick();
	
	for(int i=0;i<4;i++){
		gpio_output_enable(GPIOE,i);
	}
	for(int i=0;i<16;i++){
		gpio_output_enable(GPIOD,i);
	}
	
	GPIOE->ODR |= (0xF<<0);
	
			
	/*
	for(int i=0;i<(int)sizeof(sentence);i++){
			segment_display(sentence[i]);
			delay_ms(500);
		}
	*/
	
	while(1){
		animation();
	}
	
	return 0;
}