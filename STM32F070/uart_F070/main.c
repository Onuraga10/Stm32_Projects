#include "stm32f0xx.h"
#include <string.h>
#include "usart.h"



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

const char *long_message = 
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
"Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
"nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in "
"reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur."
"Lorem ipsum dolor sit amet, consectetur adipiscing elit. "
"Sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.\n"
"Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris "
"nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in "
"reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur.\r\n";

int main(){
	TIM3_Init();
	
	usart2_Init();
	
	GPIOA->MODER &= ~(0xF<<10); 
	GPIOA->MODER |= (0x1<<10);
	
	usart2_send_string_DMA("--- MULTITASKING DEMO ---\r\n");
  usart2_send_string_DMA("Press 'p' for Polling \r\n");
  usart2_send_string_DMA("Press 'd' for DMA \r\n");

  uint32_t last_blink_time = 0;
    
  while(1) {
        //Using a manual delay to not block the CPU
        if ((TIM3->CNT - last_blink_time) > 100) {
            GPIOA->ODR ^= (1U << 5); 
            last_blink_time = TIM3->CNT;
        }
        if (usart2_data_available()) {
            char cmd = usart2_read_buffer();
            
						//POLLING
            if (cmd == 'p' || cmd == 'P') {
                usart2_send_string("\r\n[POLLING] Sending data... CPU is BLOCKED.\r\n");
                usart2_send_string((char*)long_message);        
                usart2_send_string("[POLLING] Done. CPU released.\r\n");
            }
            
            //DMA
            else if (cmd == 'd' || cmd == 'D') {               
                usart2_send_string_DMA("\r\n[DMA] Sending data... CPU is FREE.\r\n");
                while(DMA1_Channel4->CNDTR > 0); // Brief safety wait
                usart2_send_string_DMA(long_message);
							
            }
        }
    }
		return 0;
}