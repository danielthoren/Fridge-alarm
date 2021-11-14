
#include "millis.h"

#define TIMEOUT_1            10000 // Open time until first beep      [ms] 10 sek
#define TIMEOUT_2            20000 // Open time until second beep     [ms] 20 sek
#define TIMEOUT_3            30000 // Open time until continuous beep [ms] 30 sek

class Door
{
public:
	Door(millis_t timeout_1 = TIMEOUT_1,
		 millis_t timeout_2 = TIMEOUT_2,
		 millis_t timeout_3 = TIMEOUT_3);
		 
	void update();
	
	void set_state(bool state);
	
private:

	volatile bool     is_open;
	volatile millis_t open_time;
	
	bool first_beep;
	bool second_beep;
	
	millis_t timeout_1;
	millis_t timeout_2;
	millis_t timeout_3;
};