#include "device.h"
#include "led.h"
#include "motor.h"
#include "protocol.h"
#include "uart.h"

static void System_Init(void)
{
    /* Enable the Cortex-M4 floating-point unit before using hard-float code. */
    SCB->CPACR |= (0x3UL << 20) | (0x3UL << 22);
    Clock_Init();
    Led_Init();
    Motor_Init();
    Uart1_Init(115200U);
}

void Main(void)
{
    System_Init();
    Protocol_Init();
    Uart1_WriteLine("READY STM32F411");

    for (;;)
    {
        unsigned char byte;

        while (Uart1_ReadByte(&byte))
        {
            Protocol_ReceiveByte(byte);
        }

    }
}
