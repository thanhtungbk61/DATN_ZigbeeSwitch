

/*
 * myUart.c
 *
 *  Created on: Dec 7, 2019
 *      Author: who_you_are
 */
//
//
#include "myUart.h"
#include "app/framework/include/af.h"
#include "jsonStruct.h"
#include "string.h"
#include "inttypes.h"
#include "userMemory.h"
#include "structCommand.h"




#include EMBER_AF_API_NETWORK_CREATOR
#include EMBER_AF_API_NETWORK_CREATOR_SECURITY

packet globalCommandFrame;

static int flag = 0;
uint8_t addCount=1;
bool uartFlag;
uint8_t volatile uart_rx_data;
void set_data(uint8_t data){
	if ( flag == 0){
		uart_rx_data = data;
		flag = 1;
	}
}


bool get_flag()
{
	return uartFlag;
}

void clear_flag()
{
	 uartFlag = 0;
}

void set_flag()
{
	 uartFlag = 1;
}

int get_data(uint8_t *data){
	if( flag == 1){
		*data = uart_rx_data;
		flag  = 0;
		return 1;
	}
	return 0;
}

static flag_rx_status 	  flag_rx;
static rx_state_t	      rx_state;
static uart_packet_t      rx_packet;

void restart_receive(void)
{
	for(int i =0;i<LEN_PACKET;i++)
	{
		rx_packet.packet[i] = 0;
	}
	flag_rx 		= UNCOMPLETE;
	rx_state 		= RX_HEADER;
}

flag_rx_status receive_packet(uint8_t data)
{
    static uint16_t 	i;
    static uint8_t crc;

    if(flag_rx == COMPLETE)
        return COMPLETE;

    switch(rx_state)
    {
        case RX_HEADER:
        	emberAfCorePrintln("H: %c",data);
         if( data == HEADER_PACKET)
            {
                rx_state                      = RX_LEN_PAYLOAD;
                i                             = 0;
                rx_packet.packet_struct.len_payload = 0;
                crc                           = HEADER_PACKET;
            }
        break;

        case RX_LEN_PAYLOAD:
            rx_packet.packet_struct.len_payload = (rx_packet.packet_struct.len_payload << 8) | data;
            i++;
            if( i == LEN_LEN_PAYLOAD )
            {
                if( rx_packet.packet_struct.len_payload >  MAX_LEN_PAYLOAD )
                {
                    // rx_packet.packet_struct.len_payload = MAX_LEN_PAYLOAD;
                    flag_rx             = ERROR_LEN;
                    rx_state            = RX_HEADER;
                } else {
                    rx_state                = RX_PAYLOAD;
                    i                       = 0;
                }
            }
            crc += data;
        break;

//        case RX_CMD:
//        	emberAfCorePrintln("C: %c",data);
//            rx_packet.packet_struct.cmd   = data;
//            rx_state                      = RX_PAYLOAD;
//            i                             = 0;
//            crc += data;
        break;

        case RX_PAYLOAD:
            rx_packet.packet_struct.payload[i++] 	= data;

            if( i == (rx_packet.packet_struct.len_payload) )
            {
                rx_state                      = RX_CRC;
                i                             = 0;
            }
            crc += data;
        break;

        case RX_CRC:
        	emberAfCorePrintln("R: %c",rx_packet.packet_struct.payload[1]);
//            if ( data == crc ) {
                flag_rx         = COMPLETE;
//            } else {
//                flag_rx         = ERROR_CRC;
//                rx_state        = RX_HEADER;
//            }
        break;

        default:
        break;
    }

    return flag_rx;
}

void process()
{
	uint8_t status;
	emberAfCorePrintln("process---payload:%s",(char*)rx_packet.packet_struct.payload);
	receivePacket = getPacketStruct((char*)rx_packet.packet_struct.payload);
	// global variable
	getEUI64(receivePacket.MAC,receiveEUI64);
	getInstallCode(receivePacket.linkKey,receiveInstallCode);
	receiveDeviceStruct =getDeviceStruct(receivePacket.netDevice);


	//filter gateWay command
	switch(receivePacket.cmd)
	{
	// get = 0, put = 1, report = 2, add = 3, delete = 4
		case GET_COMMAND:
			emberAfCorePrintln("GET_COMMAND");
			status =getCommand();
			break;
		case PUT_COMMAND:
			emberAfCorePrintln("PUT_COMMAND");
			status = putCommand();
			break;
		case REPORT_COMMAND:
			emberAfCorePrintln("REPORT_COMMAND");
			break;
		case ADD_COMMAND:
			emberAfCorePrintln("ADD_COMMAND");
			status =addCommand();
			break;
		case DELETE_COMMAND:
			emberAfCorePrintln("DELETE_COMMAND");
//				ProvisonFrameStruct= createProvisonFrame((char*)rx_packet.packet_struct.payload);
//				emberAfCorePrintln("addObject--process MAC:%s---NameDevice:%s,PAN:%d",ProvisonFrameStruct.MAC,ProvisonFrameStruct.NameDevice,ProvisonFrameStruct.PAN);
//				addObjectRespon(ProvisonFrameStruct);
//				free(ProvisonFrameStruct.MAC);
//				free(ProvisonFrameStruct.NameDevice);
			break;


		default:

			break;
	}

}

