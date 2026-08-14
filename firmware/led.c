#include "device.h"
#include "led.h"

#define LED1_PIN 0U /* PB0 */
#define LED2_PIN 1U /* PB1 */

void Led_Init(void)
{
    Macro_Set_Bit(RCC->AHB1ENR, 1); /* GPIOB clock */

    /* PB0/PB1: output, push-pull, low speed, no pull-up/down. */
    Macro_Write_Block(GPIOB->MODER, 0xFU, 0x5U, 0);
    Macro_Clear_Bit(GPIOB->OTYPER, LED1_PIN);
    Macro_Clear_Bit(GPIOB->OTYPER, LED2_PIN);
    Macro_Write_Block(GPIOB->OSPEEDR, 0xFU, 0x0U, 0);
    Macro_Write_Block(GPIOB->PUPDR, 0xFU, 0x0U, 0);

    /* Both external LEDs start off. BSRR upper half resets output bits. */
    GPIOB->BSRR = (1U << (LED1_PIN + 16U)) | (1U << (LED2_PIN + 16U));
}
