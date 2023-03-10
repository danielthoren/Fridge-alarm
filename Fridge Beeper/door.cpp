#include "door.h"
#include "buzzer.h"
#include "millis.h"
#include "common_defines.h"

#include <util/delay.h>

Door::Door(millis_t timeout_1,
           millis_t timeout_2,
           millis_t timeout_3,
           millis_t timeout_continuous)
  : is_open{false},
    open_time{0},
    first_beep{false},
    second_beep{false},
    third_beep{false},
    timeout_1{timeout_1},
    timeout_2{timeout_2},
    timeout_3{timeout_3},
    timeout_continuous{timeout_continuous}
{
}


void Door::update()
{
  if (is_open && !first_beep && has_expired(open_time + timeout_1))
  {
    beep_once(100);
    first_beep = true;
  }
  else if (is_open && !second_beep && has_expired(open_time + timeout_2))
  {
    beep_once(100);
    beep_once(100);
    second_beep = true;
  }
  else if (is_open && !third_beep && has_expired(open_time + timeout_3))
  {
    beep_once(100);
    beep_once(100);
    beep_once(100);
    third_beep = true;
    continuous_timer = millis() + timeout_continuous;
  }
  else if (third_beep && has_expired(continuous_timer))
  {
    beep_once(100);
    beep_once(100);
    beep_once(100);
    continuous_timer = millis() + timeout_continuous;
  }
}

void Door::set_state(bool door_open)
{
  this->is_open = door_open;

  if (is_open)
  {
    open_time = millis();
  }
  else
  {
    first_beep = false;
    second_beep = false;
    third_beep = false;
    open_time = 0;
  }
}
