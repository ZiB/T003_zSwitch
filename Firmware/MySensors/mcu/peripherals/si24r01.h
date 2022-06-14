/*
 * File: si24r01.h
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#ifndef SI24R01_H_
#define SI24R01_H_

enum si24r01_settings
{
	SI24R01_POWER_DELAY = 2, // [мс] пауза после включения питания для стабилизации напряжения питания
	SI24R01_RX_TIMEOUT_MS = 5, // таймаут приема ответа
	SI24R01_TX_TIMEOUT_MS = 3, // таймаут передачи
	SI24R01_CHANNEL_NUMBER = 76,
	SI24R01_MAX_PACKET_SIZE = 32,

	SI24R01_BASE_ADDRESS_0 = 0xA8, // базаовый адрес
	SI24R01_BASE_ADDRESS_1 = 0xA8,
	SI24R01_BASE_ADDRESS_2 = 0xE1,
	SI24R01_BASE_ADDRESS_3 = 0xFC
};

void si24r01_power_on(void);
void si24r01_power_off(void);
_Bool si24r01_send(uint8_t destination, mysensors_packet_type * packet);
uint32_t si24r01_get_packet_transmitted(void);

#endif /* SI24R01_H_ */
