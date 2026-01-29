//ONUR AGAOGLU LCD DRIVER HEADER FILE


#define RS (1U<<9)
#define E (1U<<8)

void LCD_Enable_Pulse(void);
void LCD_Send_Nibble(unsigned char nibble);
void LCD_Send_Command(unsigned char cmd);
void LCD_Send_Data(unsigned char data);
void LCD_Print(char *str);
void LCD_Init(void);
void LCD_SetCursor(unsigned char row, unsigned char col);
void LCD_Clear(void);