#include "timer.h"

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

static Entry timers[maxConsecutiveTimers];

static TimerHandle InsertTimer(Entry const& entry);

void InitTimer()
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
  Entry entry{
    true,
    millis() + delay,
    functionPtr
  };

  return InsertTimer(entry);
}


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
