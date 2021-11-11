/*
 * Fridge Beeper.cpp
 *
 * Created: 11/9/2021 9:48:48 PM
 * Author : Daniel Thorén
 */ 

#define F_CPU  1000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <time.h>

#include "millis.h"

#define REFRIG_PIN  PB3 // Same as PCINT3
#define FREEZER_PIN PB4 // Same as PCINT4
#define BEEP_PIN    PB1 // Same as PCINT1
#define LED_PIN     PB2 // Same as PCINT0

#define TIMEOUT            1000 // Open time until beep [ms] 100 sek
#define BEEP_TIME_PERIOD   500     // Time period for beeping

int const level = 120;

volatile bool refrig_open = false;
volatile time_t refrig_open_time = 0;
volatile bool freezer_open = false;
volatile time_t freezer_open_time = 0;


void beep_off()
{
	OCR1A = 0x00;
	DDRB = 0 << BEEP_PIN;
}

void beep_on()
{
	DDRB = 1 << BEEP_PIN;
	OCR1A = 150;
}

void led_on()
{
	PORTB |= (1 << LED_PIN);    // Turning ON LED connected to PB1
}

void led_off()
{
	PORTB &= ~(1 << LED_PIN);   //Turning the LED off
}


void init()
{
	cli();

 // Set output pin PB1 (6) as ouptut and PB3 (2), PB4 (3) as input
 DDRB = 0;
 DDRB |= (1 << BEEP_PIN | 0 << REFRIG_PIN| 0 << FREEZER_PIN | 1 << LED_PIN);

 SREG |= 0x10000000;     //global interrupt enable

 // Set interrupt on any logical change
 MCUCR = 0 << ISC01 | 1 << ISC00;

 // INT0: Set 'External Interrupt Request 0' enable
 // PCIE: Set 'Pin Change Interrupt Enable' enable to trigger interrupt on any logical change
 GIMSK = 1 << PCIE;

 // Enable interrupts on PB3 and PB4
 PCMSK = 0;
 PCMSK = 1 << REFRIG_PIN | 1 << FREEZER_PIN;

 // PWM1A: Enable PWM mode: Reset counter value on match wich OCR1A
 // COM1A1:  COM1A0:
 //   0         1    -> Toggle the OC1A output line.
 // CS13, CS12, CS11, CS10 : Prescaler
 TCCR1 = 0;
 TCCR1 = 1 << PWM1A | 0 << COM1A1 | 1 << COM1A0 | 0 << CS13 | 0 << CS12 | 1 << CS11 | 0 << CS10;

 // Disable pullup for all pins
 MCUCR = 1 << PUD;
 
 sei();
}

void beep_once()
{
	beep_on();
	led_on();
	
	_delay_ms(BEEP_TIME_PERIOD);
	
	beep_off();
	led_off();
	
	_delay_ms(BEEP_TIME_PERIOD);
}

// This is the interrupt handler called when there is any change on the INT_PIN
// ISR is defined in the headers - the ATtiny45 only has one handler
ISR(PCINT0_vect)
{
	bool refrig_state = PINB & (1 << REFRIG_PIN);
	bool freezer_state = PINB & (1 << FREEZER_PIN);
	
	if( refrig_state ) {
		refrig_open = true;
		refrig_open_time = time(NULL);
			beep_once();
	}
	else
	{
		refrig_open = false;
		refrig_open_time = 0;
	}

	if( freezer_state ) {
		freezer_open= true;
		freezer_open_time = time(NULL);
	}
	else
	{
		freezer_open = false;
		freezer_open_time = 0;
	}
}

int main(void)
{
	init();
	
	
    /* Replace with your application code */
    while (1) 
    {
		  led_on();
		  _delay_ms(500);
		  led_off();
		  _delay_ms(500);
		  
		  if (refrig_open && time(NULL) - refrig_open_time > TIMEOUT)
		  {
			  beep_once();
		  }
		  else if (freezer_open && time(NULL) - freezer_open_time > TIMEOUT)
		  {
			  beep_once();
		  }
		
    }
}

