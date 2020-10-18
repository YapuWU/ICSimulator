/*
 * EEPROMInternal.h
 *
 * Created: 18/10/2020 3:37:36 PM
 *  Author: Yarpo
 */ 


#ifndef EEPROMINTERNAL_H_
#define EEPROMINTERNAL_H_
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "EEPROMBase.h"

template<
int iBufferSize = 128,
int iSize = 1024
>
class EEPROMInternal
{
	public:
	EEPROMInternal()
	{
		m_iLen = 0;
		m_IsReady = false;
	}
	//Start to write the data received from a master
	virtual void StartWrite()
	{
		for(uint32_t i=0;i<m_iLen;i++)
		{
			eeprom_write_byte((uint8_t*)(m_iOPAddress++),m_Buffer[i]);
		}
		m_iLen = 0;
		m_IsReady = 0;
	}
	
	//read a data from given address
	virtual uint8_t Read(uint32_t iAddr)
	{
		return 0;
	}
	
	//push a data to buffer,return 0 is there is only one place left, otherwise return 1
	uint8_t Push(uint8_t data)
	{
		m_Buffer[m_iLen] = data;
		m_iLen++;
		return (iBufferSize - m_iLen)==1 ? 0 : 1;
	}
	//setup the address of next operation
	void SetOPAddress(uint32_t iAddr)
	{
		m_iOPAddress = iAddr;
	}
	
	uint8_t IsReadyToWrite(){return m_IsReady;}
	void SetReady()
	{
		m_IsReady = 1;
	}
	protected:
	uint8_t m_Buffer[iBufferSize];
	uint16_t m_iOPAddress;
	uint16_t m_iLen;
	volatile uint8_t m_IsReady;
	private:
	
};


#endif /* EEPROMINTERNAL_H_ */