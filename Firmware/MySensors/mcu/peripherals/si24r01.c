/*
 * File: si24r01.c
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

enum si24r01_commands
{
	CMD_R_REGISTER = 0x00,
	CMD_W_REGISTER = 0x20,
	CMD_R_RX_PLOAD_WID = 0x60,
	CMD_R_RX_PLOAD = 0x61,
	CMD_W_TX_PLOAD = 0xA0,
	CMD_FLUSH_TX = 0xE1,
	CMD_FLUSH_RX = 0xE2,
	CMD_REUSE_TX_PL = 0xE3,
	CMD_W_ACK_PLOAD = 0xA8,
	CMD_W_TX_PLOAD_NOACK = 0xB0,
	CMD_NOP = 0xFF
};

enum si24r01_registers
{
	REG_CONFIG = 0x00,
	REG_EN_AA = 0x01,
	REG_EN_RXADDR = 0x02,
	REG_SETUP_AW = 0x03,
	REG_SETUP_RETR = 0x04,
	REG_RF_CH = 0x05,
	REG_RF_SETUP = 0x06,
	REG_STATUS = 0x07,
	REG_OBSERVE_TX = 0x08,
	REG_CD = 0x09,
	REG_RX_ADDR_P0 = 0x0A,
	REG_RX_ADDR_P1 = 0x0B,
	REG_RX_ADDR_P2 = 0x0C,
	REG_RX_ADDR_P3 = 0x0D,
	REG_RX_ADDR_P4 = 0x0E,
	REG_RX_ADDR_P5 = 0x0F,
	REG_TX_ADDR = 0x10,
	REG_RX_PW_P0 = 0x11,
	REG_RX_PW_P1 = 0x12,
	REG_RX_PW_P2 = 0x13,
	REG_RX_PW_P3 = 0x14,
	REG_RX_PW_P4 = 0x15,
	REG_RX_PW_P5 = 0x16,
	REG_FIFO_STATUS = 0x17,
	REG_DYNPD = 0x1C,
	REG_FEATURE = 0x1D
};

enum si24r01_reg_config
{
	REG_CONFIG_TX_ENABLE = 0,
	REG_CONFIG_RX_ENABLE = BIT(0),
	REG_CONFIG_POWER_DOWN = 0,
	REG_CONFIG_POWER_UP = BIT(1),
	REG_CONFIG_CRC8 = BIT(3),
	REG_CONFIG_CRC16 = BIT(2) | BIT(3),
	REG_CONFIG_CRC_DISABLE = 0,
	REG_CONFIG_MASK_INTERRUPT_MAX_RT = BIT(4),
	REG_CONFIG_MASK_INTERRUPT_TX_DS = BIT(5),
	REG_CONFIG_MASK_INTERRUPT_RX_DR = BIT(6)
};

enum si24r01_reg_enable_autoack
{
	REG_EN_AA_DISABLE_ALL = 0,
	REG_EN_AA_PIPE0 = BIT(0),
	REG_EN_AA_PIPE1 = BIT(1),
	REG_EN_AA_PIPE2 = BIT(2),
	REG_EN_AA_PIPE3 = BIT(3),
	REG_EN_AA_PIPE4 = BIT(4),
	REG_EN_AA_PIPE5 = BIT(5)
};

enum si24r01_reg_enable_rx_pipe
{
	REG_ENABLE_RX_PIPE_0 = BIT(0),
	REG_ENABLE_RX_PIPE_1 = BIT(1),
	REG_ENABLE_RX_PIPE_2 = BIT(2),
	REG_ENABLE_RX_PIPE_3 = BIT(3),
	REG_ENABLE_RX_PIPE_4 = BIT(4),
	REG_ENABLE_RX_PIPE_5 = BIT(5)
};

enum si24r01_reg_setup_address_width
{
	REG_SETUP_AW_3_BYTES = BIT(0), //
	REG_SETUP_AW_4_BYTES = BIT(1),
	REG_SETUP_AW_5_BYTES = BIT(1) | BIT(0)
};

enum si24r01_reg_setup_retransmission
{
	REG_SETUP_RETR_DELAY_250US = 0,
	REG_SETUP_RETR_DELAY_500US = BIT(4),
	REG_SETUP_RETR_DELAY_750US = BIT(5),
	REG_SETUP_RETR_DELAY_1000US = BIT(5) | BIT(4),
	REG_SETUP_RETR_DELAY_1250US = BIT(6),
	REG_SETUP_RETR_DELAY_1500US = BIT(6) | BIT(4),
	REG_SETUP_RETR_DELAY_1750US = BIT(6) | BIT(5),
	REG_SETUP_RETR_DELAY_2000US = BIT(6) | BIT(5) | BIT(4),
	REG_SETUP_RETR_DELAY_2250US = BIT(7),
	REG_SETUP_RETR_DELAY_2500US = BIT(7) | BIT(4),
	REG_SETUP_RETR_DELAY_2750US = BIT(7) | BIT(5),
	REG_SETUP_RETR_DELAY_3000US = BIT(7) | BIT(5) | BIT(4),
	REG_SETUP_RETR_DELAY_3250US = BIT(7) | BIT(6),
	REG_SETUP_RETR_DELAY_3500US = BIT(7) | BIT(6) | BIT(4),
	REG_SETUP_RETR_DELAY_3750US = BIT(7) | BIT(6) | BIT(5),
	REG_SETUP_RETR_DELAY_4000US = BIT(7) | BIT(6) | BIT(5) | BIT(4),
	REG_SETUP_RETR_DISABLE = 0,
	REG_SETUP_RETR_COUNT_1 = BIT(0),
	REG_SETUP_RETR_COUNT_2 = BIT(1),
	REG_SETUP_RETR_COUNT_3 = BIT(1) | BIT(0),
	REG_SETUP_RETR_COUNT_4 = BIT(2),
	REG_SETUP_RETR_COUNT_5 = BIT(2) | BIT(0),
	REG_SETUP_RETR_COUNT_6 = BIT(2) | BIT(1),
	REG_SETUP_RETR_COUNT_7 = BIT(2) | BIT(1) | BIT(0),
	REG_SETUP_RETR_COUNT_8 = BIT(3),
	REG_SETUP_RETR_COUNT_9 = BIT(3) | BIT(0),
	REG_SETUP_RETR_COUNT_10 = BIT(3) | BIT(1),
	REG_SETUP_RETR_COUNT_11 = BIT(3) | BIT(1) | BIT(0),
	REG_SETUP_RETR_COUNT_12 = BIT(3) | BIT(2),
	REG_SETUP_RETR_COUNT_13 = BIT(3) | BIT(2) | BIT(0),
	REG_SETUP_RETR_COUNT_14 = BIT(3) | BIT(2) | BIT(1),
	REG_SETUP_RETR_COUNT_15 = BIT(3) | BIT(2) | BIT(1) | BIT(0)
};

enum si24r01_reg_rf_setup
{
	REG_RF_SETUP_POWER__12DBM = 0,
	REG_RF_SETUP_POWER__6DBM = BIT(0),
	REG_RF_SETUP_POWER__4DBM = BIT(1),
	REG_RF_SETUP_POWER_0DBM = BIT(1) | BIT(0),
	REG_RF_SETUP_POWER_1DBM = BIT(2),
	REG_RF_SETUP_POWER_3DBM = BIT(2) | BIT(0),
	REG_RF_SETUP_POWER_4DBM = BIT(2) | BIT(1),
	REG_RF_SETUP_POWER_7DBM = BIT(2) | BIT(1) | BIT(0),
	REG_RF_SETUP_DR_250KB = BIT(5),
	REG_RF_SETUP_DR_1MB = 0,
	REG_RF_SETUP_DR_2MB = BIT(3),
	REG_RF_SETUP_PLL_LOCK = BIT(4),
	REG_RF_SETUP_CONT_WAVE = BIT(7)
};

enum si24r01_reg_feature
{
	REG_FEATURE_DYN_ACK = BIT(0), //
	REG_FEATURE_ACK_PAY = BIT(1),
	REG_FEATURE_DPL_EN = BIT(2)
};

enum si24r01_reg_dynpd
{
	REG_DYNPD_ALL_DISABLE = 0, //
	REG_DYNPD_PIPE0 = BIT(0),
	REG_DYNPD_PIPE1 = BIT(1),
	REG_DYNPD_PIPE2 = BIT(2),
	REG_DYNPD_PIPE3 = BIT(3),
	REG_DYNPD_PIPE4 = BIT(4),
	REG_DYNPD_PIPE5 = BIT(5)
};

enum si24r01_reg_status
{
	REG_STATUS_TX_FULL = BIT(0),
	REG_STATUS_RX_FIFO_PIPE0_READY = 0,
	REG_STATUS_RX_FIFO_PIPE1_READY = BIT(1),
	REG_STATUS_RX_FIFO_PIPE2_READY = BIT(2),
	REG_STATUS_RX_FIFO_PIPE3_READY = BIT(2) | BIT(1),
	REG_STATUS_RX_FIFO_PIPE4_READY = BIT(3),
	REG_STATUS_RX_FIFO_PIPE5_READY = BIT(3) | BIT(1),
	REG_STATUS_RX_FIFO_EMPTY = BIT(3) | BIT(2) | BIT(1),
	REG_STATUS_RX_FIFO_MASK = BIT(3) | BIT(2) | BIT(1),
	REG_STATUS_MAX_RT = BIT(4),
	REG_STATUS_TX_DS = BIT(5),
	REG_STATUS_RX_DR = BIT(6)
};

typedef struct
{
	uint32_t transmitted;
}__attribute__((__packed__)) si24r01_type;

si24r01_type si24r01;

uint32_t si24r01_get_packet_transmitted(void)
{
	return si24r01.transmitted;
}

uint8_t si24r01_read_byte(uint8_t address)
{
	PIN_ON(PIN_RF_CS);

	SPI1->DR = address;
	while (!(SPI1->SR & SPI_SR_RXNE))
	{
	}
	(void) SPI1->DR;

	SPI1->DR = 0x00;
	while (!(SPI1->SR & SPI_SR_RXNE))
	{
	}

	PIN_OFF(PIN_RF_CS);

	return SPI1->DR;
}

void si24r01_write_byte(uint8_t address, uint8_t value)
{
	PIN_ON(PIN_RF_CS);

	SPI1->DR = address;
	while (!(SPI1->SR & SPI_SR_RXNE))
	{
	}
	(void) SPI1->DR;

	SPI1->DR = value;
	while (!(SPI1->SR & SPI_SR_RXNE))
	{
	}
	(void) SPI1->DR;

	PIN_OFF(PIN_RF_CS);
}

void si24r01_write(uint8_t address, uint8_t data[], uint8_t length)
{
	PIN_ON(PIN_RF_CS);

	SPI1->DR = address;
	while (!(SPI1->SR & SPI_SR_RXNE))
	{
	}
	(void) SPI1->DR;

	while (length-- > 0)
	{
		SPI1->DR = *data++;
		while (!(SPI1->SR & SPI_SR_RXNE))
		{
		}
		(void) SPI1->DR;
	}

	PIN_OFF(PIN_RF_CS);
}

void si24r01_read(uint8_t address, uint8_t data[], uint8_t length)
{
	PIN_ON(PIN_RF_CS);

	SPI1->DR = address;
	while (!(SPI1->SR & SPI_SR_RXNE))
	{
	}
	(void) SPI1->DR;

	while (length-- > 0)
	{
		SPI1->DR = 0x00;
		while (!(SPI1->SR & SPI_SR_RXNE))
		{
		}
		*data++ = SPI1->DR;
	}

	PIN_OFF(PIN_RF_CS);
}

void si24r01_power_on(void)
{
	PIN_ON(PIN_RF_PWR);

	macros_delay_ms(SI24R01_POWER_DELAY);

	// общие настройки
	si24r01_write_byte(CMD_W_REGISTER + REG_CONFIG, REG_CONFIG_POWER_DOWN);
	si24r01_write_byte(CMD_W_REGISTER + REG_SETUP_AW, REG_SETUP_AW_5_BYTES);
	si24r01_write_byte(CMD_W_REGISTER + REG_SETUP_RETR, REG_SETUP_RETR_DISABLE);
	si24r01_write_byte(CMD_W_REGISTER + REG_RF_CH, SI24R01_CHANNEL_NUMBER);
	si24r01_write_byte(CMD_W_REGISTER + REG_RF_SETUP, REG_RF_SETUP_DR_250KB | REG_RF_SETUP_POWER_7DBM);
	si24r01_write_byte(CMD_W_REGISTER + REG_FEATURE, REG_FEATURE_DPL_EN);
	si24r01_write_byte(CMD_W_REGISTER + REG_EN_AA, REG_EN_AA_DISABLE_ALL);
	si24r01_write_byte(CMD_W_REGISTER + REG_DYNPD, REG_DYNPD_PIPE0);

	// настройки приемника
	uint8_t address[] = { MYSENSORS_DEVICE_ADDRESS, SI24R01_BASE_ADDRESS_3, SI24R01_BASE_ADDRESS_2, SI24R01_BASE_ADDRESS_1, SI24R01_BASE_ADDRESS_0 };
	si24r01_write(CMD_W_REGISTER + REG_RX_ADDR_P0, &address[0], ARRAY_LENGHT(address));

	si24r01_write_byte(CMD_FLUSH_RX, 0);
	si24r01_write_byte(CMD_FLUSH_TX, 0);
	si24r01_write_byte(CMD_W_REGISTER + REG_STATUS, 0xFF);
	si24r01_write_byte(CMD_W_REGISTER + REG_EN_RXADDR, REG_ENABLE_RX_PIPE_0);
}

void si24r01_power_off(void)
{
	PIN_OFF(PIN_RF_PWR);
}

_Bool si24r01_send(uint8_t destination, mysensors_packet_type * packet)
{
	// пауза, шлюз на esp8266 не успевает принимать пакеты
	macros_delay_ms(100);

	uint8_t address[] = { destination, SI24R01_BASE_ADDRESS_3, SI24R01_BASE_ADDRESS_2, SI24R01_BASE_ADDRESS_1, SI24R01_BASE_ADDRESS_0 };

	si24r01_write(CMD_W_REGISTER + REG_TX_ADDR, (uint8_t*) &address[0], ARRAY_LENGHT(address));
	si24r01_write(CMD_W_TX_PLOAD_NOACK, (uint8_t*) packet, packet->payload_length + MYSENSORS_PACKET_HEADER_SIZE);
	si24r01_write_byte(CMD_W_REGISTER + REG_STATUS, REG_STATUS_MAX_RT | REG_STATUS_TX_DS);
	si24r01_write_byte(CMD_W_REGISTER + REG_CONFIG, REG_CONFIG_POWER_UP | REG_CONFIG_CRC16 | REG_CONFIG_MASK_INTERRUPT_RX_DR | REG_CONFIG_TX_ENABLE);

	PIN_ON(PIN_RF_CE);

	macros_delay_ms_start(SI24R01_TX_TIMEOUT_MS);

	while ((!PIN_SIGNAL(PIN_RF_IRQ)) && (macros_delay_ms_wait() == FALSE))
	{
	}

	PIN_OFF(PIN_RF_CE);

	macros_delay_ms_stop();

	si24r01.transmitted++;

	return FALSE;
}

