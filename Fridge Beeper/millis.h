#ifndef MILLIS
#define MILLIS

#include <stdint.h>

// Used internally, not for external use
using signed_millis_t = int32_t;

using millis_t = uint32_t;

/*
 * This module uses timer0 of the avr
 */
void init_millis();

/*
 * Returns current milliseconds
 */
millis_t millis();

/**
 * Checks if the given time point has passed. This function takes
 * overflow into account.
 */
bool has_expired(millis_t const& time_point);

#endif
