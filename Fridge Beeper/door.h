#ifndef DOOR
#define DOOR

#include "millis.h"

namespace Constants
{
// Default values
static const constexpr millis_t kDebounceTime =        100;   // Time to wait for debounce check [ms] 0.1 sec
static const constexpr millis_t kTimeOut1 =            10000; // Open time until first beep      [ms] 10 sec
static const constexpr millis_t kTimeOut2 =            20000; // Open time until second beep     [ms] 20 sec
static const constexpr millis_t kTimeOut3 =            30000; // Open time until continuous beep [ms] 30 sec
static const constexpr millis_t kTimeOutContinuous =   5000;  // After all steps, this timeout
                                                              // determines how often the beep
                                                              // sequence occur
};

class Door
{

public:
  Door(volatile uint8_t* doorPort,
       uint8_t doorPin,
       millis_t debounceTime = Constants::kDebounceTime,
       millis_t timeout1 = Constants::kTimeOut1,
       millis_t timeout2 = Constants::kTimeOut2,
       millis_t timeout3 = Constants::kTimeOut3,
       millis_t timeoutContinuous = Constants::kTimeOutContinuous);

  /**
   * @brief Updates this module, must be called periodically
   */
  void update();

 /**
  * @brief May be called when the state (open/closed) changes
  * @details Will save the state, then use a timer to debounce the input
  */
  void DoorStateChanged();

private:

  void DebounceHelper();

  void SetState(bool door_open);

  volatile bool     m_isOpenAtChange;
  volatile bool     m_isOpen;
  volatile millis_t m_openTime;
  millis_t m_continuousTimer;

  bool m_firstBeep;
  bool m_secondBeep;
  bool m_thirdBeep;

  volatile uint8_t* const m_doorPort;
  uint8_t const m_doorPin;

  millis_t const m_debounceTime;
  millis_t const m_timeout1;
  millis_t const m_timeout2;
  millis_t const m_timeout3;
  millis_t const m_timeoutContinuous;
};

#endif
