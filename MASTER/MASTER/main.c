/*
 * MAESTRO.c
 *
 * Created: 2/7/2026 11:33:12 AM
 * Author : itzle
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "SPI_LIB/SPI_LIB.h"
#include "LCD/LCD_LIB.h"
#include "I2C_LIB/I2C_LIB.h"

#define ESCLAVO_1 0x08
#define ESCLAVO_COLOR DDB2
#define SS_LOW()  PORTB &= ~(1 << ESCLAVO_COLOR)
#define SS_HIGH() PORTB |=  (1 << ESCLAVO_COLOR)

void setup(){
	DDRB |= (1 << ESCLAVO_COLOR);
	SS_HIGH();
	INICIAR_LCD();
	_delay_ms(100);
	SPI_INIT(SPI_MASTER_DIV16, DATA_MSB, CLOCK_LOW, FIRST_EDGE);
	I2C_Master_Init(100000, 1);
}

int main(void) {
	setup();
	
	uint8_t COLOR_RECIBIDO = 0;
	uint8_t DISTANCIA_RECIBIDA = 0;
	uint8_t PESO_RECIBIDO = 0;
	char buffer_lcd[16];
	_delay_ms(300);
	while (1) {
		
		SS_LOW();
		SPI_WRITE(1);
		_delay_us(100);
		SPI_WRITE(0x00);
		COLOR_RECIBIDO = SPDR;
		SS_HIGH();
		

		if (I2C_Master_Start()) {
			uint8_t ack_dir = I2C_Master_Write((ESCLAVO_1 << 1) | 1);
			
			if (ack_dir == 1 || ack_dir == 0x40) {
				//PRIMER BYTE, ACK = 1
				I2C_Master_Read(&DISTANCIA_RECIBIDA, 1);
				
				//SEGUNDO BYTE, NACK = 0
				I2C_Master_Read(&PESO_RECIBIDO, 0);
			}
			I2C_Master_Stop();
		}

		CURSOR(0,0);
		LCD_WRITE_STRING("COL  DIST  PESO ");

		//VALORES DE COLOR
		CURSOR(1,0);
		if (COLOR_RECIBIDO == '1')   
		{
		   LCD_WRITE_STRING("AZUL ");
		}
		else if (COLOR_RECIBIDO == '2') 
		{
			LCD_WRITE_STRING("ROJO ");	
		}
		
		else
		{                            
			LCD_WRITE_STRING("NADA ");
		}
	
		//VALORES DE DISTANCIA
		sprintf(buffer_lcd, "%3dcm ", DISTANCIA_RECIBIDA);
		CURSOR(1,5);
		LCD_WRITE_STRING(buffer_lcd);

		//VALORES DE PESO RECIBIDO
		sprintf(buffer_lcd, "%3dg", PESO_RECIBIDO);
		CURSOR(1,11); 
		LCD_WRITE_STRING(buffer_lcd);

		_delay_ms(200);
	}
}
