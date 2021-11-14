#include <avr/io.h>

void init_buzzer(int pin);

void beep_off();

void beep_on();

void beep_once(uint16_t time_ms);