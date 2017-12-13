#include <stdio.h>
#include <wiringPi.h>

#define LED 5 // BCM_GPIO 24

void LED_start(void *data)
{
	printf("LED sensor is started\n");
}

void LED_stop(void *data)
{
	printf("LED sensor is terminated\n");
}

char* LED_set(void *data)
{
  if(wiringPiSetup() == -1)
    return 1;

  pinMode(LED, OUTPUT);

  digitalWrite(LED, 1);

  return 0;
}


