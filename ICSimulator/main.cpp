/*
 * ICSimulator.cpp
 *
 * Created: 18/10/2020 3:16:54 PM
 * Author : Yarpo
 */ 

#include <avr/io.h>
#include <avr/eeprom.h>
#include "EEPROMInternal.h"
#include "CTWISlave.h"


EEPROMInternal<256,1024> eeprom;

int main(void)
{
	eeprom.SetOPAddress(0x0200);
	for (uint16_t i=0;i<=0xFF;i++)
	{
		eeprom.Push(i);
	}
	eeprom.SetReady();
    while (1) 
    {
		if(eeprom.IsReadyToWrite())
		{
			eeprom.StartWrite();
		}
    }
}

