/*
 * File: button.h
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#ifndef BUTTON_H_
#define BUTTON_H_

enum button_settings
{
	BUTTON_1 = 0, //
	BUTTON_2 = 1,
	BUTTON_3 = 2,
	BUTTON_COUNT = 3,
};

void button_capture_refresh(void);
void button_capture(uint8_t button_number, _Bool state);

#endif /* BUTTON_H_ */
