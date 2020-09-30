/*
 * jsonStruct.c
 *
 *  Created on: Dec 7, 2019
 *      Author: who_you_are
 */

#include"jsonStruct.h"

/*
 * jsonStruct.c
 *
 *  Created on: Dec 7, 2019
 *      Author: who_you_are
 */
#include<stdio.h>
#include <stdlib.h>
#include"jsonStruct.h"
#include "userBase64.h"


void setAcknowledgeFlag()
{
	acknowledgeFlag =1;
}

void clearAcknowledgeFlag()
{
	acknowledgeFlag =0;
}

bool getAcknowledgeFlag()
{
	return acknowledgeFlag;
}
void resetNetResource(netResourceStruct netR)
{
	netR.attribute =0xff;
	netR.cluster   = 0xff;
	netR.profile   =0xff;
}
packet getPacketStruct(char* packetString)
{
 	packet  packetStruct={0,0,NULL,NULL,NULL,NULL,0,NULL,0,NULL};
    cJSON *string_json = cJSON_Parse(packetString);
    //JSONArray arr = obj.getJSONArray("value");
    if (string_json == NULL)
    {
    	goto end;
    }
    if(cJSON_GetObjectItemCaseSensitive(string_json, "cmd")!=NULL)
    {
    	packetStruct.cmd =cJSON_GetObjectItemCaseSensitive(string_json, "cmd")->valuedouble;
    }

    if(cJSON_GetObjectItemCaseSensitive(string_json, "sc")!=NULL)
    {
    	packetStruct.statusCode =cJSON_GetObjectItemCaseSensitive(string_json, "sc")->valuedouble;
    }

        if(cJSON_GetObjectItemCaseSensitive(string_json, "sm")!=NULL)
    {
    	packetStruct.statusMessage =strdup(cJSON_GetObjectItemCaseSensitive(string_json, "sm")->valuestring);
    }

        if(cJSON_GetObjectItemCaseSensitive(string_json, "mac")!=NULL)
    {
    	packetStruct.MAC =strdup(cJSON_GetObjectItemCaseSensitive(string_json, "mac")->valuestring);
    }

        if(cJSON_GetObjectItemCaseSensitive(string_json, "lk")!=NULL)
    {
    	packetStruct.linkKey =strdup(cJSON_GetObjectItemCaseSensitive(string_json, "lk")->valuestring);
    }

        if(cJSON_GetObjectItemCaseSensitive(string_json, "dev")!=NULL)
    {
    	packetStruct.netDevice =strdup(cJSON_GetObjectItemCaseSensitive(string_json, "dev")->valuestring);
    }

        if(cJSON_GetObjectItemCaseSensitive(string_json, "to")!=NULL)
    {
    	packetStruct.timeOut =cJSON_GetObjectItemCaseSensitive(string_json, "to")->valuedouble;
    }

    if(cJSON_GetObjectItemCaseSensitive(string_json, "ct")!=NULL)
    {
    	packetStruct.content =strdup(cJSON_GetObjectItemCaseSensitive(string_json, "ct")->valuestring);
    }
    cJSON* jnetReadings =cJSON_GetObjectItem(string_json, "evt"); // array
    // struct Vector *retVal = malloc (sizeof (struct Vector));
	packetStruct.netReadingsCount=cJSON_GetArraySize(jnetReadings);
	emberAfCorePrintln("netReadingsCount:%d",packetStruct.netReadingsCount);
    packetStruct.netReadings = malloc(sizeof(netReading)*cJSON_GetArraySize(jnetReadings));

	// emberAfCorePrintln("--------------:%d",cJSON_GetArraySize(jnetReadings));

    for (int i = 0 ; i <packetStruct.netReadingsCount; i++)
	{
		cJSON * subitem = cJSON_GetArrayItem(jnetReadings, i);
		packetStruct.netReadings[i].netResource = strdup(cJSON_GetObjectItem(subitem, "r")->valuestring);
		packetStruct.netReadings[i].netValueType = cJSON_GetObjectItem(subitem, "t")->valuedouble;
		packetStruct.netReadings[i].netValue= strdup(cJSON_GetObjectItem(subitem, "v")->valuestring);
		packetStruct.netReadings[i].netOrigin =	cJSON_GetObjectItem(subitem, "o")->valuedouble;
		//cJSON_free(subitem);
		//packetStruct.netReadings[i].netResource =resource->valuestring;
		//emberAfCorePrintln("Event[%d]:\n\tresource:%s\n\ttype:%d\n\tvalue:%s\n", i, packetStruct.netReadings[i].netResource, packetStruct.netReadings[i].netValueType, packetStruct.netReadings[i].netValue);
	}
end:

    cJSON_Delete(string_json);

     return packetStruct;
}


