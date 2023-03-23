#include "AtTiny45_HAL.h"

volatile bool ReadPin(volatile uint8_t* port, uint8_t pin)
{
  return *port & (1 << pin);
}
