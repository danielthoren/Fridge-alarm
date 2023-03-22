#ifndef TIMER
#define TIMER

#include "millis.h"

using TimerHandle = int32_t;

constexpr int maxConsecutiveTimers = 4;

/**
 * @brief Registers a relative timer, executing the given function after the given delay [ms]
 *
 * @param[in] delay The delay in ms after which the given function is executed
 * @param[in] functionPtr The function to run when the timer has elapsed
 * @return The handle used to refer to this timer. Becomes invalid after timer has elapsed.
 */
TimerHandle RegisterRelativeTimer(millis_t const delay, void (*functionPtr)());

void Update();


#endif
