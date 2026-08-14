#include "device.h"
#include "led.h"
#include "motor.h"

#include "uart.h"

#define USART_TEST_LINE_SIZE 64U

static void System_Init(void)
{
    /* Enable the Cortex-M4 floating-point unit before using hard-float code. */
    SCB->CPACR |= (0x3UL << 20) | (0x3UL << 22);
    Clock_Init();
    Led_Init();
    Motor_Init();
    Uart2_Init(115200U);
}

void Main(void)
{
    char line[USART_TEST_LINE_SIZE];
    unsigned int length = 0U;

    System_Init();
    Uart2_WriteLine("USART2 TEST READY");
    Uart2_WriteLine("TYPE A LINE AND PRESS ENTER");

    for (;;)
    {
        unsigned char byte;

        while (Uart2_ReadByte(&byte))
        {
            if (byte == '\r')
            {
                continue;
            }

            if (byte == '\n')
            {
                line[length] = '\0';
                Uart2_Write("ECHO: ");
                Uart2_WriteLine(line);
                length = 0U;
            }
            else if (length < (USART_TEST_LINE_SIZE - 1U))
            {
                line[length++] = (char)byte;
            }
            else
            {
                length = 0U;
                Uart2_WriteLine("ERROR: LINE TOO LONG");
            }

        }
    }
}
