#ifndef UART_H
#define UART_H

void Uart1_Init(unsigned int baud);
int Uart1_ReadByte(unsigned char *byte);
void Uart1_Write(const char *text);
void Uart1_WriteLine(const char *text);

#endif

