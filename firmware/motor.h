#ifndef MOTOR_H
#define MOTOR_H

typedef enum
{
    MOTOR_STOP = 0,
    MOTOR_CW,
    MOTOR_CCW
} MotorDirection;

void Motor_Init(void);
void Motor_Set(MotorDirection direction, unsigned int duty_percent);
MotorDirection Motor_GetDirection(void);
unsigned int Motor_GetDuty(void);

#endif

