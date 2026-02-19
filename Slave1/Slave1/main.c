/*
 * Universidad del Valle de Guatemala
 * IE3054: Electrónica Digital 2
 * Slave1.c
 * Autor: Alma Lisbeth Mata Ixcayau
 * Proyecto: Proyecto 1 - Digital 2, Esclavo 1
 * Descripcion: Esclavo encargado de mover el motor servo y stepper, además del sensor de color
 * Creado: 09/02/2026
*/

#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "I2C_LIB/I2C_LIB.h"
#include "COLOR_TCS34725/TCS34725_LIB.h"
#include "SPI_LIB/SPI_LIB.h"
#include "PWM1/PWM1.h"
#include "PWM2/PWM2.h"

#define PIN_ROJO  PIND2
#define PIN_AZUL  PIND4
#define PIN_DEBUG PIND5
#define DIR_STEPPER PIND7

volatile uint16_t contador_pasos = 0;
volatile char COLOR_DETECTADO = '0';
char ultimo_color = '0';

DatosColor c;

void setup(){
	DDRD |= (1 << PIN_ROJO) | (1 << PIN_AZUL) | (1 << PIN_DEBUG) | (1 << DIR_STEPPER);
	PORTD &= ~((1 << PIN_ROJO) | (1 << PIN_AZUL) | (1 << PIN_DEBUG));
	
	DDRB |= (1 << PORTB0);
	
	SPI_INIT(SPI_SLAVE, DATA_MSB, CLOCK_LOW, FIRST_EDGE);
	SPCR |= (1 << SPIE);
	
	I2C_Master_Init(100000, 1);
	Iniciar_Sensor(TIEMPO_50MS, GANANCIA_4X);
	
	PWM1_Init();
	PWM2_Init();
	
	update_DutyCycle1(1000);
	TIMSK2 &= ~(1 << OCIE2A);
	
	sei();
}

int main(void) {
	setup();
	while (1) {
		Leer_Sensor(&c);

		PORTD ^= (1 << PIN_DEBUG);
		
		
		if (c.c > 30) {
			
			// 1. ES AZUL
			if ( ((uint32_t)c.b * 3 > (c.r + c.g) * 1.2) && (c.b > 50) ) {
				COLOR_DETECTADO = '1';
				PORTD |= (1 << PIN_AZUL);
				PORTD &= ~(1 << PIN_ROJO);
				
				
				if (ultimo_color != '1') {
					PORTD |= (1 << DIR_STEPPER);
					contador_pasos = 200;
					TIMSK2 |= (1 << OCIE2A);
					ultimo_color = '1';
					_delay_ms(200);
				}
				update_DutyCycle1(1500);
			}
			
			// 2. ES ROJO
			else if ( (c.r > c.g * 3.5) && (c.r > c.b * 1.8) ) {
				COLOR_DETECTADO = '2';
				PORTD |= (1 << PIN_ROJO);
				PORTD &= ~(1 << PIN_AZUL);
				
				
				if (ultimo_color != '2') {
					PORTD &= ~(1 << DIR_STEPPER);
					contador_pasos = 200;
					TIMSK2 |= (1 << OCIE2A);
					ultimo_color = '2';
					_delay_ms(200);
				}
				update_DutyCycle1(1500);
			}
			
			
			else {
				COLOR_DETECTADO = '0';
				PORTD &= ~((1 << PIN_ROJO) | (1 << PIN_AZUL));
				
				ultimo_color = '3';
				update_DutyCycle1(1000);
			}
		}


		else {
			COLOR_DETECTADO = '0';
			PORTD &= ~((1 << PIN_ROJO) | (1 << PIN_AZUL));
			
			
		}
		
		_delay_ms(50);
	}
}

ISR(SPI_STC_vect){
	SPDR = COLOR_DETECTADO;
}

ISR(TIMER2_COMPA_vect)
{
	if (contador_pasos > 0) {
		PORTB ^= (1 << PORTB0);
		contador_pasos--;
		} else {
		PORTB &= ~(1 << PORTB0);
		TIMSK2 &= ~(1 << OCIE2A);
	}
}