/*
 * File: interrupt.c
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

void EXTI0_1_IRQHandler(void)
{
	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
	{
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);

		button_capture(BUTTON_3, PIN_SIGNAL(PIN_SB3));
	}

	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
	{
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);

		button_capture(BUTTON_2, PIN_SIGNAL(PIN_SB2));
	}
}

void EXTI2_3_IRQHandler(void)
{
	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
	{
		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);

		button_capture(BUTTON_1, PIN_SIGNAL(PIN_SB1));
	}
}

void LPTIM1_IRQHandler(void)
{
	if (LL_LPTIM_IsActiveFlag_ARRM(LPTIM1) != FALSE)
	{
		LL_LPTIM_ClearFLAG_ARRM(LPTIM1);

		button_capture_refresh();
		mcu_set_wakeup_event(MCU_WAKEUP_TIMER);
	}
}
