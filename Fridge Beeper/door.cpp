#include "door.h"
#include "AtTiny45_HAL.h"
#include "buzzer.h"
#include "millis.h"
#include "timer.h"
#include "common_defines.h"

#include <util/delay.h>

Door::Door(volatile uint8_t* doorPort,
           uint8_t doorPin,
           millis_t debounceTime,
           millis_t timeout1,
           millis_t timeout2,
           millis_t timeout3,
           millis_t timeoutContinuous):
    m_isOpen{false},
    m_openTime{0},
    m_firstBeep{false},
    m_secondBeep{false},
    m_thirdBeep{false},
    m_doorPort{doorPort},
    m_doorPin{doorPin},
    m_debounceTime{debounceTime},
    m_timeout1{timeout1},
    m_timeout2{timeout2},
    m_timeout3{timeout3},
    m_timeoutContinuous{timeoutContinuous}
{
}

void Door::update()
{
  if (m_isOpen && !m_firstBeep && has_expired(m_openTime + m_timeout1))
  {
    beep_once(100);
    m_firstBeep = true;
  }
  else if (m_isOpen && !m_secondBeep && has_expired(m_openTime + m_timeout2))
  {
    beep_once(100);
    beep_once(100);
    m_secondBeep = true;
  }
  else if (m_isOpen && !m_thirdBeep && has_expired(m_openTime + m_timeout3))
  {
    beep_once(100);
    beep_once(100);
    beep_once(100);
    m_thirdBeep = true;
    m_continuousTimer = millis() + m_timeoutContinuous;
  }
  else if (m_thirdBeep && has_expired(m_continuousTimer))
  {
    beep_once(100);
    beep_once(100);
    beep_once(100);
    m_continuousTimer = millis() + m_timeoutContinuous;
  }
}

void Door::DoorStateChanged()
{
  m_isOpenAtChange = ReadPin(m_doorPort, m_doorPin);

  // TODO: Lambdas work, but std::function does not exist. Find workaround
  auto lam = [this] () {
               if (this->m_isOpenAtChange)
               {
                 this->m_isOpenAtChange = false;
               }
             };

  // Timer::RegisterRelativeTimer(m_debounceTime, &this->DebounceHelper);
}

void Door::DebounceHelper()
{
  if (m_isOpenAtChange == ReadPin(m_doorPort, m_doorPin))
  {
    SetState(m_isOpenAtChange);
  }
}

void Door::SetState(bool door_open)
{
  this->m_isOpen = door_open;

  if (m_isOpen)
  {
    m_openTime = millis();
  }
  else
  {
    m_firstBeep = false;
    m_secondBeep = false;
    m_thirdBeep = false;
    m_openTime = 0;
  }
}
