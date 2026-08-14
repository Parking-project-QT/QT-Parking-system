#ifndef LED_H
#define LED_H

void Led_Init(void);
int Led_Set(unsigned int led_number, int on);
int Led_Get(unsigned int led_number);

#endif

