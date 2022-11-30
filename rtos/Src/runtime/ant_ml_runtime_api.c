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
 * \file ant_ml_runtime_api.c
 * \brief The runtime API for the TVM generated C code.
 */

// LINT_C_FILE

#include <runtime/ant_ml_runtime_api.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =======================================================
//                    ant_ml_network_t
// =======================================================

typedef struct {
  ant_ml_model_info* info;
  ant_ml_tensor** inputs;
  ant_ml_tensor** outputs;
  ant_ml_ptr activations;
  const char* error;
} ant_ml_network_t;

//
// .nn_models_info section
//
extern uintptr_t __models_section_start__;
extern uintptr_t __models_section_end__;

uint32_t _modelsSection_start = (uint32_t)(&__models_section_start__);
uint32_t _modelsSection_end = (uint32_t)(&__models_section_end__);

// =======================================================
//                       Iterator
// =======================================================
ant_ml_model_iterator ant_ml_model_iterator_begin() {
  return _modelsSection_start;  // begin()
}

ant_ml_model_iterator ant_ml_model_iterator_end() { return _modelsSection_end; }

ant_ml_model_iterator ant_ml_model_iterator_next(ant_ml_model_iterator idx) {
  return (idx + sizeof(ant_ml_model_info));
}

ant_ml_model_info* ant_ml_model_iterator_value(ant_ml_model_iterator idx) { return (ant_ml_model_info*)idx; }

// =======================================================
//   ant_ml_create
// =======================================================
ANT_ML_API_ENTRY ant_ml_status ant_ml_create(ant_ml_model_info* nn, ant_ml_ptr activations, ant_ml_handle* handle) {
  uint32_t n_inputs = ANT_ML_MODEL_n_inputs(nn);
  uint32_t n_outputs = ANT_ML_MODEL_n_outputs(nn);

  ant_ml_status status = ANT_ML_STATUS_OK;

  //
  // Create internal network representation
  //
  ant_ml_network_t* network = (ant_ml_network_t*)malloc(sizeof(ant_ml_network_t));

  network->info = nn;

  for (int i = 0; i < n_inputs; i++) {
    network->inputs = ANT_ML_MODEL_inputs(nn);
  }
  for (int i = 0; i < n_outputs; i++) {
    network->outputs = ANT_ML_MODEL_outputs(nn);
  }

  network->activations = activations;

  network->error = NULL;

  const ant_ml_ptr params = nn->ant_ml_get_params();
  status = nn->ant_ml_create(params, activations);
  if (status != ANT_ML_STATUS_OK) {
    network->error = TVMGetLastError();
  }

  //
  // Setup weights and activations
  //
  *handle = network;

  return status;
}

// =======================================================
//   ant_ml_destroy
// =======================================================
ANT_ML_API_ENTRY ant_ml_status ant_ml_destroy(ant_ml_handle handle) {
  if (handle == NULL) {
    return ANT_ML_STATUS_ERROR;
  }

  ant_ml_network_t* network = (ant_ml_network_t*)handle;

  free(network);

  return ANT_ML_STATUS_OK;
}

