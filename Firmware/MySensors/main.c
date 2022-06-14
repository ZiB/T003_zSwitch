/*
 * File: main.c
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

volatile uint8_t mcu_wakeup_event;

static inline void gpio_init(void)
{
	RCC->IOPENR |= RCC_IOPENR_GPIOAEN | RCC_IOPENR_GPIOBEN | RCC_IOPENR_GPIOCEN;

	__NOP();
	__NOP();

	// вход, подтяжка к минусу
	GPIOA->MODER = 0xA8000000;
	GPIOA->PUPDR = 0x66AAAAAA;
	GPIOB->MODER = 0x00000000;
	GPIOB->PUPDR = 0xAAAAAAAA;
	GPIOC->MODER = 0x00000000;
	GPIOC->PUPDR = 0xAAAAAAAA;

	// радиомодуль
	PIN_INIT(PIN_RF_PWR);
	PIN_INIT(PIN_RF_CE);
	PIN_INIT(PIN_RF_CS);
	PIN_INIT(PIN_RF_IRQ);
	PIN_INIT(PIN_RF_SCK);
	PIN_INIT(PIN_RF_MISO);
	PIN_INIT(PIN_RF_MOSI);
	PIN_OFF(PIN_RF_PWR);
	PIN_OFF(PIN_RF_CE);
	PIN_OFF(PIN_RF_CS);

	// кнпоки
	PIN_INIT(PIN_SB1);
	PIN_INIT(PIN_SB2);
	PIN_INIT(PIN_SB3);

	// светодиоды
	PIN_INIT(PIN_LED1);
	PIN_INIT(PIN_LED2);
	PIN_INIT(PIN_LED3);
	PIN_OFF(PIN_LED1);
	PIN_OFF(PIN_LED2);
	PIN_OFF(PIN_LED3);
}

static inline void rcc_init(void)
{
	if (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
	{
		LL_RCC_MSI_Enable();
		while (LL_RCC_MSI_IsReady() == FALSE)
		{
		}

		LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_MSI);
		while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_MSI)
		{
		}
	}

	LL_RCC_MSI_SetRange(LL_RCC_MSIRANGE_4);

	LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE3);

	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

	LL_RCC_PLL_Disable();
	LL_RCC_HSI_Disable();

	if (LL_RCC_LSI_IsReady() == FALSE)
	{
		LL_RCC_LSI_Enable();

		while (LL_RCC_LSI_IsReady() == FALSE)
		{
		}
	}
}

static inline void power_init(void)
{
	LL_PWR_EnableUltraLowPower();
	LL_PWR_EnableFastWakeUp();
}

static inline void exti_init(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);

	// кнопки
	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE0);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_0);
	LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_0);
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);

	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE1);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_1);
	LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_1);
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_1);
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);

	LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE2);
	LL_EXTI_EnableRisingTrig_0_31(LL_EXTI_LINE_2);
	LL_EXTI_EnableFallingTrig_0_31(LL_EXTI_LINE_2);
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_2);
	LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
}

static inline void timers_init(void)
{
	LL_RCC_SetLPTIMClockSource(LL_RCC_LPTIM1_CLKSOURCE_LSI);

	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_LPTIM1);
	LL_LPTIM_SetCounterMode(LPTIM1, LL_LPTIM_COUNTER_MODE_INTERNAL);
	LL_LPTIM_EnableIT_ARRM(LPTIM1);
	LL_LPTIM_ClearFLAG_ARRM(LPTIM1);
	LL_LPTIM_Enable(LPTIM1);
	LL_LPTIM_SetPrescaler(LPTIM1, LL_LPTIM_PRESCALER_DIV128);
	LL_LPTIM_SetAutoReload(LPTIM1, TIMER_WAKEUP_MAX_VALUE);
	LL_LPTIM_StartCounter(LPTIM1, LL_LPTIM_OPERATING_MODE_CONTINUOUS);
}

uint16_t timer_wakeup_get_counter(void)
{
	uint16_t timer_counter;

	// таймер тактируется асинхронно, требуется минимум два чтения до совпадения идентичности значений
	while ((timer_counter = LL_LPTIM_GetCounter(LPTIM1)) != LL_LPTIM_GetCounter(LPTIM1))
	{
	}

	return timer_counter;
}

static inline void interrupt_init(void)
{
	extern const uint32_t _btext;

	// ремап таблицы векторов прерываний
	SCB->VTOR = (uint32_t) &_btext;

	uint8_t priority = 0;

	// прерывания от кнопок
	NVIC_SetPriority(EXTI0_1_IRQn, priority++);
	NVIC_EnableIRQ(EXTI0_1_IRQn);

	// прерывания от кнопок
	NVIC_SetPriority(EXTI2_3_IRQn, priority++);
	NVIC_EnableIRQ(EXTI2_3_IRQn);

	// периодическое пробуждение МК
	NVIC_SetPriority(LPTIM1_IRQn, priority++);
	NVIC_EnableIRQ(LPTIM1_IRQn);
}

static inline void spi_init(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

	LL_SPI_SetBaudRatePrescaler(SPI1, LL_SPI_BAUDRATEPRESCALER_DIV2);
	LL_SPI_SetTransferDirection(SPI1, LL_SPI_FULL_DUPLEX);
	LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_1EDGE);
	LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_LOW);
	LL_SPI_SetTransferBitOrder(SPI1, LL_SPI_MSB_FIRST);
	LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_8BIT);
	LL_SPI_SetNSSMode(SPI1, LL_SPI_NSS_SOFT);
	LL_SPI_SetMode(SPI1, LL_SPI_MODE_MASTER);

	LL_SPI_Enable(SPI1);
}

void mcu_sleep(void)
{
	PIN_OFF(PIN_LED1);
	PIN_OFF(PIN_LED2);
	PIN_OFF(PIN_LED3);

	PIN_INIT(PIN_LED1_OFF);
	PIN_INIT(PIN_LED2_OFF);
	PIN_INIT(PIN_LED3_OFF);

	LL_PWR_SetRegulModeLP(LL_PWR_REGU_LPMODES_LOW_POWER);
	LL_PWR_SetPowerMode(LL_PWR_MODE_STOP);
	LL_LPM_EnableDeepSleep();
	__WFI();
	LL_LPM_EnableSleep();

	PIN_INIT(PIN_LED1);
	PIN_INIT(PIN_LED2);
	PIN_INIT(PIN_LED3);
}

void mcu_set_wakeup_event(uint8_t value)
{
	mcu_wakeup_event |= value;
}

void main(void)
{
	gpio_init();
	rcc_init();
	power_init();
	exti_init();
	timers_init();
	spi_init();
	interrupt_init();

	// пауза для удобства программирования МК
	macros_delay_ms(10);

	uint8_t value = 255;

	// презентация ноды
	mysensors_send(MYSENSORS_C_PRESENTATION, MYSENSORS_S_ARDUINO_NODE, 255, MYSENSORS_P_STRING, (uint8_t *) "2.3.2-beta", 0);
	mysensors_send(MYSENSORS_C_INTERNAL, MYSENSORS_I_CONFIG, 255, MYSENSORS_P_BYTE, &value, 0);
	mysensors_send(MYSENSORS_C_INTERNAL, MYSENSORS_I_SKETCH_NAME, 255, MYSENSORS_P_STRING, (uint8_t *) "zSwitch", 0);
	mysensors_send(MYSENSORS_C_INTERNAL, MYSENSORS_I_SKETCH_VERSION, 255, MYSENSORS_P_STRING, (uint8_t *) FIRMWARE_BUILD_STRING, 0);

	//
	mysensors_send(MYSENSORS_C_PRESENTATION, MYSENSORS_S_BINARY, 0, MYSENSORS_P_STRING, (uint8_t *) "Кнопка 1", 0);
	mysensors_send(MYSENSORS_C_PRESENTATION, MYSENSORS_S_BINARY, 1, MYSENSORS_P_STRING, (uint8_t *) "Кнопка 2", 0);
	mysensors_send(MYSENSORS_C_PRESENTATION, MYSENSORS_S_BINARY, 2, MYSENSORS_P_STRING, (uint8_t *) "Кнопка 3", 0);
	mysensors_send(MYSENSORS_C_PRESENTATION, MYSENSORS_S_MULTIMETER, 3, MYSENSORS_P_STRING, (uint8_t *) "Батарейка", 0);
	mysensors_send(MYSENSORS_C_PRESENTATION, MYSENSORS_S_CUSTOM, 4, MYSENSORS_P_STRING, (uint8_t *) "Отправлено", 0);

	uint8_t button_state[BUTTON_COUNT];

	while (1)
	{
		mcu_sleep();

		while (mcu_wakeup_event != 0)
		{
			// события пробуждения
			uint8_t wakeup = mcu_wakeup_event;
			mcu_wakeup_event &= ~wakeup;

			for (uint8_t button_number = 0; button_number < BUTTON_COUNT; button_number++)
			{
				if ((wakeup & BIT(button_number)) != 0)
				{
					if (BIT(button_number) == MCU_WAKEUP_BUTTON_1)
					{
						PIN_ON(PIN_LED1);
					}
					if (BIT(button_number) == MCU_WAKEUP_BUTTON_2)
					{
						PIN_ON(PIN_LED2);
					}
					if (BIT(button_number) == MCU_WAKEUP_BUTTON_3)
					{
						PIN_ON(PIN_LED3);
					}

					button_state[button_number] = !button_state[button_number];

					mysensors_send(MYSENSORS_C_SET, MYSENSORS_V_STATUS, button_number, MYSENSORS_P_BYTE, &button_state[button_number], 0);
				}
			}

			if ((wakeup & MCU_WAKEUP_TIMER) != 0)
			{
				LL_RCC_HSI_Enable();

				while (LL_RCC_HSI_IsReady() != 1)
				{
				}

				LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

				if (__LL_ADC_IS_ENABLED_ALL_COMMON_INSTANCE() == 0)
				{
					LL_ADC_SetCommonClock(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_CLOCK_SYNC_PCLK_DIV1);
					LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
				}

				if (LL_ADC_IsEnabled(ADC1) == 0)
				{
					LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV2);
					LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
					LL_ADC_SetResolution(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
					LL_ADC_SetLowPowerMode(ADC1, LL_ADC_LP_AUTOWAIT_AUTOPOWEROFF);
					LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_39CYCLES_5);
				}

				if ((LL_ADC_IsEnabled(ADC1) == 0) || (LL_ADC_REG_IsConversionOngoing(ADC1) == 0))
				{
					LL_ADC_REG_SetTriggerSource(ADC1, LL_ADC_REG_TRIG_SOFTWARE);
					LL_ADC_REG_SetContinuousMode(ADC1, LL_ADC_REG_CONV_SINGLE);
					LL_ADC_REG_SetOverrun(ADC1, LL_ADC_REG_OVR_DATA_OVERWRITTEN);
					LL_ADC_REG_SetSequencerChannels(ADC1, LL_ADC_CHANNEL_17);
				}

				LL_ADC_SetOverSamplingScope(ADC1, LL_ADC_OVS_DISABLE);
				LL_ADC_ConfigOverSamplingRatioShift(ADC1, LL_ADC_OVS_RATIO_2, LL_ADC_OVS_SHIFT_NONE);

				if (LL_ADC_IsEnabled(ADC1) == 0)
				{
					LL_ADC_StartCalibration(ADC1);

					while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0)
					{
					}
				}

				if ((LL_ADC_IsDisableOngoing(ADC1) == 0) && (LL_ADC_REG_IsConversionOngoing(ADC1) == 0))
				{
					LL_ADC_REG_StartConversion(ADC1);
				}

				while (LL_ADC_IsActiveFlag_EOC(ADC1) == 0)
				{
				}

				LL_ADC_ClearFlag_EOC(ADC1);

				float battery_voltage = 3.0 * (float) (*(uint16_t*) 0x1FF80078) / (float) LL_ADC_REG_ReadConversionData12(ADC1);

				LL_APB2_GRP1_DisableClock(LL_APB2_GRP1_PERIPH_ADC1);

				LL_RCC_HSI_Disable();

				mysensors_send(MYSENSORS_C_SET, MYSENSORS_V_VOLTAGE, 3, MYSENSORS_P_FLOAT32, (uint8_t *) &battery_voltage, 3);

				uint32_t packet_transmitted = si24r01_get_packet_transmitted();
				mysensors_send(MYSENSORS_C_SET, MYSENSORS_V_VAR1, 4, MYSENSORS_P_ULONG32, (uint8_t *) &packet_transmitted, 0);
			}
		}
	}
}
