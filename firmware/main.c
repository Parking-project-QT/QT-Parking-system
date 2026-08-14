#include "device.h"

static void System_Init(void)
{
    /* Enable the Cortex-M4 floating-point unit before using hard-float code. */
    SCB->CPACR |= (0x3UL << 20) | (0x3UL << 22);
    Clock_Init();
}

void Main(void)
{
    System_Init();

    for (;;)
    {
        __WFI();
    }
}

