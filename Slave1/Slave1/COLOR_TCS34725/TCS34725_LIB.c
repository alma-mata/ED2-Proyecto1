/*
 * TCS34725_LIB.c
 *
 * Created: 2/10/2026 8:09:00 AM
 *  Author: itzle
 */ 



#include "TCS34725_LIB.h"
#include <util/delay.h>


#define ADDR_WRITE  ((DIRECCION_SENSOR << 1) | 0)
#define ADDR_READ   ((DIRECCION_SENSOR << 1) | 1)

void Escribir_Reg(uint8_t reg, uint8_t valor) {
	I2C_Master_Start();
	I2C_Master_Write(ADDR_WRITE);
	I2C_Master_Write(COMANDO_BIT | reg);
	I2C_Master_Write(valor);
	I2C_Master_Stop();
}

void Iniciar_Sensor(tiempo_t t, ganancia_t g) {

	Escribir_Reg(REG_ENABLE, CMD_ENCENDER);
	_delay_ms(3);
	

	Escribir_Reg(REG_ENABLE, CMD_ENCENDER | CMD_ACTIVAR_ADC);
	

	Escribir_Reg(REG_TIEMPO, t);
	Escribir_Reg(REG_CONTROL, g);
}


void Leer_Sensor(DatosColor *d) {
	uint8_t bajo, alto;

	I2C_Master_Start();
	I2C_Master_Write(ADDR_WRITE);
	I2C_Master_Write(COMANDO_BIT | REG_DATOS); 
	
	I2C_Master_RepeatedStart();
	I2C_Master_Write(ADDR_READ);

	I2C_Master_Read(&bajo, 1);
	I2C_Master_Read(&alto, 1);
	d->c = (uint16_t)(alto << 8) | bajo;

	//LEER ROJO
	I2C_Master_Read(&bajo, 1);
	I2C_Master_Read(&alto, 1);
	d->r = (uint16_t)(alto << 8) | bajo;

	//LEER VERDE
	I2C_Master_Read(&bajo, 1);
	I2C_Master_Read(&alto, 1);
	d->g = (uint16_t)(alto << 8) | bajo;

	//LEER AZUL
	I2C_Master_Read(&bajo, 1);
	I2C_Master_Read(&alto, 0);
	d->b = (uint16_t)(alto << 8) | bajo;

	I2C_Master_Stop();
}