
/***************************************************************************//**
 * @file
 * @brief
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

// This callback file is created for your convenience. You may add application
// code to this file. If you regenerate this file over a previous version, the
// previous version will be overwritten and any code you have added will be
// lost.

//#include "zigbee-device-common.c"
#include "em_msc.h"
//#include "myUart.h"
#include "app/framework/include/af.h"
#include "em_emu.h"
// my include
#include "cJSON.h"
#include "myUart.h"
#include "myInit.h"
#include "em_timer.h"
#include "cJSON.h"
#include "structCommand.h"
#include "stdlib.h"
#include "jsonStruct.h"
#include "myCommand.h"
#include "userMemory.h"

#include "userMemory.h"
// test write and read eeprom
//#define USERDATA ((uint32_t*)USERDATA_BASE)

uint32_t Cleared_value;
uint32_t Set_value,Set_value2;
uint8_t myValue=0;
uint8_t nodeAdd=0;
//
//EmberKeyData userKey;
//
#include EMBER_AF_API_NETWORK_CREATOR
#include EMBER_AF_API_NETWORK_CREATOR_SECURITY
#include EMBER_AF_API_NETWORK_STEERING
#include EMBER_AF_API_ZLL_PROFILE
#include EMBER_AF_API_FIND_AND_BIND_INITIATOR

#define SWITCH_ENDPOINT (1)

static bool commissioning = false;


EmberEventControl commissioningEventControl;
EmberEventControl ledEventControl;
EmberEventControl findingAndBindingEventControl;
static uint8_t lastButton;
EmberEventControl myDelay;
#define MY_DELAY_IN_MS 1000
uint8_t statusCallback, status2=99;
uint8_t count=0;
uint8_t count2=0;
uint8_t RxBuffer[100];
uint8_t test_duty=0;

#define IntType 0
#define StrType 1
typedef struct {
   uint16_t addr;
   char name[17];
   char value[8];
} schedule;

schedule convert2schedule(char* in) {
    return *((schedule*)in);
}



void emberAfTrustCenterJoinCallback(EmberNodeId newNodeId,
                                    EmberEUI64 newNodeEui64,
                                    EmberNodeId parentOfNewNode,
                                    EmberDeviceUpdate status,
                                    EmberJoinDecision decision)
{
	int i;
	for(i=0;i<8;i++)
	{
		emberAfCorePrintln("newNodeEui64:%x",newNodeEui64[i]);
	}
	userNewNodeId = newNodeId;
	emberAfCorePrintln("emberAfTrustCenterJoinCallback--newNodeId:%d--newNodeEui64:%d--status:%d",newNodeId,newNodeEui64,status);
	//uint8_t status2 = emberAfPluginNetworkCreatorSecurityCloseNetwork();
	//uint8_t status2 =emberLeaveRequest(userNewNodeId,newNodeEui64,EMBER_ZIGBEE_LEAVE_WITHOUT_REJOIN ,0);
	//uint8_t status2 = emberAfPluginNetworkCreatorSecurityCloseNetwork();
	//emberPermitJoining(0);


}
//void attributeRespon(EmberAfClusterId clusterId,
//									  uint8_t *buffer,
//									  uint16_t bufLen,uint8_t typeRespon)
//{
//	ResponFrame respon ={
//			.MAC = "",
//			.PAN = 0x00, //ProvisonFrameStruct.PAN,
//			.Address =emberGetSender(),   // typpe // endpoint // cmd =1 //atr
//			//.Address =2,
//			.Type = 0,
//			.Endpoint =1,
//			.StatusResponse = buffer[2],  // tru Push event se khong co StatusResponse
//			.NameDevice = "testDevice",
//			.Description = "testDescription",
//			.ProfileID =0x0104,
//			.ClusterID =clusterId,
//			.AttributeID = ((uint16_t)buffer[0]<<8) + (uint16_t)buffer[1],
//			.valueType=1,
//			.strValue=NULL,
//			.intValue=0
//	};
//	if(buffer[2]==0)
//	{
//		respon.intValue =buffer[4];
//	}
//	char * responJ = responJson( respon);
//	//uint8_t cmd =1;
//	emberAfCorePrintln("AttributesResponse:%s",responJ);
//	uartSendRespon(typeRespon,responJ);
//	free(responJ);
//}

void myDelayHandler(void )
{
	// test long message
//	emberFragmentSendUnicast();

	//
	emberEventControlSetInactive(myDelay);
	  EmberStatus status1, status2;

	  status1 =emberAfNetworkState();
	  if (emberAfNetworkState()!= EMBER_JOINED_NETWORK)
	  {
		  emberAfCorePrintln("=emberAfPluginNetworkCreatorNetworkForm--- networkStatus:%d1",status1);
		  emberAfPluginNetworkCreatorNetworkForm(true,
	                                                     15,
	                                                     3,
	                                                     20);
//		  emberAfPluginNetworkCreatorSecurityOpenNetwork();
		  emberAfPluginNetworkCreatorSecurityCloseNetwork();
		  emberEventControlSetDelayMS(myDelay, 5 * MY_DELAY_IN_MS);
	  }
	  else
	  {

		  uint16_t nodeId =emberAfGetNodeId();
		  status2 =emberAfNetworkState();
		//  emberPermitJoining(60);
		  emberEventControlSetDelayMS(myDelay, 100 * MY_DELAY_IN_MS);
		  emberAfCorePrintln("myDelayHandler-- networkstatus:%d--nodeId:%d",status2,nodeId);

	  }

}

void printfSchedule(schedule s, int typeValue){
	emberAfCorePrintln("addr:%d\nname:%s\n", s.addr, s.name);
    if( typeValue == StrType) {
    	emberAfCorePrintln("value:%s\n", s.value);
    }
    if( typeValue == IntType ){
        uint64_t v;
        v = * ((uint64_t*)(s.value));
        emberAfCorePrintln("value:%ld\n", v);
    }
}

void emberAfMainInitCallback(void)
{
	int i;

	// test Led
	  GPIO_PinModeSet(gpioPortD, 8, gpioModePushPull, 0);
	  GPIO_PinModeSet(gpioPortD, 9, gpioModePushPull, 1);
	 //
	  GPIO_PinOutSet(gpioPortD,8);
	  GPIO_PinOutSet(gpioPortD,9);
	  //------------------init PWM---------------
	  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
	  EMU_DCDCInit(&dcdcInit);

	  // Initializations
	  //initGpio();
	  initTimer();
	  //---------------------------

	  USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
	// Enable oscillator to GPIO and USART3 modules
	  init.baudrate = 9600;
	  CMU_ClockEnable(cmuClock_GPIO, true);
	  CMU_ClockEnable(cmuClock_USART3, true);

	  // set pin modes for USART TX and RX pins
	  GPIO_PinModeSet(gpioPortB, 7, gpioModeInput, 0);
	  GPIO_PinModeSet(gpioPortB, 6, gpioModePushPull, 1);

	  // Initialize USART asynchronous mode and route pins
	  USART_InitAsync(USART3, &init);
	  /* Enable I/O and set location */
	  USART3->ROUTEPEN |= USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;
	  USART3->ROUTELOC0 = (USART3->ROUTELOC0 &
	                       ~(_USART_ROUTELOC0_TXLOC_MASK
	                         | _USART_ROUTELOC0_RXLOC_MASK ))
	                       | (PORTIO_USART3_TX_LOC << _USART_ROUTELOC0_TXLOC_SHIFT)
	                       | (PORTIO_USART3_RX_LOC << _USART_ROUTELOC0_RXLOC_SHIFT);

	  //Initializae USART Interrupts
	  USART_IntEnable(USART3, USART_IEN_RXDATAV);

	  //Enabling USART Interrupts
	  NVIC_EnableIRQ(USART3_RX_IRQn);
	emberEventControlSetDelayMS(myDelay, 20 * MY_DELAY_IN_MS);
}

