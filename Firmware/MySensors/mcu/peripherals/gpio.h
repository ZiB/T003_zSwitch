/*
 * File: gpio.h
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "main.h"

#if !defined (STM32F0XX_LD) && !defined (STM32F0XX_MD) && \
	!defined (STM32L1XX_MD) && !defined (STM32L1XX_MDP) && !defined (STM32L1XX_HD) && \
	!defined (STM8L05X_LD_VL) && !defined(STM8L05X_MD_VL) && !defined(STM8L05X_HD_VL) && !defined(STM8AL31_L_MD) && \
	!defined(STM8L15X_MD) && !defined(STM8L15X_MDP) && !defined(STM8L15X_HD) && !defined(STM8L15X_LD) && \
	!defined(STM32L476xx) && !defined(STM32L0)
#error "Is not supported by the target device! Please select first the target device used in your application! (gpio.h)"
#else

#if defined (STM32F0XX_LD) || defined (STM32F0XX_MD) || \
	defined (STM32L1XX_MD) || defined (STM32L1XX_MDP) || defined (STM32L1XX_HD) || \
	defined (STM32L476xx) || defined (STM32L0)

//------------------------------------------------------------------------------
#define MODE_INPUT_FLOATING						MODE_INPUT, 0, 0 // MODE, OTYPER, PUPDR
#define MODE_INPUT_PULL_UP						MODE_INPUT, 0, 1
#define MODE_INPUT_PULL_DOWN					MODE_INPUT, 0, 2

#define MODE_OUTPUT_PUSH_PULL					MODE_OUTPUT, 0, 0
#define MODE_OUTPUT_PUSH_PULL_PULL_UP			MODE_OUTPUT, 0, 1
#define MODE_OUTPUT_PUSH_PULL_PULL_DOWN			MODE_OUTPUT, 0, 2
#define MODE_OUTPUT_OPEN_DRAIN					MODE_OUTPUT, 1, 0
#define MODE_OUTPUT_OPEN_DRAIN_PULL_UP			MODE_OUTPUT, 1, 1
#define MODE_OUTPUT_OPEN_DRAIN_PULL_DOWN		MODE_OUTPUT, 1, 2

#define MODE_AF_PUSH_PULL						MODE_ALTERNATE, 0, 0
#define MODE_AF_PUSH_PULL_PULL_UP				MODE_ALTERNATE, 0, 1
#define MODE_AF_PUSH_PULL_PULL_DOWN				MODE_ALTERNATE, 0, 2
#define MODE_AF_OPEN_DRAIN						MODE_ALTERNATE, 1, 0
#define MODE_AF_OPEN_DRAIN_PULL_UP				MODE_ALTERNATE, 1, 1
#define MODE_AF_OPEN_DRAIN_PULL_DOWN			MODE_ALTERNATE, 1, 2

#define MODE_ANALOG								MODE_ANALOG, 0, 0

#define MODE_ANALOG_ADC							MODE_ANALOG_ADC, 0, 0

//------------------------------------------------------------------------------
#if defined (STM32F0XX_LD) || defined (STM32F0XX_MD)

#define SPEED_LOW								0 // OSPEEDR
#define SPEED_MEDIUM							1
#define SPEED_HIGH								3

#elif defined (STM32L1XX_MD) || defined (STM32L1XX_MDP) || defined (STM32L1XX_HD)

#define SPEED_400KHZ							0 // OSPEEDR
#define SPEED_2MHZ								1
#define SPEED_10MHZ								2
#define SPEED_40MHZ								3

#elif defined(STM32L476xx) || defined(STM32L0)

#define SPEED_LOW								0 // OSPEEDR
#define SPEED_MEDIUM							1
#define SPEED_HIGH								2
#define SPEED_VERY_HIGH							3

#endif

//------------------------------------------------------------------------------
#define AF_NO									0
#define AF0										0
#define AF1										1
#define AF2										2
#define AF3										3
#define AF4										4
#define AF5										5
#define AF6										6
#define AF7										7
#define AF8										8
#define AF9										9
#define AF10									10
#define AF11									11
#define AF12									12
#define AF13									13
#define AF14									14
#define AF15									15

//------------------------------------------------------------------------------
#define GPIO_PIN_MODE_INPUT(PORT, PIN, LEVEL, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
	{ \
		GPIO##PORT->MODER = GPIO##PORT->MODER & ~(3UL << (PIN * 2UL)); \
		GPIO##PORT->PUPDR = (GPIO##PORT->PUPDR & ~(3UL << (PIN * 2UL))) | (PUPDR_VALUE << (PIN * 2UL)); \
	}

//------------------------------------------------------------------------------
#define GPIO_PIN_MODE_OUTPUT(PORT, PIN, LEVEL, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
	{ \
		GPIO##PORT->MODER = GPIO##PORT->MODER & ~(3UL << (PIN * 2UL)); \
		GPIO##PORT->OTYPER = (GPIO##PORT->OTYPER & ~(1UL << (PIN * 1UL))) | (OTYPER_VALUE << (PIN * 1UL)); \
		GPIO##PORT->PUPDR = (GPIO##PORT->PUPDR & ~(3UL << (PIN * 2UL))) | (PUPDR_VALUE << (PIN * 2UL)); \
		GPIO##PORT->OSPEEDR = (GPIO##PORT->OSPEEDR & ~(3UL << (PIN * 2UL))) | (OSPEEDR_VALUE << (PIN * 2UL)); \
		GPIO##PORT->MODER = GPIO##PORT->MODER | (1UL << (PIN * 2UL)); \
	}

//------------------------------------------------------------------------------
#define GPIO_PIN_MODE_ALTERNATE(PORT, PIN, LEVEL, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
	{ \
		GPIO##PORT->MODER = GPIO##PORT->MODER & ~(3UL << (PIN * 2UL)); \
		GPIO##PORT->OTYPER = (GPIO##PORT->OTYPER & ~(1UL << (PIN * 1UL))) | (OTYPER_VALUE << (PIN * 1UL)); \
		GPIO##PORT->PUPDR = (GPIO##PORT->PUPDR & ~(3UL << (PIN * 2UL))) | (PUPDR_VALUE << (PIN * 2UL)); \
		GPIO##PORT->OSPEEDR = (GPIO##PORT->OSPEEDR & ~(3UL << (PIN * 2UL))) | (OSPEEDR_VALUE << (PIN * 2UL)); \
		GPIO##PORT->AFR[PIN / 8UL] = (GPIO##PORT->AFR[PIN / 8UL] & ~(15UL << ((PIN & 7UL) * 4UL))) | ((AF_NUMBER) << ((PIN & 7UL) * 4UL)); \
		GPIO##PORT->MODER = GPIO##PORT->MODER | (2UL << (PIN * 2UL)); \
	}

//------------------------------------------------------------------------------
#define GPIO_PIN_MODE_ANALOG(PORT, PIN, LEVEL, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
	{ \
		GPIO##PORT->MODER = GPIO##PORT->MODER | (3UL << (PIN * 2UL)); \
		GPIO##PORT->PUPDR = (GPIO##PORT->PUPDR & ~(3UL << (PIN * 2UL))) | (PUPDR_VALUE << (PIN * 2UL)); \
	}

//------------------------------------------------------------------------------
#define GPIO_PIN_MODE_ANALOG_ADC(PORT, PIN, LEVEL, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
	{ \
		GPIO##PORT->ASCR |= (1UL << PIN); \
		GPIO##PORT->MODER = GPIO##PORT->MODER | (3UL << (PIN * 2UL)); \
		GPIO##PORT->PUPDR = (GPIO##PORT->PUPDR & ~(3UL << (PIN * 2UL))) | (PUPDR_VALUE << (PIN * 2UL)); \
	}

//------------------------------------------------------------------------------
#define GPIO_PIN_INIT(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
	{ GPIO_PIN_##MODE(PORT, PIN, LEVEL, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER); }

//------------------------------------------------------------------------------
#if defined (STM32F0XX_LD) || defined (STM32F0XX_MD)

#define GPIO_PIN_ON_HIGH(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BSRR = (1UL << PIN); }

#define GPIO_PIN_ON_LOW(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BRR = (1UL << PIN); }

#define GPIO_PIN_OFF_HIGH(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BRR = (1UL << PIN); }

#define GPIO_PIN_OFF_LOW(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BSRR = (1UL << PIN); }

#elif defined (STM32L1XX_MD) || defined (STM32L1XX_MDP) || defined (STM32L1XX_HD)

#define GPIO_PIN_ON_HIGH(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BSRRL = (1UL << PIN); }

#define GPIO_PIN_ON_LOW(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BSRRH = (1UL << PIN); }

#define GPIO_PIN_OFF_HIGH(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BSRRH = (1UL << PIN); }

#define GPIO_PIN_OFF_LOW(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BSRRL = (1UL << PIN); }

#elif defined (STM32L476xx) || defined(STM32L0)

#define GPIO_PIN_ON_HIGH(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BSRR = (1UL << PIN); }

#define GPIO_PIN_ON_LOW(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BSRR = (1UL << (PIN + 16)); }

#define GPIO_PIN_OFF_HIGH(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BSRR = (1UL << (PIN + 16)); }

#define GPIO_PIN_OFF_LOW(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO##PORT->BSRR = (1UL << PIN); }

#endif

//------------------------------------------------------------------------------
#define GPIO_PIN_ON(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO_PIN_ON_##LEVEL(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) }

#define GPIO_PIN_OFF(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			{ GPIO_PIN_OFF_##LEVEL(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) }

#define GPIO_PIN_TOGGLE(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
		{ GPIO##PORT->ODR ^= (1UL << PIN); }

//------------------------------------------------------------------------------
#define GPIO_PIN_SIGNAL_HIGH(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			( (GPIO##PORT->IDR & (1UL << PIN)) == (1UL << PIN) )

#define GPIO_PIN_SIGNAL_LOW(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
		( (GPIO##PORT->IDR & (1UL << PIN)) != (1UL << PIN) )

#define GPIO_PIN_SIGNAL(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) \
			( GPIO_PIN_SIGNAL_##LEVEL(PORT, PIN, LEVEL, MODE, OTYPER_VALUE, PUPDR_VALUE, OSPEEDR_VALUE, AF_NUMBER) )

#else

#define INPUT_FLOATING_WITHOUT_ITERRUPT     	INPUT_FLOATING_WITHOUT_ITERRUPT
#define INPUT_FLOATING_WITH_ITERRUPT        	INPUT_FLOATING_WITH_ITERRUPT
#define INPUT_PULL_UP_WITHOUT_ITERRUPT      	INPUT_PULL_UP_WITHOUT_ITERRUPT
#define INPUT_PULL_UP_WITH_ITERRUPT         	INPUT_PULL_UP_WITH_ITERRUPT
#define OUTPUT_OPEN_DRAIN_NOSPEED_LIMIT     	OUTPUT_OPEN_DRAIN_NOSPEED_LIMIT
#define OUTPUT_OPEN_DRAIN_SPEED_LIMIT_10MHZ 	OUTPUT_OPEN_DRAIN_SPEED_LIMIT_10MHZ
#define OUTPUT_PUSH_PULL_NOSPEED_LIMIT      	OUTPUT_PUSH_PULL_NOSPEED_LIMIT
#define OUTPUT_PUSH_PULL_SPEED_LIMIT_10MHZ  	OUTPUT_PUSH_PULL_SPEED_LIMIT_10MHZ

//------------------------------------------------------------------------------
#define GPIO_PIN_INIT_OUTPUT_PUSH_PULL_SPEED_LIMIT_10MHZ(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR |= (1 << PIN); \
			GPIO##PORT->CR1 |= (1 << PIN); \
			GPIO##PORT->CR2 |= (1 << PIN); \
		}

#define GPIO_PIN_INIT_OUTPUT_PUSH_PULL_NOSPEED_LIMIT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR |= (1 << PIN); \
			GPIO##PORT->CR1 |= (1 << PIN); \
			GPIO##PORT->CR2 &= (uint8_t)(~(1 << PIN)); \
		}

#define GPIO_PIN_INIT_OUTPUT_OPEN_DRAIN_SPEED_LIMIT_10MHZ(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR |= (1 << PIN); \
			GPIO##PORT->CR1 &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR2 |= (1 << PIN); \
		}

#define GPIO_PIN_INIT_OUTPUT_OPEN_DRAIN_NOSPEED_LIMIT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR |= (1 << PIN); \
			GPIO##PORT->CR1 &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR2 &= (uint8_t)(~(1 << PIN)); \
		}

#define GPIO_PIN_INIT_INPUT_PULL_UP_WITH_ITERRUPT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR1 |= (1 << PIN); \
			GPIO##PORT->CR2 |= (1 << PIN); \
		}

#define GPIO_PIN_INIT_INPUT_PULL_UP_WITHOUT_ITERRUPT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR1 |= (1 << PIN); \
			GPIO##PORT->CR2 &= (uint8_t)(~(1 << PIN)); \
		}

#define GPIO_PIN_INIT_INPUT_FLOATING_WITH_ITERRUPT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR1 &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR2 |= (1 << PIN); \
		}

#define GPIO_PIN_INIT_INPUT_FLOATING_WITHOUT_ITERRUPT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO##PORT->DDR &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR1 &= (uint8_t)(~(1 << PIN)); \
			GPIO##PORT->CR2 &= (uint8_t)(~(1 << PIN)); \
		}

//------------------------------------------------------------------------------
#define GPIO_PIN_INIT(PORT, PIN, LEVEL, MODE) \
		{ \
			GPIO_PIN_INIT_##MODE(PORT, PIN, LEVEL, MODE); \
		}

//------------------------------------------------------------------------------
#define GPIO_PIN_OFF_HIGH(PORT, PIN, LEVEL, MODE) \
		{ GPIO##PORT->ODR &= (uint8_t)((uint8_t)(~(1 << PIN))); }

#define GPIO_PIN_OFF_LOW(PORT, PIN, LEVEL, MODE) \
		{ GPIO##PORT->ODR |= (1 << PIN); }

#define GPIO_PIN_OFF(PORT, PIN, LEVEL, MODE) \
		{ GPIO_PIN_OFF_##LEVEL(PORT, PIN, LEVEL, MODE) }

//------------------------------------------------------------------------------
#define GPIO_PIN_ON_HIGH(PORT, PIN, LEVEL, MODE) \
		{ GPIO##PORT->ODR |= (1 << PIN); }

#define GPIO_PIN_ON_LOW(PORT, PIN, LEVEL, MODE) \
		{ GPIO##PORT->ODR &= (uint8_t)(~(1 << PIN)); }

#define GPIO_PIN_ON(PORT, PIN, LEVEL, MODE) \
		{ GPIO_PIN_ON_##LEVEL(PORT, PIN, LEVEL, MODE) }

//------------------------------------------------------------------------------
#define GPIO_PIN_TOGGLE(PORT, PIN, LEVEL, MODE) \
		{ GPIO##PORT->ODR ^= (uint8_t)(1 << PIN); }

//------------------------------------------------------------------------------
#define GPIO_PIN_SIGNAL_HIGH(PORT, PIN, LEVEL, MODE) \
		( (GPIO##PORT->IDR & (1 << PIN)) == (1 << PIN) )

#define GPIO_PIN_SIGNAL_LOW(PORT, PIN, LEVEL, MODE) \
	( (GPIO##PORT->IDR & (1 << PIN)) != (1 << PIN) )

#define GPIO_PIN_SIGNAL(PORT, PIN, LEVEL, MODE) \
	( GPIO_PIN_SIGNAL_##LEVEL(PORT, PIN, LEVEL, MODE) )

#endif

//------------------------------------------------------------------------------
#define PIN_INIT(PIN_DESCRIPTION) GPIO_PIN_INIT(PIN_DESCRIPTION)
#define PIN_ON(PIN_DESCRIPTION) GPIO_PIN_ON(PIN_DESCRIPTION)
#define PIN_OFF(PIN_DESCRIPTION) GPIO_PIN_OFF(PIN_DESCRIPTION)
#define PIN_TOGGLE(PIN_DESCRIPTION) GPIO_PIN_TOGGLE(PIN_DESCRIPTION)
#define PIN_SIGNAL(PIN_DESCRIPTION) GPIO_PIN_SIGNAL(PIN_DESCRIPTION)

#endif

#endif /* GPIO_H_ */
