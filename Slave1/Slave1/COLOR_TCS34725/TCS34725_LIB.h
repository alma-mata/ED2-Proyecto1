/*
 * TCS34725_LIB.h
 *
 * Created: 2/10/2026 8:09:09 AM
 *  Author: itzle
 */ 


#ifndef TCS34725_LIB_H_
#define TCS34725_LIB_H_

#include <stdint.h>

#include "../I2C_LIB/I2C_LIB.h"


#define DIRECCION_SENSOR    0x29
#define COMANDO_BIT         0x80

#define REG_ENABLE          0x00
#define REG_TIEMPO          0x01
#define REG_CONTROL         0x0F
#define REG_DATOS           0x14

#define CMD_ENCENDER        0x01
#define CMD_ACTIVAR_ADC     0x02


typedef enum {
	TIEMPO_2_4MS  = 0xFF,
	TIEMPO_24MS   = 0xF6,
	TIEMPO_50MS   = 0xEB,
	TIEMPO_101MS  = 0xD5,
	TIEMPO_154MS  = 0xC0,
	TIEMPO_700MS  = 0x00
} tiempo_t;


typedef enum {
	GANANCIA_1X   = 0x00,
	GANANCIA_4X   = 0x01,
	GANANCIA_16X  = 0x02,
	GANANCIA_60X  = 0x03
} ganancia_t;

typedef struct {
	uint16_t r;
	uint16_t g;
	uint16_t b;
	uint16_t c;
} DatosColor;

void Iniciar_Sensor(tiempo_t t, ganancia_t g);
void Leer_Sensor(DatosColor *d);


#endif /* TCS34725_LIB_H_ */