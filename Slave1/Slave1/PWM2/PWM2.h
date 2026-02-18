/*
 * PWM2.h
 *
 * Created: 17/02/2026 18:06:00
 *  Author: Bren
 */ 


#ifndef PWM2_H_
#define PWM2_H_

#include <avr/io.h>

void PWM2_Init(void);
void update_DutyCycle2(uint16_t dutyCycle);



#endif /* PWM2_H_ */