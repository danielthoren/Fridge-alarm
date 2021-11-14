
#define F_CPU  1000000

#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>

#include "buzzer.h"

static int beep_pin = PB1;

void init_buzzer(int pin)
{
	beep_pin = pin;
}

void beep_off()
{
	OCR1A = 0x00;
	DDRB = 0 << beep_pin;
}

void beep_on()
{
	DDRB = 1 << beep_pin;
	OCR1A = 150;
}

void beep_once(uint16_t time_ms)
{
	beep_on();
	_delay_ms(time_ms);
	beep_off();
	_delay_ms(time_ms);
}