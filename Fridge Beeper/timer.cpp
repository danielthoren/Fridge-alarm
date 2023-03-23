#include "timer.h"

namespace Timer
{

/*******************************************************************************
 ***  Type declarations                                                       ***
 *******************************************************************************/

struct Entry
{
  Entry()
    :isActive{false},
     targetTime{0},
     functionPtr{nullptr}
  {}

  Entry(bool isActive,
        millis_t targetTime,
        void (*functionPtr)())
    :isActive{isActive},
     targetTime{targetTime},
     functionPtr{functionPtr}
  {}

  bool isActive;
  millis_t targetTime;
  void (*functionPtr)();
};

/*******************************************************************************
 ***  Local variable declarations                                             ***
 *******************************************************************************/

static Entry timers[maxConsecutiveTimers];

/*******************************************************************************
 ***  Local function declarations                                             ***
 *******************************************************************************/

static TimerHandle InsertTimer(Entry const& entry);

/*******************************************************************************
 ***  Public function defenitions                                             ***
 *******************************************************************************/

void Init()
{
  for (int i{0}; i < maxConsecutiveTimers; i++)
  {
    timers[i] = Entry{};
  }
}

void Update()
{
  int curr{0};
  while (curr < maxConsecutiveTimers)
  {
    if (timers[curr].isActive && has_expired(timers[curr].targetTime))
    {
      timers[curr].functionPtr();
      timers[curr] = Entry{};
    }
  }
}

TimerHandle RegisterRelativeTimer(millis_t const delay, void (*functionPtr)())
{
  return RegisterAbsoluteTimer(millis() + delay, functionPtr);
}

TimerHandle RegisterAbsoluteTimer(millis_t const timePoint, void (*functionPtr)())
{
  Entry entry{
    true,
    timePoint,
    functionPtr
  };

  return InsertTimer(entry);
}

void RemoveTimer(TimerHandle handle)
{
  if (0 < handle || handle < maxConsecutiveTimers)
  {
    timers[handle] = Entry{};
  }
}

/*******************************************************************************
 ***  Local function defenitions                                              ***
 *******************************************************************************/

static TimerHandle InsertTimer(Entry const& entry)
{
  int curr{0};
  while (curr < maxConsecutiveTimers)
  {
    if (!timers[curr].isActive)
    {
      timers[curr] = entry;
      return curr;
    }
  }
  return -1;
}

}