//
/*

typedef struct NetDeviceStruct
{
	uint16_t type;
	uint16_t endpoint;
	uint32_t address;
}netDeviceStruct;

*/
netDeviceStruct getDeviceStruct(char* str)
{
	netDeviceStruct netDeviceStructTest ={0,0,0};
	uint64_t intNetDevice = strtol(str,NULL,16);// 000002// 11120013
	netDeviceStructTest.type = (intNetDevice >>24)&0xff;//
	netDeviceStructTest.endpoint = (intNetDevice >>16)&0xff;
	netDeviceStructTest.address = intNetDevice&0xffff;
	emberAfCorePrintln("\ngetDeviceStruct\ntype:%d endpoint:%d address:%d\n",
				netDeviceStructTest.type,
				netDeviceStructTest.endpoint,
				netDeviceStructTest.address);
	return netDeviceStructTest;
}
/*
	uint16_t type;
	uint16_t endpoint;
	uint32_t address;
*/
//0001DBF7
char* createDeviceJson(netDeviceStruct D)
{
	char str[100];
	uint64_t type =(uint64_t)D.type;
	uint64_t endpoint =(uint64_t)D.endpoint;
	uint64_t address    =(uint64_t)D.address;
	uint64_t value = (type<<24)+ (endpoint<<16)+(address);
	emberAfCorePrintln("%d",value);
	sprintf(str,"%08X",value);
	str[8] = '\0';
	//emberAfCorePrintln("\nstr:%s\nstr2:%s\nstr3:%s\n",str,str2,str3);
	char *string =(char*) malloc(sizeof(char)* strlen(str));
	strcpy(string,str);
	emberAfCorePrintln("createDeviceJson---:\n%s\n",str);
	return string;
}

netResourceStruct getResourceStruct(char* str)
{
	emberAfCorePrintln("getResourceStruct:%s",str);
	netResourceStruct resourceStruct;
	uint64_t intRes=0;
	emberAfCorePrintln("intRes:%ld",intRes);
	intRes =strtoll(str,NULL,16);
	//emberAfCorePrintln("\n\nintRes:%lx--%ld\n",intRes,intRes); //110012013--4563476499
	resourceStruct.profile =(intRes>>32)&0xffff;
	resourceStruct.cluster =(intRes>>16)&0xffff;
	resourceStruct.attribute =(intRes)&0xffff;
	emberAfCorePrintln("\ngetResourceStruct\nprofile:%d  cluster:%d  attribute:%d\n",resourceStruct.profile,resourceStruct.cluster,resourceStruct.attribute);
	return resourceStruct;
}

char* createResourceJson(netResourceStruct R)
{
	char str[100];
	uint64_t profile =  0x0104;//(uint64_t)R.profile;
	uint64_t cluster =(uint64_t)R.cluster;
	uint64_t attr    =(uint64_t)R.attribute;
	uint64_t value = (profile<<32)+ (cluster<<16)+(attr);   // 0xff =16bit ffffffff
	//sprintf(str,"%0")
	emberAfCorePrintln("createResourceJson:%l",value);
	sprintf(str,"%04lX",profile);
	sprintf(str+4,"%04lX",cluster);
	sprintf(str+8,"%04lX",attr);
	str[12] ='\0';
	char *string =(char*) malloc(sizeof(char)*13);
	strcpy(string,str);
	return string;
}


