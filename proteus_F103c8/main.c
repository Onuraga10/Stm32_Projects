#include "stm32f10x.h"




int main(){
		
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRL &= ~(0xF<<0); //clear PA0
	GPIOA->CRL |= (0x02<<0); //general output push pull speed=2MHZ CNF:MODE=0010 
	 
	
	
}