#include "driver_common.h"

#define ACC_X 0
#define ACC_Y 1
#define ACC_Z 2

void ACC_start(void *data) {
  // printf("ACC sensor is started\n");
}

void ACC_stop(void *data) {
  // printf("ACC sensor is terminated\n");
}

char *ACC_get(void *data) {
  int i;
  int val_x, val_y, val_z;
  double b;
  float a;
  static char value[20];

  val_x = 0;
  val_y = 0;
  val_z = 0;

  for (i = 0; i < 10; i++) {
    val_x += ADC_Read(ACC_X);
    val_y += ADC_Read(ACC_Y);
    val_z += ADC_Read(ACC_Z);
  }

  val_x = val_x / 10;
  val_y = val_y / 10;
  val_z = val_z / 10;

  delay(300);
  b = (double)(abs(val_x - 320)) / (abs(val_z - 320));

  a = atan(b);
  a = a / 3.14 * 180;

  sprintf(value, "%d %d %d\n", val_x, val_y, val_z);

  return value;
}