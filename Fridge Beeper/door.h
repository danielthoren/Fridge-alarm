#ifndef DOOR
#define DOOR

#include "millis.h"

#define TIMEOUT_1            10000 // Open time until first beep      [ms] 10 sek
#define TIMEOUT_2            20000 // Open time until second beep     [ms] 20 sek
#define TIMEOUT_3            30000 // Open time until continuous beep [ms] 30 sek
#define TIMEOUT_CONTINUOUS   5000  // After all steps, this timeout
                                   // determines how often the beep
                                   // sequence occur

class Door
{
public:
  Door(millis_t timeout_1 = TIMEOUT_1,
       millis_t timeout_2 = TIMEOUT_2,
       millis_t timeout_3 = TIMEOUT_3,
       millis_t timeout_continuous = TIMEOUT_CONTINUOUS);

  void update();

  void set_state(bool door_open);

private:

  volatile bool     is_open;
  volatile millis_t open_time;
  millis_t continuous_timer;

  bool first_beep;
  bool second_beep;
  bool third_beep;

  millis_t const timeout_1;
  millis_t const timeout_2;
  millis_t const timeout_3;
  millis_t const timeout_continuous;
};

#endif
