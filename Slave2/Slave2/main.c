/*
 * _2Y0K.c
 *
 * Created: 2/14/2026 7:05:51 PM
 *  Author: itzle
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "2Y0K_LIB/G2Y0K.h"
#include "I2C_LIB/I2C_LIB.h"
#include "PWM1/PWM1.h"
#include "PESO/S_PESO.h"

#define LED_ROJO  PIND2
#define LED_VERDE PIND3
#define LED_AZUL  PIND4

#define MOTOR_IN1_PWM PINB1
#define MOTOR_IN2_DIR PINB2

volatile uint8_t distancia_actual = 80;
volatile uint8_t peso_actual = 0;

void SETUP(void) {
	DDRD |= (1<<LED_ROJO)|(1<< LED_VERDE)|(1<<LED_AZUL);
	DDRB |= (1<<MOTOR_IN2_DIR);
	PORTB &= ~(1<<MOTOR_IN2_DIR);
}

int main(void) {
	SETUP();
	PWM1_Init();
	I2C_Slave_Init(0x08);
	SHARP_INIT(0);
	
	PESO_init();
	PORTD |= (1<<LED_AZUL);
	PESO_tara();
	PORTD &= ~(1<<LED_AZUL);

	sei();

	while (1) {
		distancia_actual = SHARP_OBTENER_DISTANCIA();
		peso_actual = PESO_get();
		
		PORTD &= ~((1<<LED_ROJO)|(1<< LED_VERDE)|(1<< LED_AZUL));
		
		if (distancia_actual < 15)
		{
			PORTD |= (1 << LED_ROJO);
			update_DutyCycle1(0);
		}
		else if (distancia_actual >= 15 && distancia_actual <= 40)
		{
			PORTD |= (1<<LED_AZUL);
			update_DutyCycle1(12000);
		}
		else
		{
			PORTD |= (1<<LED_VERDE);
			update_DutyCycle1(19000);
		}

		_delay_ms(50);
	}
}

ISR(TWI_vect) {
	uint8_t estado = TWSR & 0xF8;
	
	if (estado == 0xA8)
	{
		TWDR = distancia_actual;
		TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	}
	else if (estado == 0xB8)
	{
		TWDR = peso_actual;
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWIE);
	}
	else
	{
		TWCR = (1<<TWINT)|(1<<TWEA)|(1<<TWEN)|(1<<TWIE);
	}
}
