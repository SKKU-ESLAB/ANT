/* Copyright (c) 2017-2021 SKKU ESLAB, and contributors. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include <python3.6m/Python.h>

#include "../../../common/native/ant_common.h"
#include "./ant_ml_dfe_internal.h"

void interpreters_destroy(void *handle) {
  // TODO: python-object intrepreters destroy (ref-count decrease)
  // oc_endpoint_t *endpoint = (oc_endpoint_t *)handle;
  // free(endpoint);
}

void *ant_ml_dfeLoad_internal(const char *modelName, int numFragments) {
  // TODO: return python-object interpreters
  PyObject *pName = PyUnicode_FromString("antml_dfe");
  PyObject *pModule = PyImport_Import(pName);
  Py_DECREF(pName);

  if (pModule) {
    PyObject *pFunc = PyObject_GetAttrString(pModule, "dfe_load");
    if (pFunc && PyCallable_Check(pFunc)) {
      PyObject *pValue = PyObject_CallObject(pFunc, NULL);
      printf("C: run_model() = %ld\n", PyLong_AsLong(pValue));
      Py_DECREF(pValue);
    } else {
      printf("ERROR: function getInteger()\n");
    }
    Py_DECREF(pFunc);
  } else {
    printf("ERROR: Module not imported\n");
  }
  Py_DECREF(pModule);

  return 0;
}

void *ant_ml_dfeExecute_internal(void *interpreters, void *inputTensor,
                                 size_t inputTensorLength, int startLayerNum,
                                 int endLayerNum, size_t *pOutputTensorLength) {
  // TODO:
}

void initANTMLDFEInternal(void) {
  // Initialize Python interpreter
  Py_Initialize();
}