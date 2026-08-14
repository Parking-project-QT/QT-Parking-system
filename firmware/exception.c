#include "device.h"

void _Invalid_ISR(void)
{
    __disable_irq();
    for (;;)
    {
        /* Stay here so a debugger can inspect the faulting exception. */
    }
}

