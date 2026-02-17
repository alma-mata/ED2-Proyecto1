/*
 * Universidad del Valle de Guatemala
 * IE2023: Programacion de Microcontroladores
 * PWM1.h
 * Autor: Alma Lisbeth Mata Ixcayau
 * Proyecto: Laboratorio 5
 * Descripcion: Control de servomotor con PWM usando Timer1 a 16MHz
 * Creado: 10/04/2025
*/

#ifndef PWM1_H_
#define PWM1_H_

#include <avr/io.h>

void PWM1_Init(void);
void update_DutyCycle1(uint16_t dutyCycle);

#endif /* PWM1_H_ */