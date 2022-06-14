/*
 * File: mysensors.c
 * Denis Zheleznyakov aka ZiB @ http://ziblog.ru
 */

#include "main.h"

typedef struct
{
}__attribute__((__packed__)) mysensors_type;

mysensors_type mysensors;

void mysensors_send(uint8_t command_type, uint8_t type, uint8_t sensor_id, uint8_t payload_data_type, uint8_t * value, uint8_t param)
{
	mysensors_packet_type packet;

	packet.last = MYSENSORS_DEVICE_ADDRESS;
	packet.sender = MYSENSORS_DEVICE_ADDRESS;
	packet.destination = MYSENSORS_GATE_ADDRESS;

	packet.protocol_version = 2;
	packet.signed_flag = FALSE;
	packet.request_an_echo = FALSE;
	packet.is_echo_message = FALSE;

	packet.command_type = command_type;
	packet.type = type;
	packet.payload_data_type = payload_data_type;
	packet.sensor = sensor_id;
	packet.payload_length = 0;

	switch (payload_data_type)
	{
		case (MYSENSORS_P_STRING):
		{
			while ((packet.payload_length < MYSENSORS_PACKET_PAYLOAD_SIZE) && (*value != '\0'))
			{
				packet.data[packet.payload_length++] = *value++;
			}
			break;
		}
		case (MYSENSORS_P_BYTE):
		{
			packet.payload_length = sizeof(packet.bValue);
			packet.bValue = *value;
			break;
		}
		case (MYSENSORS_P_INT16):
		{
			packet.payload_length = sizeof(packet.iValue);
			packet.iValue = *((int16_t *) value);
			break;
		}
		case (MYSENSORS_P_UINT16):
		{
			packet.payload_length = sizeof(packet.uiValue);
			packet.uiValue = *((uint16_t *) value);
			break;
		}
		case (MYSENSORS_P_LONG32):
		{
			packet.payload_length = sizeof(packet.lValue);
			packet.lValue = *((int32_t *) value);
			break;
		}
		case (MYSENSORS_P_ULONG32):
		{
			packet.payload_length = sizeof(packet.ulValue);
			packet.ulValue = *((uint32_t *) value);
			break;
		}
		case (MYSENSORS_P_CUSTOM):
		{
			while ((packet.payload_length < MYSENSORS_PACKET_PAYLOAD_SIZE) && (packet.payload_length < param))
			{
				packet.data[packet.payload_length++] = *value++;
			}
			break;
		}
		case (MYSENSORS_P_FLOAT32):
		{
			packet.payload_length = sizeof(packet.fValue) + sizeof(packet.fPrecision);
			packet.fValue = *((float *) value);
			packet.fPrecision = param;
			break;
		}
	}

	si24r01_power_on();
	si24r01_send(MYSENSORS_GATE_ADDRESS, &packet);
	si24r01_power_off();
}
