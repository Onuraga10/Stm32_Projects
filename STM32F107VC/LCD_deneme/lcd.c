#include "main.h"
#include "lcd.h"



void LCD_Enable_Pulse(void) {
    GPIOE->ODR |= E;   // EN = 1
    delay_ms(1);               // Short delay
    GPIOE->ODR &= ~E;  // EN = 0
    delay_ms(1);              // Command execution time
}

void LCD_Send_Nibble(unsigned char nibble) {
    // Clear D4-7 and set new value
		GPIOE->ODR &= ~(0xF000); //Clear d4-7
		GPIOE->ODR |= ((nibble & 0x0F) << 12);
    LCD_Enable_Pulse();
}

void LCD_Send_Command(unsigned char cmd) {
    GPIOE->ODR &= ~RS;  // RS = 0 (Command mode)
    LCD_Send_Nibble(cmd >>4);   // Send upper nibble
    LCD_Send_Nibble(cmd & 0x0F); // Send lower nibble
    delay_ms(2);
}

void LCD_Send_Data(unsigned char data) {
    GPIOE->ODR |= RS;   // RS = 1 (PE9 - Data mode)
    LCD_Send_Nibble(data >>4);   // Send upper nibble
    LCD_Send_Nibble(data & 0x0F); // Send lower nibble
    delay_ms(1);
}

void LCD_Init(void){
	
	GPIOE->ODR &= ~(0xFF << 8);
	delay_ms(50);
	
	LCD_Send_Nibble(0x03);
  delay_ms(5);   
    
  LCD_Send_Nibble(0x03);
  delay_us(150); 
    
  LCD_Send_Nibble(0x03);
  delay_us(50);  
    
   
  LCD_Send_Nibble(0x02);
  delay_us(50);
	LCD_Send_Command(0x28);
	delay_us(50);
	
	
	LCD_Send_Command(0x0C); //Display ON cursor OFF
	delay_ms(1);
	LCD_Send_Command(0x01); //Display Clear
	delay_ms(2);
	LCD_Send_Command(0x06); //Entry mode set
}

void LCD_Print(char *str) {
    while(*str) {
        LCD_Send_Data(*str++);
    }
}

void LCD_SetCursor(unsigned char row, unsigned char col) {
    uint8_t address = (row == 0) ? 0x00 : 0x40;
    address += col;
    LCD_Send_Command(0x80 | address);  // Set DDRAM address
}

void LCD_Clear(void){
	LCD_Send_Command(0x01);
	delay_ms(2);	
}