void getEUI64(char* str,uint8_t* eui64)
{
//	uint64_t intEUI64 =strtol(str,NULL,16);
	//userEUI64 Mac;
//	for(int i=0;i<EUI64SIZE;i++)
//	{
//		eui64[i]= (intEUI64>>((EUI64SIZE-i-1)*8))&0xff;
//		//emberAfCorePrintln("MAC[%d]:%d",i,eui64[i]);
//	}
	char data[3];
	data[2] ='\0';
	for(int i=0;i<EUI64_SIZE;i++)
	{
		data[0] = str[i*2];  // 018
		data[1] = str[1+ i*2];
		eui64[i] = (uint8_t)strtol(data,NULL,16);
		emberAfCorePrintln("getEUI64[%d]:%x",i,eui64[i]);
	}
}

char * createEUI64Str(uint8_t* eui64)
{
	int p;
	char str[100];
	for(int i=0;i<EUI64_SIZE;i++)
	{
		sprintf(str+p,"%02X",eui64[i]);
		p+=2;
	}
	char *string =(char*) malloc(sizeof(char)* strlen(str));
	strcpy(string,str);
	return string;
}

void getInstallCode(char* str,uint8_t* installCode)
{
//	uint64_t intCode = strtol(str,NULL,16);
//	for(int i=0;i<INSTALLCODE_LENGTH;i++)
//	{
//		installCode[i] =(intCode>>((INSTALLCODE_LENGTH-i-1)*8))&0xff;
//	}

	char data[3];
	data[2] ='\0';
	for(int i=0;i<INSTALLCODE_LENGTH;i++)
	{
		data[0] = str[i*2];  // 018
		data[1] = str[1+ i*2];
		installCode[i] = (uint8_t)strtol(data,NULL,16);
		emberAfCorePrintln("installCode[%d]:%x",i,installCode[i]);
	}

}

char* createInstallcodeStr(uint8_t* installCode)
{
	int p;
	char str[100];
	for(int i=0;i<EUI64_SIZE;i++)
	{
		sprintf(str+p,"%02X",installCode[i]);
		p+=2;
	}
	char *string =(char*) malloc(sizeof(char)* strlen(str));
	strcpy(string,str);
	return string;
}
void emberAfCorePrintlnPacket(packet p)
{
	emberAfCorePrintln("cmd:%d\nstatusCode:%d\nstatusMessage:%sMAC:%s\nlinkKey:%s\nnetDevice:%s\ntimeOut:%d\ncontent:%s\nnetReadingsCount:%d\n",p.cmd,p.statusCode,p.statusMessage,p.MAC,p.linkKey,p.netDevice,p.timeOut,p.content,p.netReadingsCount);
}
/*
typedef struct  NetReading
{
	uint64_t netOrigin;
	char* 	 netResource;
	uint8_t  netValueType;
	char* 	 netValue;
} netReading;
*/

char* createNetReadingsJson(netReading* netR, uint8_t count)
{
	char *returnNULL ="";
	if(count ==0)
	{
		return returnNULL;
	}
	char str[100];
	uint32_t lengthR=0;
	emberAfCorePrintln("\ncount:%d\n",count);
	for(int i=0;i<count;i++)
	{
		//netReading rTest ={0,"netResource",0,"netValue"};
		if(i==count-1)
		{
			sprintf(str+lengthR,"{\"r\":\"%s\",\"v\":\"%s\"}",netR[i].netResource,netR[i].netValue);
		}
		else
		{
			sprintf(str+lengthR,"{\"r\":\"%s\",\"v\":\"%s\"},",netR[i].netResource,netR[i].netValue);
		}
		lengthR+=strlen(netR[i].netResource)+strlen(netR[i].netValue) + 16;
	}
	char *string =(char*) malloc(sizeof(char)* strlen(str));
	strcpy(string,str);
	emberAfCorePrintln("createNetReadingsJson:%s",string);
	return string;
}
/*
	uint8_t cmd;
	uint8_t statusCode;
	char* statusMessage;
	char* MAC;
	char* linkKey;
	char* netDevice;    //   -> int --> <<  // maybe call create netDeviceJson
	uint8_t timeOut;
	char* content;
	uint8_t netReadingsCount;
	//cJSON* netReadings;  // json array// or convert to struct  netReadings struct NetReading
	netReading *netReadings;  call create netReadingJson
*/
char* getRespon(packet p)
{
	char str[100];
	sprintf(str,"{\"cmd\":%d,\"dev\":\"%s\",\"sc\":%d,\"sm\":\"%s\"}",p.cmd,p.netDevice,p.statusCode,"statusMessage");

	 char *string =(char*) malloc(sizeof(char)* strlen(str));
	 strcpy(string,str);
	 emberAfCorePrintln("prepare---packet:\n%s\n",string);
	 return string;
}

