/*
 * Universidad del Valle de Guatemala
 * IE3054: Electrónica Digital 2
 * Slave1.c
 * Autor: Alma Lisbeth Mata Ixcayau
 * Proyecto: Proyecto 1 - Digital 2, Esclavo 1
 * Descripcion: Esclavo encargado de mover el motor servo y stepper, además del sensor de color
 * Creado: 09/02/2026
*/
/****************************************/
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "I2C_LIB/I2C_LIB.h"
#include "COLOR_TCS34725/TCS34725_LIB.h"
#include "SPI_LIB/SPI_LIB.h"

#define PIN_ROJO  PIND2
#define PIN_AZUL  PIND4
#define PIN_DEBUG PIND5

DatosColor c;

volatile char COLOR_DETECTADO = 0;

void setup(){
	DDRD |= (1 << PIN_ROJO) | (1 << PIN_AZUL) | (1 << PIN_DEBUG);
	PORTD &= ~((1 << PIN_ROJO) | (1 << PIN_AZUL) | (1 << PIN_DEBUG));
	
	SPI_INIT(SPI_SLAVE, DATA_MSB, CLOCK_LOW, FIRST_EDGE);
	SPCR |= (1 << SPIE);
	
	I2C_Master_Init(100000, 1);
	Iniciar_Sensor(TIEMPO_50MS, GANANCIA_4X);
	sei();
}

int main(void) {
	setup();
	while (1) {
		Leer_Sensor(&c);

		PORTD ^= (1 << PIN_DEBUG);
		PORTD &= ~((1 << PIN_ROJO) | (1 << PIN_AZUL));
		
		if (c.c > 30) {
			if ( ((uint32_t)c.b * 3 > (c.r + c.g) * 1.2) && (c.b > 50) ) {
				PORTD |= (1 << PIN_AZUL);
				COLOR_DETECTADO = '1';
			}
			else if ( (c.r > c.g * 3.5) && (c.r > c.b * 1.8) ) {
				PORTD |= (1 << PIN_ROJO);
				COLOR_DETECTADO = '2';
			}
			else {
				COLOR_DETECTADO = '0';
			}
			} else {
			COLOR_DETECTADO = '0';
		}
		
		
		_delay_ms(100);
	}
}

ISR(SPI_STC_vect){

	SPDR = COLOR_DETECTADO;
	
}
