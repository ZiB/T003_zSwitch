/*
 * File: main.h
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#ifndef MAIN_H_
#define MAIN_H_

#include "define.h"

#if defined STM32L011F4P6

#define STM32L011XX

#include "stm32l0xx.h"

#include "stm32l0xx_ll_bus.h"
#include "stm32l0xx_ll_rcc.h"
#include "stm32l0xx_ll_system.h"
#include "stm32l0xx_ll_utils.h"
#include "stm32l0xx_ll_pwr.h"
#include "stm32l0xx_ll_exti.h"
#include "stm32l0xx_ll_gpio.h"
#include "stm32l0xx_ll_adc.h"
#include "stm32l0xx_ll_comp.h"
#include "stm32l0xx_ll_cortex.h"
#include "stm32l0xx_ll_crc.h"
#include "stm32l0xx_ll_crs.h"
#include "stm32l0xx_ll_dac.h"
#include "stm32l0xx_ll_dma.h"
#include "stm32l0xx_ll_i2c.h"
#include "stm32l0xx_ll_iwdg.h"
#include "stm32l0xx_ll_lptim.h"
#include "stm32l0xx_ll_lpuart.h"
#include "stm32l0xx_ll_rng.h"
#include "stm32l0xx_ll_rtc.h"
#include "stm32l0xx_ll_spi.h"
#include "stm32l0xx_ll_tim.h"
#include "stm32l0xx_ll_usart.h"
#include "stm32l0xx_ll_wwdg.h"

#if !defined  (HSE_VALUE)
#define HSE_VALUE    ((uint32_t)8000000U)
#define MCU_SYSCLK_MHZ MSI_VALUE
#endif

#if !defined  (MSI_VALUE)
#define MSI_VALUE    ((uint32_t)1048576U)
#define MCU_SYSCLK_MHZ MSI_VALUE
#endif

#if !defined  (HSI_VALUE)
#define HSI_VALUE    ((uint32_t)16000000U)
#define MCU_SYSCLK_MHZ MSI_VALUE
#endif

#endif

#include "gpio.h"
#include "macros.h"
#include "button.h"
#include "mysensors.h"
#include "si24r01.h"

#if defined PCB_R1

#define PIN_SB1				A, 2, HIGH, MODE_INPUT_PULL_DOWN, SPEED_LOW, AF_NO
#define PIN_SB2				A, 1, HIGH, MODE_INPUT_PULL_DOWN, SPEED_LOW, AF_NO
#define PIN_SB3				A, 0, HIGH, MODE_INPUT_PULL_DOWN, SPEED_LOW, AF_NO

#define PIN_LED1			A, 4, LOW, MODE_OUTPUT_OPEN_DRAIN, SPEED_LOW, AF_NO
#define PIN_LED2			A, 3, LOW, MODE_OUTPUT_OPEN_DRAIN, SPEED_LOW, AF_NO
#define PIN_LED3			C, 15, LOW, MODE_OUTPUT_OPEN_DRAIN, SPEED_LOW, AF_NO

#define PIN_LED1_OFF		A, 4, LOW, MODE_INPUT_PULL_UP, SPEED_LOW, AF_NO
#define PIN_LED2_OFF		A, 3, LOW, MODE_INPUT_PULL_UP, SPEED_LOW, AF_NO
#define PIN_LED3_OFF		C, 15, LOW, MODE_INPUT_PULL_UP, SPEED_LOW, AF_NO

#define PIN_RF_PWR			A, 9, LOW, MODE_OUTPUT_PUSH_PULL, SPEED_LOW, AF_NO
#define PIN_RF_CE			B, 1, HIGH, MODE_OUTPUT_PUSH_PULL, SPEED_MEDIUM, AF_NO
#define PIN_RF_CS			A, 10, LOW, MODE_OUTPUT_PUSH_PULL, SPEED_MEDIUM, AF_NO
#define PIN_RF_SCK			A, 5, HIGH, MODE_AF_PUSH_PULL_PULL_DOWN, SPEED_MEDIUM, AF0
#define PIN_RF_MISO			A, 6, HIGH, MODE_AF_PUSH_PULL_PULL_DOWN, SPEED_MEDIUM, AF0
#define PIN_RF_MOSI			A, 7, HIGH, MODE_AF_PUSH_PULL_PULL_DOWN, SPEED_MEDIUM, AF0
#define PIN_RF_IRQ			B, 9, LOW, MODE_INPUT_PULL_DOWN, SPEED_LOW, AF_NO

#endif

enum timer_wakeup_settings
{
	TIMER_WAKEUP_MAX_VALUE = 26250, // период от 1 до 2 минут, т.к. частота LSI в пределах (26..56) кГц
	TIMER_WAKEUP_TICKS_20MS = 4, // кол-во "тиков" таймера (минимальное!!!)
	TIMER_WAKEUP_
};

enum mcu_wakeup_events
{
	MCU_WAKEUP_BUTTON_1 = BIT(0), //
	MCU_WAKEUP_BUTTON_2 = BIT(1),
	MCU_WAKEUP_BUTTON_3 = BIT(2),
	MCU_WAKEUP_TIMER = BIT(3),
	MCU_WAKEUP_
};

void mcu_set_wakeup_event(uint8_t value);
uint16_t timer_wakeup_get_counter(void);

#endif /* MAIN_H_ */
