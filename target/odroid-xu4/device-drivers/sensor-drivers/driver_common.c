#include "driver_common.h"
#include <wiringPi.h>
#include <wiringPiI2C.h>

#include <stdio.h>

#define Clock 27
#define Address 28
#define DataOut 29

int ADC_INIT = 0;
int ADC_LOCK = 0; 
int wiring_pi_setup;

int ADC_init(){
	if(setupWiringPi() < 0){
		printf("Error : Fail to init WiringPi\n");
		return 1;
	}

	pinMode(DataOut,INPUT);
	pullUpDnControl(DataOut, PUD_UP);

	pinMode(Clock, OUTPUT);
	pinMode(Address, OUTPUT);
	ADC_INIT = 1;
}

unsigned int ADC_Read(unsigned char channel)
{
	unsigned int value;
	unsigned char i;
	unsigned char LSB = 0, MSB = 0;

	if(!ADC_INIT)
		ADC_init();

	if(ADC_LOCK){
		while(ADC_LOCK){}
	}

	ADC_LOCK = 1;

	channel = channel << 4;
	for(i=0; i<4; i++)
	{
		if (channel & 0x80)
			digitalWrite(Address, 1);
		else
			digitalWrite(Address, 0);
		digitalWrite(Clock, 1);
		digitalWrite(Clock, 0);
		channel = channel << 1;
	}

	for(i=0; i<6; i++){
		digitalWrite(Clock, 1);
		digitalWrite(Clock, 0);
	}

	delayMicroseconds(15);

	for(i=0; i<2; i++)
	{
		digitalWrite(Clock, 1);
		MSB <<= 1;
		if(digitalRead(DataOut))
			MSB |= 0x1;
		digitalWrite(Clock, 0);
	}
	for(i=0; i<8; i++)
	{
		digitalWrite(Clock, 1);
		LSB <<= 1;
		if (digitalRead(DataOut))
			LSB |= 0x1;
		digitalWrite(Clock, 0);
	}

	value = MSB;
	value <<= 8;
	value |= LSB;

	ADC_LOCK = 0;
	return value;

}

int isWiringPiSetup(){
	return wiring_pi_setup;
}

int setupWiringPi()
{
	int res=0;
	if(!wiring_pi_setup){
		res = wiringPiSetup();
		wiring_pi_setup=1;
	}
	return res;
}


