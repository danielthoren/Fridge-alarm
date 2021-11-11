
// Calculate the value needed for 
// the CTC match value in OCR1A.
#define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 8) 
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <limits>
 
volatile unsigned long timer1_millis;
long milliseconds_since;
static bool is_inited;
 
ISR (TIMER0_COMPA_vect)
{
    timer1_millis++;
}

void init()
{
	
	
	is_inited = true;
}

unsigned long millis ()
{
	if (!is_inited)
	{
		return std::numeric_limits<unsigned long>::max();
	}
	
    unsigned long millis_return;

    // Ensure this cannot be disrupted
    ATOMIC_BLOCK(ATOMIC_FORCEON) {
        millis_return = timer1_millis;
    }
 
    return millis_return;
}