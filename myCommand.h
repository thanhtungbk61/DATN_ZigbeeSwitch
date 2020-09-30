/*
 * myCommand.h
 *
 *  Created on: Dec 10, 2019
 *      Author: who_you_are
 */

#ifndef MYCOMMAND_H_
#define MYCOMMAND_H_
#include "jsonStruct.h"
#include "af.h"
#include "myUart.h"
#include "app/framework/include/af.h"
//#include "ember-types.h"
#define INSTALL_CODE_LENGTH_ 18
//get = 0, put = 1, report = 2, add = 3, delete = 4
typedef enum
{
	GET_COMMAND =0,
    PUT_COMMAND,
	REPORT_COMMAND,
	ADD_COMMAND,
	DELETE_COMMAND
} commandId;
//command to communicate with GateWay

/*
#define BIGENDIAN
*/
void fix32bit(char* data,uint8_t length);
// respond
void putRespondCommand(packet p,uint8_t status);
void getRespondCommand(packet p,uint8_t status);
void reportRespondCommand(packet p,netReading* net, uint8_t status);
void addRespondCommand(packet p,uint8_t status); // maybe need linkey
void deleteRespondCommand(packet p,uint8_t status);

// Handling

uint8_t putCommand();
uint8_t getCommand();
uint8_t reportCommand();
uint8_t addCommand();
uint8_t deleteCommand();


// send to Node V2
//get
uint8_t readAttributeSensor(netDeviceStruct netD, netResourceStruct netR);
uint8_t readAttributeLight(netDeviceStruct netD, netResourceStruct netR);
uint8_t readDimmingAttributeLight(netDeviceStruct netD, netResourceStruct netR);
uint8_t readPingAttributeLight(netDeviceStruct netD, netResourceStruct netR);
uint8_t onOffLight(netDeviceStruct netD, netResourceStruct netR,uint8_t value);
//
uint8_t putDimming(netDeviceStruct netD, netResourceStruct netR,uint16_t value);

uint8_t putRealtime(netDeviceStruct netD, netResourceStruct netR,uint8_t* value);

uint8_t putPing(netDeviceStruct netD, netResourceStruct netR,uint8_t* value);

uint8_t putReportTime(netDeviceStruct netD, netResourceStruct netR,uint8_t* value);
uint8_t getPing(netDeviceStruct netD, netResourceStruct netR);
// extended function
uint8_t putOnOffSchedual(char* scheString);
//
uint8_t putDimmingSchedual(netDeviceStruct netD,char* buffer,int leng);
uint8_t putOnOffSchV2(netDeviceStruct netD,char* buffer,int leng);
uint8_t putGroupV2(netDeviceStruct netD,char* buffer,int leng);
//

// node report to coordinator V2

void reportAttributes(EmberAfClusterId clusterId,
                                     uint8_t *buffer,
                                     uint16_t bufLen,
									 uint8_t typeCmd);
// report Ping

void reportPing(EmberAfClusterId clusterId,
                                     uint8_t *buffer,
                                     uint16_t bufLen,
									 uint8_t typeCmd);

//#define ONOFF 0
//#define SUBCRIBER 16
//
uint8_t levelControl(netDeviceStruct netD, netResourceStruct netR,uint8_t value);
//

void toggleLight(uint16_t address);
//uint8_t onTrigger(CommandFrame frame);
//uint8_t setSubcriber(CommandFrame frame);

//uint8_t offLight(CommandFrame frame);
uint8_t onLight1();
uint8_t offLight1();
//uint8_t readAttributeLight(CommandFrame frame);
uint8_t readAttributeLightOntime();
uint8_t readAttributeLightStartUP();
uint8_t readAttributeLightOffWaitTime();
uint8_t readAttributeLightGlobal();

//
uint8_t sendAddGroupCommand1(uint16_t nodeId,uint16_t groupId);
//uint8_t addGroup(uint16_t groupAdd);
#endif /* MYCOMMAND_H_ */
