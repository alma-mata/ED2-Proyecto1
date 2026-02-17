/*
 * S_PESO.h
 *
 * Created: 2/15/2026 4:30:08 PM
 *  Author: itzle
 */ 


#ifndef S_PESO_H_
#define S_PESO_H_

#include <avr/io.h>
#include <stdint.h>

#define PESO_DDR   DDRC
#define PESO_PORT  PORTC
#define PESO_PIN   PINC
#define DT         PC2
#define SCK        PC3

void PESO_init(void);
int32_t PESO_read(void);
void PESO_tara(void);
uint8_t PESO_get(void);


#endif /* S_PESO_H_ */