bool emberAfReadAttributesResponseCallback(EmberAfClusterId clusterId,
                                           uint8_t *buffer,
                                           uint16_t bufLen)
{
//	userNewNodeId =emberGetSender();
//	reportAttributes(clusterId,buffer,bufLen);
	emberAfCorePrintln("emberAfReadAttributesResponseCallback---clusterId:%d----Sender:%d",clusterId,userNewNodeId);
	for(int i=0;i<bufLen;i++)
	{
		emberAfCorePrintln("%d",buffer[i]);
	}
	reportAttributes(clusterId,buffer,bufLen,GET_COMMAND);
	return  true;
}

void emberAfOnOffClusterClientDefaultResponseCallback(int8u endpoint,
                                                      int8u commandId,
                                                      EmberAfStatus status)
{
	emberAfCorePrintln("emberAfOnOffClusterClientDefaultResponseCallback endpoint:%d  commandId:%d  status:%d",endpoint,commandId,status);
}

// all callback
bool emberAfReportAttributesCallback(EmberAfClusterId clusterId,
                                     uint8_t *buffer,
                                     uint16_t bufLen)
{
	userNewNodeId =emberGetSender();
	emberAfCorePrintln("emberAfReportAttributesCallback---clusterId:%d----all callback",clusterId);
	for(int i=0;i<bufLen;i++)
	{
		emberAfCorePrintln("%d",buffer[i]);
	}
	reportAttributes(clusterId,buffer,bufLen,REPORT_COMMAND);
	return 1;
}

