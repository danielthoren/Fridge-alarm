/*
 * Fridge Beeper.cpp
 *
 * Created: 11/9/2021 9:48:48 PM
 * Author : Daniel Thorén
 */

#include "common_defines.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "millis.h"
#include "buzzer.h"
#include "door.h"

#define REFRIG_PIN  PB3 // Same as PCINT3 (Pin 1)
#define FREEZER_PIN PB4 // Same as PCINT4 (Pin 3)
#define BEEP_PIN    PB1 // Same as PCINT1 (Pin 6)
#define LED_PIN     PB2 // Same as PCINT0 (Pin 7)

static constexpr int DEBOUNCE_TIME_MS{5};

Door fridge{};
Door freezer{};

void init()
{
  cli();

  // Set output/input (1 = output, 0 = input)
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

// This is the interrupt handler called when there is any change on the INT_PIN
// ISR is defined in the headers - the ATtiny45 only has one handler
ISR(PCINT0_vect)
{
  bool initial_refrig_state = PINB & (1 << REFRIG_PIN);
  bool initial_freezer_state = PINB & (1 << FREEZER_PIN);

  // _delay_ms(DEBOUNCE_TIME_MS);

  bool refrig_state = PINB & (1 << REFRIG_PIN);
  bool freezer_state = PINB & (1 << FREEZER_PIN);

  if (refrig_state == initial_refrig_state)
  {
    fridge.set_state(refrig_state);
  }

  if (freezer_state == initial_freezer_state)
  {
    freezer.set_state(freezer_state);
  }
}

void set_led_on()
{
  PORTB |= (1 << LED_PIN);    // Turning ON LED connected to PB1
}

void set_led_off()
{
  PORTB &= ~(1 << LED_PIN);   //Turning the LED off
}

bool led_on{false};
millis_t timer{0};

void toggle_led(millis_t timeout)
{
  if (has_expired(timer))
  {
    if (led_on)
    {
      set_led_off();
      led_on = false;
    }
    else
    {
      set_led_on();
      led_on = true;
    }

    timer = millis() + timeout;
  }
}

int main(void)
{
  init();
  init_millis();
  init_buzzer(BEEP_PIN);

  while (1)
  {
    fridge.update();
    freezer.update();

    toggle_led(1000);
  }
}
