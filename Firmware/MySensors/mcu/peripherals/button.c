/*
 * File: button.c
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

typedef struct
{
	volatile uint8_t is_capture;
	volatile uint8_t is_timer_update;
	uint16_t capture[BUTTON_COUNT];
	uint8_t event[BUTTON_COUNT];
}__attribute__((__packed__)) button_type;

button_type button;

void button_capture_refresh(void)
{
	for (uint8_t button_number = 0; button_number < BUTTON_COUNT; button_number++)
	{
		if ((button.is_capture & BIT(button_number)) != 0)
		{
			button.is_timer_update |= BIT(button_number);
		}
	}
}

void button_capture(uint8_t button_number, _Bool state)
{
	if (button_number < BUTTON_COUNT)
	{
		if (state == TRUE)
		{
			button.is_timer_update &= ~BIT(button_number);
			button.is_capture |= BIT(button_number);
			button.capture[button_number] = timer_wakeup_get_counter();
		}
		else if ((button.is_capture & BIT(button_number)) != 0)
		{
			uint16_t timer_counter = timer_wakeup_get_counter();

			if ((button.is_timer_update & BIT(button_number)) != 0)
			{
				timer_counter += TIMER_WAKEUP_MAX_VALUE;
			}

			if ((timer_counter - button.capture[button_number]) > TIMER_WAKEUP_TICKS_20MS)
			{
				mcu_set_wakeup_event(BIT(button_number));
			}

			button.is_capture &= ~BIT(button_number);
		}
	}
}
