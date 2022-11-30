/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

/*!
 * \file ant_ml_runtime_api.h
 * \brief The runtime API for the TVM generated C code.
 */

#ifndef TVM_RUNTIME_CRT_CONTRIB_STM32_ANT_ML_RUNTIME_API_H_
#define TVM_RUNTIME_CRT_CONTRIB_STM32_ANT_ML_RUNTIME_API_H_

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include "dlpack/dlpack.h"              // From TVM
#include "tvm/runtime/c_runtime_api.h"  // From TVM

//
// This describes current ant_ml_runtime version
//
#define ANT_ML_PLATFORM_RUNTIME_MAJOR 1
#define ANT_ML_PLATFORM_RUNTIME_MINOR 0
#define ANT_ML_PLATFORM_RUNTIME_MICRO 0

#define ANT_ML_STATIC static

#if defined(_MSC_VER)
#define ANT_ML_INLINE __inline
#define ANT_ML_API_ENTRY __declspec(dllexport)
#define ANT_ML_ALIGNED(x) /* ANT_ML_ALIGNED(x) */
#elif defined(__ICCARM__) || defined(__IAR_SYSTEMS_ICC__)
#define ANT_ML_INLINE inline
#define ANT_ML_API_ENTRY /* ANT_ML_API_ENTRY */
#define ANT_ML_ALIGNED(x) ANT_ML_CONCAT(ANT_ML_ALIGNED_, x)
#elif defined(__CC_ARM)
#define ANT_ML_INLINE __inline
#define ANT_ML_API_ENTRY __attribute__((visibility("default")))
#define ANT_ML_ALIGNED(x) __attribute__((aligned(x)))
/* Keil disallows anonymous union initialization by default */
#pragma anon_unions
#elif defined(__GNUC__)
#define ANT_ML_INLINE __inline
#define ANT_ML_API_ENTRY __attribute__((visibility("default")))
#define ANT_ML_ALIGNED(x) __attribute__((aligned(x)))
#else
/* Dynamic libraries are not supported by the compiler */
#define ANT_ML_API_ENTRY  /* ANT_ML_API_ENTRY */
#define ANT_ML_ALIGNED(x) /* ANT_ML_ALIGNED(x) */
#endif

/*********************************************************/

typedef void* ant_ml_handle;

#define ANT_ML_HANDLE_PTR(ptr_) ((ant_ml_handle)(ptr_))
#define ANT_ML_HANDLE_NULL ANT_ML_HANDLE_PTR(NULL)

typedef uint8_t* ant_ml_ptr;

typedef enum { ANT_ML_STATUS_OK = 0, ANT_ML_STATUS_ERROR = 1, ANT_ML_STATUS_DELEGATE_ERROR = 2 } ant_ml_status;

// =======================================================
//                  ant_ml_quantization_info
//
//   Parameters for asymmetric quantization across a dimension (i.e
//   per output channel quantization).
//   quantized_dimension specifies which dimension the scales and
//   zero_points correspond to.
//   For a particular value in quantized_dimension, quantized values
//   can be converted back to float using:
//     real_value = scale * (quantized_value - zero_point)
// =======================================================

typedef struct {
  /*!
   * \brief The quantization info, if quantized
   */
  float* scale;
  int32_t* zero_point;
  int32_t dim;
} ant_ml_quantization_info;

// =======================================================
//                       ant_ml_tensor
// =======================================================

typedef struct {
  /*!
   * \brief The TVM tensor.
   */
  DLTensor dltensor;
  /*!
   * \brief The quantization info, if quantized
   */
  ant_ml_quantization_info* quant;
} ant_ml_tensor;

// =======================================================
//   get_dltensor
// =======================================================
ANT_ML_STATIC ANT_ML_INLINE DLTensor* get_dltensor(ant_ml_tensor* tensor) { return &tensor->dltensor; }

// =======================================================
//   get_tensor_elts
// =======================================================
ANT_ML_STATIC ANT_ML_INLINE uint32_t get_tensor_elts(const ant_ml_tensor* tensor) {
  const DLTensor* t = &tensor->dltensor;
  uint32_t elts = 1;
  for (int i = 0; i < t->ndim; ++i) {
    elts *= t->shape[i];
  }
  return elts;
}

// =======================================================
//   get_tensor_size
// =======================================================
ANT_ML_STATIC ANT_ML_INLINE uint32_t get_tensor_size(const ant_ml_tensor* tensor) {
  const DLTensor* t = &tensor->dltensor;
  uint32_t size = 1;
  for (int i = 0; i < t->ndim; ++i) {
    size *= t->shape[i];
  }
  size *= (t->dtype.bits * t->dtype.lanes + 7) / 8;
  return size;
}

