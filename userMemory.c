

/*
 * userMemory.c
 *
 *  Created on: Dec 13, 2019
 *      Author: who_you_are
 */
#include "userMemory.h"
#include "em_msc.h"

#include "em_emu.h"
#include "stdlib.h"


data_flash readSchRam(uint32_t add )
{
	data_flash data ={0,0};
	for(uint8_t i=0;i<FLASHMAX;i++)
	{
		 data_flash read = ramSchData[i];
		 emberAfCorePrintln("--read--x:%d--y:%d",read.x,read.y);
		if(ramSchData[i].x==add)
		{
			return ramSchData[i];
		}
		else if(ramSchData[i].x ==-1)
		{
			return data;
		}
	}
	return data;
}
void writeData(data_flash dataIn)
{
		for(uint8_t i=0;i<FLASHMAX;i++)
		{
			data_flash data = *((data_flash*)(USERDATA + 2+i*sizeof(data_flash)/4));
			if(data.x==-1)
			{
				 MSC_Init();
				 MSC_WriteWord(((uint32_t*)USERDATA_BASE + 2+ i*sizeof(data_flash)/4),&dataIn,sizeof(data_flash));
				 MSC_Deinit();
				 flashToRam();
				 return;
			}
			if(i == (FLASHMAX -1))
			{
				flashToRam();
				MSC_ErasePage((uint32_t*)USERDATA_BASE );
				//ramToFlash();
				writeData(data);
			}
		}
}
void deleteData(uint32_t add)
{
	data_flash delete={0,0};
	for(uint8_t i=0;i<FLASHMAX;i++)
	{
		data_flash data =*((data_flash*)(USERDATA + 2+i*sizeof(data_flash)/4));
		if(data.x ==add)
		{
			 MSC_Init();
			 MSC_WriteWord(((uint32_t*)USERDATA_BASE + 2+ i*sizeof(data_flash)/4),&delete,sizeof(data_flash));
			 MSC_Deinit();

			 // cap nhat  lai bang gtri
			 flashToRam();
			 return;
		}
		else if(data.x ==-1)
		{
			return;
		}
	}
}
void flashToRam()
{
	uint8_t count =0;
	data_flash init ={-1,0};
	for(uint8_t i=0;i<FLASHMAX;i++)
	{
		data_flash data =*((data_flash*)(USERDATA + 2+i*sizeof(data_flash)/4));
		if(data.x != 0 && data.x!= -1)
		{
			ramSchData[count] = data;
			count++;
		}
		else if(data.x == -1)
		{
			ramSchData[count] = init;
			return;
		}
	}
}
void ramToFlash()
{
	   //Clear the Userdata page of any previous data stored
	MSC_ErasePage((uint32_t*)USERDATA_BASE );
	for(uint8_t i =0;i<FLASHMAX;i++)
	{
		if(ramSchData[i].x!=0)
		{
			MSC_Init();
			MSC_WriteWord(((uint32_t*)(USERDATA_BASE + 2+i*sizeof(data_flash)/4)),&ramSchData[i],sizeof(data_flash));
			MSC_Deinit();
		}
		else
		{
			return;
		}
	}
}
