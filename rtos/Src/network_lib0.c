#include <stdio.h>
#include <math.h>

// tvm target: c -keys=arm_cpu,cpu -device=arm_cpu -link-params=0
#define TVM_EXPORTS
#include "tvm/runtime/c_runtime_api.h"
#include "tvm/runtime/c_backend_api.h"
#include <math.h>
#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_T_subtract = (((TVMValue*)args)[2].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[2];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 2500; ++ax0_ax1_fused_ax2_fused) {
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused] = (((int16_t)((int8_t*)placeholder)[ax0_ax1_fused_ax2_fused]) - ((int16_t*)placeholder1)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_1(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_T_subtract = (((TVMValue*)args)[2].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[2];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 169; ++ax0_ax1_fused_ax2_fused) {
    for (int32_t ax3_outer = 0; ax3_outer < 3; ++ax3_outer) {
      for (int32_t ax3_inner = 0; ax3_inner < 8; ++ax3_inner) {
        int32_t cse_var_1 = (((ax0_ax1_fused_ax2_fused * 24) + (ax3_outer * 8)) + ax3_inner);
        ((int16_t*)T_subtract)[cse_var_1] = (((int16_t)((int8_t*)placeholder)[cse_var_1]) - ((int16_t*)placeholder1)[0]);
      }
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_2(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_T_subtract = (((TVMValue*)args)[2].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[2];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 49; ++ax0_ax1_fused_ax2_fused) {
    for (int32_t ax3_outer = 0; ax3_outer < 4; ++ax3_outer) {
      for (int32_t ax3_inner = 0; ax3_inner < 8; ++ax3_inner) {
        if (((ax3_outer * 2) + (ax3_inner >> 2)) < 7) {
          int32_t cse_var_1 = (((ax0_ax1_fused_ax2_fused * 28) + (ax3_outer * 8)) + ax3_inner);
          ((int16_t*)T_subtract)[cse_var_1] = (((int16_t)((int8_t*)placeholder)[cse_var_1]) - ((int16_t*)placeholder1)[0]);
        }
      }
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_3(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_T_subtract = (((TVMValue*)args)[2].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[2];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 16; ++ax0_ax1_fused_ax2_fused) {
    for (int32_t ax3_outer = 0; ax3_outer < 3; ++ax3_outer) {
      for (int32_t ax3_inner = 0; ax3_inner < 8; ++ax3_inner) {
        int32_t cse_var_1 = (((ax0_ax1_fused_ax2_fused * 24) + (ax3_outer * 8)) + ax3_inner);
        ((int16_t*)T_subtract)[cse_var_1] = (((int16_t)((int8_t*)placeholder)[cse_var_1]) - ((int16_t*)placeholder1)[0]);
      }
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_4(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_T_subtract = (((TVMValue*)args)[2].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[2];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 16; ++ax0_ax1_fused_ax2_fused) {
    for (int32_t ax3_outer = 0; ax3_outer < 2; ++ax3_outer) {
      for (int32_t ax3_inner = 0; ax3_inner < 8; ++ax3_inner) {
        int32_t cse_var_1 = (((ax0_ax1_fused_ax2_fused * 16) + (ax3_outer * 8)) + ax3_inner);
        ((int16_t*)T_subtract)[cse_var_1] = (((int16_t)((int8_t*)placeholder)[cse_var_1]) - ((int16_t*)placeholder1)[0]);
      }
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_5(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_T_subtract = (((TVMValue*)args)[2].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[2];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 16; ++ax0_ax1_fused_ax2_fused) {
    for (int32_t ax3_outer = 0; ax3_outer < 4; ++ax3_outer) {
      for (int32_t ax3_inner = 0; ax3_inner < 8; ++ax3_inner) {
        if (((ax3_outer * 2) + (ax3_inner >> 2)) < 7) {
          int32_t cse_var_1 = (((ax0_ax1_fused_ax2_fused * 28) + (ax3_outer * 8)) + ax3_inner);
          ((int16_t*)T_subtract)[cse_var_1] = (((int16_t)((int8_t*)placeholder)[cse_var_1]) - ((int16_t*)placeholder1)[0]);
        }
      }
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_6(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_T_subtract = (((TVMValue*)args)[2].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[2];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 4; ++ax0_ax1_fused_ax2_fused) {
    for (int32_t ax3_outer = 0; ax3_outer < 2; ++ax3_outer) {
      for (int32_t ax3_inner = 0; ax3_inner < 8; ++ax3_inner) {
        int32_t cse_var_1 = (((ax0_ax1_fused_ax2_fused * 16) + (ax3_outer * 8)) + ax3_inner);
        ((int16_t*)T_subtract)[cse_var_1] = (((int16_t)((int8_t*)placeholder)[cse_var_1]) - ((int16_t*)placeholder1)[0]);
      }
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9_(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_placeholder8 = (((TVMValue*)args)[8].v_handle);
  int32_t arg_placeholder_code8 = arg_type_ids[8];
  void* arg_placeholder9 = (((TVMValue*)args)[9].v_handle);
  int32_t arg_placeholder_code9 = arg_type_ids[9];
  void* arg_T_cast = (((TVMValue*)args)[10].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[10];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* placeholder8 = (((DLTensor*)arg_placeholder8)[0].data);
  void* arg_placeholder_shape8 = (((DLTensor*)arg_placeholder8)[0].shape);
  void* arg_placeholder_strides8 = (((DLTensor*)arg_placeholder8)[0].strides);
  void* placeholder9 = (((DLTensor*)arg_placeholder9)[0].data);
  void* arg_placeholder_shape9 = (((DLTensor*)arg_placeholder9)[0].shape);
  void* arg_placeholder_strides9 = (((DLTensor*)arg_placeholder9)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_placeholder_strides6 == NULL)) {
  }
  if (!(arg_placeholder_strides7 == NULL)) {
  }
  if (!(arg_placeholder_strides9 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)3584, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 112; ++i3) {
        int32_t cse_var_1 = (((i1 * 448) + (i2 * 112)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder2)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[112];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)5376, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[24];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 112; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 448) + (ax2_outer * 112)) + ic)];
      }
      for (int32_t oco = 0; oco < 24; ++oco) {
        int32_t cse_var_2 = (oco * 112);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder3)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder3)[(oco + 24)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder3)[(oco + 48)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder3)[(oco + 72)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder3)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder3)[(oco + 120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder3)[(oco + 144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder3)[(oco + 168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder3)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder3)[(oco + 216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder3)[(oco + 240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder3)[(oco + 264)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder3)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder3)[(oco + 312)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder3)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder3)[(oco + 360)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder3)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder3)[(oco + 408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder3)[(oco + 432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder3)[(oco + 456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder3)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder3)[(oco + 504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder3)[(oco + 528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder3)[(oco + 552)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder3)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder3)[(oco + 600)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder3)[(oco + 624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder3)[(oco + 648)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder3)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder3)[(oco + 696)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder3)[(oco + 720)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder3)[(oco + 744)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder3)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder3)[(oco + 792)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder3)[(oco + 816)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder3)[(oco + 840)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder3)[(oco + 864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder3)[(oco + 888)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder3)[(oco + 912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder3)[(oco + 936)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder3)[(oco + 960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder3)[(oco + 984)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder3)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder3)[(oco + 1032)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder3)[(oco + 1056)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder3)[(oco + 1080)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder3)[(oco + 1104)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder3)[(oco + 1128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder3)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder3)[(oco + 1176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder3)[(oco + 1200)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder3)[(oco + 1224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder3)[(oco + 1248)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder3)[(oco + 1272)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder3)[(oco + 1296)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder3)[(oco + 1320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 56)] = ((int16_t*)placeholder3)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 57)] = ((int16_t*)placeholder3)[(oco + 1368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 58)] = ((int16_t*)placeholder3)[(oco + 1392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 59)] = ((int16_t*)placeholder3)[(oco + 1416)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 60)] = ((int16_t*)placeholder3)[(oco + 1440)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 61)] = ((int16_t*)placeholder3)[(oco + 1464)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 62)] = ((int16_t*)placeholder3)[(oco + 1488)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 63)] = ((int16_t*)placeholder3)[(oco + 1512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 64)] = ((int16_t*)placeholder3)[(oco + 1536)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 65)] = ((int16_t*)placeholder3)[(oco + 1560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 66)] = ((int16_t*)placeholder3)[(oco + 1584)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 67)] = ((int16_t*)placeholder3)[(oco + 1608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 68)] = ((int16_t*)placeholder3)[(oco + 1632)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 69)] = ((int16_t*)placeholder3)[(oco + 1656)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 70)] = ((int16_t*)placeholder3)[(oco + 1680)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 71)] = ((int16_t*)placeholder3)[(oco + 1704)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 72)] = ((int16_t*)placeholder3)[(oco + 1728)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 73)] = ((int16_t*)placeholder3)[(oco + 1752)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 74)] = ((int16_t*)placeholder3)[(oco + 1776)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 75)] = ((int16_t*)placeholder3)[(oco + 1800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 76)] = ((int16_t*)placeholder3)[(oco + 1824)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 77)] = ((int16_t*)placeholder3)[(oco + 1848)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 78)] = ((int16_t*)placeholder3)[(oco + 1872)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 79)] = ((int16_t*)placeholder3)[(oco + 1896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 80)] = ((int16_t*)placeholder3)[(oco + 1920)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 81)] = ((int16_t*)placeholder3)[(oco + 1944)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 82)] = ((int16_t*)placeholder3)[(oco + 1968)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 83)] = ((int16_t*)placeholder3)[(oco + 1992)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 84)] = ((int16_t*)placeholder3)[(oco + 2016)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 85)] = ((int16_t*)placeholder3)[(oco + 2040)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 86)] = ((int16_t*)placeholder3)[(oco + 2064)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 87)] = ((int16_t*)placeholder3)[(oco + 2088)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 88)] = ((int16_t*)placeholder3)[(oco + 2112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 89)] = ((int16_t*)placeholder3)[(oco + 2136)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 90)] = ((int16_t*)placeholder3)[(oco + 2160)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 91)] = ((int16_t*)placeholder3)[(oco + 2184)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 92)] = ((int16_t*)placeholder3)[(oco + 2208)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 93)] = ((int16_t*)placeholder3)[(oco + 2232)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 94)] = ((int16_t*)placeholder3)[(oco + 2256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 95)] = ((int16_t*)placeholder3)[(oco + 2280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 96)] = ((int16_t*)placeholder3)[(oco + 2304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 97)] = ((int16_t*)placeholder3)[(oco + 2328)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 98)] = ((int16_t*)placeholder3)[(oco + 2352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 99)] = ((int16_t*)placeholder3)[(oco + 2376)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 100)] = ((int16_t*)placeholder3)[(oco + 2400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 101)] = ((int16_t*)placeholder3)[(oco + 2424)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 102)] = ((int16_t*)placeholder3)[(oco + 2448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 103)] = ((int16_t*)placeholder3)[(oco + 2472)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 104)] = ((int16_t*)placeholder3)[(oco + 2496)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 105)] = ((int16_t*)placeholder3)[(oco + 2520)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 106)] = ((int16_t*)placeholder3)[(oco + 2544)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 107)] = ((int16_t*)placeholder3)[(oco + 2568)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 108)] = ((int16_t*)placeholder3)[(oco + 2592)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 109)] = ((int16_t*)placeholder3)[(oco + 2616)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 110)] = ((int16_t*)placeholder3)[(oco + 2640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 111)] = ((int16_t*)placeholder3)[(oco + 2664)];
      }
      for (int32_t oco1 = 0; oco1 < 24; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 112; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 112) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 24; ++ax3_outer) {
        int32_t cse_var_3 = (((ax0_ax1_outer_fused * 96) + (ax2_outer * 24)) + ax3_outer);
        int32_t _1 = ((int32_t*)placeholder8)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder4)[ax3_outer])) * ((int64_t*)placeholder5)[ax3_outer]) + ((int64_t*)placeholder6)[ax3_outer]) >> ((int64_t*)placeholder7)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int32_t _3 = (((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0]))) * (int64_t)1877850864) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31)))) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder9)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder9)[0]))) * (int64_t)2070884007) + ((int64_t)1 << ((int64_t)((1 + 31) - 1)))) >> ((int64_t)(1 + 31))))) - 10;
        int32_t _4 = (_3) < (127) ? (_3) : (127);
        ((int8_t*)T_cast)[cse_var_3] = ((int8_t)((_4) > (-128) ? (_4) : (-128)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_placeholder8 = (((TVMValue*)args)[8].v_handle);
  int32_t arg_placeholder_code8 = arg_type_ids[8];
  void* arg_placeholder9 = (((TVMValue*)args)[9].v_handle);
  int32_t arg_placeholder_code9 = arg_type_ids[9];
  void* arg_T_cast = (((TVMValue*)args)[10].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[10];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* placeholder8 = (((DLTensor*)arg_placeholder8)[0].data);
  void* arg_placeholder_shape8 = (((DLTensor*)arg_placeholder8)[0].shape);
  void* arg_placeholder_strides8 = (((DLTensor*)arg_placeholder8)[0].strides);
  void* placeholder9 = (((DLTensor*)arg_placeholder9)[0].data);
  void* arg_placeholder_shape9 = (((DLTensor*)arg_placeholder9)[0].shape);
  void* arg_placeholder_strides9 = (((DLTensor*)arg_placeholder9)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_placeholder_strides6 == NULL)) {
  }
  if (!(arg_placeholder_strides7 == NULL)) {
  }
  if (!(arg_placeholder_strides9 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)1792, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 56; ++i3) {
        int32_t cse_var_1 = (((i1 * 224) + (i2 * 56)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder2)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[56];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)3136, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[28];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 56; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 224) + (ax2_outer * 56)) + ic)];
      }
      for (int32_t oco = 0; oco < 28; ++oco) {
        int32_t cse_var_2 = (oco * 56);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder3)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder3)[(oco + 28)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder3)[(oco + 56)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder3)[(oco + 84)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder3)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder3)[(oco + 140)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder3)[(oco + 168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder3)[(oco + 196)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder3)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder3)[(oco + 252)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder3)[(oco + 280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder3)[(oco + 308)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder3)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder3)[(oco + 364)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder3)[(oco + 392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder3)[(oco + 420)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder3)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder3)[(oco + 476)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder3)[(oco + 504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder3)[(oco + 532)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder3)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder3)[(oco + 588)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder3)[(oco + 616)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder3)[(oco + 644)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder3)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder3)[(oco + 700)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder3)[(oco + 728)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder3)[(oco + 756)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder3)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder3)[(oco + 812)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder3)[(oco + 840)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder3)[(oco + 868)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder3)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder3)[(oco + 924)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder3)[(oco + 952)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder3)[(oco + 980)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder3)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder3)[(oco + 1036)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder3)[(oco + 1064)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder3)[(oco + 1092)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder3)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder3)[(oco + 1148)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder3)[(oco + 1176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder3)[(oco + 1204)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder3)[(oco + 1232)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder3)[(oco + 1260)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder3)[(oco + 1288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder3)[(oco + 1316)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder3)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder3)[(oco + 1372)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder3)[(oco + 1400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder3)[(oco + 1428)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder3)[(oco + 1456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder3)[(oco + 1484)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder3)[(oco + 1512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder3)[(oco + 1540)];
      }
      for (int32_t oco1 = 0; oco1 < 28; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 56; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 56) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 28; ++ax3_outer) {
        int32_t cse_var_3 = (((ax0_ax1_outer_fused * 112) + (ax2_outer * 28)) + ax3_outer);
        int32_t _1 = ((int32_t*)placeholder8)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder4)[ax3_outer])) * ((int64_t*)placeholder5)[ax3_outer]) + ((int64_t*)placeholder6)[ax3_outer]) >> ((int64_t*)placeholder7)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int32_t _3 = (((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0]))) * (int64_t)1761101078) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31)))) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder9)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder9)[0]))) * (int64_t)2006769919) + ((int64_t)1 << ((int64_t)((1 + 31) - 1)))) >> ((int64_t)(1 + 31))))) - 5;
        int32_t _4 = (_3) < (127) ? (_3) : (127);
        ((int8_t*)T_cast)[cse_var_3] = ((int8_t)((_4) > (-128) ? (_4) : (-128)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_placeholder8 = (((TVMValue*)args)[8].v_handle);
  int32_t arg_placeholder_code8 = arg_type_ids[8];
  void* arg_placeholder9 = (((TVMValue*)args)[9].v_handle);
  int32_t arg_placeholder_code9 = arg_type_ids[9];
  void* arg_placeholder10 = (((TVMValue*)args)[10].v_handle);
  int32_t arg_placeholder_code10 = arg_type_ids[10];
  void* arg_T_cast = (((TVMValue*)args)[11].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[11];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* placeholder8 = (((DLTensor*)arg_placeholder8)[0].data);
  void* arg_placeholder_shape8 = (((DLTensor*)arg_placeholder8)[0].shape);
  void* arg_placeholder_strides8 = (((DLTensor*)arg_placeholder8)[0].strides);
  void* placeholder9 = (((DLTensor*)arg_placeholder9)[0].data);
  void* arg_placeholder_shape9 = (((DLTensor*)arg_placeholder9)[0].shape);
  void* arg_placeholder_strides9 = (((DLTensor*)arg_placeholder9)[0].strides);
  void* placeholder10 = (((DLTensor*)arg_placeholder10)[0].data);
  void* arg_placeholder_shape10 = (((DLTensor*)arg_placeholder10)[0].shape);
  void* arg_placeholder_strides10 = (((DLTensor*)arg_placeholder10)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_placeholder_strides6 == NULL)) {
  }
  if (!(arg_placeholder_strides7 == NULL)) {
  }
  if (!(arg_placeholder_strides8 == NULL)) {
  }
  if (!(arg_placeholder_strides10 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  int16_t PadInput[384];
  for (int32_t i1 = 0; i1 < 2; ++i1) {
    for (int32_t i2 = 0; i2 < 2; ++i2) {
      for (int32_t i3 = 0; i3 < 96; ++i3) {
        int32_t cse_var_1 = (((i1 * 192) + (i2 * 96)) + i3);
        PadInput[cse_var_1] = ((int16_t*)placeholder3)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 2; ++ax0_ax1_outer_fused) {
    int16_t data_vec[96];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)3072, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[16];
    for (int32_t ax2_outer = 0; ax2_outer < 2; ++ax2_outer) {
      for (int32_t ic = 0; ic < 96; ++ic) {
        data_vec[ic] = PadInput[(((ax0_ax1_outer_fused * 192) + (ax2_outer * 96)) + ic)];
      }
      for (int32_t oco = 0; oco < 16; ++oco) {
        int32_t cse_var_2 = (oco * 96);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder4)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder4)[(oco + 16)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder4)[(oco + 32)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder4)[(oco + 48)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder4)[(oco + 64)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder4)[(oco + 80)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder4)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder4)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder4)[(oco + 128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder4)[(oco + 144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder4)[(oco + 160)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder4)[(oco + 176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder4)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder4)[(oco + 208)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder4)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder4)[(oco + 240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder4)[(oco + 256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder4)[(oco + 272)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder4)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder4)[(oco + 304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder4)[(oco + 320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder4)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder4)[(oco + 352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder4)[(oco + 368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder4)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder4)[(oco + 400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder4)[(oco + 416)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder4)[(oco + 432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder4)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder4)[(oco + 464)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder4)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder4)[(oco + 496)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder4)[(oco + 512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder4)[(oco + 528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder4)[(oco + 544)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder4)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder4)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder4)[(oco + 592)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder4)[(oco + 608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder4)[(oco + 624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder4)[(oco + 640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder4)[(oco + 656)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder4)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder4)[(oco + 688)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder4)[(oco + 704)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder4)[(oco + 720)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder4)[(oco + 736)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder4)[(oco + 752)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder4)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder4)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder4)[(oco + 800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder4)[(oco + 816)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder4)[(oco + 832)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder4)[(oco + 848)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder4)[(oco + 864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder4)[(oco + 880)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 56)] = ((int16_t*)placeholder4)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 57)] = ((int16_t*)placeholder4)[(oco + 912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 58)] = ((int16_t*)placeholder4)[(oco + 928)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 59)] = ((int16_t*)placeholder4)[(oco + 944)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 60)] = ((int16_t*)placeholder4)[(oco + 960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 61)] = ((int16_t*)placeholder4)[(oco + 976)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 62)] = ((int16_t*)placeholder4)[(oco + 992)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 63)] = ((int16_t*)placeholder4)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 64)] = ((int16_t*)placeholder4)[(oco + 1024)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 65)] = ((int16_t*)placeholder4)[(oco + 1040)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 66)] = ((int16_t*)placeholder4)[(oco + 1056)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 67)] = ((int16_t*)placeholder4)[(oco + 1072)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 68)] = ((int16_t*)placeholder4)[(oco + 1088)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 69)] = ((int16_t*)placeholder4)[(oco + 1104)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 70)] = ((int16_t*)placeholder4)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 71)] = ((int16_t*)placeholder4)[(oco + 1136)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 72)] = ((int16_t*)placeholder4)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 73)] = ((int16_t*)placeholder4)[(oco + 1168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 74)] = ((int16_t*)placeholder4)[(oco + 1184)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 75)] = ((int16_t*)placeholder4)[(oco + 1200)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 76)] = ((int16_t*)placeholder4)[(oco + 1216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 77)] = ((int16_t*)placeholder4)[(oco + 1232)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 78)] = ((int16_t*)placeholder4)[(oco + 1248)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 79)] = ((int16_t*)placeholder4)[(oco + 1264)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 80)] = ((int16_t*)placeholder4)[(oco + 1280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 81)] = ((int16_t*)placeholder4)[(oco + 1296)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 82)] = ((int16_t*)placeholder4)[(oco + 1312)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 83)] = ((int16_t*)placeholder4)[(oco + 1328)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 84)] = ((int16_t*)placeholder4)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 85)] = ((int16_t*)placeholder4)[(oco + 1360)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 86)] = ((int16_t*)placeholder4)[(oco + 1376)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 87)] = ((int16_t*)placeholder4)[(oco + 1392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 88)] = ((int16_t*)placeholder4)[(oco + 1408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 89)] = ((int16_t*)placeholder4)[(oco + 1424)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 90)] = ((int16_t*)placeholder4)[(oco + 1440)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 91)] = ((int16_t*)placeholder4)[(oco + 1456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 92)] = ((int16_t*)placeholder4)[(oco + 1472)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 93)] = ((int16_t*)placeholder4)[(oco + 1488)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 94)] = ((int16_t*)placeholder4)[(oco + 1504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 95)] = ((int16_t*)placeholder4)[(oco + 1520)];
      }
      for (int32_t oco1 = 0; oco1 < 16; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 96; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 96) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 16; ++ax3_outer) {
        int32_t cse_var_3 = (((ax0_ax1_outer_fused * 32) + (ax2_outer * 16)) + ax3_outer);
        int32_t _1 = ((int32_t*)placeholder9)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder5)[ax3_outer])) * ((int64_t*)placeholder6)[ax3_outer]) + ((int64_t*)placeholder7)[ax3_outer]) >> ((int64_t*)placeholder8)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int32_t _3 = (((((int32_t*)placeholder2)[0] * 2) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0]))) * (int64_t)1798132199) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31))))) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder10)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder10)[0]))) * (int64_t)2029331409) + ((int64_t)1 << ((int64_t)((1 + 31) - 1)))) >> ((int64_t)(1 + 31))))) + 13;
        int32_t _4 = (_3) < (127) ? (_3) : (127);
        ((int8_t*)T_cast)[cse_var_3] = ((int8_t)((_4) > (-128) ? (_4) : (-128)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4_(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_placeholder8 = (((TVMValue*)args)[8].v_handle);
  int32_t arg_placeholder_code8 = arg_type_ids[8];
  void* arg_placeholder9 = (((TVMValue*)args)[9].v_handle);
  int32_t arg_placeholder_code9 = arg_type_ids[9];
  void* arg_placeholder10 = (((TVMValue*)args)[10].v_handle);
  int32_t arg_placeholder_code10 = arg_type_ids[10];
  void* arg_T_subtract = (((TVMValue*)args)[11].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[11];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* placeholder8 = (((DLTensor*)arg_placeholder8)[0].data);
  void* arg_placeholder_shape8 = (((DLTensor*)arg_placeholder8)[0].shape);
  void* arg_placeholder_strides8 = (((DLTensor*)arg_placeholder8)[0].strides);
  void* placeholder9 = (((DLTensor*)arg_placeholder9)[0].data);
  void* arg_placeholder_shape9 = (((DLTensor*)arg_placeholder9)[0].shape);
  void* arg_placeholder_strides9 = (((DLTensor*)arg_placeholder9)[0].strides);
  void* placeholder10 = (((DLTensor*)arg_placeholder10)[0].data);
  void* arg_placeholder_shape10 = (((DLTensor*)arg_placeholder10)[0].shape);
  void* arg_placeholder_strides10 = (((DLTensor*)arg_placeholder10)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_placeholder_strides6 == NULL)) {
  }
  if (!(arg_placeholder_strides7 == NULL)) {
  }
  if (!(arg_placeholder_strides9 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)14896, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 7; ++i1) {
    for (int32_t i2 = 0; i2 < 7; ++i2) {
      for (int32_t i3 = 0; i3 < 152; ++i3) {
        int32_t cse_var_1 = (((i1 * 1064) + (i2 * 152)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder2)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 7; ++ax0_ax1_outer_fused) {
    int16_t data_vec[152];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)8512, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[28];
    for (int32_t ax2_outer = 0; ax2_outer < 7; ++ax2_outer) {
      for (int32_t ic = 0; ic < 152; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 1064) + (ax2_outer * 152)) + ic)];
      }
      for (int32_t oco = 0; oco < 28; ++oco) {
        int32_t cse_var_2 = (oco * 152);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder3)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder3)[(oco + 28)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder3)[(oco + 56)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder3)[(oco + 84)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder3)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder3)[(oco + 140)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder3)[(oco + 168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder3)[(oco + 196)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder3)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder3)[(oco + 252)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder3)[(oco + 280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder3)[(oco + 308)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder3)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder3)[(oco + 364)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder3)[(oco + 392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder3)[(oco + 420)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder3)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder3)[(oco + 476)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder3)[(oco + 504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder3)[(oco + 532)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder3)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder3)[(oco + 588)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder3)[(oco + 616)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder3)[(oco + 644)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder3)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder3)[(oco + 700)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder3)[(oco + 728)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder3)[(oco + 756)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder3)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder3)[(oco + 812)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder3)[(oco + 840)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder3)[(oco + 868)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder3)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder3)[(oco + 924)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder3)[(oco + 952)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder3)[(oco + 980)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder3)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder3)[(oco + 1036)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder3)[(oco + 1064)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder3)[(oco + 1092)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder3)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder3)[(oco + 1148)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder3)[(oco + 1176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder3)[(oco + 1204)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder3)[(oco + 1232)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder3)[(oco + 1260)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder3)[(oco + 1288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder3)[(oco + 1316)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder3)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder3)[(oco + 1372)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder3)[(oco + 1400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder3)[(oco + 1428)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder3)[(oco + 1456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder3)[(oco + 1484)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder3)[(oco + 1512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder3)[(oco + 1540)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 56)] = ((int16_t*)placeholder3)[(oco + 1568)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 57)] = ((int16_t*)placeholder3)[(oco + 1596)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 58)] = ((int16_t*)placeholder3)[(oco + 1624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 59)] = ((int16_t*)placeholder3)[(oco + 1652)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 60)] = ((int16_t*)placeholder3)[(oco + 1680)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 61)] = ((int16_t*)placeholder3)[(oco + 1708)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 62)] = ((int16_t*)placeholder3)[(oco + 1736)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 63)] = ((int16_t*)placeholder3)[(oco + 1764)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 64)] = ((int16_t*)placeholder3)[(oco + 1792)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 65)] = ((int16_t*)placeholder3)[(oco + 1820)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 66)] = ((int16_t*)placeholder3)[(oco + 1848)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 67)] = ((int16_t*)placeholder3)[(oco + 1876)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 68)] = ((int16_t*)placeholder3)[(oco + 1904)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 69)] = ((int16_t*)placeholder3)[(oco + 1932)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 70)] = ((int16_t*)placeholder3)[(oco + 1960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 71)] = ((int16_t*)placeholder3)[(oco + 1988)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 72)] = ((int16_t*)placeholder3)[(oco + 2016)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 73)] = ((int16_t*)placeholder3)[(oco + 2044)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 74)] = ((int16_t*)placeholder3)[(oco + 2072)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 75)] = ((int16_t*)placeholder3)[(oco + 2100)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 76)] = ((int16_t*)placeholder3)[(oco + 2128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 77)] = ((int16_t*)placeholder3)[(oco + 2156)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 78)] = ((int16_t*)placeholder3)[(oco + 2184)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 79)] = ((int16_t*)placeholder3)[(oco + 2212)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 80)] = ((int16_t*)placeholder3)[(oco + 2240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 81)] = ((int16_t*)placeholder3)[(oco + 2268)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 82)] = ((int16_t*)placeholder3)[(oco + 2296)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 83)] = ((int16_t*)placeholder3)[(oco + 2324)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 84)] = ((int16_t*)placeholder3)[(oco + 2352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 85)] = ((int16_t*)placeholder3)[(oco + 2380)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 86)] = ((int16_t*)placeholder3)[(oco + 2408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 87)] = ((int16_t*)placeholder3)[(oco + 2436)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 88)] = ((int16_t*)placeholder3)[(oco + 2464)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 89)] = ((int16_t*)placeholder3)[(oco + 2492)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 90)] = ((int16_t*)placeholder3)[(oco + 2520)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 91)] = ((int16_t*)placeholder3)[(oco + 2548)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 92)] = ((int16_t*)placeholder3)[(oco + 2576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 93)] = ((int16_t*)placeholder3)[(oco + 2604)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 94)] = ((int16_t*)placeholder3)[(oco + 2632)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 95)] = ((int16_t*)placeholder3)[(oco + 2660)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 96)] = ((int16_t*)placeholder3)[(oco + 2688)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 97)] = ((int16_t*)placeholder3)[(oco + 2716)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 98)] = ((int16_t*)placeholder3)[(oco + 2744)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 99)] = ((int16_t*)placeholder3)[(oco + 2772)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 100)] = ((int16_t*)placeholder3)[(oco + 2800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 101)] = ((int16_t*)placeholder3)[(oco + 2828)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 102)] = ((int16_t*)placeholder3)[(oco + 2856)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 103)] = ((int16_t*)placeholder3)[(oco + 2884)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 104)] = ((int16_t*)placeholder3)[(oco + 2912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 105)] = ((int16_t*)placeholder3)[(oco + 2940)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 106)] = ((int16_t*)placeholder3)[(oco + 2968)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 107)] = ((int16_t*)placeholder3)[(oco + 2996)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 108)] = ((int16_t*)placeholder3)[(oco + 3024)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 109)] = ((int16_t*)placeholder3)[(oco + 3052)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 110)] = ((int16_t*)placeholder3)[(oco + 3080)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 111)] = ((int16_t*)placeholder3)[(oco + 3108)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 112)] = ((int16_t*)placeholder3)[(oco + 3136)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 113)] = ((int16_t*)placeholder3)[(oco + 3164)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 114)] = ((int16_t*)placeholder3)[(oco + 3192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 115)] = ((int16_t*)placeholder3)[(oco + 3220)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 116)] = ((int16_t*)placeholder3)[(oco + 3248)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 117)] = ((int16_t*)placeholder3)[(oco + 3276)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 118)] = ((int16_t*)placeholder3)[(oco + 3304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 119)] = ((int16_t*)placeholder3)[(oco + 3332)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 120)] = ((int16_t*)placeholder3)[(oco + 3360)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 121)] = ((int16_t*)placeholder3)[(oco + 3388)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 122)] = ((int16_t*)placeholder3)[(oco + 3416)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 123)] = ((int16_t*)placeholder3)[(oco + 3444)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 124)] = ((int16_t*)placeholder3)[(oco + 3472)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 125)] = ((int16_t*)placeholder3)[(oco + 3500)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 126)] = ((int16_t*)placeholder3)[(oco + 3528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 127)] = ((int16_t*)placeholder3)[(oco + 3556)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 128)] = ((int16_t*)placeholder3)[(oco + 3584)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 129)] = ((int16_t*)placeholder3)[(oco + 3612)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 130)] = ((int16_t*)placeholder3)[(oco + 3640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 131)] = ((int16_t*)placeholder3)[(oco + 3668)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 132)] = ((int16_t*)placeholder3)[(oco + 3696)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 133)] = ((int16_t*)placeholder3)[(oco + 3724)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 134)] = ((int16_t*)placeholder3)[(oco + 3752)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 135)] = ((int16_t*)placeholder3)[(oco + 3780)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 136)] = ((int16_t*)placeholder3)[(oco + 3808)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 137)] = ((int16_t*)placeholder3)[(oco + 3836)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 138)] = ((int16_t*)placeholder3)[(oco + 3864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 139)] = ((int16_t*)placeholder3)[(oco + 3892)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 140)] = ((int16_t*)placeholder3)[(oco + 3920)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 141)] = ((int16_t*)placeholder3)[(oco + 3948)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 142)] = ((int16_t*)placeholder3)[(oco + 3976)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 143)] = ((int16_t*)placeholder3)[(oco + 4004)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 144)] = ((int16_t*)placeholder3)[(oco + 4032)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 145)] = ((int16_t*)placeholder3)[(oco + 4060)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 146)] = ((int16_t*)placeholder3)[(oco + 4088)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 147)] = ((int16_t*)placeholder3)[(oco + 4116)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 148)] = ((int16_t*)placeholder3)[(oco + 4144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 149)] = ((int16_t*)placeholder3)[(oco + 4172)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 150)] = ((int16_t*)placeholder3)[(oco + 4200)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 151)] = ((int16_t*)placeholder3)[(oco + 4228)];
      }
      for (int32_t oco1 = 0; oco1 < 28; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 152; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 152) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 28; ++ax3_outer) {
        int32_t cse_var_3 = (((ax0_ax1_outer_fused * 196) + (ax2_outer * 28)) + ax3_outer);
        int32_t _1 = ((int32_t*)placeholder8)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder4)[ax3_outer])) * ((int64_t*)placeholder5)[ax3_outer]) + ((int64_t*)placeholder6)[ax3_outer]) >> ((int64_t*)placeholder7)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int32_t _3 = (((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0]))) * (int64_t)1691437717) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31)))) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder9)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder9)[0]))) * (int64_t)1290719891) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31))))) - 7;
        int32_t _4 = (_3) < (127) ? (_3) : (127);
        ((int16_t*)T_subtract)[cse_var_3] = (((int16_t)((int8_t)((_4) > (-128) ? (_4) : (-128)))) - ((int16_t*)placeholder10)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_placeholder8 = (((TVMValue*)args)[8].v_handle);
  int32_t arg_placeholder_code8 = arg_type_ids[8];
  void* arg_placeholder9 = (((TVMValue*)args)[9].v_handle);
  int32_t arg_placeholder_code9 = arg_type_ids[9];
  void* arg_placeholder10 = (((TVMValue*)args)[10].v_handle);
  int32_t arg_placeholder_code10 = arg_type_ids[10];
  void* arg_T_subtract = (((TVMValue*)args)[11].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[11];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* placeholder8 = (((DLTensor*)arg_placeholder8)[0].data);
  void* arg_placeholder_shape8 = (((DLTensor*)arg_placeholder8)[0].shape);
  void* arg_placeholder_strides8 = (((DLTensor*)arg_placeholder8)[0].strides);
  void* placeholder9 = (((DLTensor*)arg_placeholder9)[0].data);
  void* arg_placeholder_shape9 = (((DLTensor*)arg_placeholder9)[0].shape);
  void* arg_placeholder_strides9 = (((DLTensor*)arg_placeholder9)[0].strides);
  void* placeholder10 = (((DLTensor*)arg_placeholder10)[0].data);
  void* arg_placeholder_shape10 = (((DLTensor*)arg_placeholder10)[0].shape);
  void* arg_placeholder_strides10 = (((DLTensor*)arg_placeholder10)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_placeholder_strides6 == NULL)) {
  }
  if (!(arg_placeholder_strides7 == NULL)) {
  }
  if (!(arg_placeholder_strides8 == NULL)) {
  }
  if (!(arg_placeholder_strides9 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)2432, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 76; ++i3) {
        int32_t cse_var_1 = (((i1 * 304) + (i2 * 76)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder3)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[76];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)4864, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[32];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 76; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 304) + (ax2_outer * 76)) + ic)];
      }
      for (int32_t oco = 0; oco < 32; ++oco) {
        int32_t cse_var_2 = (oco * 76);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder4)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder4)[(oco + 32)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder4)[(oco + 64)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder4)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder4)[(oco + 128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder4)[(oco + 160)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder4)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder4)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder4)[(oco + 256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder4)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder4)[(oco + 320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder4)[(oco + 352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder4)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder4)[(oco + 416)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder4)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder4)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder4)[(oco + 512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder4)[(oco + 544)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder4)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder4)[(oco + 608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder4)[(oco + 640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder4)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder4)[(oco + 704)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder4)[(oco + 736)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder4)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder4)[(oco + 800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder4)[(oco + 832)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder4)[(oco + 864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder4)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder4)[(oco + 928)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder4)[(oco + 960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder4)[(oco + 992)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder4)[(oco + 1024)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder4)[(oco + 1056)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder4)[(oco + 1088)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder4)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder4)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder4)[(oco + 1184)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder4)[(oco + 1216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder4)[(oco + 1248)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder4)[(oco + 1280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder4)[(oco + 1312)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder4)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder4)[(oco + 1376)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder4)[(oco + 1408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder4)[(oco + 1440)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder4)[(oco + 1472)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder4)[(oco + 1504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder4)[(oco + 1536)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder4)[(oco + 1568)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder4)[(oco + 1600)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder4)[(oco + 1632)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder4)[(oco + 1664)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder4)[(oco + 1696)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder4)[(oco + 1728)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder4)[(oco + 1760)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 56)] = ((int16_t*)placeholder4)[(oco + 1792)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 57)] = ((int16_t*)placeholder4)[(oco + 1824)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 58)] = ((int16_t*)placeholder4)[(oco + 1856)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 59)] = ((int16_t*)placeholder4)[(oco + 1888)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 60)] = ((int16_t*)placeholder4)[(oco + 1920)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 61)] = ((int16_t*)placeholder4)[(oco + 1952)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 62)] = ((int16_t*)placeholder4)[(oco + 1984)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 63)] = ((int16_t*)placeholder4)[(oco + 2016)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 64)] = ((int16_t*)placeholder4)[(oco + 2048)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 65)] = ((int16_t*)placeholder4)[(oco + 2080)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 66)] = ((int16_t*)placeholder4)[(oco + 2112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 67)] = ((int16_t*)placeholder4)[(oco + 2144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 68)] = ((int16_t*)placeholder4)[(oco + 2176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 69)] = ((int16_t*)placeholder4)[(oco + 2208)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 70)] = ((int16_t*)placeholder4)[(oco + 2240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 71)] = ((int16_t*)placeholder4)[(oco + 2272)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 72)] = ((int16_t*)placeholder4)[(oco + 2304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 73)] = ((int16_t*)placeholder4)[(oco + 2336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 74)] = ((int16_t*)placeholder4)[(oco + 2368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 75)] = ((int16_t*)placeholder4)[(oco + 2400)];
      }
      for (int32_t oco1 = 0; oco1 < 32; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 76; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 76) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 32; ++ax3_outer) {
        int32_t cse_var_3 = (((ax0_ax1_outer_fused * 128) + (ax2_outer * 32)) + ax3_outer);
        int32_t _1 = ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder5)[ax3_outer])) * ((int64_t*)placeholder6)[ax3_outer]) + ((int64_t*)placeholder7)[ax3_outer]) >> ((int64_t*)placeholder8)[ax3_outer])) - 30;
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int32_t _3 = (((((int32_t*)placeholder2)[0] * 2) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0]))) * (int64_t)2116706860) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31))))) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder9)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder9)[0]))) * (int64_t)1384854072) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31))))) + 9;
        int32_t _4 = (_3) < (127) ? (_3) : (127);
        ((int16_t*)T_subtract)[cse_var_3] = (((int16_t)((int8_t)((_4) > (-128) ? (_4) : (-128)))) - ((int16_t*)placeholder10)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_placeholder8 = (((TVMValue*)args)[8].v_handle);
  int32_t arg_placeholder_code8 = arg_type_ids[8];
  void* arg_placeholder9 = (((TVMValue*)args)[9].v_handle);
  int32_t arg_placeholder_code9 = arg_type_ids[9];
  void* arg_placeholder10 = (((TVMValue*)args)[10].v_handle);
  int32_t arg_placeholder_code10 = arg_type_ids[10];
  void* arg_T_subtract = (((TVMValue*)args)[11].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[11];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* placeholder8 = (((DLTensor*)arg_placeholder8)[0].data);
  void* arg_placeholder_shape8 = (((DLTensor*)arg_placeholder8)[0].shape);
  void* arg_placeholder_strides8 = (((DLTensor*)arg_placeholder8)[0].strides);
  void* placeholder9 = (((DLTensor*)arg_placeholder9)[0].data);
  void* arg_placeholder_shape9 = (((DLTensor*)arg_placeholder9)[0].shape);
  void* arg_placeholder_strides9 = (((DLTensor*)arg_placeholder9)[0].strides);
  void* placeholder10 = (((DLTensor*)arg_placeholder10)[0].data);
  void* arg_placeholder_shape10 = (((DLTensor*)arg_placeholder10)[0].shape);
  void* arg_placeholder_strides10 = (((DLTensor*)arg_placeholder10)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_placeholder_strides6 == NULL)) {
  }
  if (!(arg_placeholder_strides7 == NULL)) {
  }
  if (!(arg_placeholder_strides8 == NULL)) {
  }
  if (!(arg_placeholder_strides9 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)1792, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 56; ++i3) {
        int32_t cse_var_1 = (((i1 * 224) + (i2 * 56)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder3)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[56];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)3136, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[28];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 56; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 224) + (ax2_outer * 56)) + ic)];
      }
      for (int32_t oco = 0; oco < 28; ++oco) {
        int32_t cse_var_2 = (oco * 56);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder4)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder4)[(oco + 28)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder4)[(oco + 56)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder4)[(oco + 84)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder4)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder4)[(oco + 140)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder4)[(oco + 168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder4)[(oco + 196)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder4)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder4)[(oco + 252)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder4)[(oco + 280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder4)[(oco + 308)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder4)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder4)[(oco + 364)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder4)[(oco + 392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder4)[(oco + 420)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder4)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder4)[(oco + 476)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder4)[(oco + 504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder4)[(oco + 532)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder4)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder4)[(oco + 588)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder4)[(oco + 616)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder4)[(oco + 644)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder4)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder4)[(oco + 700)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder4)[(oco + 728)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder4)[(oco + 756)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder4)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder4)[(oco + 812)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder4)[(oco + 840)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder4)[(oco + 868)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder4)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder4)[(oco + 924)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder4)[(oco + 952)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder4)[(oco + 980)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder4)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder4)[(oco + 1036)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder4)[(oco + 1064)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder4)[(oco + 1092)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder4)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder4)[(oco + 1148)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder4)[(oco + 1176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder4)[(oco + 1204)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder4)[(oco + 1232)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder4)[(oco + 1260)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder4)[(oco + 1288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder4)[(oco + 1316)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder4)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder4)[(oco + 1372)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder4)[(oco + 1400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder4)[(oco + 1428)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder4)[(oco + 1456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder4)[(oco + 1484)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder4)[(oco + 1512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder4)[(oco + 1540)];
      }
      for (int32_t oco1 = 0; oco1 < 28; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 56; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 56) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 28; ++ax3_outer) {
        int32_t cse_var_3 = (((ax0_ax1_outer_fused * 112) + (ax2_outer * 28)) + ax3_outer);
        int32_t _1 = ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder5)[ax3_outer])) * ((int64_t*)placeholder6)[ax3_outer]) + ((int64_t*)placeholder7)[ax3_outer]) >> ((int64_t*)placeholder8)[ax3_outer])) + 18;
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int32_t _3 = (((((int32_t*)placeholder2)[0] * 2) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0]))) * (int64_t)1962263408) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31))))) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder9)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int32_t*)placeholder9)[0]))) * (int64_t)1719096268) + ((int64_t)1 << ((int64_t)((1 + 31) - 1)))) >> ((int64_t)(1 + 31))))) + 11;
        int32_t _4 = (_3) < (127) ? (_3) : (127);
        ((int16_t*)T_subtract)[cse_var_3] = (((int16_t)((int8_t)((_4) > (-128) ? (_4) : (-128)))) - ((int16_t*)placeholder10)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078_(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_placeholder8 = (((TVMValue*)args)[8].v_handle);
  int32_t arg_placeholder_code8 = arg_type_ids[8];
  void* arg_placeholder9 = (((TVMValue*)args)[9].v_handle);
  int32_t arg_placeholder_code9 = arg_type_ids[9];
  void* arg_T_subtract = (((TVMValue*)args)[10].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[10];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* placeholder8 = (((DLTensor*)arg_placeholder8)[0].data);
  void* arg_placeholder_shape8 = (((DLTensor*)arg_placeholder8)[0].shape);
  void* arg_placeholder_strides8 = (((DLTensor*)arg_placeholder8)[0].strides);
  void* placeholder9 = (((DLTensor*)arg_placeholder9)[0].data);
  void* arg_placeholder_shape9 = (((DLTensor*)arg_placeholder9)[0].shape);
  void* arg_placeholder_strides9 = (((DLTensor*)arg_placeholder9)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_placeholder_strides6 == NULL)) {
  }
  if (!(arg_placeholder_strides7 == NULL)) {
  }
  if (!(arg_placeholder_strides8 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int16_t PadInput[384];
  for (int32_t i1 = 0; i1 < 2; ++i1) {
    for (int32_t i2 = 0; i2 < 2; ++i2) {
      for (int32_t i3 = 0; i3 < 96; ++i3) {
        int32_t cse_var_1 = (((i1 * 192) + (i2 * 96)) + i3);
        PadInput[cse_var_1] = ((int16_t*)placeholder3)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 2; ++ax0_ax1_outer_fused) {
    int16_t data_vec[96];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)3072, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[16];
    for (int32_t ax2_outer = 0; ax2_outer < 2; ++ax2_outer) {
      for (int32_t ic = 0; ic < 96; ++ic) {
        data_vec[ic] = PadInput[(((ax0_ax1_outer_fused * 192) + (ax2_outer * 96)) + ic)];
      }
      for (int32_t oco = 0; oco < 16; ++oco) {
        int32_t cse_var_2 = (oco * 96);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder4)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder4)[(oco + 16)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder4)[(oco + 32)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder4)[(oco + 48)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder4)[(oco + 64)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder4)[(oco + 80)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder4)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder4)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder4)[(oco + 128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder4)[(oco + 144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder4)[(oco + 160)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder4)[(oco + 176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder4)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder4)[(oco + 208)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder4)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder4)[(oco + 240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder4)[(oco + 256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder4)[(oco + 272)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder4)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder4)[(oco + 304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder4)[(oco + 320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder4)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder4)[(oco + 352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder4)[(oco + 368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder4)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder4)[(oco + 400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder4)[(oco + 416)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder4)[(oco + 432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder4)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder4)[(oco + 464)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder4)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder4)[(oco + 496)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder4)[(oco + 512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder4)[(oco + 528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder4)[(oco + 544)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder4)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder4)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder4)[(oco + 592)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder4)[(oco + 608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder4)[(oco + 624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder4)[(oco + 640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder4)[(oco + 656)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder4)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder4)[(oco + 688)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder4)[(oco + 704)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder4)[(oco + 720)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder4)[(oco + 736)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder4)[(oco + 752)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder4)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder4)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder4)[(oco + 800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder4)[(oco + 816)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder4)[(oco + 832)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder4)[(oco + 848)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder4)[(oco + 864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder4)[(oco + 880)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 56)] = ((int16_t*)placeholder4)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 57)] = ((int16_t*)placeholder4)[(oco + 912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 58)] = ((int16_t*)placeholder4)[(oco + 928)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 59)] = ((int16_t*)placeholder4)[(oco + 944)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 60)] = ((int16_t*)placeholder4)[(oco + 960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 61)] = ((int16_t*)placeholder4)[(oco + 976)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 62)] = ((int16_t*)placeholder4)[(oco + 992)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 63)] = ((int16_t*)placeholder4)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 64)] = ((int16_t*)placeholder4)[(oco + 1024)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 65)] = ((int16_t*)placeholder4)[(oco + 1040)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 66)] = ((int16_t*)placeholder4)[(oco + 1056)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 67)] = ((int16_t*)placeholder4)[(oco + 1072)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 68)] = ((int16_t*)placeholder4)[(oco + 1088)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 69)] = ((int16_t*)placeholder4)[(oco + 1104)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 70)] = ((int16_t*)placeholder4)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 71)] = ((int16_t*)placeholder4)[(oco + 1136)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 72)] = ((int16_t*)placeholder4)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 73)] = ((int16_t*)placeholder4)[(oco + 1168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 74)] = ((int16_t*)placeholder4)[(oco + 1184)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 75)] = ((int16_t*)placeholder4)[(oco + 1200)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 76)] = ((int16_t*)placeholder4)[(oco + 1216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 77)] = ((int16_t*)placeholder4)[(oco + 1232)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 78)] = ((int16_t*)placeholder4)[(oco + 1248)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 79)] = ((int16_t*)placeholder4)[(oco + 1264)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 80)] = ((int16_t*)placeholder4)[(oco + 1280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 81)] = ((int16_t*)placeholder4)[(oco + 1296)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 82)] = ((int16_t*)placeholder4)[(oco + 1312)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 83)] = ((int16_t*)placeholder4)[(oco + 1328)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 84)] = ((int16_t*)placeholder4)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 85)] = ((int16_t*)placeholder4)[(oco + 1360)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 86)] = ((int16_t*)placeholder4)[(oco + 1376)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 87)] = ((int16_t*)placeholder4)[(oco + 1392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 88)] = ((int16_t*)placeholder4)[(oco + 1408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 89)] = ((int16_t*)placeholder4)[(oco + 1424)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 90)] = ((int16_t*)placeholder4)[(oco + 1440)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 91)] = ((int16_t*)placeholder4)[(oco + 1456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 92)] = ((int16_t*)placeholder4)[(oco + 1472)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 93)] = ((int16_t*)placeholder4)[(oco + 1488)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 94)] = ((int16_t*)placeholder4)[(oco + 1504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 95)] = ((int16_t*)placeholder4)[(oco + 1520)];
      }
      for (int32_t oco1 = 0; oco1 < 16; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 96; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 96) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 16; ++ax3_outer) {
        int32_t cse_var_3 = (((ax0_ax1_outer_fused * 32) + (ax2_outer * 16)) + ax3_outer);
        int32_t _1 = (int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder5)[ax3_outer])) * ((int64_t*)placeholder6)[ax3_outer]) + ((int64_t*)placeholder7)[ax3_outer]) >> ((int64_t*)placeholder8)[ax3_outer]);
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int32_t _3 = (((((int32_t*)placeholder2)[0] * 2) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_3]) - ((int32_t*)placeholder1)[0]))) * (int64_t)1573865243) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31))))) + ((int32_t)(((((0 != 0) ? (((int64_t)((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128))))) << ((int64_t)0)) : ((int64_t)((int32_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))))) * (int64_t)1665979088) + ((int64_t)1 << ((int64_t)((1 + 31) - 1)))) >> ((int64_t)(1 + 31))))) + 11;
        int32_t _4 = (_3) < (127) ? (_3) : (127);
        ((int16_t*)T_subtract)[cse_var_3] = (((int16_t)((int8_t)((_4) > (-128) ? (_4) : (-128)))) - ((int16_t*)placeholder9)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b_(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_T_cast = (((TVMValue*)args)[5].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[5];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 49; ++ax0_ax1_fused_ax2_fused) {
    for (int32_t ax3_outer = 0; ax3_outer < 2; ++ax3_outer) {
      for (int32_t ax3_inner = 0; ax3_inner < 16; ++ax3_inner) {
        if (((ax3_outer * 4) + (ax3_inner >> 2)) < 7) {
          int32_t cse_var_2 = (ax3_outer * 16);
          int32_t cse_var_1 = (((ax0_ax1_fused_ax2_fused * 28) + cse_var_2) + ax3_inner);
          int32_t _1 = (((((int32_t*)placeholder2)[0] * 2) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_1]) - ((int32_t*)placeholder1)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_1]) - ((int32_t*)placeholder1)[0]))) * (int64_t)2064753155) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31))))) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((ax3_outer < 1) ? ((int8_t*)placeholder3)[(((ax0_ax1_fused_ax2_fused * 16) + cse_var_2) + ax3_inner)] : (int8_t)-15)) - ((int32_t*)placeholder4)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((ax3_outer < 1) ? ((int8_t*)placeholder3)[(((ax0_ax1_fused_ax2_fused * 16) + cse_var_2) + ax3_inner)] : (int8_t)-15)) - ((int32_t*)placeholder4)[0]))) * (int64_t)1607949353) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31))))) + 11;
          int32_t _2 = (_1) < (127) ? (_1) : (127);
          ((int8_t*)T_cast)[cse_var_1] = ((int8_t)((_2) > (-128) ? (_2) : (-128)));
        }
      }
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_T_cast = (((TVMValue*)args)[5].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[5];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 16; ++ax0_ax1_fused_ax2_fused) {
    for (int32_t ax3_outer = 0; ax3_outer < 2; ++ax3_outer) {
      for (int32_t ax3_inner = 0; ax3_inner < 16; ++ax3_inner) {
        if (((ax3_outer * 2) + (ax3_inner >> 3)) < 3) {
          int32_t cse_var_2 = (ax3_outer * 16);
          int32_t cse_var_1 = (((ax0_ax1_fused_ax2_fused * 24) + cse_var_2) + ax3_inner);
          int32_t _1 = (((((int32_t*)placeholder2)[0] * 2) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_1]) - ((int32_t*)placeholder1)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_1]) - ((int32_t*)placeholder1)[0]))) * (int64_t)1713974428) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31))))) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((((ax3_outer * 4) + (ax3_inner >> 2)) < 3) ? ((int8_t*)placeholder3)[((cse_var_2 + (ax0_ax1_fused_ax2_fused * 12)) + ax3_inner)] : (int8_t)-6)) - ((int32_t*)placeholder4)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((((ax3_outer * 4) + (ax3_inner >> 2)) < 3) ? ((int8_t*)placeholder3)[((cse_var_2 + (ax0_ax1_fused_ax2_fused * 12)) + ax3_inner)] : (int8_t)-6)) - ((int32_t*)placeholder4)[0]))) * (int64_t)1256049877) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31))))) + 6;
          int32_t _2 = (_1) < (127) ? (_1) : (127);
          ((int8_t*)T_cast)[cse_var_1] = ((int8_t)((_2) > (-128) ? (_2) : (-128)));
        }
      }
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5_(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_T_subtract = (((TVMValue*)args)[5].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[5];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 169; ++ax0_ax1_fused_ax2_fused) {
    for (int32_t ax3_outer = 0; ax3_outer < 3; ++ax3_outer) {
      for (int32_t ax3_inner = 0; ax3_inner < 8; ++ax3_inner) {
        int32_t cse_var_2 = (ax3_outer * 8);
        int32_t cse_var_1 = (((ax0_ax1_fused_ax2_fused * 24) + cse_var_2) + ax3_inner);
        int32_t _1 = (((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_1]) - ((int32_t*)placeholder1)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((int8_t*)placeholder)[cse_var_1]) - ((int32_t*)placeholder1)[0]))) * (int64_t)2045428378) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31)))) + ((int32_t)(((((0 != 0) ? (((int64_t)(((int32_t)((((ax3_outer * 2) + (ax3_inner >> 2)) < 5) ? ((int8_t*)placeholder2)[(((ax0_ax1_fused_ax2_fused * 20) + cse_var_2) + ax3_inner)] : (int8_t)4)) - ((int32_t*)placeholder3)[0])) << ((int64_t)0)) : ((int64_t)(((int32_t)((((ax3_outer * 2) + (ax3_inner >> 2)) < 5) ? ((int8_t*)placeholder2)[(((ax0_ax1_fused_ax2_fused * 20) + cse_var_2) + ax3_inner)] : (int8_t)4)) - ((int32_t*)placeholder3)[0]))) * (int64_t)1142747040) + ((int64_t)1 << ((int64_t)((0 + 31) - 1)))) >> ((int64_t)(0 + 31))))) + 8;
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        ((int16_t*)T_subtract)[cse_var_1] = (((int16_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int16_t*)placeholder4)[0]);
      }
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_T_subtract = (((TVMValue*)args)[2].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[2];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t tensor[128];
  int8_t T_cast[128];
  for (int32_t ax3 = 0; ax3 < 128; ++ax3) {
    tensor[ax3] = 0;
    for (int32_t rv0 = 0; rv0 < 2; ++rv0) {
      for (int32_t rv1 = 0; rv1 < 2; ++rv1) {
        tensor[ax3] = (tensor[ax3] + ((int32_t*)placeholder)[(((rv0 * 256) + (rv1 * 128)) + ax3)]);
      }
    }
  }
  for (int32_t ax31 = 0; ax31 < 128; ++ax31) {
    tensor[ax31] = (tensor[ax31] / 4);
  }
  for (int32_t ax32 = 0; ax32 < 128; ++ax32) {
    T_cast[ax32] = ((int8_t)tensor[ax32]);
  }
  for (int32_t ax33 = 0; ax33 < 128; ++ax33) {
    ((int16_t*)tensor)[ax33] = ((int16_t)T_cast[ax33]);
  }
  for (int32_t ax34 = 0; ax34 < 128; ++ax34) {
    ((int16_t*)T_subtract)[ax34] = (((int16_t*)tensor)[ax34] - ((int16_t*)placeholder1)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_T_cast = (((TVMValue*)args)[7].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[7];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)9464, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 13; ++i1) {
    for (int32_t i2 = 0; i2 < 13; ++i2) {
      for (int32_t i3 = 0; i3 < 28; ++i3) {
        int32_t cse_var_1 = (((i1 * 364) + (i2 * 28)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 13; ++ax0_ax1_outer_fused) {
    int16_t data_vec[28];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)1344, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[24];
    for (int32_t ax2_outer = 0; ax2_outer < 13; ++ax2_outer) {
      for (int32_t ic = 0; ic < 28; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 364) + (ax2_outer * 28)) + ic)];
      }
      for (int32_t oco = 0; oco < 24; ++oco) {
        int32_t cse_var_2 = (oco * 28);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 24)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 48)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 72)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 264)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 312)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 360)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 552)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 600)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 648)];
      }
      for (int32_t oco1 = 0; oco1 < 24; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 28; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 28) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 24; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        ((int8_t*)T_cast)[(((ax0_ax1_outer_fused * 312) + (ax2_outer * 24)) + ax3_outer)] = ((int8_t)((_2) > (-128) ? (_2) : (-128)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_T_cast = (((TVMValue*)args)[7].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[7];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)9464, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 13; ++i1) {
    for (int32_t i2 = 0; i2 < 13; ++i2) {
      for (int32_t i3 = 0; i3 < 28; ++i3) {
        int32_t cse_var_1 = (((i1 * 364) + (i2 * 28)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 13; ++ax0_ax1_outer_fused) {
    int16_t data_vec[28];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)1120, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[20];
    for (int32_t ax2_outer = 0; ax2_outer < 13; ++ax2_outer) {
      for (int32_t ic = 0; ic < 28; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 364) + (ax2_outer * 28)) + ic)];
      }
      for (int32_t oco = 0; oco < 20; ++oco) {
        int32_t cse_var_2 = (oco * 28);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 20)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 40)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 60)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 80)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 100)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 140)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 160)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 180)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 200)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 220)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 260)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 300)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 340)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 360)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 380)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 420)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 440)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 460)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 500)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 520)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 540)];
      }
      for (int32_t oco1 = 0; oco1 < 20; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 28; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 28) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 20; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        ((int8_t*)T_cast)[(((ax0_ax1_outer_fused * 260) + (ax2_outer * 20)) + ax3_outer)] = ((int8_t)((_2) > (-128) ? (_2) : (-128)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_T_cast = (((TVMValue*)args)[7].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[7];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)12544, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 7; ++i1) {
    for (int32_t i2 = 0; i2 < 7; ++i2) {
      for (int32_t i3 = 0; i3 < 128; ++i3) {
        int32_t cse_var_1 = (((i1 * 896) + (i2 * 128)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 7; ++ax0_ax1_outer_fused) {
    int16_t data_vec[128];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)7168, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[28];
    for (int32_t ax2_outer = 0; ax2_outer < 7; ++ax2_outer) {
      for (int32_t ic = 0; ic < 128; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 896) + (ax2_outer * 128)) + ic)];
      }
      for (int32_t oco = 0; oco < 28; ++oco) {
        int32_t cse_var_2 = (oco * 128);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 28)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 56)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 84)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 140)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 196)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 252)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 308)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 364)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 420)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 476)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 532)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 588)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 616)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 644)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 700)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 728)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 756)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder1)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder1)[(oco + 812)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder1)[(oco + 840)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder1)[(oco + 868)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder1)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder1)[(oco + 924)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder1)[(oco + 952)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder1)[(oco + 980)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder1)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder1)[(oco + 1036)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder1)[(oco + 1064)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder1)[(oco + 1092)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder1)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder1)[(oco + 1148)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder1)[(oco + 1176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder1)[(oco + 1204)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder1)[(oco + 1232)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder1)[(oco + 1260)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder1)[(oco + 1288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder1)[(oco + 1316)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder1)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder1)[(oco + 1372)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder1)[(oco + 1400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder1)[(oco + 1428)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder1)[(oco + 1456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder1)[(oco + 1484)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder1)[(oco + 1512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder1)[(oco + 1540)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 56)] = ((int16_t*)placeholder1)[(oco + 1568)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 57)] = ((int16_t*)placeholder1)[(oco + 1596)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 58)] = ((int16_t*)placeholder1)[(oco + 1624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 59)] = ((int16_t*)placeholder1)[(oco + 1652)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 60)] = ((int16_t*)placeholder1)[(oco + 1680)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 61)] = ((int16_t*)placeholder1)[(oco + 1708)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 62)] = ((int16_t*)placeholder1)[(oco + 1736)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 63)] = ((int16_t*)placeholder1)[(oco + 1764)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 64)] = ((int16_t*)placeholder1)[(oco + 1792)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 65)] = ((int16_t*)placeholder1)[(oco + 1820)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 66)] = ((int16_t*)placeholder1)[(oco + 1848)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 67)] = ((int16_t*)placeholder1)[(oco + 1876)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 68)] = ((int16_t*)placeholder1)[(oco + 1904)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 69)] = ((int16_t*)placeholder1)[(oco + 1932)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 70)] = ((int16_t*)placeholder1)[(oco + 1960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 71)] = ((int16_t*)placeholder1)[(oco + 1988)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 72)] = ((int16_t*)placeholder1)[(oco + 2016)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 73)] = ((int16_t*)placeholder1)[(oco + 2044)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 74)] = ((int16_t*)placeholder1)[(oco + 2072)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 75)] = ((int16_t*)placeholder1)[(oco + 2100)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 76)] = ((int16_t*)placeholder1)[(oco + 2128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 77)] = ((int16_t*)placeholder1)[(oco + 2156)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 78)] = ((int16_t*)placeholder1)[(oco + 2184)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 79)] = ((int16_t*)placeholder1)[(oco + 2212)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 80)] = ((int16_t*)placeholder1)[(oco + 2240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 81)] = ((int16_t*)placeholder1)[(oco + 2268)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 82)] = ((int16_t*)placeholder1)[(oco + 2296)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 83)] = ((int16_t*)placeholder1)[(oco + 2324)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 84)] = ((int16_t*)placeholder1)[(oco + 2352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 85)] = ((int16_t*)placeholder1)[(oco + 2380)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 86)] = ((int16_t*)placeholder1)[(oco + 2408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 87)] = ((int16_t*)placeholder1)[(oco + 2436)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 88)] = ((int16_t*)placeholder1)[(oco + 2464)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 89)] = ((int16_t*)placeholder1)[(oco + 2492)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 90)] = ((int16_t*)placeholder1)[(oco + 2520)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 91)] = ((int16_t*)placeholder1)[(oco + 2548)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 92)] = ((int16_t*)placeholder1)[(oco + 2576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 93)] = ((int16_t*)placeholder1)[(oco + 2604)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 94)] = ((int16_t*)placeholder1)[(oco + 2632)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 95)] = ((int16_t*)placeholder1)[(oco + 2660)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 96)] = ((int16_t*)placeholder1)[(oco + 2688)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 97)] = ((int16_t*)placeholder1)[(oco + 2716)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 98)] = ((int16_t*)placeholder1)[(oco + 2744)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 99)] = ((int16_t*)placeholder1)[(oco + 2772)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 100)] = ((int16_t*)placeholder1)[(oco + 2800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 101)] = ((int16_t*)placeholder1)[(oco + 2828)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 102)] = ((int16_t*)placeholder1)[(oco + 2856)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 103)] = ((int16_t*)placeholder1)[(oco + 2884)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 104)] = ((int16_t*)placeholder1)[(oco + 2912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 105)] = ((int16_t*)placeholder1)[(oco + 2940)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 106)] = ((int16_t*)placeholder1)[(oco + 2968)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 107)] = ((int16_t*)placeholder1)[(oco + 2996)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 108)] = ((int16_t*)placeholder1)[(oco + 3024)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 109)] = ((int16_t*)placeholder1)[(oco + 3052)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 110)] = ((int16_t*)placeholder1)[(oco + 3080)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 111)] = ((int16_t*)placeholder1)[(oco + 3108)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 112)] = ((int16_t*)placeholder1)[(oco + 3136)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 113)] = ((int16_t*)placeholder1)[(oco + 3164)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 114)] = ((int16_t*)placeholder1)[(oco + 3192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 115)] = ((int16_t*)placeholder1)[(oco + 3220)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 116)] = ((int16_t*)placeholder1)[(oco + 3248)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 117)] = ((int16_t*)placeholder1)[(oco + 3276)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 118)] = ((int16_t*)placeholder1)[(oco + 3304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 119)] = ((int16_t*)placeholder1)[(oco + 3332)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 120)] = ((int16_t*)placeholder1)[(oco + 3360)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 121)] = ((int16_t*)placeholder1)[(oco + 3388)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 122)] = ((int16_t*)placeholder1)[(oco + 3416)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 123)] = ((int16_t*)placeholder1)[(oco + 3444)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 124)] = ((int16_t*)placeholder1)[(oco + 3472)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 125)] = ((int16_t*)placeholder1)[(oco + 3500)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 126)] = ((int16_t*)placeholder1)[(oco + 3528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 127)] = ((int16_t*)placeholder1)[(oco + 3556)];
      }
      for (int32_t oco1 = 0; oco1 < 28; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 128; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 128) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 28; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        ((int8_t*)T_cast)[(((ax0_ax1_outer_fused * 196) + (ax2_outer * 28)) + ax3_outer)] = ((int8_t)((_2) > (-128) ? (_2) : (-128)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_T_cast = (((TVMValue*)args)[6].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[6];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)14896, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 7; ++i1) {
    for (int32_t i2 = 0; i2 < 7; ++i2) {
      for (int32_t i3 = 0; i3 < 152; ++i3) {
        int32_t cse_var_1 = (((i1 * 1064) + (i2 * 152)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 7; ++ax0_ax1_outer_fused) {
    int16_t data_vec[152];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)4864, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[16];
    for (int32_t ax2_outer = 0; ax2_outer < 7; ++ax2_outer) {
      for (int32_t ic = 0; ic < 152; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 1064) + (ax2_outer * 152)) + ic)];
      }
      for (int32_t oco = 0; oco < 16; ++oco) {
        int32_t cse_var_2 = (oco * 152);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 16)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 32)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 48)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 64)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 80)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 160)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 208)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 272)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 416)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder1)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder1)[(oco + 464)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder1)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder1)[(oco + 496)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder1)[(oco + 512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder1)[(oco + 528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder1)[(oco + 544)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder1)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder1)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder1)[(oco + 592)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder1)[(oco + 608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder1)[(oco + 624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder1)[(oco + 640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder1)[(oco + 656)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder1)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder1)[(oco + 688)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder1)[(oco + 704)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder1)[(oco + 720)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder1)[(oco + 736)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder1)[(oco + 752)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder1)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder1)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder1)[(oco + 800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder1)[(oco + 816)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder1)[(oco + 832)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder1)[(oco + 848)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder1)[(oco + 864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder1)[(oco + 880)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 56)] = ((int16_t*)placeholder1)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 57)] = ((int16_t*)placeholder1)[(oco + 912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 58)] = ((int16_t*)placeholder1)[(oco + 928)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 59)] = ((int16_t*)placeholder1)[(oco + 944)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 60)] = ((int16_t*)placeholder1)[(oco + 960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 61)] = ((int16_t*)placeholder1)[(oco + 976)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 62)] = ((int16_t*)placeholder1)[(oco + 992)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 63)] = ((int16_t*)placeholder1)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 64)] = ((int16_t*)placeholder1)[(oco + 1024)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 65)] = ((int16_t*)placeholder1)[(oco + 1040)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 66)] = ((int16_t*)placeholder1)[(oco + 1056)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 67)] = ((int16_t*)placeholder1)[(oco + 1072)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 68)] = ((int16_t*)placeholder1)[(oco + 1088)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 69)] = ((int16_t*)placeholder1)[(oco + 1104)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 70)] = ((int16_t*)placeholder1)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 71)] = ((int16_t*)placeholder1)[(oco + 1136)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 72)] = ((int16_t*)placeholder1)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 73)] = ((int16_t*)placeholder1)[(oco + 1168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 74)] = ((int16_t*)placeholder1)[(oco + 1184)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 75)] = ((int16_t*)placeholder1)[(oco + 1200)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 76)] = ((int16_t*)placeholder1)[(oco + 1216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 77)] = ((int16_t*)placeholder1)[(oco + 1232)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 78)] = ((int16_t*)placeholder1)[(oco + 1248)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 79)] = ((int16_t*)placeholder1)[(oco + 1264)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 80)] = ((int16_t*)placeholder1)[(oco + 1280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 81)] = ((int16_t*)placeholder1)[(oco + 1296)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 82)] = ((int16_t*)placeholder1)[(oco + 1312)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 83)] = ((int16_t*)placeholder1)[(oco + 1328)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 84)] = ((int16_t*)placeholder1)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 85)] = ((int16_t*)placeholder1)[(oco + 1360)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 86)] = ((int16_t*)placeholder1)[(oco + 1376)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 87)] = ((int16_t*)placeholder1)[(oco + 1392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 88)] = ((int16_t*)placeholder1)[(oco + 1408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 89)] = ((int16_t*)placeholder1)[(oco + 1424)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 90)] = ((int16_t*)placeholder1)[(oco + 1440)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 91)] = ((int16_t*)placeholder1)[(oco + 1456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 92)] = ((int16_t*)placeholder1)[(oco + 1472)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 93)] = ((int16_t*)placeholder1)[(oco + 1488)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 94)] = ((int16_t*)placeholder1)[(oco + 1504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 95)] = ((int16_t*)placeholder1)[(oco + 1520)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 96)] = ((int16_t*)placeholder1)[(oco + 1536)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 97)] = ((int16_t*)placeholder1)[(oco + 1552)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 98)] = ((int16_t*)placeholder1)[(oco + 1568)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 99)] = ((int16_t*)placeholder1)[(oco + 1584)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 100)] = ((int16_t*)placeholder1)[(oco + 1600)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 101)] = ((int16_t*)placeholder1)[(oco + 1616)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 102)] = ((int16_t*)placeholder1)[(oco + 1632)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 103)] = ((int16_t*)placeholder1)[(oco + 1648)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 104)] = ((int16_t*)placeholder1)[(oco + 1664)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 105)] = ((int16_t*)placeholder1)[(oco + 1680)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 106)] = ((int16_t*)placeholder1)[(oco + 1696)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 107)] = ((int16_t*)placeholder1)[(oco + 1712)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 108)] = ((int16_t*)placeholder1)[(oco + 1728)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 109)] = ((int16_t*)placeholder1)[(oco + 1744)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 110)] = ((int16_t*)placeholder1)[(oco + 1760)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 111)] = ((int16_t*)placeholder1)[(oco + 1776)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 112)] = ((int16_t*)placeholder1)[(oco + 1792)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 113)] = ((int16_t*)placeholder1)[(oco + 1808)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 114)] = ((int16_t*)placeholder1)[(oco + 1824)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 115)] = ((int16_t*)placeholder1)[(oco + 1840)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 116)] = ((int16_t*)placeholder1)[(oco + 1856)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 117)] = ((int16_t*)placeholder1)[(oco + 1872)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 118)] = ((int16_t*)placeholder1)[(oco + 1888)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 119)] = ((int16_t*)placeholder1)[(oco + 1904)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 120)] = ((int16_t*)placeholder1)[(oco + 1920)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 121)] = ((int16_t*)placeholder1)[(oco + 1936)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 122)] = ((int16_t*)placeholder1)[(oco + 1952)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 123)] = ((int16_t*)placeholder1)[(oco + 1968)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 124)] = ((int16_t*)placeholder1)[(oco + 1984)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 125)] = ((int16_t*)placeholder1)[(oco + 2000)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 126)] = ((int16_t*)placeholder1)[(oco + 2016)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 127)] = ((int16_t*)placeholder1)[(oco + 2032)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 128)] = ((int16_t*)placeholder1)[(oco + 2048)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 129)] = ((int16_t*)placeholder1)[(oco + 2064)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 130)] = ((int16_t*)placeholder1)[(oco + 2080)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 131)] = ((int16_t*)placeholder1)[(oco + 2096)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 132)] = ((int16_t*)placeholder1)[(oco + 2112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 133)] = ((int16_t*)placeholder1)[(oco + 2128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 134)] = ((int16_t*)placeholder1)[(oco + 2144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 135)] = ((int16_t*)placeholder1)[(oco + 2160)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 136)] = ((int16_t*)placeholder1)[(oco + 2176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 137)] = ((int16_t*)placeholder1)[(oco + 2192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 138)] = ((int16_t*)placeholder1)[(oco + 2208)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 139)] = ((int16_t*)placeholder1)[(oco + 2224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 140)] = ((int16_t*)placeholder1)[(oco + 2240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 141)] = ((int16_t*)placeholder1)[(oco + 2256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 142)] = ((int16_t*)placeholder1)[(oco + 2272)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 143)] = ((int16_t*)placeholder1)[(oco + 2288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 144)] = ((int16_t*)placeholder1)[(oco + 2304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 145)] = ((int16_t*)placeholder1)[(oco + 2320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 146)] = ((int16_t*)placeholder1)[(oco + 2336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 147)] = ((int16_t*)placeholder1)[(oco + 2352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 148)] = ((int16_t*)placeholder1)[(oco + 2368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 149)] = ((int16_t*)placeholder1)[(oco + 2384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 150)] = ((int16_t*)placeholder1)[(oco + 2400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 151)] = ((int16_t*)placeholder1)[(oco + 2416)];
      }
      for (int32_t oco1 = 0; oco1 < 16; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 152; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 152) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 16; ++ax3_outer) {
        int32_t _1 = ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer])) - 15;
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        ((int8_t*)T_cast)[(((ax0_ax1_outer_fused * 112) + (ax2_outer * 16)) + ax3_outer)] = ((int8_t)((_2) > (-128) ? (_2) : (-128)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_T_cast = (((TVMValue*)args)[7].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[7];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)4864, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 152; ++i3) {
        int32_t cse_var_1 = (((i1 * 608) + (i2 * 152)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[152];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)7296, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[24];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 152; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 608) + (ax2_outer * 152)) + ic)];
      }
      for (int32_t oco = 0; oco < 24; ++oco) {
        int32_t cse_var_2 = (oco * 152);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 24)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 48)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 72)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 264)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 312)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 360)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 552)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 600)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 648)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder1)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder1)[(oco + 696)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder1)[(oco + 720)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder1)[(oco + 744)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder1)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder1)[(oco + 792)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder1)[(oco + 816)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder1)[(oco + 840)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder1)[(oco + 864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder1)[(oco + 888)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder1)[(oco + 912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder1)[(oco + 936)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder1)[(oco + 960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder1)[(oco + 984)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder1)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder1)[(oco + 1032)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder1)[(oco + 1056)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder1)[(oco + 1080)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder1)[(oco + 1104)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder1)[(oco + 1128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder1)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder1)[(oco + 1176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder1)[(oco + 1200)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder1)[(oco + 1224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder1)[(oco + 1248)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder1)[(oco + 1272)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder1)[(oco + 1296)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder1)[(oco + 1320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 56)] = ((int16_t*)placeholder1)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 57)] = ((int16_t*)placeholder1)[(oco + 1368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 58)] = ((int16_t*)placeholder1)[(oco + 1392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 59)] = ((int16_t*)placeholder1)[(oco + 1416)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 60)] = ((int16_t*)placeholder1)[(oco + 1440)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 61)] = ((int16_t*)placeholder1)[(oco + 1464)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 62)] = ((int16_t*)placeholder1)[(oco + 1488)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 63)] = ((int16_t*)placeholder1)[(oco + 1512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 64)] = ((int16_t*)placeholder1)[(oco + 1536)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 65)] = ((int16_t*)placeholder1)[(oco + 1560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 66)] = ((int16_t*)placeholder1)[(oco + 1584)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 67)] = ((int16_t*)placeholder1)[(oco + 1608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 68)] = ((int16_t*)placeholder1)[(oco + 1632)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 69)] = ((int16_t*)placeholder1)[(oco + 1656)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 70)] = ((int16_t*)placeholder1)[(oco + 1680)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 71)] = ((int16_t*)placeholder1)[(oco + 1704)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 72)] = ((int16_t*)placeholder1)[(oco + 1728)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 73)] = ((int16_t*)placeholder1)[(oco + 1752)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 74)] = ((int16_t*)placeholder1)[(oco + 1776)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 75)] = ((int16_t*)placeholder1)[(oco + 1800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 76)] = ((int16_t*)placeholder1)[(oco + 1824)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 77)] = ((int16_t*)placeholder1)[(oco + 1848)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 78)] = ((int16_t*)placeholder1)[(oco + 1872)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 79)] = ((int16_t*)placeholder1)[(oco + 1896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 80)] = ((int16_t*)placeholder1)[(oco + 1920)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 81)] = ((int16_t*)placeholder1)[(oco + 1944)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 82)] = ((int16_t*)placeholder1)[(oco + 1968)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 83)] = ((int16_t*)placeholder1)[(oco + 1992)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 84)] = ((int16_t*)placeholder1)[(oco + 2016)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 85)] = ((int16_t*)placeholder1)[(oco + 2040)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 86)] = ((int16_t*)placeholder1)[(oco + 2064)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 87)] = ((int16_t*)placeholder1)[(oco + 2088)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 88)] = ((int16_t*)placeholder1)[(oco + 2112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 89)] = ((int16_t*)placeholder1)[(oco + 2136)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 90)] = ((int16_t*)placeholder1)[(oco + 2160)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 91)] = ((int16_t*)placeholder1)[(oco + 2184)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 92)] = ((int16_t*)placeholder1)[(oco + 2208)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 93)] = ((int16_t*)placeholder1)[(oco + 2232)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 94)] = ((int16_t*)placeholder1)[(oco + 2256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 95)] = ((int16_t*)placeholder1)[(oco + 2280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 96)] = ((int16_t*)placeholder1)[(oco + 2304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 97)] = ((int16_t*)placeholder1)[(oco + 2328)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 98)] = ((int16_t*)placeholder1)[(oco + 2352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 99)] = ((int16_t*)placeholder1)[(oco + 2376)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 100)] = ((int16_t*)placeholder1)[(oco + 2400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 101)] = ((int16_t*)placeholder1)[(oco + 2424)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 102)] = ((int16_t*)placeholder1)[(oco + 2448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 103)] = ((int16_t*)placeholder1)[(oco + 2472)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 104)] = ((int16_t*)placeholder1)[(oco + 2496)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 105)] = ((int16_t*)placeholder1)[(oco + 2520)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 106)] = ((int16_t*)placeholder1)[(oco + 2544)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 107)] = ((int16_t*)placeholder1)[(oco + 2568)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 108)] = ((int16_t*)placeholder1)[(oco + 2592)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 109)] = ((int16_t*)placeholder1)[(oco + 2616)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 110)] = ((int16_t*)placeholder1)[(oco + 2640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 111)] = ((int16_t*)placeholder1)[(oco + 2664)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 112)] = ((int16_t*)placeholder1)[(oco + 2688)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 113)] = ((int16_t*)placeholder1)[(oco + 2712)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 114)] = ((int16_t*)placeholder1)[(oco + 2736)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 115)] = ((int16_t*)placeholder1)[(oco + 2760)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 116)] = ((int16_t*)placeholder1)[(oco + 2784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 117)] = ((int16_t*)placeholder1)[(oco + 2808)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 118)] = ((int16_t*)placeholder1)[(oco + 2832)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 119)] = ((int16_t*)placeholder1)[(oco + 2856)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 120)] = ((int16_t*)placeholder1)[(oco + 2880)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 121)] = ((int16_t*)placeholder1)[(oco + 2904)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 122)] = ((int16_t*)placeholder1)[(oco + 2928)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 123)] = ((int16_t*)placeholder1)[(oco + 2952)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 124)] = ((int16_t*)placeholder1)[(oco + 2976)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 125)] = ((int16_t*)placeholder1)[(oco + 3000)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 126)] = ((int16_t*)placeholder1)[(oco + 3024)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 127)] = ((int16_t*)placeholder1)[(oco + 3048)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 128)] = ((int16_t*)placeholder1)[(oco + 3072)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 129)] = ((int16_t*)placeholder1)[(oco + 3096)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 130)] = ((int16_t*)placeholder1)[(oco + 3120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 131)] = ((int16_t*)placeholder1)[(oco + 3144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 132)] = ((int16_t*)placeholder1)[(oco + 3168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 133)] = ((int16_t*)placeholder1)[(oco + 3192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 134)] = ((int16_t*)placeholder1)[(oco + 3216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 135)] = ((int16_t*)placeholder1)[(oco + 3240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 136)] = ((int16_t*)placeholder1)[(oco + 3264)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 137)] = ((int16_t*)placeholder1)[(oco + 3288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 138)] = ((int16_t*)placeholder1)[(oco + 3312)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 139)] = ((int16_t*)placeholder1)[(oco + 3336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 140)] = ((int16_t*)placeholder1)[(oco + 3360)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 141)] = ((int16_t*)placeholder1)[(oco + 3384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 142)] = ((int16_t*)placeholder1)[(oco + 3408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 143)] = ((int16_t*)placeholder1)[(oco + 3432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 144)] = ((int16_t*)placeholder1)[(oco + 3456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 145)] = ((int16_t*)placeholder1)[(oco + 3480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 146)] = ((int16_t*)placeholder1)[(oco + 3504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 147)] = ((int16_t*)placeholder1)[(oco + 3528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 148)] = ((int16_t*)placeholder1)[(oco + 3552)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 149)] = ((int16_t*)placeholder1)[(oco + 3576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 150)] = ((int16_t*)placeholder1)[(oco + 3600)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 151)] = ((int16_t*)placeholder1)[(oco + 3624)];
      }
      for (int32_t oco1 = 0; oco1 < 24; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 152; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 152) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 24; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        ((int8_t*)T_cast)[(((ax0_ax1_outer_fused * 96) + (ax2_outer * 24)) + ax3_outer)] = ((int8_t)((_2) > (-128) ? (_2) : (-128)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_T_cast = (((TVMValue*)args)[7].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[7];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)3584, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 112; ++i3) {
        int32_t cse_var_1 = (((i1 * 448) + (i2 * 112)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[112];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)2688, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[12];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 112; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 448) + (ax2_outer * 112)) + ic)];
      }
      for (int32_t oco = 0; oco < 12; ++oco) {
        int32_t cse_var_2 = (oco * 112);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 12)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 24)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 36)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 48)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 60)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 72)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 84)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 108)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 132)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 156)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 180)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 204)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 228)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 252)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 264)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 276)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 300)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 312)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 324)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder1)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder1)[(oco + 348)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder1)[(oco + 360)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder1)[(oco + 372)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder1)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder1)[(oco + 396)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder1)[(oco + 408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder1)[(oco + 420)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder1)[(oco + 432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder1)[(oco + 444)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder1)[(oco + 456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder1)[(oco + 468)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder1)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder1)[(oco + 492)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder1)[(oco + 504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder1)[(oco + 516)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder1)[(oco + 528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder1)[(oco + 540)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder1)[(oco + 552)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder1)[(oco + 564)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder1)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder1)[(oco + 588)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder1)[(oco + 600)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder1)[(oco + 612)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder1)[(oco + 624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder1)[(oco + 636)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder1)[(oco + 648)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder1)[(oco + 660)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 56)] = ((int16_t*)placeholder1)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 57)] = ((int16_t*)placeholder1)[(oco + 684)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 58)] = ((int16_t*)placeholder1)[(oco + 696)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 59)] = ((int16_t*)placeholder1)[(oco + 708)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 60)] = ((int16_t*)placeholder1)[(oco + 720)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 61)] = ((int16_t*)placeholder1)[(oco + 732)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 62)] = ((int16_t*)placeholder1)[(oco + 744)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 63)] = ((int16_t*)placeholder1)[(oco + 756)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 64)] = ((int16_t*)placeholder1)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 65)] = ((int16_t*)placeholder1)[(oco + 780)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 66)] = ((int16_t*)placeholder1)[(oco + 792)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 67)] = ((int16_t*)placeholder1)[(oco + 804)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 68)] = ((int16_t*)placeholder1)[(oco + 816)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 69)] = ((int16_t*)placeholder1)[(oco + 828)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 70)] = ((int16_t*)placeholder1)[(oco + 840)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 71)] = ((int16_t*)placeholder1)[(oco + 852)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 72)] = ((int16_t*)placeholder1)[(oco + 864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 73)] = ((int16_t*)placeholder1)[(oco + 876)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 74)] = ((int16_t*)placeholder1)[(oco + 888)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 75)] = ((int16_t*)placeholder1)[(oco + 900)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 76)] = ((int16_t*)placeholder1)[(oco + 912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 77)] = ((int16_t*)placeholder1)[(oco + 924)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 78)] = ((int16_t*)placeholder1)[(oco + 936)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 79)] = ((int16_t*)placeholder1)[(oco + 948)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 80)] = ((int16_t*)placeholder1)[(oco + 960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 81)] = ((int16_t*)placeholder1)[(oco + 972)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 82)] = ((int16_t*)placeholder1)[(oco + 984)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 83)] = ((int16_t*)placeholder1)[(oco + 996)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 84)] = ((int16_t*)placeholder1)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 85)] = ((int16_t*)placeholder1)[(oco + 1020)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 86)] = ((int16_t*)placeholder1)[(oco + 1032)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 87)] = ((int16_t*)placeholder1)[(oco + 1044)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 88)] = ((int16_t*)placeholder1)[(oco + 1056)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 89)] = ((int16_t*)placeholder1)[(oco + 1068)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 90)] = ((int16_t*)placeholder1)[(oco + 1080)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 91)] = ((int16_t*)placeholder1)[(oco + 1092)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 92)] = ((int16_t*)placeholder1)[(oco + 1104)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 93)] = ((int16_t*)placeholder1)[(oco + 1116)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 94)] = ((int16_t*)placeholder1)[(oco + 1128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 95)] = ((int16_t*)placeholder1)[(oco + 1140)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 96)] = ((int16_t*)placeholder1)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 97)] = ((int16_t*)placeholder1)[(oco + 1164)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 98)] = ((int16_t*)placeholder1)[(oco + 1176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 99)] = ((int16_t*)placeholder1)[(oco + 1188)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 100)] = ((int16_t*)placeholder1)[(oco + 1200)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 101)] = ((int16_t*)placeholder1)[(oco + 1212)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 102)] = ((int16_t*)placeholder1)[(oco + 1224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 103)] = ((int16_t*)placeholder1)[(oco + 1236)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 104)] = ((int16_t*)placeholder1)[(oco + 1248)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 105)] = ((int16_t*)placeholder1)[(oco + 1260)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 106)] = ((int16_t*)placeholder1)[(oco + 1272)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 107)] = ((int16_t*)placeholder1)[(oco + 1284)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 108)] = ((int16_t*)placeholder1)[(oco + 1296)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 109)] = ((int16_t*)placeholder1)[(oco + 1308)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 110)] = ((int16_t*)placeholder1)[(oco + 1320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 111)] = ((int16_t*)placeholder1)[(oco + 1332)];
      }
      for (int32_t oco1 = 0; oco1 < 12; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 112; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 112) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 12; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        ((int8_t*)T_cast)[(((ax0_ax1_outer_fused * 48) + (ax2_outer * 12)) + ax3_outer)] = ((int8_t)((_2) > (-128) ? (_2) : (-128)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_T_cast = (((TVMValue*)args)[6].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[6];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)2432, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 76; ++i3) {
        int32_t cse_var_1 = (((i1 * 304) + (i2 * 76)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[76];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)2432, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[16];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 76; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 304) + (ax2_outer * 76)) + ic)];
      }
      for (int32_t oco = 0; oco < 16; ++oco) {
        int32_t cse_var_2 = (oco * 76);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 16)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 32)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 48)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 64)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 80)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 160)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 208)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 272)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 416)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder1)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder1)[(oco + 464)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder1)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder1)[(oco + 496)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder1)[(oco + 512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder1)[(oco + 528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder1)[(oco + 544)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder1)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder1)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder1)[(oco + 592)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder1)[(oco + 608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder1)[(oco + 624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder1)[(oco + 640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder1)[(oco + 656)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder1)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder1)[(oco + 688)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder1)[(oco + 704)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder1)[(oco + 720)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder1)[(oco + 736)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder1)[(oco + 752)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder1)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder1)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder1)[(oco + 800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder1)[(oco + 816)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder1)[(oco + 832)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder1)[(oco + 848)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder1)[(oco + 864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder1)[(oco + 880)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 56)] = ((int16_t*)placeholder1)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 57)] = ((int16_t*)placeholder1)[(oco + 912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 58)] = ((int16_t*)placeholder1)[(oco + 928)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 59)] = ((int16_t*)placeholder1)[(oco + 944)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 60)] = ((int16_t*)placeholder1)[(oco + 960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 61)] = ((int16_t*)placeholder1)[(oco + 976)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 62)] = ((int16_t*)placeholder1)[(oco + 992)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 63)] = ((int16_t*)placeholder1)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 64)] = ((int16_t*)placeholder1)[(oco + 1024)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 65)] = ((int16_t*)placeholder1)[(oco + 1040)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 66)] = ((int16_t*)placeholder1)[(oco + 1056)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 67)] = ((int16_t*)placeholder1)[(oco + 1072)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 68)] = ((int16_t*)placeholder1)[(oco + 1088)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 69)] = ((int16_t*)placeholder1)[(oco + 1104)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 70)] = ((int16_t*)placeholder1)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 71)] = ((int16_t*)placeholder1)[(oco + 1136)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 72)] = ((int16_t*)placeholder1)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 73)] = ((int16_t*)placeholder1)[(oco + 1168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 74)] = ((int16_t*)placeholder1)[(oco + 1184)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 75)] = ((int16_t*)placeholder1)[(oco + 1200)];
      }
      for (int32_t oco1 = 0; oco1 < 16; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 76; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 76) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 16; ++ax3_outer) {
        int32_t _1 = ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer])) - 14;
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        ((int8_t*)T_cast)[(((ax0_ax1_outer_fused * 64) + (ax2_outer * 16)) + ax3_outer)] = ((int8_t)((_2) > (-128) ? (_2) : (-128)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_T_cast = (((TVMValue*)args)[7].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[7];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  int16_t PadInput[224];
  for (int32_t i1 = 0; i1 < 2; ++i1) {
    for (int32_t i2 = 0; i2 < 2; ++i2) {
      for (int32_t i3 = 0; i3 < 56; ++i3) {
        int32_t cse_var_1 = (((i1 * 112) + (i2 * 56)) + i3);
        PadInput[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 2; ++ax0_ax1_outer_fused) {
    int16_t data_vec[56];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)1792, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[16];
    for (int32_t ax2_outer = 0; ax2_outer < 2; ++ax2_outer) {
      for (int32_t ic = 0; ic < 56; ++ic) {
        data_vec[ic] = PadInput[(((ax0_ax1_outer_fused * 112) + (ax2_outer * 56)) + ic)];
      }
      for (int32_t oco = 0; oco < 16; ++oco) {
        int32_t cse_var_2 = (oco * 56);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 16)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 32)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 48)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 64)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 80)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 160)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 208)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 272)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 416)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder1)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder1)[(oco + 464)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder1)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder1)[(oco + 496)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder1)[(oco + 512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder1)[(oco + 528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder1)[(oco + 544)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder1)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder1)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder1)[(oco + 592)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder1)[(oco + 608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder1)[(oco + 624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder1)[(oco + 640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder1)[(oco + 656)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder1)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder1)[(oco + 688)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder1)[(oco + 704)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder1)[(oco + 720)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder1)[(oco + 736)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder1)[(oco + 752)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder1)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder1)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder1)[(oco + 800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder1)[(oco + 816)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder1)[(oco + 832)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder1)[(oco + 848)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder1)[(oco + 864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder1)[(oco + 880)];
      }
      for (int32_t oco1 = 0; oco1 < 16; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 56; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 56) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 16; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        ((int8_t*)T_cast)[(((ax0_ax1_outer_fused * 32) + (ax2_outer * 16)) + ax3_outer)] = ((int8_t)((_2) > (-128) ? (_2) : (-128)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_T_cast = (((TVMValue*)args)[6].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[6];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  int16_t PadInput[128];
  int16_t kernel_vec[256];
  int32_t conv[2];
  for (int32_t i3 = 0; i3 < 128; ++i3) {
    PadInput[i3] = ((int16_t*)placeholder)[i3];
  }
  for (int32_t oco = 0; oco < 2; ++oco) {
    int32_t cse_var_1 = (oco * 128);
    kernel_vec[cse_var_1] = ((int16_t*)placeholder1)[oco];
    kernel_vec[(cse_var_1 + 1)] = ((int16_t*)placeholder1)[(oco + 2)];
    kernel_vec[(cse_var_1 + 2)] = ((int16_t*)placeholder1)[(oco + 4)];
    kernel_vec[(cse_var_1 + 3)] = ((int16_t*)placeholder1)[(oco + 6)];
    kernel_vec[(cse_var_1 + 4)] = ((int16_t*)placeholder1)[(oco + 8)];
    kernel_vec[(cse_var_1 + 5)] = ((int16_t*)placeholder1)[(oco + 10)];
    kernel_vec[(cse_var_1 + 6)] = ((int16_t*)placeholder1)[(oco + 12)];
    kernel_vec[(cse_var_1 + 7)] = ((int16_t*)placeholder1)[(oco + 14)];
    kernel_vec[(cse_var_1 + 8)] = ((int16_t*)placeholder1)[(oco + 16)];
    kernel_vec[(cse_var_1 + 9)] = ((int16_t*)placeholder1)[(oco + 18)];
    kernel_vec[(cse_var_1 + 10)] = ((int16_t*)placeholder1)[(oco + 20)];
    kernel_vec[(cse_var_1 + 11)] = ((int16_t*)placeholder1)[(oco + 22)];
    kernel_vec[(cse_var_1 + 12)] = ((int16_t*)placeholder1)[(oco + 24)];
    kernel_vec[(cse_var_1 + 13)] = ((int16_t*)placeholder1)[(oco + 26)];
    kernel_vec[(cse_var_1 + 14)] = ((int16_t*)placeholder1)[(oco + 28)];
    kernel_vec[(cse_var_1 + 15)] = ((int16_t*)placeholder1)[(oco + 30)];
    kernel_vec[(cse_var_1 + 16)] = ((int16_t*)placeholder1)[(oco + 32)];
    kernel_vec[(cse_var_1 + 17)] = ((int16_t*)placeholder1)[(oco + 34)];
    kernel_vec[(cse_var_1 + 18)] = ((int16_t*)placeholder1)[(oco + 36)];
    kernel_vec[(cse_var_1 + 19)] = ((int16_t*)placeholder1)[(oco + 38)];
    kernel_vec[(cse_var_1 + 20)] = ((int16_t*)placeholder1)[(oco + 40)];
    kernel_vec[(cse_var_1 + 21)] = ((int16_t*)placeholder1)[(oco + 42)];
    kernel_vec[(cse_var_1 + 22)] = ((int16_t*)placeholder1)[(oco + 44)];
    kernel_vec[(cse_var_1 + 23)] = ((int16_t*)placeholder1)[(oco + 46)];
    kernel_vec[(cse_var_1 + 24)] = ((int16_t*)placeholder1)[(oco + 48)];
    kernel_vec[(cse_var_1 + 25)] = ((int16_t*)placeholder1)[(oco + 50)];
    kernel_vec[(cse_var_1 + 26)] = ((int16_t*)placeholder1)[(oco + 52)];
    kernel_vec[(cse_var_1 + 27)] = ((int16_t*)placeholder1)[(oco + 54)];
    kernel_vec[(cse_var_1 + 28)] = ((int16_t*)placeholder1)[(oco + 56)];
    kernel_vec[(cse_var_1 + 29)] = ((int16_t*)placeholder1)[(oco + 58)];
    kernel_vec[(cse_var_1 + 30)] = ((int16_t*)placeholder1)[(oco + 60)];
    kernel_vec[(cse_var_1 + 31)] = ((int16_t*)placeholder1)[(oco + 62)];
    kernel_vec[(cse_var_1 + 32)] = ((int16_t*)placeholder1)[(oco + 64)];
    kernel_vec[(cse_var_1 + 33)] = ((int16_t*)placeholder1)[(oco + 66)];
    kernel_vec[(cse_var_1 + 34)] = ((int16_t*)placeholder1)[(oco + 68)];
    kernel_vec[(cse_var_1 + 35)] = ((int16_t*)placeholder1)[(oco + 70)];
    kernel_vec[(cse_var_1 + 36)] = ((int16_t*)placeholder1)[(oco + 72)];
    kernel_vec[(cse_var_1 + 37)] = ((int16_t*)placeholder1)[(oco + 74)];
    kernel_vec[(cse_var_1 + 38)] = ((int16_t*)placeholder1)[(oco + 76)];
    kernel_vec[(cse_var_1 + 39)] = ((int16_t*)placeholder1)[(oco + 78)];
    kernel_vec[(cse_var_1 + 40)] = ((int16_t*)placeholder1)[(oco + 80)];
    kernel_vec[(cse_var_1 + 41)] = ((int16_t*)placeholder1)[(oco + 82)];
    kernel_vec[(cse_var_1 + 42)] = ((int16_t*)placeholder1)[(oco + 84)];
    kernel_vec[(cse_var_1 + 43)] = ((int16_t*)placeholder1)[(oco + 86)];
    kernel_vec[(cse_var_1 + 44)] = ((int16_t*)placeholder1)[(oco + 88)];
    kernel_vec[(cse_var_1 + 45)] = ((int16_t*)placeholder1)[(oco + 90)];
    kernel_vec[(cse_var_1 + 46)] = ((int16_t*)placeholder1)[(oco + 92)];
    kernel_vec[(cse_var_1 + 47)] = ((int16_t*)placeholder1)[(oco + 94)];
    kernel_vec[(cse_var_1 + 48)] = ((int16_t*)placeholder1)[(oco + 96)];
    kernel_vec[(cse_var_1 + 49)] = ((int16_t*)placeholder1)[(oco + 98)];
    kernel_vec[(cse_var_1 + 50)] = ((int16_t*)placeholder1)[(oco + 100)];
    kernel_vec[(cse_var_1 + 51)] = ((int16_t*)placeholder1)[(oco + 102)];
    kernel_vec[(cse_var_1 + 52)] = ((int16_t*)placeholder1)[(oco + 104)];
    kernel_vec[(cse_var_1 + 53)] = ((int16_t*)placeholder1)[(oco + 106)];
    kernel_vec[(cse_var_1 + 54)] = ((int16_t*)placeholder1)[(oco + 108)];
    kernel_vec[(cse_var_1 + 55)] = ((int16_t*)placeholder1)[(oco + 110)];
    kernel_vec[(cse_var_1 + 56)] = ((int16_t*)placeholder1)[(oco + 112)];
    kernel_vec[(cse_var_1 + 57)] = ((int16_t*)placeholder1)[(oco + 114)];
    kernel_vec[(cse_var_1 + 58)] = ((int16_t*)placeholder1)[(oco + 116)];
    kernel_vec[(cse_var_1 + 59)] = ((int16_t*)placeholder1)[(oco + 118)];
    kernel_vec[(cse_var_1 + 60)] = ((int16_t*)placeholder1)[(oco + 120)];
    kernel_vec[(cse_var_1 + 61)] = ((int16_t*)placeholder1)[(oco + 122)];
    kernel_vec[(cse_var_1 + 62)] = ((int16_t*)placeholder1)[(oco + 124)];
    kernel_vec[(cse_var_1 + 63)] = ((int16_t*)placeholder1)[(oco + 126)];
    kernel_vec[(cse_var_1 + 64)] = ((int16_t*)placeholder1)[(oco + 128)];
    kernel_vec[(cse_var_1 + 65)] = ((int16_t*)placeholder1)[(oco + 130)];
    kernel_vec[(cse_var_1 + 66)] = ((int16_t*)placeholder1)[(oco + 132)];
    kernel_vec[(cse_var_1 + 67)] = ((int16_t*)placeholder1)[(oco + 134)];
    kernel_vec[(cse_var_1 + 68)] = ((int16_t*)placeholder1)[(oco + 136)];
    kernel_vec[(cse_var_1 + 69)] = ((int16_t*)placeholder1)[(oco + 138)];
    kernel_vec[(cse_var_1 + 70)] = ((int16_t*)placeholder1)[(oco + 140)];
    kernel_vec[(cse_var_1 + 71)] = ((int16_t*)placeholder1)[(oco + 142)];
    kernel_vec[(cse_var_1 + 72)] = ((int16_t*)placeholder1)[(oco + 144)];
    kernel_vec[(cse_var_1 + 73)] = ((int16_t*)placeholder1)[(oco + 146)];
    kernel_vec[(cse_var_1 + 74)] = ((int16_t*)placeholder1)[(oco + 148)];
    kernel_vec[(cse_var_1 + 75)] = ((int16_t*)placeholder1)[(oco + 150)];
    kernel_vec[(cse_var_1 + 76)] = ((int16_t*)placeholder1)[(oco + 152)];
    kernel_vec[(cse_var_1 + 77)] = ((int16_t*)placeholder1)[(oco + 154)];
    kernel_vec[(cse_var_1 + 78)] = ((int16_t*)placeholder1)[(oco + 156)];
    kernel_vec[(cse_var_1 + 79)] = ((int16_t*)placeholder1)[(oco + 158)];
    kernel_vec[(cse_var_1 + 80)] = ((int16_t*)placeholder1)[(oco + 160)];
    kernel_vec[(cse_var_1 + 81)] = ((int16_t*)placeholder1)[(oco + 162)];
    kernel_vec[(cse_var_1 + 82)] = ((int16_t*)placeholder1)[(oco + 164)];
    kernel_vec[(cse_var_1 + 83)] = ((int16_t*)placeholder1)[(oco + 166)];
    kernel_vec[(cse_var_1 + 84)] = ((int16_t*)placeholder1)[(oco + 168)];
    kernel_vec[(cse_var_1 + 85)] = ((int16_t*)placeholder1)[(oco + 170)];
    kernel_vec[(cse_var_1 + 86)] = ((int16_t*)placeholder1)[(oco + 172)];
    kernel_vec[(cse_var_1 + 87)] = ((int16_t*)placeholder1)[(oco + 174)];
    kernel_vec[(cse_var_1 + 88)] = ((int16_t*)placeholder1)[(oco + 176)];
    kernel_vec[(cse_var_1 + 89)] = ((int16_t*)placeholder1)[(oco + 178)];
    kernel_vec[(cse_var_1 + 90)] = ((int16_t*)placeholder1)[(oco + 180)];
    kernel_vec[(cse_var_1 + 91)] = ((int16_t*)placeholder1)[(oco + 182)];
    kernel_vec[(cse_var_1 + 92)] = ((int16_t*)placeholder1)[(oco + 184)];
    kernel_vec[(cse_var_1 + 93)] = ((int16_t*)placeholder1)[(oco + 186)];
    kernel_vec[(cse_var_1 + 94)] = ((int16_t*)placeholder1)[(oco + 188)];
    kernel_vec[(cse_var_1 + 95)] = ((int16_t*)placeholder1)[(oco + 190)];
    kernel_vec[(cse_var_1 + 96)] = ((int16_t*)placeholder1)[(oco + 192)];
    kernel_vec[(cse_var_1 + 97)] = ((int16_t*)placeholder1)[(oco + 194)];
    kernel_vec[(cse_var_1 + 98)] = ((int16_t*)placeholder1)[(oco + 196)];
    kernel_vec[(cse_var_1 + 99)] = ((int16_t*)placeholder1)[(oco + 198)];
    kernel_vec[(cse_var_1 + 100)] = ((int16_t*)placeholder1)[(oco + 200)];
    kernel_vec[(cse_var_1 + 101)] = ((int16_t*)placeholder1)[(oco + 202)];
    kernel_vec[(cse_var_1 + 102)] = ((int16_t*)placeholder1)[(oco + 204)];
    kernel_vec[(cse_var_1 + 103)] = ((int16_t*)placeholder1)[(oco + 206)];
    kernel_vec[(cse_var_1 + 104)] = ((int16_t*)placeholder1)[(oco + 208)];
    kernel_vec[(cse_var_1 + 105)] = ((int16_t*)placeholder1)[(oco + 210)];
    kernel_vec[(cse_var_1 + 106)] = ((int16_t*)placeholder1)[(oco + 212)];
    kernel_vec[(cse_var_1 + 107)] = ((int16_t*)placeholder1)[(oco + 214)];
    kernel_vec[(cse_var_1 + 108)] = ((int16_t*)placeholder1)[(oco + 216)];
    kernel_vec[(cse_var_1 + 109)] = ((int16_t*)placeholder1)[(oco + 218)];
    kernel_vec[(cse_var_1 + 110)] = ((int16_t*)placeholder1)[(oco + 220)];
    kernel_vec[(cse_var_1 + 111)] = ((int16_t*)placeholder1)[(oco + 222)];
    kernel_vec[(cse_var_1 + 112)] = ((int16_t*)placeholder1)[(oco + 224)];
    kernel_vec[(cse_var_1 + 113)] = ((int16_t*)placeholder1)[(oco + 226)];
    kernel_vec[(cse_var_1 + 114)] = ((int16_t*)placeholder1)[(oco + 228)];
    kernel_vec[(cse_var_1 + 115)] = ((int16_t*)placeholder1)[(oco + 230)];
    kernel_vec[(cse_var_1 + 116)] = ((int16_t*)placeholder1)[(oco + 232)];
    kernel_vec[(cse_var_1 + 117)] = ((int16_t*)placeholder1)[(oco + 234)];
    kernel_vec[(cse_var_1 + 118)] = ((int16_t*)placeholder1)[(oco + 236)];
    kernel_vec[(cse_var_1 + 119)] = ((int16_t*)placeholder1)[(oco + 238)];
    kernel_vec[(cse_var_1 + 120)] = ((int16_t*)placeholder1)[(oco + 240)];
    kernel_vec[(cse_var_1 + 121)] = ((int16_t*)placeholder1)[(oco + 242)];
    kernel_vec[(cse_var_1 + 122)] = ((int16_t*)placeholder1)[(oco + 244)];
    kernel_vec[(cse_var_1 + 123)] = ((int16_t*)placeholder1)[(oco + 246)];
    kernel_vec[(cse_var_1 + 124)] = ((int16_t*)placeholder1)[(oco + 248)];
    kernel_vec[(cse_var_1 + 125)] = ((int16_t*)placeholder1)[(oco + 250)];
    kernel_vec[(cse_var_1 + 126)] = ((int16_t*)placeholder1)[(oco + 252)];
    kernel_vec[(cse_var_1 + 127)] = ((int16_t*)placeholder1)[(oco + 254)];
  }
  for (int32_t oco1 = 0; oco1 < 2; ++oco1) {
    conv[oco1] = 0;
    for (int32_t ic = 0; ic < 128; ++ic) {
      conv[oco1] = (conv[oco1] + (((int32_t)PadInput[ic]) * ((int32_t)kernel_vec[((oco1 * 128) + ic)])));
    }
  }
  for (int32_t ax3_outer = 0; ax3_outer < 2; ++ax3_outer) {
    int32_t _1 = ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer])) + 34;
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    ((int8_t*)T_cast)[ax3_outer] = ((int8_t)((_2) > (-128) ? (_2) : (-128)));
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1_(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_T_subtract = (((TVMValue*)args)[7].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[7];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)5000, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 25; ++i1) {
    for (int32_t i2 = 0; i2 < 25; ++i2) {
      for (int32_t i3 = 0; i3 < 4; ++i3) {
        int32_t cse_var_1 = (((i1 * 100) + (i2 * 4)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 25; ++ax0_ax1_outer_fused) {
    int16_t data_vec[4];
    int16_t kernel_vec[32];
    int32_t conv[8];
    for (int32_t ax2_outer = 0; ax2_outer < 25; ++ax2_outer) {
      for (int32_t ic = 0; ic < 4; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 100) + (ax2_outer * 4)) + ic)];
      }
      for (int32_t oco = 0; oco < 8; ++oco) {
        int32_t cse_var_2 = (oco * 4);
        kernel_vec[cse_var_2] = ((int16_t*)placeholder1)[oco];
        kernel_vec[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 8)];
        kernel_vec[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 16)];
        kernel_vec[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 24)];
      }
      for (int32_t oco1 = 0; oco1 < 8; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 4; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)kernel_vec[((oco1 * 4) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 8; ++ax3_outer) {
        int32_t _1 = ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer])) + 6;
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 200) + (ax2_outer * 8)) + ax3_outer)] = (((int16_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int16_t*)placeholder6)[0]);
      }
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int16_t PadInput[384];
  for (int32_t i1 = 0; i1 < 2; ++i1) {
    for (int32_t i2 = 0; i2 < 2; ++i2) {
      for (int32_t i3 = 0; i3 < 96; ++i3) {
        int32_t cse_var_1 = (((i1 * 192) + (i2 * 96)) + i3);
        PadInput[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 2; ++ax0_ax1_outer_fused) {
    int16_t data_vec[96];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)6144, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[32];
    for (int32_t ax2_outer = 0; ax2_outer < 2; ++ax2_outer) {
      for (int32_t ic = 0; ic < 96; ++ic) {
        data_vec[ic] = PadInput[(((ax0_ax1_outer_fused * 192) + (ax2_outer * 96)) + ic)];
      }
      for (int32_t oco = 0; oco < 32; ++oco) {
        int32_t cse_var_2 = (oco * 96);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 32)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 64)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 160)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 320)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 416)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 544)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 704)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 736)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 832)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder1)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder1)[(oco + 928)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder1)[(oco + 960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder1)[(oco + 992)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 32)] = ((int16_t*)placeholder1)[(oco + 1024)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 33)] = ((int16_t*)placeholder1)[(oco + 1056)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 34)] = ((int16_t*)placeholder1)[(oco + 1088)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 35)] = ((int16_t*)placeholder1)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 36)] = ((int16_t*)placeholder1)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 37)] = ((int16_t*)placeholder1)[(oco + 1184)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 38)] = ((int16_t*)placeholder1)[(oco + 1216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 39)] = ((int16_t*)placeholder1)[(oco + 1248)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 40)] = ((int16_t*)placeholder1)[(oco + 1280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 41)] = ((int16_t*)placeholder1)[(oco + 1312)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 42)] = ((int16_t*)placeholder1)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 43)] = ((int16_t*)placeholder1)[(oco + 1376)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 44)] = ((int16_t*)placeholder1)[(oco + 1408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 45)] = ((int16_t*)placeholder1)[(oco + 1440)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 46)] = ((int16_t*)placeholder1)[(oco + 1472)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 47)] = ((int16_t*)placeholder1)[(oco + 1504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 48)] = ((int16_t*)placeholder1)[(oco + 1536)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 49)] = ((int16_t*)placeholder1)[(oco + 1568)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 50)] = ((int16_t*)placeholder1)[(oco + 1600)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 51)] = ((int16_t*)placeholder1)[(oco + 1632)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 52)] = ((int16_t*)placeholder1)[(oco + 1664)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 53)] = ((int16_t*)placeholder1)[(oco + 1696)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 54)] = ((int16_t*)placeholder1)[(oco + 1728)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 55)] = ((int16_t*)placeholder1)[(oco + 1760)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 56)] = ((int16_t*)placeholder1)[(oco + 1792)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 57)] = ((int16_t*)placeholder1)[(oco + 1824)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 58)] = ((int16_t*)placeholder1)[(oco + 1856)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 59)] = ((int16_t*)placeholder1)[(oco + 1888)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 60)] = ((int16_t*)placeholder1)[(oco + 1920)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 61)] = ((int16_t*)placeholder1)[(oco + 1952)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 62)] = ((int16_t*)placeholder1)[(oco + 1984)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 63)] = ((int16_t*)placeholder1)[(oco + 2016)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 64)] = ((int16_t*)placeholder1)[(oco + 2048)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 65)] = ((int16_t*)placeholder1)[(oco + 2080)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 66)] = ((int16_t*)placeholder1)[(oco + 2112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 67)] = ((int16_t*)placeholder1)[(oco + 2144)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 68)] = ((int16_t*)placeholder1)[(oco + 2176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 69)] = ((int16_t*)placeholder1)[(oco + 2208)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 70)] = ((int16_t*)placeholder1)[(oco + 2240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 71)] = ((int16_t*)placeholder1)[(oco + 2272)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 72)] = ((int16_t*)placeholder1)[(oco + 2304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 73)] = ((int16_t*)placeholder1)[(oco + 2336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 74)] = ((int16_t*)placeholder1)[(oco + 2368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 75)] = ((int16_t*)placeholder1)[(oco + 2400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 76)] = ((int16_t*)placeholder1)[(oco + 2432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 77)] = ((int16_t*)placeholder1)[(oco + 2464)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 78)] = ((int16_t*)placeholder1)[(oco + 2496)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 79)] = ((int16_t*)placeholder1)[(oco + 2528)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 80)] = ((int16_t*)placeholder1)[(oco + 2560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 81)] = ((int16_t*)placeholder1)[(oco + 2592)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 82)] = ((int16_t*)placeholder1)[(oco + 2624)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 83)] = ((int16_t*)placeholder1)[(oco + 2656)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 84)] = ((int16_t*)placeholder1)[(oco + 2688)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 85)] = ((int16_t*)placeholder1)[(oco + 2720)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 86)] = ((int16_t*)placeholder1)[(oco + 2752)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 87)] = ((int16_t*)placeholder1)[(oco + 2784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 88)] = ((int16_t*)placeholder1)[(oco + 2816)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 89)] = ((int16_t*)placeholder1)[(oco + 2848)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 90)] = ((int16_t*)placeholder1)[(oco + 2880)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 91)] = ((int16_t*)placeholder1)[(oco + 2912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 92)] = ((int16_t*)placeholder1)[(oco + 2944)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 93)] = ((int16_t*)placeholder1)[(oco + 2976)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 94)] = ((int16_t*)placeholder1)[(oco + 3008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 95)] = ((int16_t*)placeholder1)[(oco + 3040)];
      }
      for (int32_t oco1 = 0; oco1 < 32; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 96; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 96) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 32; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 64) + (ax2_outer * 32)) + ax3_outer)] = (((int16_t)((int8_t)((_2) > (-128) ? (_2) : (-128)))) - ((int16_t*)placeholder7)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_T_cast = (((TVMValue*)args)[7].v_handle);
  int32_t arg_T_cast_code = arg_type_ids[7];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* T_cast = (((DLTensor*)arg_T_cast)[0].data);
  void* arg_T_cast_shape = (((DLTensor*)arg_T_cast)[0].shape);
  void* arg_T_cast_strides = (((DLTensor*)arg_T_cast)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_cast_strides == NULL)) {
  }
  int16_t PadInput[128];
  for (int32_t i1 = 0; i1 < 2; ++i1) {
    for (int32_t i2 = 0; i2 < 2; ++i2) {
      for (int32_t i3 = 0; i3 < 32; ++i3) {
        int32_t cse_var_1 = (((i1 * 64) + (i2 * 32)) + i3);
        PadInput[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 2; ++ax0_ax1_outer_fused) {
    int16_t data_vec[32];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)8192, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[128];
    for (int32_t ax2_outer = 0; ax2_outer < 2; ++ax2_outer) {
      for (int32_t ic = 0; ic < 32; ++ic) {
        data_vec[ic] = PadInput[(((ax0_ax1_outer_fused * 64) + (ax2_outer * 32)) + ic)];
      }
      for (int32_t oco = 0; oco < 128; ++oco) {
        int32_t cse_var_2 = (oco * 32);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 1024)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 1280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 1408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 1536)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 1664)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 1792)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 1920)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 2048)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 2176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 2304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 2432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 2560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 2688)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 2816)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 2944)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 3072)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 3200)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 3328)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 3456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder1)[(oco + 3584)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder1)[(oco + 3712)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder1)[(oco + 3840)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder1)[(oco + 3968)];
      }
      for (int32_t oco1 = 0; oco1 < 128; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 32; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 32) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 128; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int32_t*)T_cast)[(((ax0_ax1_outer_fused * 256) + (ax2_outer * 128)) + ax3_outer)] = ((int32_t)((_5) > (_6) ? (_5) : (_6)));
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245_(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)5202, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 51; ++i1) {
    for (int32_t i2 = 0; i2 < 51; ++i2) {
      ((int16_t*)PadInput)[((i1 * 51) + i2)] = (((i1 < 50) && (i2 < 50)) ? ((int16_t*)placeholder)[((i1 * 50) + i2)] : (int16_t)0);
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 25; ++ax0_ax1_outer_fused) {
    int16_t data_vec[9];
    int16_t kernel_vec[108];
    int32_t conv[12];
    for (int32_t ax2_outer = 0; ax2_outer < 25; ++ax2_outer) {
      for (int32_t ohi = 0; ohi < 3; ++ohi) {
        int32_t cse_var_2 = (ohi * 3);
        int32_t cse_var_1 = (((ax0_ax1_outer_fused * 102) + (ohi * 51)) + (ax2_outer * 2));
        data_vec[cse_var_2] = ((int16_t*)PadInput)[cse_var_1];
        data_vec[(cse_var_2 + 1)] = ((int16_t*)PadInput)[(cse_var_1 + 1)];
        data_vec[(cse_var_2 + 2)] = ((int16_t*)PadInput)[(cse_var_1 + 2)];
      }
      for (int32_t oco = 0; oco < 12; ++oco) {
        for (int32_t kh = 0; kh < 3; ++kh) {
          for (int32_t kw = 0; kw < 3; ++kw) {
            kernel_vec[(((oco * 9) + (kh * 3)) + kw)] = ((int16_t*)placeholder1)[(((kh * 36) + (kw * 12)) + oco)];
          }
        }
      }
      for (int32_t oco1 = 0; oco1 < 12; ++oco1) {
        conv[oco1] = 0;
        for (int32_t kh1 = 0; kh1 < 3; ++kh1) {
          for (int32_t kw1 = 0; kw1 < 3; ++kw1) {
            int32_t cse_var_3 = (kh1 * 3);
            conv[oco1] = (conv[oco1] + (((int32_t)data_vec[(cse_var_3 + kw1)]) * ((int32_t)kernel_vec[(((oco1 * 9) + cse_var_3) + kw1)])));
          }
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 12; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 300) + (ax2_outer * 12)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)15000, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 25; ++i1) {
    for (int32_t i2 = 0; i2 < 25; ++i2) {
      for (int32_t i3 = 0; i3 < 12; ++i3) {
        int32_t cse_var_1 = (((i1 * 300) + (i2 * 12)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 25; ++ax0_ax1_outer_fused) {
    int16_t data_vec[12];
    int16_t kernel_vec[48];
    int32_t conv[4];
    for (int32_t ax2_outer = 0; ax2_outer < 25; ++ax2_outer) {
      for (int32_t ic = 0; ic < 12; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 300) + (ax2_outer * 12)) + ic)];
      }
      for (int32_t oco = 0; oco < 4; ++oco) {
        int32_t cse_var_2 = (oco * 12);
        kernel_vec[cse_var_2] = ((int16_t*)placeholder1)[oco];
        kernel_vec[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 4)];
        kernel_vec[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 8)];
        kernel_vec[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 12)];
        kernel_vec[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 16)];
        kernel_vec[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 20)];
        kernel_vec[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 24)];
        kernel_vec[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 28)];
        kernel_vec[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 32)];
        kernel_vec[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 36)];
        kernel_vec[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 40)];
        kernel_vec[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 44)];
      }
      for (int32_t oco1 = 0; oco1 < 4; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 12; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)kernel_vec[((oco1 * 12) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 4; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 100) + (ax2_outer * 4)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t DepthwiseConv2d[1];
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_fused = 0; ax0_ax1_fused_ax2_fused_ax3_fused < 7448; ++ax0_ax1_fused_ax2_fused_ax3_fused) {
    int32_t cse_var_1 = (ax0_ax1_fused_ax2_fused_ax3_fused % 152);
    DepthwiseConv2d[0] = 0;
    for (int32_t di = 0; di < 3; ++di) {
      for (int32_t dj = 0; dj < 3; ++dj) {
        int32_t cse_var_4 = (dj * 152);
        int32_t cse_var_3 = ((ax0_ax1_fused_ax2_fused_ax3_fused / 1064) + di);
        int32_t cse_var_2 = (((ax0_ax1_fused_ax2_fused_ax3_fused % 1064) / 152) + dj);
        DepthwiseConv2d[0] = (DepthwiseConv2d[0] + (((int32_t)(((((1 <= cse_var_3) && (cse_var_3 < 8)) && (1 <= cse_var_2)) && (cse_var_2 < 8)) ? ((int16_t*)placeholder)[((((di * 1064) + cse_var_4) + ax0_ax1_fused_ax2_fused_ax3_fused) - 1216)] : (int16_t)0)) * ((int32_t)((int16_t*)placeholder1)[(((di * 456) + cse_var_4) + cse_var_1)])));
      }
    }
    int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)DepthwiseConv2d[0]) + ((int64_t)((int32_t*)placeholder2)[cse_var_1])) * ((int64_t*)placeholder3)[cse_var_1]) + ((int64_t*)placeholder4)[cse_var_1]) >> ((int64_t*)placeholder5)[cse_var_1]));
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
    int8_t _4 = (int8_t)127;
    int8_t _5 = (_3) < (_4) ? (_3) : (_4);
    int8_t _6 = (int8_t)-128;
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused_ax3_fused] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t DepthwiseConv2d[1];
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_fused = 0; ax0_ax1_fused_ax2_fused_ax3_fused < 2432; ++ax0_ax1_fused_ax2_fused_ax3_fused) {
    int32_t cse_var_1 = (ax0_ax1_fused_ax2_fused_ax3_fused % 152);
    DepthwiseConv2d[0] = 0;
    for (int32_t di = 0; di < 3; ++di) {
      for (int32_t dj = 0; dj < 3; ++dj) {
        int32_t cse_var_4 = (ax0_ax1_fused_ax2_fused_ax3_fused / 608);
        int32_t cse_var_3 = (dj * 152);
        int32_t cse_var_2 = ((ax0_ax1_fused_ax2_fused_ax3_fused % 608) / 152);
        DepthwiseConv2d[0] = (DepthwiseConv2d[0] + (((int32_t)(((((1 <= ((cse_var_4 * 2) + di)) && ((cse_var_4 + (di >> 1)) < 4)) && (1 <= ((cse_var_2 * 2) + dj))) && ((cse_var_2 + (dj >> 1)) < 4)) ? ((int16_t*)placeholder)[((((((cse_var_4 * 2128) + (di * 1064)) + (cse_var_2 * 304)) + cse_var_3) + cse_var_1) - 1216)] : (int16_t)0)) * ((int32_t)((int16_t*)placeholder1)[(((di * 456) + cse_var_3) + cse_var_1)])));
      }
    }
    int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)DepthwiseConv2d[0]) + ((int64_t)((int32_t*)placeholder2)[cse_var_1])) * ((int64_t*)placeholder3)[cse_var_1]) + ((int64_t*)placeholder4)[cse_var_1]) >> ((int64_t*)placeholder5)[cse_var_1]));
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
    int8_t _4 = (int8_t)127;
    int8_t _5 = (_3) < (_4) ? (_3) : (_4);
    int8_t _6 = (int8_t)-128;
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused_ax3_fused] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int16_t PadInput[384];
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 24; ++i3) {
        int32_t cse_var_1 = (((i1 * 96) + (i2 * 24)) + i3);
        PadInput[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[24];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)5376, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[112];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 24; ++ic) {
        data_vec[ic] = PadInput[(((ax0_ax1_outer_fused * 96) + (ax2_outer * 24)) + ic)];
      }
      for (int32_t oco = 0; oco < 112; ++oco) {
        int32_t cse_var_2 = (oco * 24);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 1232)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 1456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 1568)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 1680)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 1792)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 1904)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 2016)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 2128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 2240)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 2352)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 2464)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 2576)];
      }
      for (int32_t oco1 = 0; oco1 < 112; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 24; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 24) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 112; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 448) + (ax2_outer * 112)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t DepthwiseConv2d[1];
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_fused = 0; ax0_ax1_fused_ax2_fused_ax3_fused < 1792; ++ax0_ax1_fused_ax2_fused_ax3_fused) {
    int32_t cse_var_1 = (ax0_ax1_fused_ax2_fused_ax3_fused % 112);
    DepthwiseConv2d[0] = 0;
    for (int32_t di = 0; di < 3; ++di) {
      for (int32_t dj = 0; dj < 3; ++dj) {
        int32_t cse_var_4 = (dj * 112);
        int32_t cse_var_3 = ((ax0_ax1_fused_ax2_fused_ax3_fused / 448) + di);
        int32_t cse_var_2 = (((ax0_ax1_fused_ax2_fused_ax3_fused % 448) / 112) + dj);
        DepthwiseConv2d[0] = (DepthwiseConv2d[0] + (((int32_t)(((((1 <= cse_var_3) && (cse_var_3 < 5)) && (1 <= cse_var_2)) && (cse_var_2 < 5)) ? ((int16_t*)placeholder)[((((di * 448) + cse_var_4) + ax0_ax1_fused_ax2_fused_ax3_fused) - 560)] : (int16_t)0)) * ((int32_t)((int16_t*)placeholder1)[(((di * 336) + cse_var_4) + cse_var_1)])));
      }
    }
    int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)DepthwiseConv2d[0]) + ((int64_t)((int32_t*)placeholder2)[cse_var_1])) * ((int64_t*)placeholder3)[cse_var_1]) + ((int64_t*)placeholder4)[cse_var_1]) >> ((int64_t*)placeholder5)[cse_var_1]));
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
    int8_t _4 = (int8_t)127;
    int8_t _5 = (_3) < (_4) ? (_3) : (_4);
    int8_t _6 = (int8_t)-128;
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused_ax3_fused] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int16_t PadInput[384];
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 24; ++i3) {
        int32_t cse_var_1 = (((i1 * 96) + (i2 * 24)) + i3);
        PadInput[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[24];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)3648, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[76];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 24; ++ic) {
        data_vec[ic] = PadInput[(((ax0_ax1_outer_fused * 96) + (ax2_outer * 24)) + ic)];
      }
      for (int32_t oco = 0; oco < 76; ++oco) {
        int32_t cse_var_2 = (oco * 24);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 76)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 228)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 380)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 532)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 684)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 760)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 836)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 988)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 1064)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 1140)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 1216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 1292)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 1368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 1444)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 1520)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 1596)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 1672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 1748)];
      }
      for (int32_t oco1 = 0; oco1 < 76; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 24; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 24) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 76; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 304) + (ax2_outer * 76)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t DepthwiseConv2d[1];
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_fused = 0; ax0_ax1_fused_ax2_fused_ax3_fused < 1216; ++ax0_ax1_fused_ax2_fused_ax3_fused) {
    int32_t cse_var_1 = (ax0_ax1_fused_ax2_fused_ax3_fused % 76);
    DepthwiseConv2d[0] = 0;
    for (int32_t di = 0; di < 3; ++di) {
      for (int32_t dj = 0; dj < 3; ++dj) {
        int32_t cse_var_4 = (dj * 76);
        int32_t cse_var_3 = ((ax0_ax1_fused_ax2_fused_ax3_fused / 304) + di);
        int32_t cse_var_2 = (((ax0_ax1_fused_ax2_fused_ax3_fused % 304) / 76) + dj);
        DepthwiseConv2d[0] = (DepthwiseConv2d[0] + (((int32_t)(((((1 <= cse_var_3) && (cse_var_3 < 5)) && (1 <= cse_var_2)) && (cse_var_2 < 5)) ? ((int16_t*)placeholder)[((((di * 304) + cse_var_4) + ax0_ax1_fused_ax2_fused_ax3_fused) - 380)] : (int16_t)0)) * ((int32_t)((int16_t*)placeholder1)[(((di * 228) + cse_var_4) + cse_var_1)])));
      }
    }
    int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)DepthwiseConv2d[0]) + ((int64_t)((int32_t*)placeholder2)[cse_var_1])) * ((int64_t*)placeholder3)[cse_var_1]) + ((int64_t*)placeholder4)[cse_var_1]) >> ((int64_t*)placeholder5)[cse_var_1]));
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
    int8_t _4 = (int8_t)127;
    int8_t _5 = (_3) < (_4) ? (_3) : (_4);
    int8_t _6 = (int8_t)-128;
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused_ax3_fused] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)1024, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 32; ++i3) {
        int32_t cse_var_1 = (((i1 * 128) + (i2 * 32)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[32];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)4864, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[76];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 32; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 128) + (ax2_outer * 32)) + ic)];
      }
      for (int32_t oco = 0; oco < 76; ++oco) {
        int32_t cse_var_2 = (oco * 32);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 76)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 228)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 380)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 532)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 684)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 760)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 836)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 988)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 1064)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 1140)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 1216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 1292)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 1368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 1444)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 1520)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 1596)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 1672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 1748)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 1824)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 1900)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 1976)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 2052)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 28)] = ((int16_t*)placeholder1)[(oco + 2128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 29)] = ((int16_t*)placeholder1)[(oco + 2204)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 30)] = ((int16_t*)placeholder1)[(oco + 2280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 31)] = ((int16_t*)placeholder1)[(oco + 2356)];
      }
      for (int32_t oco1 = 0; oco1 < 76; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 32; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 32) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 76; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 304) + (ax2_outer * 76)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int16_t PadInput[256];
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 16; ++i3) {
        int32_t cse_var_1 = (((i1 * 64) + (i2 * 16)) + i3);
        PadInput[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[16];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)1792, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[56];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 16; ++ic) {
        data_vec[ic] = PadInput[(((ax0_ax1_outer_fused * 64) + (ax2_outer * 16)) + ic)];
      }
      for (int32_t oco = 0; oco < 56; ++oco) {
        int32_t cse_var_2 = (oco * 16);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 56)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 616)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 728)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 840)];
      }
      for (int32_t oco1 = 0; oco1 < 56; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 16; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 16) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 56; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 224) + (ax2_outer * 56)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t DepthwiseConv2d[1];
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_fused = 0; ax0_ax1_fused_ax2_fused_ax3_fused < 896; ++ax0_ax1_fused_ax2_fused_ax3_fused) {
    int32_t cse_var_1 = (ax0_ax1_fused_ax2_fused_ax3_fused % 56);
    DepthwiseConv2d[0] = 0;
    for (int32_t di = 0; di < 3; ++di) {
      for (int32_t dj = 0; dj < 3; ++dj) {
        int32_t cse_var_4 = (dj * 56);
        int32_t cse_var_3 = ((ax0_ax1_fused_ax2_fused_ax3_fused / 224) + di);
        int32_t cse_var_2 = (((ax0_ax1_fused_ax2_fused_ax3_fused % 224) / 56) + dj);
        DepthwiseConv2d[0] = (DepthwiseConv2d[0] + (((int32_t)(((((1 <= cse_var_3) && (cse_var_3 < 5)) && (1 <= cse_var_2)) && (cse_var_2 < 5)) ? ((int16_t*)placeholder)[((((di * 224) + cse_var_4) + ax0_ax1_fused_ax2_fused_ax3_fused) - 280)] : (int16_t)0)) * ((int32_t)((int16_t*)placeholder1)[(((di * 168) + cse_var_4) + cse_var_1)])));
      }
    }
    int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)DepthwiseConv2d[0]) + ((int64_t)((int32_t*)placeholder2)[cse_var_1])) * ((int64_t*)placeholder3)[cse_var_1]) + ((int64_t*)placeholder4)[cse_var_1]) >> ((int64_t*)placeholder5)[cse_var_1]));
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
    int8_t _4 = (int8_t)127;
    int8_t _5 = (_3) < (_4) ? (_3) : (_4);
    int8_t _6 = (int8_t)-128;
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused_ax3_fused] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int16_t PadInput[448];
  for (int32_t i1 = 0; i1 < 4; ++i1) {
    for (int32_t i2 = 0; i2 < 4; ++i2) {
      for (int32_t i3 = 0; i3 < 28; ++i3) {
        int32_t cse_var_1 = (((i1 * 112) + (i2 * 28)) + i3);
        PadInput[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 4; ++ax0_ax1_outer_fused) {
    int16_t data_vec[28];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)3136, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[56];
    for (int32_t ax2_outer = 0; ax2_outer < 4; ++ax2_outer) {
      for (int32_t ic = 0; ic < 28; ++ic) {
        data_vec[ic] = PadInput[(((ax0_ax1_outer_fused * 112) + (ax2_outer * 28)) + ic)];
      }
      for (int32_t oco = 0; oco < 56; ++oco) {
        int32_t cse_var_2 = (oco * 28);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 56)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 616)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 728)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 784)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 840)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 952)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 1008)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 1064)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 1120)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 1176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 1232)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 1288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 1400)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 1456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 1512)];
      }
      for (int32_t oco1 = 0; oco1 < 56; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 28; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 28) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 56; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 224) + (ax2_outer * 56)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t DepthwiseConv2d[1];
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_fused = 0; ax0_ax1_fused_ax2_fused_ax3_fused < 2500; ++ax0_ax1_fused_ax2_fused_ax3_fused) {
    int32_t cse_var_1 = (ax0_ax1_fused_ax2_fused_ax3_fused & 3);
    DepthwiseConv2d[0] = 0;
    for (int32_t di = 0; di < 3; ++di) {
      for (int32_t dj = 0; dj < 3; ++dj) {
        int32_t cse_var_4 = (dj * 4);
        int32_t cse_var_3 = ((ax0_ax1_fused_ax2_fused_ax3_fused / 100) + di);
        int32_t cse_var_2 = (((ax0_ax1_fused_ax2_fused_ax3_fused % 100) >> 2) + dj);
        DepthwiseConv2d[0] = (DepthwiseConv2d[0] + (((int32_t)(((((1 <= cse_var_3) && (cse_var_3 < 26)) && (1 <= cse_var_2)) && (cse_var_2 < 26)) ? ((int16_t*)placeholder)[((((di * 100) + cse_var_4) + ax0_ax1_fused_ax2_fused_ax3_fused) - 104)] : (int16_t)0)) * ((int32_t)((int16_t*)placeholder1)[(((di * 12) + cse_var_4) + cse_var_1)])));
      }
    }
    int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)DepthwiseConv2d[0]) + ((int64_t)((int32_t*)placeholder2)[cse_var_1])) * ((int64_t*)placeholder3)[cse_var_1]) + ((int64_t*)placeholder4)[cse_var_1]) >> ((int64_t*)placeholder5)[cse_var_1]));
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
    int8_t _4 = (int8_t)127;
    int8_t _5 = (_3) < (_4) ? (_3) : (_4);
    int8_t _6 = (int8_t)-128;
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused_ax3_fused] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t DepthwiseConv2d[1];
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_fused = 0; ax0_ax1_fused_ax2_fused_ax3_fused < 224; ++ax0_ax1_fused_ax2_fused_ax3_fused) {
    int32_t cse_var_1 = (ax0_ax1_fused_ax2_fused_ax3_fused % 56);
    DepthwiseConv2d[0] = 0;
    for (int32_t di = 0; di < 3; ++di) {
      for (int32_t dj = 0; dj < 3; ++dj) {
        int32_t cse_var_4 = (ax0_ax1_fused_ax2_fused_ax3_fused / 112);
        int32_t cse_var_3 = (dj * 56);
        int32_t cse_var_2 = ((ax0_ax1_fused_ax2_fused_ax3_fused % 112) / 56);
        DepthwiseConv2d[0] = (DepthwiseConv2d[0] + (((int32_t)((((cse_var_4 + (di >> 1)) < 2) && ((cse_var_2 + (dj >> 1)) < 2)) ? ((int16_t*)placeholder)[(((((cse_var_4 * 448) + (di * 224)) + (cse_var_2 * 112)) + cse_var_3) + cse_var_1)] : (int16_t)0)) * ((int32_t)((int16_t*)placeholder1)[(((di * 168) + cse_var_3) + cse_var_1)])));
      }
    }
    int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)DepthwiseConv2d[0]) + ((int64_t)((int32_t*)placeholder2)[cse_var_1])) * ((int64_t*)placeholder3)[cse_var_1]) + ((int64_t*)placeholder4)[cse_var_1]) >> ((int64_t*)placeholder5)[cse_var_1]));
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
    int8_t _4 = (int8_t)127;
    int8_t _5 = (_3) < (_4) ? (_3) : (_4);
    int8_t _6 = (int8_t)-128;
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused_ax3_fused] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int16_t PadInput[64];
  for (int32_t i1 = 0; i1 < 2; ++i1) {
    for (int32_t i2 = 0; i2 < 2; ++i2) {
      for (int32_t i3 = 0; i3 < 16; ++i3) {
        int32_t cse_var_1 = (((i1 * 32) + (i2 * 16)) + i3);
        PadInput[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 2; ++ax0_ax1_outer_fused) {
    int16_t data_vec[16];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)3072, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[96];
    for (int32_t ax2_outer = 0; ax2_outer < 2; ++ax2_outer) {
      for (int32_t ic = 0; ic < 16; ++ic) {
        data_vec[ic] = PadInput[(((ax0_ax1_outer_fused * 32) + (ax2_outer * 16)) + ic)];
      }
      for (int32_t oco = 0; oco < 96; ++oco) {
        int32_t cse_var_2 = (oco * 16);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 96)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 288)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 480)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 576)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 864)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 960)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 1056)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 1248)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 1344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 1440)];
      }
      for (int32_t oco1 = 0; oco1 < 96; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 16; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 16) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 96; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 192) + (ax2_outer * 96)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t DepthwiseConv2d[1];
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_fused = 0; ax0_ax1_fused_ax2_fused_ax3_fused < 384; ++ax0_ax1_fused_ax2_fused_ax3_fused) {
    int32_t cse_var_1 = (ax0_ax1_fused_ax2_fused_ax3_fused % 96);
    DepthwiseConv2d[0] = 0;
    for (int32_t di = 0; di < 3; ++di) {
      for (int32_t dj = 0; dj < 3; ++dj) {
        int32_t cse_var_4 = (dj * 96);
        int32_t cse_var_3 = ((ax0_ax1_fused_ax2_fused_ax3_fused / 192) + di);
        int32_t cse_var_2 = (((ax0_ax1_fused_ax2_fused_ax3_fused % 192) / 96) + dj);
        DepthwiseConv2d[0] = (DepthwiseConv2d[0] + (((int32_t)(((((1 <= cse_var_3) && (cse_var_3 < 3)) && (1 <= cse_var_2)) && (cse_var_2 < 3)) ? ((int16_t*)placeholder)[((((di * 192) + cse_var_4) + ax0_ax1_fused_ax2_fused_ax3_fused) - 288)] : (int16_t)0)) * ((int32_t)((int16_t*)placeholder1)[(((di * 288) + cse_var_4) + cse_var_1)])));
      }
    }
    int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)DepthwiseConv2d[0]) + ((int64_t)((int32_t*)placeholder2)[cse_var_1])) * ((int64_t*)placeholder3)[cse_var_1]) + ((int64_t*)placeholder4)[cse_var_1]) >> ((int64_t*)placeholder5)[cse_var_1]));
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
    int8_t _4 = (int8_t)127;
    int8_t _5 = (_3) < (_4) ? (_3) : (_4);
    int8_t _6 = (int8_t)-128;
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused_ax3_fused] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)10000, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 25; ++i1) {
    for (int32_t i2 = 0; i2 < 25; ++i2) {
      for (int32_t i3 = 0; i3 < 8; ++i3) {
        int32_t cse_var_1 = (((i1 * 200) + (i2 * 8)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 25; ++ax0_ax1_outer_fused) {
    int16_t data_vec[8];
    int16_t kernel_vec[224];
    int32_t conv[28];
    for (int32_t ax2_outer = 0; ax2_outer < 25; ++ax2_outer) {
      for (int32_t ic = 0; ic < 8; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 200) + (ax2_outer * 8)) + ic)];
      }
      for (int32_t oco = 0; oco < 28; ++oco) {
        int32_t cse_var_2 = (oco * 8);
        kernel_vec[cse_var_2] = ((int16_t*)placeholder1)[oco];
        kernel_vec[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 28)];
        kernel_vec[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 56)];
        kernel_vec[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 84)];
        kernel_vec[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 112)];
        kernel_vec[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 140)];
        kernel_vec[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 168)];
        kernel_vec[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 196)];
      }
      for (int32_t oco1 = 0; oco1 < 28; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 8; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)kernel_vec[((oco1 * 8) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 28; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 700) + (ax2_outer * 28)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t DepthwiseConv2d[1];
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_fused = 0; ax0_ax1_fused_ax2_fused_ax3_fused < 4732; ++ax0_ax1_fused_ax2_fused_ax3_fused) {
    int32_t cse_var_1 = (ax0_ax1_fused_ax2_fused_ax3_fused % 28);
    DepthwiseConv2d[0] = 0;
    for (int32_t di = 0; di < 3; ++di) {
      for (int32_t dj = 0; dj < 3; ++dj) {
        int32_t cse_var_4 = (ax0_ax1_fused_ax2_fused_ax3_fused / 364);
        int32_t cse_var_3 = (dj * 28);
        int32_t cse_var_2 = ((ax0_ax1_fused_ax2_fused_ax3_fused % 364) / 28);
        DepthwiseConv2d[0] = (DepthwiseConv2d[0] + (((int32_t)(((((1 <= ((cse_var_4 * 2) + di)) && ((cse_var_4 + (di >> 1)) < 13)) && (1 <= ((cse_var_2 * 2) + dj))) && ((cse_var_2 + (dj >> 1)) < 13)) ? ((int16_t*)placeholder)[((((((cse_var_4 * 1400) + (di * 700)) + (cse_var_2 * 56)) + cse_var_3) + cse_var_1) - 728)] : (int16_t)0)) * ((int32_t)((int16_t*)placeholder1)[(((di * 84) + cse_var_3) + cse_var_1)])));
      }
    }
    int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)DepthwiseConv2d[0]) + ((int64_t)((int32_t*)placeholder2)[cse_var_1])) * ((int64_t*)placeholder3)[cse_var_1]) + ((int64_t*)placeholder4)[cse_var_1]) >> ((int64_t*)placeholder5)[cse_var_1]));
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
    int8_t _4 = (int8_t)127;
    int8_t _5 = (_3) < (_4) ? (_3) : (_4);
    int8_t _6 = (int8_t)-128;
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused_ax3_fused] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)8112, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 13; ++i1) {
    for (int32_t i2 = 0; i2 < 13; ++i2) {
      for (int32_t i3 = 0; i3 < 24; ++i3) {
        int32_t cse_var_1 = (((i1 * 312) + (i2 * 24)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 13; ++ax0_ax1_outer_fused) {
    int16_t data_vec[24];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)1344, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[28];
    for (int32_t ax2_outer = 0; ax2_outer < 13; ++ax2_outer) {
      for (int32_t ic = 0; ic < 24; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 312) + (ax2_outer * 24)) + ic)];
      }
      for (int32_t oco = 0; oco < 28; ++oco) {
        int32_t cse_var_2 = (oco * 24);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 28)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 56)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 84)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 112)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 140)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 168)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 196)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 224)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 252)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 308)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 336)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 364)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 392)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 420)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 448)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 476)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 504)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 532)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 588)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 616)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 644)];
      }
      for (int32_t oco1 = 0; oco1 < 28; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 24; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 24) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 28; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 364) + (ax2_outer * 28)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t DepthwiseConv2d[1];
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_fused = 0; ax0_ax1_fused_ax2_fused_ax3_fused < 4732; ++ax0_ax1_fused_ax2_fused_ax3_fused) {
    int32_t cse_var_1 = (ax0_ax1_fused_ax2_fused_ax3_fused % 28);
    DepthwiseConv2d[0] = 0;
    for (int32_t di = 0; di < 3; ++di) {
      for (int32_t dj = 0; dj < 3; ++dj) {
        int32_t cse_var_4 = (dj * 28);
        int32_t cse_var_3 = ((ax0_ax1_fused_ax2_fused_ax3_fused / 364) + di);
        int32_t cse_var_2 = (((ax0_ax1_fused_ax2_fused_ax3_fused % 364) / 28) + dj);
        DepthwiseConv2d[0] = (DepthwiseConv2d[0] + (((int32_t)(((((1 <= cse_var_3) && (cse_var_3 < 14)) && (1 <= cse_var_2)) && (cse_var_2 < 14)) ? ((int16_t*)placeholder)[((((di * 364) + cse_var_4) + ax0_ax1_fused_ax2_fused_ax3_fused) - 392)] : (int16_t)0)) * ((int32_t)((int16_t*)placeholder1)[(((di * 84) + cse_var_4) + cse_var_1)])));
      }
    }
    int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)DepthwiseConv2d[0]) + ((int64_t)((int32_t*)placeholder2)[cse_var_1])) * ((int64_t*)placeholder3)[cse_var_1]) + ((int64_t*)placeholder4)[cse_var_1]) >> ((int64_t*)placeholder5)[cse_var_1]));
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
    int8_t _4 = (int8_t)127;
    int8_t _5 = (_3) < (_4) ? (_3) : (_4);
    int8_t _6 = (int8_t)-128;
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused_ax3_fused] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)8112, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 13; ++i1) {
    for (int32_t i2 = 0; i2 < 13; ++i2) {
      for (int32_t i3 = 0; i3 < 24; ++i3) {
        int32_t cse_var_1 = (((i1 * 312) + (i2 * 24)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 13; ++ax0_ax1_outer_fused) {
    int16_t data_vec[24];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)6144, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[128];
    for (int32_t ax2_outer = 0; ax2_outer < 13; ++ax2_outer) {
      for (int32_t ic = 0; ic < 24; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 312) + (ax2_outer * 24)) + ic)];
      }
      for (int32_t oco = 0; oco < 128; ++oco) {
        int32_t cse_var_2 = (oco * 24);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 256)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 384)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 512)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 640)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 768)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 896)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 1024)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 1152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 1280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 1408)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 1536)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 1664)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 1792)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 1920)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 2048)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 2176)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 2304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 2432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 2560)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 2688)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 2816)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 2944)];
      }
      for (int32_t oco1 = 0; oco1 < 128; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 24; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 24) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 128; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 1664) + (ax2_outer * 128)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  int32_t DepthwiseConv2d[1];
  for (int32_t ax0_ax1_fused_ax2_fused_ax3_fused = 0; ax0_ax1_fused_ax2_fused_ax3_fused < 6272; ++ax0_ax1_fused_ax2_fused_ax3_fused) {
    int32_t cse_var_1 = (ax0_ax1_fused_ax2_fused_ax3_fused & 127);
    DepthwiseConv2d[0] = 0;
    for (int32_t di = 0; di < 3; ++di) {
      for (int32_t dj = 0; dj < 3; ++dj) {
        int32_t cse_var_4 = (ax0_ax1_fused_ax2_fused_ax3_fused / 896);
        int32_t cse_var_3 = (dj * 128);
        int32_t cse_var_2 = ((ax0_ax1_fused_ax2_fused_ax3_fused % 896) >> 7);
        DepthwiseConv2d[0] = (DepthwiseConv2d[0] + (((int32_t)(((((1 <= ((cse_var_4 * 2) + di)) && ((cse_var_4 + (di >> 1)) < 7)) && (1 <= ((cse_var_2 * 2) + dj))) && ((cse_var_2 + (dj >> 1)) < 7)) ? ((int16_t*)placeholder)[((((((cse_var_4 * 3328) + (di * 1664)) + (cse_var_2 * 256)) + cse_var_3) + cse_var_1) - 1792)] : (int16_t)0)) * ((int32_t)((int16_t*)placeholder1)[(((di * 384) + cse_var_3) + cse_var_1)])));
      }
    }
    int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)DepthwiseConv2d[0]) + ((int64_t)((int32_t*)placeholder2)[cse_var_1])) * ((int64_t*)placeholder3)[cse_var_1]) + ((int64_t*)placeholder4)[cse_var_1]) >> ((int64_t*)placeholder5)[cse_var_1]));
    int32_t _2 = (_1) < (127) ? (_1) : (127);
    int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
    int8_t _4 = (int8_t)127;
    int8_t _5 = (_3) < (_4) ? (_3) : (_4);
    int8_t _6 = (int8_t)-128;
    ((int16_t*)T_subtract)[ax0_ax1_fused_ax2_fused_ax3_fused] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_placeholder1 = (((TVMValue*)args)[1].v_handle);
  int32_t arg_placeholder_code1 = arg_type_ids[1];
  void* arg_placeholder2 = (((TVMValue*)args)[2].v_handle);
  int32_t arg_placeholder_code2 = arg_type_ids[2];
  void* arg_placeholder3 = (((TVMValue*)args)[3].v_handle);
  int32_t arg_placeholder_code3 = arg_type_ids[3];
  void* arg_placeholder4 = (((TVMValue*)args)[4].v_handle);
  int32_t arg_placeholder_code4 = arg_type_ids[4];
  void* arg_placeholder5 = (((TVMValue*)args)[5].v_handle);
  int32_t arg_placeholder_code5 = arg_type_ids[5];
  void* arg_placeholder6 = (((TVMValue*)args)[6].v_handle);
  int32_t arg_placeholder_code6 = arg_type_ids[6];
  void* arg_placeholder7 = (((TVMValue*)args)[7].v_handle);
  int32_t arg_placeholder_code7 = arg_type_ids[7];
  void* arg_T_subtract = (((TVMValue*)args)[8].v_handle);
  int32_t arg_T_subtract_code = arg_type_ids[8];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* placeholder1 = (((DLTensor*)arg_placeholder1)[0].data);
  void* arg_placeholder_shape1 = (((DLTensor*)arg_placeholder1)[0].shape);
  void* arg_placeholder_strides1 = (((DLTensor*)arg_placeholder1)[0].strides);
  void* placeholder2 = (((DLTensor*)arg_placeholder2)[0].data);
  void* arg_placeholder_shape2 = (((DLTensor*)arg_placeholder2)[0].shape);
  void* arg_placeholder_strides2 = (((DLTensor*)arg_placeholder2)[0].strides);
  void* placeholder3 = (((DLTensor*)arg_placeholder3)[0].data);
  void* arg_placeholder_shape3 = (((DLTensor*)arg_placeholder3)[0].shape);
  void* arg_placeholder_strides3 = (((DLTensor*)arg_placeholder3)[0].strides);
  void* placeholder4 = (((DLTensor*)arg_placeholder4)[0].data);
  void* arg_placeholder_shape4 = (((DLTensor*)arg_placeholder4)[0].shape);
  void* arg_placeholder_strides4 = (((DLTensor*)arg_placeholder4)[0].strides);
  void* placeholder5 = (((DLTensor*)arg_placeholder5)[0].data);
  void* arg_placeholder_shape5 = (((DLTensor*)arg_placeholder5)[0].shape);
  void* arg_placeholder_strides5 = (((DLTensor*)arg_placeholder5)[0].strides);
  void* placeholder6 = (((DLTensor*)arg_placeholder6)[0].data);
  void* arg_placeholder_shape6 = (((DLTensor*)arg_placeholder6)[0].shape);
  void* arg_placeholder_strides6 = (((DLTensor*)arg_placeholder6)[0].strides);
  void* placeholder7 = (((DLTensor*)arg_placeholder7)[0].data);
  void* arg_placeholder_shape7 = (((DLTensor*)arg_placeholder7)[0].shape);
  void* arg_placeholder_strides7 = (((DLTensor*)arg_placeholder7)[0].strides);
  void* T_subtract = (((DLTensor*)arg_T_subtract)[0].data);
  void* arg_T_subtract_shape = (((DLTensor*)arg_T_subtract)[0].shape);
  void* arg_T_subtract_strides = (((DLTensor*)arg_T_subtract)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_placeholder_strides1 == NULL)) {
  }
  if (!(arg_placeholder_strides2 == NULL)) {
  }
  if (!(arg_placeholder_strides3 == NULL)) {
  }
  if (!(arg_placeholder_strides4 == NULL)) {
  }
  if (!(arg_placeholder_strides5 == NULL)) {
  }
  if (!(arg_T_subtract_strides == NULL)) {
  }
  void* PadInput = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)2744, 0, 16);
  if (PadInput == NULL) {
    return -1;
  }
  for (int32_t i1 = 0; i1 < 7; ++i1) {
    for (int32_t i2 = 0; i2 < 7; ++i2) {
      for (int32_t i3 = 0; i3 < 28; ++i3) {
        int32_t cse_var_1 = (((i1 * 196) + (i2 * 28)) + i3);
        ((int16_t*)PadInput)[cse_var_1] = ((int16_t*)placeholder)[cse_var_1];
      }
    }
  }
  for (int32_t ax0_ax1_outer_fused = 0; ax0_ax1_outer_fused < 7; ++ax0_ax1_outer_fused) {
    int16_t data_vec[28];
    void* kernel_vec = TVMBackendAllocWorkspace(1, dev_id, (uint64_t)8512, 0, 16);
    if (kernel_vec == NULL) {
      return -1;
    }
    int32_t conv[152];
    for (int32_t ax2_outer = 0; ax2_outer < 7; ++ax2_outer) {
      for (int32_t ic = 0; ic < 28; ++ic) {
        data_vec[ic] = ((int16_t*)PadInput)[(((ax0_ax1_outer_fused * 196) + (ax2_outer * 28)) + ic)];
      }
      for (int32_t oco = 0; oco < 152; ++oco) {
        int32_t cse_var_2 = (oco * 28);
        ((int16_t*)kernel_vec)[cse_var_2] = ((int16_t*)placeholder1)[oco];
        ((int16_t*)kernel_vec)[(cse_var_2 + 1)] = ((int16_t*)placeholder1)[(oco + 152)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 2)] = ((int16_t*)placeholder1)[(oco + 304)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 3)] = ((int16_t*)placeholder1)[(oco + 456)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 4)] = ((int16_t*)placeholder1)[(oco + 608)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 5)] = ((int16_t*)placeholder1)[(oco + 760)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 6)] = ((int16_t*)placeholder1)[(oco + 912)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 7)] = ((int16_t*)placeholder1)[(oco + 1064)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 8)] = ((int16_t*)placeholder1)[(oco + 1216)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 9)] = ((int16_t*)placeholder1)[(oco + 1368)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 10)] = ((int16_t*)placeholder1)[(oco + 1520)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 11)] = ((int16_t*)placeholder1)[(oco + 1672)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 12)] = ((int16_t*)placeholder1)[(oco + 1824)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 13)] = ((int16_t*)placeholder1)[(oco + 1976)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 14)] = ((int16_t*)placeholder1)[(oco + 2128)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 15)] = ((int16_t*)placeholder1)[(oco + 2280)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 16)] = ((int16_t*)placeholder1)[(oco + 2432)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 17)] = ((int16_t*)placeholder1)[(oco + 2584)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 18)] = ((int16_t*)placeholder1)[(oco + 2736)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 19)] = ((int16_t*)placeholder1)[(oco + 2888)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 20)] = ((int16_t*)placeholder1)[(oco + 3040)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 21)] = ((int16_t*)placeholder1)[(oco + 3192)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 22)] = ((int16_t*)placeholder1)[(oco + 3344)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 23)] = ((int16_t*)placeholder1)[(oco + 3496)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 24)] = ((int16_t*)placeholder1)[(oco + 3648)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 25)] = ((int16_t*)placeholder1)[(oco + 3800)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 26)] = ((int16_t*)placeholder1)[(oco + 3952)];
        ((int16_t*)kernel_vec)[(cse_var_2 + 27)] = ((int16_t*)placeholder1)[(oco + 4104)];
      }
      for (int32_t oco1 = 0; oco1 < 152; ++oco1) {
        conv[oco1] = 0;
        for (int32_t ic1 = 0; ic1 < 28; ++ic1) {
          conv[oco1] = (conv[oco1] + (((int32_t)data_vec[ic1]) * ((int32_t)((int16_t*)kernel_vec)[((oco1 * 28) + ic1)])));
        }
      }
      for (int32_t ax3_outer = 0; ax3_outer < 152; ++ax3_outer) {
        int32_t _1 = ((int32_t*)placeholder6)[0] + ((int32_t)((((((int64_t)conv[ax3_outer]) + ((int64_t)((int32_t*)placeholder2)[ax3_outer])) * ((int64_t*)placeholder3)[ax3_outer]) + ((int64_t*)placeholder4)[ax3_outer]) >> ((int64_t*)placeholder5)[ax3_outer]));
        int32_t _2 = (_1) < (127) ? (_1) : (127);
        int8_t _3 = (int8_t)((_2) > (-128) ? (_2) : (-128));
        int8_t _4 = (int8_t)127;
        int8_t _5 = (_3) < (_4) ? (_3) : (_4);
        int8_t _6 = (int8_t)-128;
        ((int16_t*)T_subtract)[(((ax0_ax1_outer_fused * 1064) + (ax2_outer * 152)) + ax3_outer)] = (((int16_t)((_5) > (_6) ? (_5) : (_6))) - ((int16_t*)placeholder7)[0]);
      }
    }
    if (TVMBackendFreeWorkspace(1, dev_id, kernel_vec) != 0) {
      return -1;
    }
  }
  if (TVMBackendFreeWorkspace(1, dev_id, PadInput) != 0) {
    return -1;
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_pad(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_T_pad = (((TVMValue*)args)[1].v_handle);
  int32_t arg_T_pad_code = arg_type_ids[1];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* T_pad = (((DLTensor*)arg_T_pad)[0].data);
  void* arg_T_pad_shape = (((DLTensor*)arg_T_pad)[0].shape);
  void* arg_T_pad_strides = (((DLTensor*)arg_T_pad)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_T_pad_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 16; ++ax0_ax1_fused_ax2_fused) {
    for (int32_t ax3_outer = 0; ax3_outer < 2; ++ax3_outer) {
      for (int32_t ax3_inner = 0; ax3_inner < 16; ++ax3_inner) {
        int32_t cse_var_1 = (ax3_outer * 16);
        ((int8_t*)T_pad)[(((ax0_ax1_fused_ax2_fused * 32) + cse_var_1) + ax3_inner)] = ((((ax3_outer * 2) + (ax3_inner >> 3)) < 3) ? ((int8_t*)placeholder)[(((ax0_ax1_fused_ax2_fused * 24) + cse_var_1) + ax3_inner)] : (int8_t)-6);
      }
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_nn_pad_1(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_T_pad = (((TVMValue*)args)[1].v_handle);
  int32_t arg_T_pad_code = arg_type_ids[1];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* T_pad = (((DLTensor*)arg_T_pad)[0].data);
  void* arg_T_pad_shape = (((DLTensor*)arg_T_pad)[0].shape);
  void* arg_T_pad_strides = (((DLTensor*)arg_T_pad)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_T_pad_strides == NULL)) {
  }
  for (int32_t ax0_ax1_fused_ax2_fused = 0; ax0_ax1_fused_ax2_fused < 16; ++ax0_ax1_fused_ax2_fused) {
    for (int32_t ax3_outer = 0; ax3_outer < 2; ++ax3_outer) {
      for (int32_t ax3_inner = 0; ax3_inner < 16; ++ax3_inner) {
        if (((ax3_outer * 4) + (ax3_inner >> 2)) < 7) {
          int32_t cse_var_1 = (ax3_outer * 16);
          ((int8_t*)T_pad)[(((ax0_ax1_fused_ax2_fused * 28) + cse_var_1) + ax3_inner)] = ((ax3_outer < 1) ? ((int8_t*)placeholder)[(((ax0_ax1_fused_ax2_fused * 16) + cse_var_1) + ax3_inner)] : (int8_t)-14);
        }
      }
    }
  }
  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
TVM_DLL int32_t tvmgen_default_fused_reshape(void* args, int32_t* arg_type_ids, int32_t num_args, void* out_ret_value, int32_t* out_ret_tcode, void* resource_handle) {
  void* arg_placeholder = (((TVMValue*)args)[0].v_handle);
  int32_t arg_placeholder_code = arg_type_ids[0];
  void* arg_T_reshape = (((TVMValue*)args)[1].v_handle);
  int32_t arg_T_reshape_code = arg_type_ids[1];
  void* placeholder = (((DLTensor*)arg_placeholder)[0].data);
  void* arg_placeholder_shape = (((DLTensor*)arg_placeholder)[0].shape);
  void* arg_placeholder_strides = (((DLTensor*)arg_placeholder)[0].strides);
  int32_t dev_id = (((DLTensor*)arg_placeholder)[0].device.device_id);
  void* T_reshape = (((DLTensor*)arg_T_reshape)[0].data);
  void* arg_T_reshape_shape = (((DLTensor*)arg_T_reshape)[0].shape);
  void* arg_T_reshape_strides = (((DLTensor*)arg_T_reshape)[0].strides);
  if (!(arg_placeholder_strides == NULL)) {
  }
  if (!(arg_T_reshape_strides == NULL)) {
  }
  for (int32_t ax1_inner = 0; ax1_inner < 2; ++ax1_inner) {
    ((int8_t*)T_reshape)[ax1_inner] = ((int8_t*)placeholder)[ax1_inner];
  }
  return 0;
}

