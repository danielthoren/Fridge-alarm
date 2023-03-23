#ifndef ATTINY45_HAL
#define ATTINY45_HAL

#include <avr/io.h>

/**
 * @brief Returns the current state of the given pin.
 *        The pin must be configured as a input
 *
 * @param[inout] port The port (io register) to read from
 * @param[in] pin The pin to read from
 * @return true if pin is high, else false
 */
volatile bool ReadPin(volatile uint8_t* port, uint8_t pin);

#endif
