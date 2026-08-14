#include "device.h"
#include "uart.h"

#define UART_RX_BUFFER_SIZE 128U
#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE - 1U)

static volatile unsigned char rx_buffer[UART_RX_BUFFER_SIZE];
static volatile unsigned int rx_head;
static volatile unsigned int rx_tail;

static void Uart2_WriteByte(unsigned char byte)
{
    while (!Macro_Check_Bit_Set(USART2->SR, 7))
    {
    }

    USART2->DR = byte;
}

void Uart2_Init(unsigned int baud)
{
    unsigned int brr;

    rx_head = 0U;
    rx_tail = 0U;

    Macro_Set_Bit(RCC->AHB1ENR, 0);  /* GPIOA clock */
    Macro_Set_Bit(RCC->APB1ENR, 17); /* USART2 clock */

    /* PA2=USART2_TX, PA3=USART2_RX, alternate function 7. */
    Macro_Write_Block(GPIOA->MODER, 0xFU, 0xAU, 4);
    Macro_Write_Block(GPIOA->AFR[0], 0xFFU, 0x77U, 8);
    Macro_Write_Block(GPIOA->PUPDR, 0xFU, 0x5U, 4);
    Macro_Write_Block(GPIOA->OSPEEDR, 0xFU, 0xAU, 4);

    /* With OVER8=0 the encoded BRR value is PCLK2 / baud. */
    brr = (PCLK1 + (baud / 2U)) / baud;
    USART2->BRR = brr;
    USART2->CR2 = 0U;
    USART2->CR3 = 0U;
    USART2->CR1 = (1U << 13) | /* USART enable */
                  (1U << 5)  | /* RX-not-empty interrupt */
                  (1U << 3)  | /* transmitter enable */
                  (1U << 2);   /* receiver enable */

    NVIC_ClearPendingIRQ(USART2_IRQn);
    NVIC_EnableIRQ(USART2_IRQn);
}

int Uart2_ReadByte(unsigned char *byte)
{
    unsigned int tail = rx_tail;

    if (tail == rx_head)
    {
        return 0;
    }

    *byte = rx_buffer[tail];
    rx_tail = (tail + 1U) & UART_RX_BUFFER_MASK;
    return 1;
}

void Uart2_Write(const char *text)
{
    while (*text != '\0')
    {
        Uart2_WriteByte((unsigned char)*text++);
    }
}

void Uart2_WriteLine(const char *text)
{
    Uart2_Write(text);
    Uart2_WriteByte('\r');
    Uart2_WriteByte('\n');
}

void Uart2_WriteUnsigned(unsigned int value)
{
    char digits[10];
    unsigned int count = 0U;

    do
    {
        digits[count++] = (char)('0' + (value % 10U));
        value /= 10U;
    }
    while (value != 0U);

    while (count != 0U)
    {
        Uart2_WriteByte((unsigned char)digits[--count]);
    }
}

void USART2_IRQHandler(void)
{
    unsigned int status = USART2->SR;

    if ((status & ((1U << 5) | (1U << 3) | (1U << 2) | (1U << 1))) != 0U)
    {
        unsigned char byte = (unsigned char)USART2->DR;

        if ((status & (1U << 5)) != 0U)
        {
            unsigned int next = (rx_head + 1U) & UART_RX_BUFFER_MASK;

            if (next != rx_tail)
            {
                rx_buffer[rx_head] = byte;
                rx_head = next;
            }
        }
    }
}
