/*
 * File: macros.h
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#ifndef MACROS_H_
#define MACROS_H_

#define assert_param(expr)			((void)0);

#define BIT(NUMBER)					(1UL << (NUMBER))

#define ARRAY_LENGHT(Value)			(sizeof(Value) / sizeof(Value[0]))

enum boolean
{
	FALSE = 0, TRUE = 1
};

typedef signed long long int64_t;
typedef signed long int32_t;
typedef signed short int16_t;
typedef signed char int8_t;

typedef unsigned long long uint64_t;
typedef unsigned long uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

void macros_delay_ms(uint16_t value);
void macros_delay_ms_start(uint16_t value);
_Bool macros_delay_ms_wait(void);
void macros_delay_ms_stop(void);
uint16_t macros_calc_crc16(uint8_t data[], uint16_t length);
uint8_t macros_get_random_byte(void);

#endif /* MACROS_H_ */
