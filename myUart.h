/*
 * myUart.h
 *
 *  Created on: Dec 7, 2019
 *      Author: who_you_are
 */

#ifndef MYUART_H_
#define MYUART_H_

#include "stdint.h"
#include "myCommand.h"
#define HEADER_PACKET     0x55
#define LEN_LEN_PAYLOAD   2
#define LEN_CMD           1
#define MAX_LEN_PAYLOAD       1024
#define LEN_PACKET    (LEN_LEN_PAYLOAD + LEN_CMD + MAX_LEN_PAYLOAD)





typedef struct __attribute__((packed))
{
  uint16_t  len_payload;
  uint8_t   cmd;
  uint8_t   payload[MAX_LEN_PAYLOAD];
} _packet_t;

typedef union __attribute__((packed))
{
    _packet_t packet_struct;
    uint8_t   packet[LEN_PACKET];
} uart_packet_t;

typedef enum
{
	RX_HEADER,
	RX_LEN_PAYLOAD,
    RX_CMD,
	RX_PAYLOAD,
    RX_CRC
} rx_state_t;

typedef enum
{
    COMPLETE = 0,
	ERROR_CRC,
    ERROR_LEN,
	UNCOMPLETE
} flag_rx_status;

typedef enum
{
    GETCOMPLETE = 0,
	GETUNCOMPLETE
} flag_GetCommand_status;
// ClusterId , AtrributeId

typedef enum
{
	GROUP =4,
    ONOFF = 6,
	MANAGER =0xFC10
} CLusterId;

typedef enum
{
	TRIGGER =16,
    SUBCRIBER = 17,
	SCHEDUAL =18
} AttributeID;

void set_data(uint8_t data);
int get_data(uint8_t *data);
void restart_receive(void);
flag_rx_status receive_packet(uint8_t data);
void process();
void uartSendStr(char *string);
void uartSendRespon(char * payload);

uint8_t commandGet(packet receivePacket);
uint8_t commandSet(packet receivePacket);
//void uartSendRespon(UARTFrame frame);
void commandRespon(packet receivePacket, uint8_t status);
void addObjectRespon(packet receivePacket);
#endif /* MYUART_H_ */
