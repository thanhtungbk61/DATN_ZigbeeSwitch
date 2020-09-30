/*
 * commandStruct.h
 *
 *  Created on: Dec 7, 2019
 *      Author: who_you_are
 */

#ifndef structCommand__h
#define structCommand__h
#include "stdint.h"

// include v2
#include "app/framework/include/af.h"
#include"string.h"
#include "cJSON.h"
// common ResponseCommonFrame :	Zigbee --> EdgeX

typedef enum
{
	command,
	pushEvent,
	addObject,
	deleteObject,
	scan
}commandType;

typedef enum
{
	CommandIdGet=1,
	CommandIdSet,
	CommandIdDelete
} CommandID;

typedef struct  UARTFrameStruct {
	uint8_t Header;
	uint16_t Lenght ;// Lenght = len(Payload) + size(Cmd)
	uint8_t Cmd ;
	char *Payload;
	uint8_t CRC;
}UARTFrame;

// number,int string-> (encode)-> string->(json)-> string(dang json)

//b1 json-> struct packet(co chua json)
//b2 ->commandPacket
// NetDev // NetRes

typedef struct NetDeviceStruct
{
	uint16_t type;
	uint16_t endpoint;
	uint32_t address;
}netDeviceStruct;

typedef struct  NetResourceStruct
{
	uint32_t profile;
	uint32_t cluster;
	uint32_t attribute;
}netResourceStruct;

typedef struct  NetReading
{
	uint64_t netOrigin;
	char* 	 netResource;
	uint8_t  netValueType;
	char* 	 netValue;
} netReading;
 //  Command(packet)  -> json
typedef struct  Packet
{   // {0,0,"/0","/0","/0","/0",0,"/0",0,"/0"}
	uint8_t cmd;
	uint8_t statusCode;
	char* statusMessage;
	char* MAC;
	char* linkKey;
	char* netDevice;    //   -> int --> <<
	uint8_t timeOut;
	char* content;
	uint8_t netReadingsCount;
	//cJSON* netReadings;  // json array// or convert to struct  netReadings struct NetReading
	netReading *netReadings;
}packet;

typedef struct  DevicePaket
{
	uint8_t Cmd;
	char* MAC;
	char* linkKey;
	char* netDevice;
	uint8_t statusCode;
	char* statusMessage;
}devicePacket;

typedef struct CommandPacket
{
	uint8_t cmd;
	char* netDevice;
	uint8_t netReadings;  //
	uint8_t statusCode;
	char* statusMessage;
}commandPacket;


// netValue

typedef struct  realTime
{
	uint8_t enable;
	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}realTime;

typedef struct  time
{
	uint8_t enable;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
}time;

//addDevice newDevice;
// defined global variable
//packet packetDefault ={0,0,NULL,NULL,NULL,NULL,0,NULL,0,NULL};
packet receivePacket,reportPacket;
uint8_t receiveEUI64[8];
uint8_t addDeviceKey[18];
uint8_t receiveInstallCode[18];
netDeviceStruct receiveDeviceStruct;

netResourceStruct receiveResourceStruct;
bool acknowledgeFlag; // flag to indicate that command need respond
uint16_t userNewNodeId;
//extern typedef EmberKeyData;
EmberKeyData userKey;

//

//
#endif
