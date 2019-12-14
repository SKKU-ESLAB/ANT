#ifndef __ANT_NATIVE_ML_H__
#define __ANT_NATIVE_ML_H__

#include <sys/types.h>

void ant_ml_getMaxOfBuffer_internal_uint8(const unsigned char *data_array,
                                          size_t data_array_len,
                                          int *result_max_index,
                                          unsigned char *result_value);
void ant_ml_getMaxOfBuffer_internal_int32(const long *data_array,
                                          size_t data_array_len,
                                          int *result_max_index,
                                          long *result_value);
void ant_ml_getMaxOfBuffer_internal_float32(const float *data_array,
                                            size_t data_array_len,
                                            int *result_max_index,
                                            float *result_value);

void initANTML(void);

#endif /* !defined(__ANT_NATIVE_ML_H__) */