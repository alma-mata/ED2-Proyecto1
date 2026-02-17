/*
 * I2C_LIB.c
 *
 * Created: 2/10/2026 8:05:56 AM
 *  Author: itzle
 */ 


#include "I2C_LIB.h"


void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler){

	DDRC &= ~((1<<DDC4)|(1<<DDC5));
	

	TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
	
	switch(Prescaler){
		case 1:
		
		TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
		break;
		case 4:
		TWSR &= ~(1<<TWPS1);
		TWSR |= (1<<TWPS0);
		break;
		case 16:
		TWSR &= ~(1<<TWPS0);
		TWSR |= (1<<TWPS1);
		break;
		case 64:
		TWSR |= (1<<TWPS1)|(1<<TWPS0);
		break;
		default:
		TWSR &= ~((1<<TWPS1)|(1<<TWPS0));
		Prescaler = 1;
		break;
	}
	

	TWBR = ((F_CPU/SCL_Clock)-16)/(2*Prescaler);

	TWCR |= (1<<TWEN);
}

uint8_t I2C_Master_Start(void){

	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	

	while (!(TWCR & (1<<TWINT)));
	
	return ((TWSR & 0xF8) == 0x08);
}

uint8_t I2C_Master_RepeatedStart(void){

	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));

	return ((TWSR & 0xF8) == 0x10);
}

void I2C_Master_Stop(void){
	TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWSTO);
	while(TWCR & (1<<TWSTO));
}

uint8_t I2C_Master_Write(uint8_t dato){
	uint8_t estado;
	
	TWDR = dato;
	TWCR = (1<<TWEN)|(1<<TWINT);
	
	while (!(TWCR & (1<<TWINT)));
	
	estado = TWSR & 0xF8;
	

	if (estado == 0x18 || estado == 0x28) {
		return 1;
		} else {
		return estado;
	}
}


uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack){
	uint8_t estado;
	
	if (ack) {

		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
		} else {

		TWCR = (1<<TWINT)|(1<<TWEN);
	}
	
	while (!(TWCR & (1<<TWINT)));
	
	estado = TWSR & 0xF8;
	

	if (ack && estado != 0x50)
	return 0;
	if (!ack && estado != 0x58)
	return 0;
	
	*buffer = TWDR;
	return 1;
}


void I2C_Slave_Init(uint8_t address){
	DDRC &= ~((1<<DDC4)|(1<<DDC5));
	

	TWAR = address << 1;
	

	TWCR = (1<<TWEA)|(1<<TWEN)|(1<<TWIE);
}