char* putRespon(packet p)
{
	char str[100];// cmd mac //lk // dev//
	//sprintf(str,"{\"cmd\":%d,\\"mac\":\"%s\",\"sc\":%d,\"sm\":\"%s\"}",p.cmd,p.netDevice,p.statusCode,"statusMessage");
	sprintf(str,"{\"cmd\":%d,\"sc\":%d,\"dev\":\"%s\",\"sm\":\"%s\"}",p.cmd,p.statusCode,p.netDevice,"put command respond");
	 char *string =(char*) malloc(sizeof(char)* strlen(str));
	 strcpy(string,str);
	 emberAfCorePrintln("prepare---packet:\n%s\n",string);
	 return string;
}

char* addRespon(packet p)
{
	char str[100];// cmd mac //lk // dev//
	//sprintf(str,"{\"cmd\":%d,\\"mac\":\"%s\",\"sc\":%d,\"sm\":\"%s\"}",p.cmd,p.netDevice,p.statusCode,"statusMessage");
	sprintf(str,"{\"cmd\":%d,\"sc\":%d,\"dev\":\"%s\",\"sm\":\"%s\"}",p.cmd,p.statusCode,p.netDevice,"add device respond");
	 char *string =(char*) malloc(sizeof(char)* strlen(str));
	 strcpy(string,str);
	 emberAfCorePrintln("prepare---packet:\n%s\n",string);
	 return string;
}

char* createRespondPacket(packet p)
{
	char str[500];
	char* netReadStr;
	netReadStr =createNetReadingsJson(p.netReadings,p.netReadingsCount);// co cap phat vung nho, phai free
	emberAfCorePrintln("netReadStr:%s",netReadStr);
	sprintf(str,"{\"cmd\":%d,\"sc\":%d,\"sm\":\"%s\",\"mac\":\"%s\",\"lk\":\"%s\",\"dev\":\"%s\",\"evt\":[%s],\"to\":%d,\"ct\":\"%s\"}",
								p.cmd,
								p.statusCode,
								p.statusMessage,
								p.MAC,
								p.linkKey,
								p.netDevice,
								netReadStr,                //p.netReadings,   // net repair
								p.timeOut,
								p.content);

	 char *string =(char*) malloc(sizeof(char)* strlen(str));
	 strcpy(string,str);
	 emberAfCorePrintln("prepare---packet:\n%s\n",string);
	 return string;
}

void freePacket(packet p)
{
	if(p.MAC!=NULL)
	{
		free(p.MAC);
	}
	if(p.statusMessage!=NULL)
	{
		free(p.statusMessage);
	}
	if(p.linkKey!=NULL)
	{
		free(p.linkKey);
	}
	if(p.netDevice!=NULL)
	{
		free(p.netDevice);
	}
	if(p.content!=NULL)
	{
		free(p.content);
	}
	for(int i=0;i<p.netReadingsCount;i++)
	{
		if(p.netReadings[i].netResource!=NULL)
		{
			free(p.netReadings[i].netResource);
		}
		if(p.netReadings[i].netValue!=NULL)
		{
			free(p.netReadings[i].netValue);
		}
	}
}

