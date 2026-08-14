#include "device.h"
#include "motor.h"

#define MOTOR_PWM_FREQUENCY 20000U
#define MOTOR_PWM_PERIOD    (TIMXCLK / MOTOR_PWM_FREQUENCY)
#define MOTOR_PWM_ARR       (MOTOR_PWM_PERIOD - 1U)

static MotorDirection current_direction;
static unsigned int current_duty;

static unsigned int DutyToCompare(unsigned int duty_percent)
{
    return ((MOTOR_PWM_PERIOD * duty_percent) + 50U) / 100U;
}

void Motor_Init(void)
{
    Macro_Set_Bit(RCC->AHB1ENR, 0); /* GPIOA clock */
    Macro_Set_Bit(RCC->APB1ENR, 3); /* TIM5 clock */

    /* PA0=TIM5_CH1, PA1=TIM5_CH2, alternate function 2. */
    Macro_Write_Block(GPIOA->MODER, 0xFU, 0xAU, 0);
    Macro_Write_Block(GPIOA->AFR[0], 0xFFU, 0x22U, 0);
    Macro_Write_Block(GPIOA->PUPDR, 0xFU, 0x0U, 0);
    Macro_Write_Block(GPIOA->OSPEEDR, 0xFU, 0xAU, 0);

    TIM5->CR1 = 1U << 7;  /* Auto-reload preload enable. */
    TIM5->PSC = 0U;
    TIM5->ARR = MOTOR_PWM_ARR;
    TIM5->CCR1 = 0U;
    TIM5->CCR2 = 0U;

    /* PWM mode 1 and preload enabled on channels 1 and 2. */
    TIM5->CCMR1 = (6U << 4) | (1U << 3) |
                  (6U << 12) | (1U << 11);
    TIM5->CCER = (1U << 0) | (1U << 4);
    TIM5->EGR = 1U;
    TIM5->SR = 0U;
    TIM5->CR1 |= 1U;

    current_direction = MOTOR_STOP;
    current_duty = 0U;
}

void Motor_Set(MotorDirection direction, unsigned int duty_percent)
{
    unsigned int compare;

    if (duty_percent > 100U)
    {
        duty_percent = 100U;
    }

    if ((direction == MOTOR_STOP) || (duty_percent == 0U))
    {
        TIM5->CCR1 = 0U;
        TIM5->CCR2 = 0U;
        current_direction = MOTOR_STOP;
        current_duty = 0U;
        return;
    }

    compare = DutyToCompare(duty_percent);

    if ((current_direction != MOTOR_STOP) && (current_direction != direction))
    {
        /* Commit a zero-output PWM cycle before reversing the H-bridge. */
        TIM5->CCR1 = 0U;
        TIM5->CCR2 = 0U;
        TIM5->EGR = 1U;
    }

    if (direction == MOTOR_CW)
    {
        /* Preserve the lab wiring: CW drives PA1/TIM5_CH2. */
        TIM5->CCR1 = 0U;
        TIM5->CCR2 = compare;
    }
    else if (direction == MOTOR_CCW)
    {
        TIM5->CCR2 = 0U;
        TIM5->CCR1 = compare;
    }
    else
    {
        TIM5->CCR1 = 0U;
        TIM5->CCR2 = 0U;
        direction = MOTOR_STOP;
        duty_percent = 0U;
    }

    current_direction = direction;
    current_duty = duty_percent;
}

MotorDirection Motor_GetDirection(void)
{
    return current_direction;
}

unsigned int Motor_GetDuty(void)
{
    return current_duty;
}
