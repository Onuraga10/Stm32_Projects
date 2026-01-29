#include "main.h"
#include "lcd.h"


volatile uint32_t msTicks = 0;

void SysTick_Handler(void)
{
    msTicks++;
}

void Systick_setup(void){
		SysTick->LOAD = (SystemCoreClock / 1000) - 1; // 1ms tick
    SysTick->VAL = 0;
    SysTick->CTRL = 0x7;
}

void delay_ms(uint32_t ms)
{
    uint32_t start = msTicks;
    while ((msTicks - start) < ms);
}

void delay_us(uint32_t us)
{
    // Approximate microsecond delay (depends on SystemCoreClock)
    // For 72MHz: 72 cycles = 1µs
    uint32_t cycles = (SystemCoreClock / 1000000) * us;
    for(uint32_t i = 0; i < cycles / 3; i++) {
        __NOP();
    }
}
void gpio_output_enable(GPIO_TypeDef * port,uint8_t pin){ //Configure as output push-pull 50mhz
			if(pin < 8 && pin>=0){
				port->CRL &= ~(0xF << pin*4);
				port->CRL |= (0x3 << pin*4);
			}
			else if(pin>=8 && pin<=15){
				port->CRH &= ~(0xF << (pin-8)*4);
				port->CRH |= (0x3 << (pin-8)*4);
			}
}
int main(){
	Systick_setup();
	RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
	
	//GPIO Output configuration
	for(int i=8;i<16;i++){
		gpio_output_enable(GPIOE,i);
	}
	
	
	LCD_Init();
	LCD_SetCursor(0,0);
	LCD_Print("Merhaba");
	LCD_SetCursor(1,0);
	LCD_Print("Ben Onur");
	
	
	

	while(1){
		}
	return 0;
}