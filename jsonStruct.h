/*
 * jsonStruct.h
 *
 *  Created on: Dec 7, 2019
 *      Author: who_you_are
 */





/*
 * jsonStruct.h
 *
 *  Created on: Dec 7, 2019
 *      Author: who_you_are
 */

#ifndef JSONSTRUCT_H_
#define JSONSTRUCT_H_

#define HEADER 0x55
#include "stdio.h"
#include "cJSON.h"
#include "structCommand.h"
#include "stdlib.h"
#include "string.h"
// include  v2
#include "userBase64.h"
#include "myUart.h"
#include "cJSON.h"
#include "structCommand.h"   // lib for malloc

#define HEADER 0x55
#define EUI64SIZE 8
#define INSTALLCODE_LENGTH 18

void setAcknowledgeFlag();
void clearAcknowledgeFlag();
bool getAcknowledgeFlag();
// getRespon
char* getRespon(packet p);
char* addRespon(packet p);
char* putRespon(packet p);
// parse receive;
packet getPacketStruct(char* packetString);
char* createRespondPacket(packet p);
void freePacket(packet p);

netDeviceStruct getDeviceStruct(char* Str);
char* createDeviceJson(netDeviceStruct D);
netResourceStruct getResourceStruct(char* Str);
char* createResourceJson(netResourceStruct R);

void getEUI64(char* str,uint8_t* eui64);
char* createEUI64Str(uint8_t* eui64);

void getInstallCode(char* str,uint8_t* installCode);
char* createInstallcodeStr(uint8_t* installCode);

void printfPacket(packet p);

// prepare to send
char* createNetReadingsJson(netReading* netR, uint8_t count);

//
void resetNetResource(netResourceStruct netR);
//
// Add Device:
// {"cmd":3,"mac":"12345678AABBCC22","lk":"This is a Link Key","dev":"","sc":0,"sm":""}
// Rep:
// {"cmd":3,"mac":"12345678AABBCC22","lk":"This is a Link Key","dev":"00001234","sc":0,"sm":""}

// Get Command:
// {"cmd":0,"dev":"000001","evt":[{"r":"123400","t":0}],"sc":0,"sm":""}
// rep: {"cmd":0,"dev":"000001","evt":[{"r":"123400","t":0,"v":"AA=="}],"sc":0,"sm":""}

// Put Command:
// {"cmd":1,"dev":"000001","evt":[{"o":1586234682954542112,"r":"123400","t":0,"v":"AQ=="}],"sc":0,"sm":""}
// Rep:
// {"cmd":1,"dev":"000001","sc":0,"sm":""}

// Delete Device:
// {"cmd":4,"mac":"12345678AABBCC22","lk":"","dev":"00001234","sc":0,"sm":""}
// Rep:
// {"cmd":4,"mac":"12345678AABBCC22","sc":0,"sm":""}


#endif /* JSONSTRUCT_H_ */
