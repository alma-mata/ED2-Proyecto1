/*
 * I2C_LIB.h
 *
 * Created: 2/10/2026 8:06:07 AM
 *  Author: itzle
 */ 


#ifndef I2C_LIB_H_
#define I2C_LIB_H_

#include <avr/io.h>
#include <stdint.h>


#ifndef F_CPU
#define F_CPU 16000000UL
#endif


void I2C_Master_Init(unsigned long SCL_Clock, uint8_t Prescaler);
uint8_t I2C_Master_Start(void);
uint8_t I2C_Master_RepeatedStart(void);
void I2C_Master_Stop(void);
uint8_t I2C_Master_Write(uint8_t dato);
uint8_t I2C_Master_Read(uint8_t *buffer, uint8_t ack);
void I2C_Slave_Init(uint8_t address);



#endif /* I2C_LIB_H_ */