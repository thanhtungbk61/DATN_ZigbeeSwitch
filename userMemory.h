
/*
 * userMemory.h
 *
 *  Created on: Dec 13, 2019
 *      Author: who_you_are
 */
#ifndef USERMEMORY_H_
#define USERMEMORY_H_

#include "stdint.h"
#include "app/framework/include/af.h"
#define USERDATA ((uint32_t*)USERDATA_BASE)
#define FLASHMAX 45
uint32_t  flashSchCount;  // = USERDATA[0]
uint32_t flashSchUsed;		// USERDATA[1]
uint32_t flashSubCount, flashSubUsed;  // 1024 - 1025
typedef struct
{
	uint32_t x;
	uint16_t y;
} data_flash;


data_flash ramSchData[48];
data_flash readSchRam(uint32_t add );

void writeCountUsed();
void writeData(data_flash data);
void deleteData(uint32_t add);
void flashToRam();
void ramToFlash();
#endif /* USERMEMORY_H_ */