void emberAfReportingAttributeChangeCallback(int8u endpoint,
                                             EmberAfClusterId clusterId,
                                             EmberAfAttributeId attributeId,
                                             int8u mask,
                                             int16u manufacturerCode,
                                             EmberAfAttributeType type,
                                             int8u *data)
{
	emberAfCorePrintln("emberAfReportingAttributeChangeCallback---");
}


void USART3_RX_IRQHandler(void)
{
	  uint32_t flags=0,data;
	  flags = USART_IntGet(USART3);
	  USART_IntClear(USART3, flags);
//	  USART_IntClear(USART3, USART_IEN_RXDATAV);
	  //USART3->IEN &= ~USART_IEN_RXDATAV;
	  data = USART_Rx(USART3);
	  if(receive_packet(data) == COMPLETE)
	  {
		  set_flag();
	  }
//	  set_data(data);
//      USART_Tx(USART3,data);

}


void USART3_TX_IRQHandler(void)
{
	  uint32_t flags;
	  flags = USART_IntGet(USART3);
	  USART_IntClear(USART3, flags);
}
void commissioningEventHandler(void)
{
  count++;
  emberEventControlSetInactive(commissioningEventControl);

  if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
  //  emberAfGetCommandApsFrame()->sourceEndpoint = SWITCH_ENDPOINT;
    if (lastButton == BUTTON0) {
    	toggleLight(userNewNodeId);
    	//putOnOffSchedual("0123456789");
//    	levelControl(userNewNodeId,10*count);
    	emberAfCorePrintln("toggleLight--userNewNodeId:%d",userNewNodeId);
    }
    else
    {
    	emberAfCorePrintln("send long message");
    	emberAfPluginNetworkCreatorSecurityOpenNetwork();
    }
  } else {
	  emberAfPluginNetworkCreatorSecurityOpenNetwork();
	  emberAfCorePrintln("OpenNetwork");
    emberEventControlSetActive(ledEventControl);
    commissioning = true;
  }
}

void ledEventHandler(void)
{
  emberEventControlSetInactive(ledEventControl);

  if (commissioning) {
    if (emberAfNetworkState() != EMBER_JOINED_NETWORK) {
      //halToggleLed(COMMISSIONING_STATUS_LED);
      emberEventControlSetDelayMS(ledEventControl, LED_BLINK_PERIOD_MS << 1);
    } else {
      //halSetLed(COMMISSIONING_STATUS_LED);
    }
  } else if (emberAfNetworkState() == EMBER_JOINED_NETWORK) {
   // halSetLed(COMMISSIONING_STATUS_LED);
  }
}

void findingAndBindingEventHandler(void)
{
  emberEventControlSetInactive(findingAndBindingEventControl);
  EmberStatus status = emberAfPluginFindAndBindInitiatorStart(SWITCH_ENDPOINT);
  emberAfCorePrintln("Find and bind initiator %p: 0x%X", "start", status);
}

static void scheduleFindingAndBindingForInitiator(void)
{
  emberEventControlSetDelayMS(findingAndBindingEventControl,
                              FINDING_AND_BINDING_DELAY_MS);
}

/** @brief Stack Status
 *
 * This function is called by the application framework from the stack status
 * handler.  This callbacks provides applications an opportunity to be notified
 * of changes to the stack status and take appropriate action.  The return code
 * from this callback is ignored by the framework.  The framework will always
 * process the stack status after the callback returns.
 *
 * @param status   Ver.: always
 */
bool emberAfStackStatusCallback(EmberStatus status)
{
  if (status == EMBER_NETWORK_DOWN) {
    halClearLed(COMMISSIONING_STATUS_LED);
  } else if (status == EMBER_NETWORK_UP) {
    halSetLed(COMMISSIONING_STATUS_LED);
  }

  // This value is ignored by the framework.
  return false;
}

/** @brief Hal Button Isr
 *
 * This callback is called by the framework whenever a button is pressed on the
 * device. This callback is called within ISR context.
 *
 * @param button The button which has changed state, either BUTTON0 or BUTTON1
 * as defined in the appropriate BOARD_HEADER.  Ver.: always
 * @param state The new state of the button referenced by the button parameter,
 * either ::BUTTON_PRESSED if the button has been pressed or ::BUTTON_RELEASED
 * if the button has been released.  Ver.: always
 */
