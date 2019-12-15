#include "ant_native_ml.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>

void ant_ml_getMaxOfBuffer_internal_uint8(const unsigned char *data_array,
                                          size_t data_array_len,
                                          int *result_max_index,
                                          unsigned char *result_value) {
  unsigned char max_value = 0;
  int max_index = -1;

  int i;
  for (i = 0; i < (int)data_array_len; i++) {
    unsigned char item = data_array[i];
    if (item > max_value) {
      max_value = item;
      max_index = i;
    }
  }
  *result_max_index = max_index;
  *result_value = max_value;
}

void ant_ml_getMaxOfBuffer_internal_int32(const long *data_array,
                                          size_t data_array_len,
                                          int *result_max_index,
                                          long *result_value) {
  long max_value = 0;
  int max_index = -1;

  int i;
  for (i = 0; i < (int)data_array_len; i++) {
    long item = data_array[i];
    if (item > max_value) {
      max_value = item;
      max_index = i;
    }
  }
  *result_max_index = max_index;
  *result_value = max_value;
}

void ant_ml_getMaxOfBuffer_internal_float32(const float *data_array,
                                            size_t data_array_len,
                                            int *result_max_index,
                                            float *result_value) {
  float max_value = 0;
  int max_index = -1;

  int i;
  for (i = 0; i < (int)data_array_len; i++) {
    float item = data_array[i];
    if (item > max_value) {
      max_value = item;
      max_index = i;
    }
  }
  *result_max_index = max_index;
  *result_value = max_value;
}

void initANTML(void) {
  // Empty function
}