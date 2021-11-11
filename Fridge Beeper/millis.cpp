
#define F_CPU  1000000

// Calculate the value needed for 
// the CTC match value in OCR1A.
#define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 8) 
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <limits.h>
 
volatile unsigned long timer1_millis;
long milliseconds_since;
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

unsigned long millis ()
{
	if (!is_inited)
	{
		return ULONG_MAX;
	}
	
    unsigned long millis_return;

    // Ensure this cannot be disrupted
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        millis_return = timer1_millis;
    }
 
    return millis_return;
}