void emberAfHalButtonIsrCallback(uint8_t button,
                                 uint8_t state)
{
  if (state == BUTTON_RELEASED) {
    lastButton = button;
    emberEventControlSetActive(commissioningEventControl);
  }
}

/** @brief Complete
 *
 * This callback is fired when the Network Steering plugin is complete.
 *
 * @param status On success this will be set to EMBER_SUCCESS to indicate a
 * network was joined successfully. On failure this will be the status code of
 * the last join or scan attempt. Ver.: always
 * @param totalBeacons The total number of 802.15.4 beacons that were heard,
 * including beacons from different devices with the same PAN ID. Ver.: always
 * @param joinAttempts The number of join attempts that were made to get onto
 * an open Zigbee network. Ver.: always
 * @param finalState The finishing state of the network steering process. From
 * this, one is able to tell on which channel mask and with which key the
 * process was complete. Ver.: always
 */
void emberAfPluginNetworkSteeringCompleteCallback(EmberStatus status,
                                                  uint8_t totalBeacons,
                                                  uint8_t joinAttempts,
                                                  uint8_t finalState)
{
  emberAfCorePrintln("%p network %p: 0x%X", "Join", "complete", status);

  if (status != EMBER_SUCCESS) {
    commissioning = false;
  } else {
    scheduleFindingAndBindingForInitiator();
  }
}

/** @brief Touch Link Complete
 *
 * This function is called by the ZLL Commissioning Common plugin when touch linking
 * completes.
 *
 * @param networkInfo The ZigBee and ZLL-specific information about the network
 * and target. Ver.: always
 * @param deviceInformationRecordCount The number of sub-device information
 * records for the target. Ver.: always
 * @param deviceInformationRecordList The list of sub-device information
 * records for the target. Ver.: always
 */
void emberAfPluginZllCommissioningCommonTouchLinkCompleteCallback(const EmberZllNetwork *networkInfo,
                                                                  uint8_t deviceInformationRecordCount,
                                                                  const EmberZllDeviceInfoRecord *deviceInformationRecordList)
{
  emberAfCorePrintln("%p network %p: 0x%X",
                     "Touchlink",
                     "complete",
                     EMBER_SUCCESS);

  scheduleFindingAndBindingForInitiator();
}

/** @brief Touch Link Failed
 *
 * This function is called by the ZLL Commissioning Client plugin if touch linking
 * fails.
 *
 * @param status The reason the touch link failed. Ver.: always
 */
void emberAfPluginZllCommissioningClientTouchLinkFailedCallback(EmberAfZllCommissioningStatus status)
{
  emberAfCorePrintln("%p network %p: 0x%X",
                     "Touchlink",
                     "complete",
                     EMBER_ERR_FATAL);

  commissioning = false;
}

/** @brief Complete
 *
 * This callback is fired by the initiator when the Find and Bind process is
 * complete.
 *
 * @param status Status code describing the completion of the find and bind
 * process Ver.: always
 */
void emberAfPluginFindAndBindInitiatorCompleteCallback(EmberStatus status)
{
  emberAfCorePrintln("Find and bind initiator %p: 0x%X", "complete", status);
  //emberLeaveRequest();
  commissioning = false;
}

// tet holiday test


void emberAfPluginDeviceDatabaseDiscoveryCompleteCallback(const EmberAfDeviceInfo*device)
{

	// test
	userNewNodeId =emberGetSender();
	emberAfCorePrintln("\n\nemberAfPluginDeviceDatabaseDiscoveryCompleteCallback  EmberEUI64:%d, nodeId:%d ,EmberAfDeviceDiscoveryStatus:%d",device->eui64,emberGetSender(),device->status);
	emberAfCorePrintln("clusterId0:%d, endpoint0:%d----clusterId1:%d, endpoint1:%d",device->endpoints[0].clusters[0].clusterId,device->endpoints[0].endpoint,device->endpoints[1].clusters[0].clusterId,device->endpoints[1].endpoint);
	//uint8_t status2 =emberLeaveRequest(userNewNodeId,device->eui64,EMBER_ZIGBEE_LEAVE_WITHOUT_REJOIN ,0);
	//uint8_t status2 = emberAfPluginNetworkCreatorSecurityCloseNetwork();
	//emberAfCorePrintln("emberLeaveRequest:%d",status2);
	// prepare for netDevice
	netDeviceStruct netD;
	netD.address =emberGetSender();
	netD.endpoint =1;  // default endpoint
	netD.type =0;       // unicast

	packet p ={0,0,NULL,NULL,NULL,NULL,0,NULL,0,NULL};
	p.cmd = ADD_COMMAND;
	p.netDevice =createDeviceJson(netD);
	emberAfCorePrintln("emberAfPluginDeviceDatabaseDiscoveryCompleteCallback---createDeviceJson");
	// hard code
	p.content =strdup("add device");
	p.statusMessage= strdup("successful");
	p.netReadingsCount =0;

	//
//	p.cmd =ADD_COMMAND;
	p.statusCode =0; // successful
	p.MAC = createEUI64Str(device->eui64);
	emberAfCorePrintln("createEUI64Str");
	p.linkKey = createInstallcodeStr(addDeviceKey);
	emberAfCorePrintln("createInstallcodeStr");
	char* respondP = addRespon(p);
	uartSendRespon(respondP);
	free(respondP);
//	freePacket(p);
}