// =======================================================
//                    ant_ml_network_info
// =======================================================

typedef struct {
  const char* name;
  const char* datetime;
  const char* revision;
  const char* tool_version;
  const char* api_version;
  uint16_t n_nodes;
  uint8_t n_inputs;
  uint8_t n_outputs;
  uint32_t activations_size;
  uint32_t params_size;
  ant_ml_ptr activations;
  ant_ml_tensor** inputs;
  ant_ml_tensor** outputs;
  const ant_ml_ptr (*ant_ml_get_params)(void);
  ant_ml_status (*ant_ml_create)(const ant_ml_ptr weights, const ant_ml_ptr activations);
  ant_ml_status (*ant_ml_destroy)();
  ant_ml_status (*ant_ml_run)(ant_ml_tensor* input[], ant_ml_tensor* output[]);
} ant_ml_model_info;

#define ANT_ML_MODEL_name(x) (x->name)
#define ANT_ML_MODEL_datetime(x) (x->datetime)
#define ANT_ML_MODEL_revision(x) (x->revision)
#define ANT_ML_MODEL_tool_version(x) (x->tool_version)
#define ANT_ML_MODEL_api_version(x) (x->api_version)
#define ANT_ML_MODEL_n_nodes(x) (x->n_nodes)
#define ANT_ML_MODEL_n_inputs(x) (x->n_inputs)
#define ANT_ML_MODEL_n_outputs(x) (x->n_outputs)
#define ANT_ML_MODEL_activations_size(x) (x->activations_size)
#define ANT_ML_MODEL_params_size(x) (x->params_size)
#define ANT_ML_MODEL_inputs(x) (x->inputs)
#define ANT_ML_MODEL_outputs(x) (x->outputs)
#define ANT_ML_MODEL_activations(x) (x->activations)

// =======================================================
//                         Iterator
//
//   Usage:
//
//     for (ant_ml_models_iterator it = ant_ml_models_iterator_begin();
//          it != ant_ml_models_iterator_end();
//          it = ant_ml_models_iterator_next(it)) {
//       const char * name = ant_ml_models_iterator_value(it);
//     }
//
// =======================================================

typedef uint32_t ant_ml_model_iterator;

ant_ml_model_iterator ant_ml_model_iterator_begin();
ant_ml_model_iterator ant_ml_model_iterator_next(ant_ml_model_iterator it);
ant_ml_model_iterator ant_ml_model_iterator_end();
ant_ml_model_info* ant_ml_model_iterator_value(ant_ml_model_iterator it);

// =======================================================
//                    External Interface
// =======================================================

ant_ml_status ant_ml_create(ant_ml_model_info* nn, ant_ml_ptr activations, ant_ml_handle* handle);

ant_ml_status ant_ml_destroy(ant_ml_handle handle);

const char* ant_ml_get_error(ant_ml_handle handle);

int32_t ant_ml_get_input_size(ant_ml_handle handle);

int32_t ant_ml_get_output_size(ant_ml_handle handle);

ant_ml_tensor* ant_ml_get_input(ant_ml_handle handle, int32_t index);

ant_ml_tensor* ant_ml_get_output(ant_ml_handle handle, int32_t index);

ant_ml_status ant_ml_run(ant_ml_handle handle);

//
// Additional methods
//
const char* ant_ml_get_name(ant_ml_handle handle);
const char* ant_ml_get_datetime(ant_ml_handle handle);
const char* ant_ml_get_revision(ant_ml_handle handle);
const char* ant_ml_get_tool_version(ant_ml_handle handle);
const char* ant_ml_get_api_version(ant_ml_handle handle);

uint32_t ant_ml_get_node_size(ant_ml_handle handle);
uint32_t ant_ml_get_activations_size(ant_ml_handle handle);
uint32_t ant_ml_get_params_size(ant_ml_handle handle);

ant_ml_ptr ant_ml_get_activations(ant_ml_handle handle);
const ant_ml_ptr ant_ml_get_params(ant_ml_handle handle);

//
// Quantization
//
const ant_ml_quantization_info* ant_ml_get_quantization(ant_ml_tensor* tensor);

#endif  // TVM_RUNTIME_CRT_CONTRIB_STM32_ANT_ML_RUNTIME_API_H_
