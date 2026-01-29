#include "stm32f10x.h"



#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_Bb4 466
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988
#define NOTE_Bb5 932




const uint16_t degmesin_melody[] = {
  NOTE_G4, NOTE_D4, NOTE_G4, NOTE_F5, 
	NOTE_C4, NOTE_Bb4,NOTE_A4, NOTE_Bb4,
	NOTE_C5,NOTE_Bb4, NOTE_A4,NOTE_Bb4,
	NOTE_C5,NOTE_A4, NOTE_F4,NOTE_A4,
	NOTE_C5, NOTE_Bb4, NOTE_A4,NOTE_Bb4,
	NOTE_A4,NOTE_G4,0,NOTE_E4,NOTE_F4,
	360,NOTE_G4
};

const uint16_t degmesin_durations[] = {
  400, 400, 400, 400,
	400, 400,120, 120,
	400, 400, 400,400,
	400, 400, 400,400,
	400, 400, 400,120,
	120,500,50,120,400,
	400,300
};




void config_systick_us(void) {
    SysTick->CTRL = 0;              // Disable first
    SysTick->LOAD = 72 - 1;         // 72 ticks = 1 us
    SysTick->VAL = 0;               // Clear counter
    SysTick->CTRL = (1 << 2) | (1 << 0);  // CLKSOURCE = core, ENABLE
}

void delay_us(uint32_t us) {
    for (uint32_t i = 0; i < us; i++) {
        SysTick->VAL = 0;                         // Restart the count
        while (!(SysTick->CTRL & (1 << 16)));     // Wait for COUNTFLAG
    }
}

void delay_ms(uint32_t ms) {
    while (ms--) delay_us(1000);
}

void beep(uint16_t frequency, uint32_t duration_ms) {
    uint32_t period_us = 1000000 / frequency;
    uint32_t half_period = period_us / 2;
    uint32_t cycles = (duration_ms * 1000) / period_us;

    for (uint32_t i = 0; i < cycles; i++) {
        GPIOC->ODR ^= (1 << 10);  // Toggle PC10
        delay_us(half_period);
    }

    GPIOC->ODR &= ~(1 << 10);  // Turn off buzzer
}

uint8_t button_pressed=0;
void play_degmesin_intro(void) {
    for (int i = 0; i < (int)(sizeof(degmesin_melody) / sizeof(uint16_t)); i++) {
        beep(degmesin_melody[i], degmesin_durations[i]);
        delay_ms(50); // Small pause between notes
    }
		button_pressed =1;
		GPIOC->ODR &= ~(1 << 10);
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

int main(void) {
    config_systick_us();

    // Enable GPIOC clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // PC10 = output push-pull, 2 MHz
    GPIOC->CRH &= ~(0xF << 8);     // ? Clear bits for PC10
    GPIOC->CRH |=  (0x2 << 8);     // MODE = 10 (2 MHz), CNF = 00 (push-pull)
		
		GPIOC->CRL &= ~(0xF <<12); //Set PC3 to input pull down
		GPIOC->CRL |= (0x8 <<12); 
		GPIOC->ODR &= ~(1<<3); //Pull down
		
		while(1){
			if(gpio_readbit(GPIOC,3)){
				play_degmesin_intro();
			}
		}
		
		
    

    return 0;

}