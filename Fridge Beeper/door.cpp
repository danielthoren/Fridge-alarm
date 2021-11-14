
#include "door.h"
#include "buzzer.h"

Door::Door(millis_t timeout_1,
		   millis_t timeout_2,
		   millis_t timeout_3) :
		   
		   is_open(false),
		   open_time(0),
		   first_beep(false),
		   second_beep(false),
		   timeout_1(timeout_1),
		   timeout_2(timeout_2),
		   timeout_3(timeout_3)
{
}

	  
void Door::update()
{
	millis_t curr_time = millis();
	
	if (is_open && curr_time - open_time > timeout_1 && !first_beep)
	{
		beep_once(100);
		beep_once(100);
		beep_once(100);
		first_beep = true;
	}
	else if (is_open && curr_time - open_time > timeout_2 && !second_beep)
	{
		beep_once(100);
		beep_once(100);
		beep_once(100);
		second_beep = true;
	}
	else if (is_open && curr_time - open_time > timeout_3)
	{
		beep_once(400);
	}
} 

void Door::set_state(bool state)
{
	is_open = state;
	
	if (is_open)
	{
		open_time = millis();
	}
	else
	{
		first_beep = false;
		second_beep = false;
		open_time = 0;
	}
}