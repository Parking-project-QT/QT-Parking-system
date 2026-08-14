#include "device.h"
#include "uart.h"

#define UART_RX_BUFFER_SIZE 128U
#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE - 1U)

static volatile unsigned char rx_buffer[UART_RX_BUFFER_SIZE];
static volatile unsigned int rx_head;
static volatile unsigned int rx_tail;

static void Uart1_WriteByte(unsigned char byte)
{
    while (!Macro_Check_Bit_Set(USART1->SR, 7))
    {
    }

    USART1->DR = byte;
}

void Uart1_Init(unsigned int baud)
{
    unsigned int brr;

    rx_head = 0U;
    rx_tail = 0U;

    Macro_Set_Bit(RCC->AHB1ENR, 0);  /* GPIOA clock */
    Macro_Set_Bit(RCC->APB2ENR, 4);  /* USART1 clock */

    /* PA9=USART1_TX, PA10=USART1_RX, alternate function 7. */
    Macro_Write_Block(GPIOA->MODER, 0xFU, 0xAU, 18);
    Macro_Write_Block(GPIOA->AFR[1], 0xFFU, 0x77U, 4);
    Macro_Write_Block(GPIOA->PUPDR, 0xFU, 0x5U, 18);
    Macro_Write_Block(GPIOA->OSPEEDR, 0xFU, 0xAU, 18);

    /* With OVER8=0 the encoded BRR value is PCLK2 / baud. */
    brr = (PCLK2 + (baud / 2U)) / baud;
    USART1->BRR = brr;
    USART1->CR2 = 0U;
    USART1->CR3 = 0U;
    USART1->CR1 = (1U << 13) | /* USART enable */
                  (1U << 5)  | /* RX-not-empty interrupt */
                  (1U << 3)  | /* transmitter enable */
                  (1U << 2);   /* receiver enable */

    NVIC_ClearPendingIRQ(USART1_IRQn);
    NVIC_EnableIRQ(USART1_IRQn);
}

int Uart1_ReadByte(unsigned char *byte)
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

void Uart1_Write(const char *text)
{
    while (*text != '\0')
    {
        Uart1_WriteByte((unsigned char)*text++);
    }
}

void Uart1_WriteLine(const char *text)
{
    Uart1_Write(text);
    Uart1_WriteByte('\r');
    Uart1_WriteByte('\n');
}

void Uart1_WriteUnsigned(unsigned int value)
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
        Uart1_WriteByte((unsigned char)digits[--count]);
    }
}

void USART1_IRQHandler(void)
{
    unsigned int status = USART1->SR;

    if ((status & ((1U << 5) | (1U << 3) | (1U << 2) | (1U << 1))) != 0U)
    {
        unsigned char byte = (unsigned char)USART1->DR;

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
