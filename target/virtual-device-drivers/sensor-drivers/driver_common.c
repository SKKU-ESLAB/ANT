#include "driver_common.h"
#include <stdio.h>
#include <time.h>

unsigned int read_sensor_value(void)
{
  srand(time(NULL));

  return rand() % 1000;
  
}
