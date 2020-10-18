/*
 * EEPROMBase.h
 *
 * Created: 18/10/2020 3:21:33 PM
 *  Author: Yarpo
 */ 


#ifndef EEPROMBASE_H_
#define EEPROMBASE_H_
template<
int iBufferSize = 128,
int iSize = 1024
>
class EEPROMBase
{
public:
	EEPROMBase()
	{
		m_iLen = 0;
		m_IsReady = false;
	}
	//Start to write the data received from a master
	virtual void StartWrite()=0;
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
	//read a data from given address
	virtual uint8_t Read(uint32_t iAddr) = 0;
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



#endif /* EEPROMBASE_H_ */