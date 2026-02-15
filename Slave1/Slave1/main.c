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
// Encabezado
#define F_CPU 16000000UL		//Frecuencia es 16Mhz
#include <avr/io.h>			
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>	// Librería de interrupciones
#include "PWM1/PWM1.h"
#include "UART/UART.h"
// #include "Stepper/Stepper.h"

/****************************************/
// Prototipos de función
void setup(void);

// Variables globales
volatile uint16_t dutyCycle = 0;
volatile uint8_t received_RX = 0;	// Dato recibido del UART
volatile uint8_t dato_ENVIADO = 0;	// Bandera de dato enviado UART
volatile uint8_t pulso = 0; 

/****************************************/
// Función principal

int main(void)
{
	setup();
    /* Replace with your application code */
    while (1) 
    {
		if (dato_ENVIADO)
		{
			if (received_RX == '1')
			{
				dutyCycle = 50;
			}
			else if (received_RX == '2')
			{
				dutyCycle = 512;
			}
			dato_ENVIADO = 0;
		}
		update_DutyCycle1(500 + (dutyCycle * 2000UL / 1023));
		
		if (pulso) PORTB |= (1 << PORTB0);
		else PORTB &= ~(1 << PORTB0);
    }
}

/****************************************/
// Subrutinas sin Interrupcion

void setup(void){
	cli();
	
	PWM1_Init();
	UART_init();
	// Configuración motor stepper
	
	// Habilitar interrupciones del TIMER2
	DDRB |= (1 << PINB0);		// El pulso se realizara en PB0
	TCCR2A = (1 << WGM21);				// Modo Normal
	TCCR2B = (1 << CS22) | (1 << CS21) ;	//Prescaler 256
	OCR2A = 250;							// Delay de 4ms
	TIMSK2 = (1 << OCIE0A);
	
	sei();
}

/****************************************/
// Subrutinas de Interrupcion

ISR(USART_RX_vect){			// Interrupción UART
	received_RX = UDR0;
	dato_ENVIADO = 1;
}

ISR(TIMER2_COMPA_vect){
	pulso = ((pulso + 1) & 0x01);
}