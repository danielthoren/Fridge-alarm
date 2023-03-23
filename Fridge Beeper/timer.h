#ifndef TIMER
#define TIMER

#include "millis.h"


namespace Timer
{
using TimerHandle = int32_t;

constexpr int maxConsecutiveTimers = 4;

/**
 * @brief Initializes the module
 */
void Init();

/**
 * @brief Updates all timers. Must be called periodically
 */
void Update();

/**
 * @brief Registers a relative timer, executing the given function after the given delay [ms]
 *
 * @param[in] delay The delay in ms after which the given function is executed
 * @param[in] functionPtr The function to run when the timer has elapsed
 * @return The handle used to refer to this timer. Becomes invalid after timer has elapsed.
 */
TimerHandle RegisterRelativeTimer(millis_t const delay, void (*functionPtr)());

/**
 * @brief Registers a absolute timer, executing the given function after the given time point has passed [ms]
 *
 * @param[in] timePoint The time point in ms after which the given function is executed
 * @param[in] functionPtr The function to run when the timer has elapsed
 * @return The handle used to refer to this timer. Becomes invalid after timer has elapsed.
 */
TimerHandle RegisterAbsoluteTimer(millis_t const timePoint, void (*functionPtr)());

/**
 * @brief Removes the timer associated with the given timer handle
 */
void RemoveTimer(TimerHandle);
}

#endif
