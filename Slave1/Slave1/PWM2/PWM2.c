/*
 * PWM2.c
 *
 * Created: 17/02/2026 18:05:52
 *  Author: Bren
 */ 


#include "PWM2.h"

void PWM2_Init(void)
{
	DDRB |= (1<<PINB0);
	TCCR2A = (1<<WGM21);
	TCCR2B = (1<<CS22)|(1<<CS21);
	OCR2A = 250;
	TIMSK2 = (1<<OCIE0A);
}

void update_DutyCycle2(uint16_t dutyCycle)
{
	OCR2A = dutyCycle;
}