// =======================================================
//   ant_ml_get_error
// =======================================================
ANT_ML_API_ENTRY
const char* ant_ml_get_error(ant_ml_handle handle) {
  if (handle == NULL) {
    return "Network handle is NULL";
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  if (network->error == NULL) {
    return "";
  }
  return network->error;
}

// =======================================================
//   ant_ml_get_input_size
// =======================================================
ANT_ML_API_ENTRY int32_t ant_ml_get_input_size(ant_ml_handle handle) {
  if (handle == NULL) {
    return 0;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return ANT_ML_MODEL_n_inputs(network->info);
}

// =======================================================
//   ant_ml_get_output_size
// =======================================================
ANT_ML_API_ENTRY int32_t ant_ml_get_output_size(ant_ml_handle handle) {
  if (handle == NULL) {
    return 0;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return ANT_ML_MODEL_n_outputs(network->info);
}

// =======================================================
//   ant_ml_get_input
// =======================================================
ANT_ML_API_ENTRY ant_ml_tensor* ant_ml_get_input(ant_ml_handle handle, int32_t index) {
  if (handle == NULL) {
    return NULL;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  if (index >= ANT_ML_MODEL_n_inputs(network->info)) {
    network->error = "Input index out of range";
    return NULL;
  }
  return (network->inputs)[index];
}

// =======================================================
//   ant_ml_get_output
// =======================================================
ANT_ML_API_ENTRY ant_ml_tensor* ant_ml_get_output(ant_ml_handle handle, int32_t index) {
  if (handle == NULL) {
    return NULL;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  if (index >= ANT_ML_MODEL_n_outputs(network->info)) {
    network->error = "Output index out of range";
    return NULL;
  }
  return (network->outputs)[index];
}

// =======================================================
//   ant_ml_run
// =======================================================
ANT_ML_API_ENTRY ant_ml_status ant_ml_run(ant_ml_handle handle) {
  if (handle == NULL) {
    return ANT_ML_STATUS_ERROR;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;

  ant_ml_model_info* nn = network->info;

  uint32_t n_inputs = ANT_ML_MODEL_n_inputs(nn);
  uint32_t n_outputs = ANT_ML_MODEL_n_outputs(nn);
  ant_ml_status status = ANT_ML_STATUS_OK;

  //
  // Check that input tensors have been specified
  //
  uint32_t i;
  for (i = 0; i < n_inputs; i++) {
    ant_ml_tensor* input_tensor = network->inputs[i];
    DLTensor* input = &input_tensor->dltensor;
    if (input->data == NULL) {
      network->error = "Network input NULL";
      return ANT_ML_STATUS_ERROR;
    }
  }
  for (i = 0; i < n_outputs; i++) {
    ant_ml_tensor* output_tensor = network->outputs[i];
    DLTensor* output = &output_tensor->dltensor;
    if (output->data == NULL) {
      network->error = "Network output NULL";
      return ANT_ML_STATUS_ERROR;
    }
  }

  status = nn->ant_ml_run(network->inputs, network->outputs);

  if (status != ANT_ML_STATUS_OK) {
    const char* err = TVMGetLastError();
    network->error = err;
  }

  return status;
}

// =======================================================
//   ant_ml_get_name
// =======================================================
const char* ant_ml_get_name(ant_ml_handle handle) {
  if (handle == NULL) {
    return NULL;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return ANT_ML_MODEL_name(network->info);
}

// =======================================================
//   ant_ml_get_datetime
// =======================================================
const char* ant_ml_get_datetime(ant_ml_handle handle) {
  if (handle == NULL) {
    return NULL;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return ANT_ML_MODEL_datetime(network->info);
}

// =======================================================
//   ant_ml_get_revision
// =======================================================
const char* ant_ml_get_revision(ant_ml_handle handle) {
  if (handle == NULL) {
    return NULL;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return ANT_ML_MODEL_revision(network->info);
}

// =======================================================
//   ant_ml_get_tool_version
// =======================================================
const char* ant_ml_get_tool_version(ant_ml_handle handle) {
  if (handle == NULL) {
    return NULL;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return ANT_ML_MODEL_tool_version(network->info);
}

// =======================================================
//   ant_ml_get_api_version
// =======================================================
const char* ant_ml_get_api_version(ant_ml_handle handle) {
  if (handle == NULL) {
    return NULL;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return ANT_ML_MODEL_api_version(network->info);
}

// =======================================================
//   ant_ml_get_node_size
// =======================================================
uint32_t ant_ml_get_node_size(ant_ml_handle handle) {
  if (handle == NULL) {
    return 0;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return ANT_ML_MODEL_n_nodes(network->info);
}

// =======================================================
//   ant_ml_get_activations_size
// =======================================================
uint32_t ant_ml_get_activations_size(ant_ml_handle handle) {
  if (handle == NULL) {
    return 0;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return ANT_ML_MODEL_activations_size(network->info);
}

// =======================================================
//   ant_ml_get_params_size
// =======================================================
uint32_t ant_ml_get_params_size(ant_ml_handle handle) {
  if (handle == NULL) {
    return 0;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return ANT_ML_MODEL_params_size(network->info);
}

// =======================================================
//   ant_ml_get_activations
// =======================================================
ant_ml_ptr ant_ml_get_activations(ant_ml_handle handle) {
  if (handle == NULL) {
    return 0;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return network->activations;
}

// =======================================================
//   ant_ml_get_params
// =======================================================
const ant_ml_ptr ant_ml_get_params(ant_ml_handle handle) {
  if (handle == NULL) {
    return NULL;
  }
  ant_ml_network_t* network = (ant_ml_network_t*)handle;
  return network->info->ant_ml_get_params();
}

// =======================================================
//   ant_ml_get_quantization
// =======================================================
const ant_ml_quantization_info* ant_ml_get_quantization(ant_ml_tensor* tensor) {
  if (tensor == NULL) {
    return NULL;
  }
  return tensor->quant;
}
