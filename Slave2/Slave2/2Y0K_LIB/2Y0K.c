/*
 * _2Y0K.c
 *
 * Created: 2/14/2026 7:24:51 PM
 *  Author: itzle
 */ 


#include "G2Y0K.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

uint16_t SHARP_ADC = 0;

void SHARP_INIT(uint8_t CANAL)
{
	//REFERENCIA A 5V
	ADMUX = (1<<REFS0)|(1<<ADLAR)|(CANAL&0x0F);
	
	//CONFIGURACION DEL ADC, INTERRUPCION
	ADCSRA = (1<<ADEN)|(1<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	
	ADCSRB = 0x00;
	ADCSRA |= (1<<ADSC);
}

uint8_t SHARP_OBTENER_DISTANCIA(void)
{
	uint16_t VALOR_SEGURO = SHARP_ADC;
	
	if(VALOR_SEGURO < 6)
	{
		return 80;
	}
	
	uint16_t distancia = 1200/(VALOR_SEGURO - 5);
	if(distancia > 80)
	{
		return 80;
	}
	if(distancia < 10)
	{
		return 10;
	}
	
	return(uint8_t)distancia;
	
}

ISR(ADC_vect)
{
	SHARP_ADC = ADCH;
}