//typedef struct {
//  EmberEUI64 eui64;
//  EmberAfEndpointInfoStruct endpoints[EMBER_AF_MAX_ENDPOINTS_PER_DEVICE];
//  EmberAfDeviceDiscoveryStatus status;
//  uint8_t discoveryFailures;
//  uint8_t capabilities;
//  uint8_t endpointCount;
//  uint8_t stackRevision;
//} EmberAfDeviceInfo;
//void emberAfPluginDeviceDatabaseDiscoveryCompleteCallback(const EmberAfDeviceInfo*device)
//{
//	emberAfCorePrintln("emberAfPluginDeviceDatabaseDiscoveryCompleteCallback");
//}

//void emberAfPluginDeviceTableNewDeviceCallback(EmberEUI64 eui64)
//{
//	emberAfCorePrintln("emberAfPluginDeviceTableNewDeviceCallback");
//}
//
bool emberAfPluginNetworkFindJoinCallback(EmberZigbeeNetwork *networkFound,
                                          uint8_t lqi,
                                          int8_t rssi)
{
	emberAfCorePrintln("emberAfPluginDeviceTableNewDeviceCallback");
	return true;
}

void emberAfGroupsClusterClientDefaultResponseCallback(int8u endpoint,
                                                       int8u commandId,
                                                       EmberAfStatus status)
{
	emberAfCorePrintln("emberAfGroupsClusterClientDefaultResponseCallback--stt:%d",status);
}


//void emberAfLevelControlClusterClientAttributeChangedCallback(int8u endpoint,
//                                                              EmberAfAttributeId attributeId)
//{
//	emberAfCorePrintln("emberAfLevelControlClusterClientAttributeChangedCallback");
//}
void emberAfLevelControlClusterClientDefaultResponseCallback(int8u endpoint,
                                                             int8u commandId,
                                                             EmberAfStatus status)
{
	emberAfCorePrintln("emberAfLevelControlClusterClientDefaultResponseCallback--status:%d",status);
}

void emberAfPluginOtaServerUpdateCompleteCallback(uint16_t manufacturerId,
                                                  uint16_t imageTypeId,
                                                  uint32_t firmwareVersion,
                                                  EmberNodeId source,
                                                  uint8_t status)
{
	emberAfCorePrintln("emberAfPluginOtaServerUpdateCompleteCallback");
}

boolean emberAfManagerGetPingCallback(int8u* Ping)
{
	emberAfCorePrintln("emberAfManagerGetPingCallback");
	reportAttributes(ZCL_MANAGER_ID,Ping,8,GET_COMMAND);
	return true;
}

boolean emberAfManagerGetReportTimeCallback(int16u reportStr)
{
	emberAfCorePrintln("emberAfManagerGetReportTimeCallback");
//	reportAttributes(ZCL_MANAGER_ID,reportStr,2,GET_COMMAND);
	return true;
}

boolean emberAfMessageSentCallback(EmberOutgoingMessageType type,
                                int16u indexOrDestination,
                                EmberApsFrame* apsFrame,
                                int16u msgLen,
                                int8u* message,
                                EmberStatus status)
{
	emberAfCorePrintln("emberAfMessageSentCallback");
	if(getAcknowledgeFlag())
	{
		if(receiveResourceStruct.cluster == apsFrame->clusterId)
		{
			char *respond = putRespon(receivePacket);
			uartSendRespon(respond);
			free(respond);
		}
		clearAcknowledgeFlag();
		resetNetResource(receiveResourceStruct);
		return true;
	}
	resetNetResource(receiveResourceStruct);
	return true;
}

boolean emberAfManagerPutPingCallback(int8u* Ping)
{//REPORT_COMMAND
	emberAfCorePrintln("emberAfManagerPutPingCallback");
	reportPing(ZCL_MANAGER_ID,Ping,8,REPORT_COMMAND);
	return true;
}
