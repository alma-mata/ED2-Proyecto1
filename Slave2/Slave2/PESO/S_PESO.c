/*
 * S_PESO.h
 *
 * Created: 2/15/2026 4:30:01 PM
 *  Author: itzle
 */ 

#include "S_PESO.h"
#include <util/delay.h>

static int32_t OFFSET = 0;

void PESO_init(void) {
	PESO_DDR |= (1 << SCK);
	PESO_DDR &= ~(1 << DT);
	PESO_PORT &= ~(1 << SCK);
}

int32_t PESO_read(void) {
	while (PESO_PIN & (1 << DT));
	int32_t val = 0;
	for (uint8_t i = 0; i < 24; i++) {
		PESO_PORT |= (1 << SCK);
		_delay_us(1);
		val = val << 1;
		PESO_PORT &= ~(1 << SCK);
		_delay_us(1);
		if (PESO_PIN & (1 << DT)) val++;
	}
	PESO_PORT |= (1 << SCK);
	_delay_us(1);
	PESO_PORT &= ~(1 << SCK);
	if (val & 0x800000) val |= 0xFF000000;
	return val;
}

void PESO_tara(void) {
	int64_t sum = 0;
	for (uint8_t i = 0; i < 10; i++) sum += PESO_read();
	OFFSET = (int32_t)(sum / 10);
}

uint8_t PESO_get(void) {

	int32_t g = (PESO_read() - OFFSET) / 420;

	if (g < 0) 
	{
		return 0;
	
	}
	if (g > 255) 
	{
		return 255;	
	}
	
	return (uint8_t)g;
}