// V1
// parse command -> netReading -> netResource --> netdevice

//uint8_t commandGet(packet receivePacket)
//{
//	uint8_t status=255;
//	for(int i=0;i<receivePacket.netReadingsCount;i++)
//	{
//		netResourceStruct Res = getResourceStruct(receivePacket.netReadings[i].netResource);
//		switch(Res.cluster)
//		{
//			case ZCL_ON_OFF_CLUSTER_ID:
//				break;
//			case ZCL_LEVEL_CONTROL_CLUSTER_ID:
//				break;
//			case ZCL_GROUPS_CLUSTER_ID:
//				break;
//			default:
//				break;
//
//		}
//	}
//	return status;
//}

//uint8_t commandSet(packet receivePacket)
//{
//	uint8_t status=1;
//	switch(frame.ClusterID)
//	{
//	case ONOFF:
//		status=onOffLight(frame);
//		break;
//
//	case MANAGER:
//		switch(frame.AttributeID)
//		{
//		case TRIGGER:
//			emberAfCorePrintln("TRIGGER");
//			status = onTrigger(frame);
//			break;
//		case SUBCRIBER:
//			status = setSubcriber(frame);
//			emberAfCorePrintln("-SUBCRIBER--:%s",frame.strValue);
//			break;
//		case SCHEDUAL:
//			break;
//		default:
//			break;
//		}
//		break;
//	default:
//		break;
//	}
//	return status;
//
////}
//void commandRespon(packet receivePacket, uint8_t status)
//{
//
//
////	char * responJ = responJson(respon);
////	//uint8_t cmd =1;
////	emberAfCorePrintln("commandRespon:%s-",responJ);
//	uartSendRespon(rx_packet.packet_struct.cmd,responJ);
////	free(responJ);
//
//}
//void addObjectRespon(packet receivePacket)
//{
//
//	ResponFrame respon ={
//			.MAC =frame.MAC,
//			.PAN =frame.PAN,
//			.Address =2,
//			.Type = 0,
//			.Endpoint =1,
//			.StatusResponse = 0x00,  // tru Push event se khong co StatusResponse
//			.NameDevice = "testDevice",
//			.Description = "testDescription",
//			.ProfileID =0x0104,
//			.ClusterID =0x0006,
//			.AttributeID = 0x0000,
//			.valueType =1,
//			.intValue =1,
//			.strValue =NULL
//	};
//	// if MAC = 0 => add group
//	// MAC != 0 => add device;
//	//respon.PAN = emberAfGetPanId();
//	if(strcmp(frame.MAC,"00000000")==0)
//		{
//		emberAfCorePrintln("respon.Address =1:%s",frame.MAC);
//		respon.Address =addCount;
//		addCount++;
//		respon.Type =EMBER_OUTGOING_MULTICAST;
//		char *payloadRes= responJson(respon);  // tao payload
//
//		emberAfCorePrintln("addObjectRespon---payload: %s",payloadRes);
//		uartSendRespon(rx_packet.packet_struct.cmd,payloadRes);
//		free(payloadRes);
//		}
//	else
//	{
//		strcpy(newDevice.MAC,frame.MAC);
//		strcpy(newDevice.NameDevice,frame.NameDevice);
//		newDevice.PAN =frame.PAN;
//		emberAfPluginNetworkCreatorSecurityOpenNetwork();
//	}
//}
void uartSendStr(char *string)
{
	for(int i=0;i<strlen(string);i++)
	{
		USART_Tx(USART3,string[i]);
	}
}

void uartSendRespon(char * payload)
{
	emberAfCorePrintln("\nuartSendRespon:%s\n",payload);
	UARTFrame frame;
	frame.Header = HEADER;
//	frame.Cmd = cmd;
	frame.Lenght =strlen(payload);
	frame.CRC = HEADER + (uint8_t)(frame.Lenght >>8) +(uint8_t)(frame.Lenght& 0x00FF);
	for(int i =0;i< strlen(payload);i++)
	{
		frame.CRC += payload[i];
	}
	USART_Tx(USART3,frame.Header);
	USART_Tx(USART3,(uint8_t)(frame.Lenght >>8));
	USART_Tx(USART3,(uint8_t)(frame.Lenght& 0x00FF));
//	USART_Tx(USART3,frame.Cmd);
	uartSendStr(payload);
	USART_Tx(USART3,frame.CRC);
}
