
typedef unsigned long millis_t;

/*
This module uses timer0 of the avr
*/

void init_millis();

/*
 * Returns current milliseconds
 */
millis_t millis();