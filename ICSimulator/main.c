/*
 * ICSimulator.c
 *
 * Created: 21/10/2020 8:13:07 PM
 * Author : Yarpo
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <avr/eeprom.h>
const uint8_t iBufLen = 32;
uint8_t buf[8];
volatile uint16_t iStartAddress = 0;
volatile uint8_t iAddressReady = 0;
volatile uint16_t iPos = 0;
volatile uint8_t iReady = 0;
void InitTWI(uint8_t iAddress)
{
	cli();
	TWAR = iAddress<<1;
	TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN | 1<<TWIE;
	sei();
}
ISR(TWI_vect)
{
	switch(TWSR & 0xFC)
	{
		case TW_SR_SLA_ACK: //the device is addressed
		iAddressReady = 0;
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN | 1<<TWIE;
		iPos = 0;
		break;
		case TW_SR_DATA_ACK: 
		if(iAddressReady)
		 buf[iPos++] = TWDR;
		else
		{
			iStartAddress = TWDR;
			iAddressReady = 1;
		}
		if((8-iPos)==1)
		{
			TWCR = 1<<TWINT | 1<<TWEN | 1<<TWIE;
		}
		else
		{
			TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN | 1<<TWIE;
		}
		break;
		case TW_SR_DATA_NACK:
		if(iAddressReady)
		buf[iPos++] = TWDR;
		else
		{
			iStartAddress = TWDR;
			iAddressReady = 1;
		}
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN | 1<<TWIE;
		break;
		case TW_SR_STOP:
		TWCR = 1<<TWINT | 1<<TWEN | 1<<TWIE;
		iReady = 1;
		break;
		
		//ST Mode
		case TW_ST_SLA_ACK:
		case TW_ST_DATA_ACK:
		TWDR = eeprom_read_byte((uint8_t*)iStartAddress);
		iStartAddress++;
		
		if(iStartAddress>=256)
		{
			iStartAddress = 0;
		}
		
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN | 1<<TWIE;
		break;
		default:
		TWCR = 1<<TWINT | 1<<TWEA | 1<<TWEN | 1<<TWIE;		
	}
}
int main(void)
{
	InitTWI(0x5C);
	sei();
    while (1) 
    {
		if(iReady)
		{
			for(uint16_t i=0;i<iPos;i++)
			{
				eeprom_write_byte((uint8_t*)(iStartAddress+i),buf[i]);
			}
			iStartAddress += iPos;
			iPos = 0;
			//Enable ACK
			InitTWI(0x5C);
			iReady = 0;
		}
    }
}

