#ifndef UART_H
#define UART_H

void Uart2_Init(unsigned int baud);
int Uart2_ReadByte(unsigned char *byte);
void Uart2_Write(const char *text);
void Uart2_WriteLine(const char *text);
void Uart2_WriteUnsigned(unsigned int value);

#endif
