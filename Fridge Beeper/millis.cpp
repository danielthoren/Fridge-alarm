#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <limits.h>

#include "millis.h"
#include "common_defines.h"

// Calculate the value needed for
// the CTC match value in OCR1A.
#define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 8)

volatile millis_t timer1_millis;
static bool is_inited;

ISR (TIMER0_COMPA_vect)
{
  timer1_millis++;
}

void init_millis()
{
  // Set prescaler to 8 (clk/8)
  TCCR0B |= (0 << CS02) |(1 << CS01) | (0 << CS00);

  // Set output compare such that ISR is triggered every ms
  OCR0A = static_cast<uint8_t>(CTC_MATCH_OVERFLOW);

  // Enable compare with register A
  TIMSK |= (1 << OCIE0A);

  is_inited = true;
}

millis_t millis()
{
  if (!is_inited)
  {
    return static_cast<millis_t>(ULONG_MAX);
  }

  millis_t millis_return;

  // Ensure this cannot be disrupted
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    millis_return = timer1_millis;
  }

  return millis_return;
}

bool has_expired(millis_t const& time_point)
{
  millis_t const delta = time_point - millis();
  return static_cast<signed_millis_t>(delta) <= 0;
}
