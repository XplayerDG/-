#ifndef USART_H
#define USART_H

#include <stdint.h>

void usart1_init(uint32_t baudrate);
void usart1_putc(char ch);
void usart1_puts(const char *s);
int usart1_getline(char *buf, uint32_t max_len);

#endif
