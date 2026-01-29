void usart2_send_char(char c);
char usart2_receive_char();
void usart2_send_string(char *s);
void usart2_receive_string(char *buffer, unsigned int max_len);
void usart2_Init(void);
int usart2_data_available(void);
char usart2_read_buffer(void);
void usart2_send_string_DMA(const char *s);