#include <stdio.h>

#include "dlpack/dlpack.h"
#include "tvm/runtime/c_runtime_api.h"
#include "network.h"
#include "network_data.h"

//
// Weights: network
//
ANT_ML_STATIC int64_t p0_shape[0] = {};
ANT_ML_STATIC int64_t p0_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p0_shape,
    .strides = p0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p1_shape[4] = {3, 3, 1, 12};
ANT_ML_STATIC int64_t p1_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p1 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p1_shape,
    .strides = p1_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p10_shape[4] = {1, 1, 1, 4};
ANT_ML_STATIC int64_t p10_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p10 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p10_shape,
    .strides = p10_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p100_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p100_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p100 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p100_shape,
    .strides = p100_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p101_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p101_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p101 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p101_shape,
    .strides = p101_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p102_shape[4] = {3, 3, 152, 1};
ANT_ML_STATIC int64_t p102_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p102 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p102_shape,
    .strides = p102_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p103_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p103_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p103 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p103_shape,
    .strides = p103_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p104_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p104_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p104 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p104_shape,
    .strides = p104_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p105_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p105_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p105 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p105_shape,
    .strides = p105_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p106_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p106_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p106 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p106_shape,
    .strides = p106_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p107_shape[4] = {1, 1, 152, 28};
ANT_ML_STATIC int64_t p107_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p107 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p107_shape,
    .strides = p107_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p108_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p108_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p108 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p108_shape,
    .strides = p108_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p109_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p109_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p109 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p109_shape,
    .strides = p109_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p11_shape[4] = {1, 1, 1, 4};
ANT_ML_STATIC int64_t p11_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p11 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p11_shape,
    .strides = p11_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p110_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p110_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p110 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p110_shape,
    .strides = p110_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p111_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p111_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p111 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p111_shape,
    .strides = p111_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p112_shape[0] = {};
ANT_ML_STATIC int64_t p112_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p112 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p112_shape,
    .strides = p112_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p113_shape[4] = {1, 1, 28, 152};
ANT_ML_STATIC int64_t p113_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p113 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p113_shape,
    .strides = p113_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p114_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p114_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p114 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p114_shape,
    .strides = p114_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p115_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p115_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p115 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p115_shape,
    .strides = p115_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p116_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p116_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p116 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p116_shape,
    .strides = p116_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p117_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p117_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p117 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p117_shape,
    .strides = p117_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p118_shape[4] = {3, 3, 152, 1};
ANT_ML_STATIC int64_t p118_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p118 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p118_shape,
    .strides = p118_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p119_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p119_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p119 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p119_shape,
    .strides = p119_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p12_shape[4] = {1, 1, 1, 4};
ANT_ML_STATIC int64_t p12_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p12 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p12_shape,
    .strides = p12_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p120_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p120_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p120 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p120_shape,
    .strides = p120_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p121_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p121_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p121 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p121_shape,
    .strides = p121_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p122_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p122_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p122 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p122_shape,
    .strides = p122_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p123_shape[4] = {1, 1, 152, 24};
ANT_ML_STATIC int64_t p123_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p123 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p123_shape,
    .strides = p123_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p124_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p124_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p124 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p124_shape,
    .strides = p124_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p125_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p125_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p125 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p125_shape,
    .strides = p125_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p126_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p126_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p126 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p126_shape,
    .strides = p126_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p127_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p127_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p127 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p127_shape,
    .strides = p127_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p128_shape[0] = {};
ANT_ML_STATIC int64_t p128_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p128 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 32, 1},
    .shape = p128_shape,
    .strides = p128_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p129_shape[1] = {1};
ANT_ML_STATIC int64_t p129_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p129 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p129_shape,
    .strides = p129_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p13_shape[4] = {3, 3, 4, 1};
ANT_ML_STATIC int64_t p13_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p13 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p13_shape,
    .strides = p13_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p130_shape[0] = {};
ANT_ML_STATIC int64_t p130_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p130 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p130_shape,
    .strides = p130_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p131_shape[4] = {1, 1, 24, 112};
ANT_ML_STATIC int64_t p131_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p131 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p131_shape,
    .strides = p131_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p132_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p132_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p132 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p132_shape,
    .strides = p132_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p133_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p133_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p133 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p133_shape,
    .strides = p133_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p134_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p134_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p134 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p134_shape,
    .strides = p134_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p135_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p135_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p135 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p135_shape,
    .strides = p135_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p136_shape[4] = {3, 3, 112, 1};
ANT_ML_STATIC int64_t p136_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p136 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p136_shape,
    .strides = p136_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p137_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p137_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p137 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p137_shape,
    .strides = p137_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p138_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p138_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p138 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p138_shape,
    .strides = p138_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p139_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p139_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p139 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p139_shape,
    .strides = p139_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p14_shape[4] = {1, 1, 1, 4};
ANT_ML_STATIC int64_t p14_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p14 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p14_shape,
    .strides = p14_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p140_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p140_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p140 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p140_shape,
    .strides = p140_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p141_shape[4] = {1, 1, 112, 24};
ANT_ML_STATIC int64_t p141_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p141 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p141_shape,
    .strides = p141_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p142_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p142_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p142 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p142_shape,
    .strides = p142_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p143_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p143_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p143 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p143_shape,
    .strides = p143_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p144_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p144_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p144 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p144_shape,
    .strides = p144_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p145_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p145_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p145 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p145_shape,
    .strides = p145_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p146_shape[1] = {1};
ANT_ML_STATIC int64_t p146_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p146 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p146_shape,
    .strides = p146_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p147_shape[0] = {};
ANT_ML_STATIC int64_t p147_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p147 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p147_shape,
    .strides = p147_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p148_shape[4] = {1, 1, 24, 112};
ANT_ML_STATIC int64_t p148_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p148 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p148_shape,
    .strides = p148_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p149_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p149_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p149 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p149_shape,
    .strides = p149_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p15_shape[4] = {1, 1, 1, 4};
ANT_ML_STATIC int64_t p15_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p15 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p15_shape,
    .strides = p15_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p150_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p150_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p150 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p150_shape,
    .strides = p150_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p151_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p151_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p151 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p151_shape,
    .strides = p151_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p152_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p152_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p152 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p152_shape,
    .strides = p152_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p153_shape[4] = {3, 3, 112, 1};
ANT_ML_STATIC int64_t p153_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p153 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p153_shape,
    .strides = p153_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p154_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p154_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p154 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p154_shape,
    .strides = p154_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p155_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p155_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p155 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p155_shape,
    .strides = p155_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p156_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p156_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p156 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p156_shape,
    .strides = p156_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p157_shape[4] = {1, 1, 1, 112};
ANT_ML_STATIC int64_t p157_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p157 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p157_shape,
    .strides = p157_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p158_shape[4] = {1, 1, 112, 12};
ANT_ML_STATIC int64_t p158_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p158 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p158_shape,
    .strides = p158_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p159_shape[4] = {1, 1, 1, 12};
ANT_ML_STATIC int64_t p159_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p159 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p159_shape,
    .strides = p159_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p16_shape[4] = {1, 1, 1, 4};
ANT_ML_STATIC int64_t p16_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p16 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p16_shape,
    .strides = p16_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p160_shape[4] = {1, 1, 1, 12};
ANT_ML_STATIC int64_t p160_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p160 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p160_shape,
    .strides = p160_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p161_shape[4] = {1, 1, 1, 12};
ANT_ML_STATIC int64_t p161_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p161 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p161_shape,
    .strides = p161_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p162_shape[4] = {1, 1, 1, 12};
ANT_ML_STATIC int64_t p162_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p162 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p162_shape,
    .strides = p162_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p163_shape[0] = {};
ANT_ML_STATIC int64_t p163_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p163 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 32, 1},
    .shape = p163_shape,
    .strides = p163_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p164_shape[4] = {1, 1, 24, 76};
ANT_ML_STATIC int64_t p164_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p164 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p164_shape,
    .strides = p164_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p165_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p165_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p165 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p165_shape,
    .strides = p165_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p166_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p166_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p166 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p166_shape,
    .strides = p166_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p167_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p167_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p167 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p167_shape,
    .strides = p167_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p168_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p168_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p168 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p168_shape,
    .strides = p168_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p169_shape[4] = {3, 3, 76, 1};
ANT_ML_STATIC int64_t p169_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p169 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p169_shape,
    .strides = p169_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p17_shape[4] = {1, 1, 1, 4};
ANT_ML_STATIC int64_t p17_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p17 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p17_shape,
    .strides = p17_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p170_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p170_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p170 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p170_shape,
    .strides = p170_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p171_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p171_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p171 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p171_shape,
    .strides = p171_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p172_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p172_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p172 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p172_shape,
    .strides = p172_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p173_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p173_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p173 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p173_shape,
    .strides = p173_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p174_shape[4] = {1, 1, 76, 32};
ANT_ML_STATIC int64_t p174_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p174 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p174_shape,
    .strides = p174_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p175_shape[4] = {1, 1, 1, 32};
ANT_ML_STATIC int64_t p175_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p175 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p175_shape,
    .strides = p175_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p176_shape[4] = {1, 1, 1, 32};
ANT_ML_STATIC int64_t p176_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p176 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p176_shape,
    .strides = p176_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p177_shape[4] = {1, 1, 1, 32};
ANT_ML_STATIC int64_t p177_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p177 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p177_shape,
    .strides = p177_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p178_shape[4] = {1, 1, 1, 32};
ANT_ML_STATIC int64_t p178_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p178 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p178_shape,
    .strides = p178_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p179_shape[1] = {1};
ANT_ML_STATIC int64_t p179_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p179 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p179_shape,
    .strides = p179_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p18_shape[4] = {1, 1, 4, 8};
ANT_ML_STATIC int64_t p18_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p18 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p18_shape,
    .strides = p18_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p180_shape[0] = {};
ANT_ML_STATIC int64_t p180_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p180 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p180_shape,
    .strides = p180_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p181_shape[4] = {1, 1, 32, 76};
ANT_ML_STATIC int64_t p181_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p181 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p181_shape,
    .strides = p181_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p182_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p182_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p182 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p182_shape,
    .strides = p182_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p183_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p183_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p183 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p183_shape,
    .strides = p183_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p184_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p184_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p184 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p184_shape,
    .strides = p184_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p185_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p185_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p185 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p185_shape,
    .strides = p185_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p186_shape[4] = {3, 3, 76, 1};
ANT_ML_STATIC int64_t p186_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p186 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p186_shape,
    .strides = p186_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p187_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p187_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p187 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p187_shape,
    .strides = p187_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p188_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p188_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p188 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p188_shape,
    .strides = p188_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p189_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p189_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p189 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p189_shape,
    .strides = p189_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p19_shape[4] = {1, 1, 1, 8};
ANT_ML_STATIC int64_t p19_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p19 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p19_shape,
    .strides = p19_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p190_shape[4] = {1, 1, 1, 76};
ANT_ML_STATIC int64_t p190_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p190 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p190_shape,
    .strides = p190_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p191_shape[4] = {1, 1, 76, 16};
ANT_ML_STATIC int64_t p191_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p191 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p191_shape,
    .strides = p191_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p192_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p192_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p192 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p192_shape,
    .strides = p192_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p193_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p193_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p193 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p193_shape,
    .strides = p193_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p194_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p194_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p194 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p194_shape,
    .strides = p194_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p195_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p195_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p195 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p195_shape,
    .strides = p195_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p196_shape[1] = {1};
ANT_ML_STATIC int64_t p196_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p196 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p196_shape,
    .strides = p196_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p197_shape[0] = {};
ANT_ML_STATIC int64_t p197_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p197 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p197_shape,
    .strides = p197_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p198_shape[4] = {1, 1, 16, 56};
ANT_ML_STATIC int64_t p198_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p198 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p198_shape,
    .strides = p198_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p199_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p199_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p199 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p199_shape,
    .strides = p199_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p2_shape[4] = {1, 1, 1, 12};
ANT_ML_STATIC int64_t p2_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p2 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p2_shape,
    .strides = p2_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p20_shape[4] = {1, 1, 1, 8};
ANT_ML_STATIC int64_t p20_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p20 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p20_shape,
    .strides = p20_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p200_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p200_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p200 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p200_shape,
    .strides = p200_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p201_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p201_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p201 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p201_shape,
    .strides = p201_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p202_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p202_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p202 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p202_shape,
    .strides = p202_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p203_shape[4] = {3, 3, 56, 1};
ANT_ML_STATIC int64_t p203_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p203 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p203_shape,
    .strides = p203_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p204_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p204_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p204 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p204_shape,
    .strides = p204_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p205_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p205_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p205 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p205_shape,
    .strides = p205_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p206_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p206_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p206 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p206_shape,
    .strides = p206_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p207_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p207_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p207 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p207_shape,
    .strides = p207_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p208_shape[4] = {1, 1, 56, 28};
ANT_ML_STATIC int64_t p208_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p208 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p208_shape,
    .strides = p208_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p209_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p209_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p209 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p209_shape,
    .strides = p209_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p21_shape[4] = {1, 1, 1, 8};
ANT_ML_STATIC int64_t p21_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p21 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p21_shape,
    .strides = p21_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p210_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p210_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p210 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p210_shape,
    .strides = p210_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p211_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p211_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p211 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p211_shape,
    .strides = p211_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p212_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p212_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p212 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p212_shape,
    .strides = p212_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p213_shape[1] = {1};
ANT_ML_STATIC int64_t p213_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p213 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p213_shape,
    .strides = p213_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p214_shape[0] = {};
ANT_ML_STATIC int64_t p214_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p214 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p214_shape,
    .strides = p214_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p215_shape[4] = {1, 1, 28, 56};
ANT_ML_STATIC int64_t p215_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p215 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p215_shape,
    .strides = p215_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p216_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p216_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p216 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p216_shape,
    .strides = p216_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p217_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p217_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p217 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p217_shape,
    .strides = p217_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p218_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p218_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p218 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p218_shape,
    .strides = p218_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p219_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p219_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p219 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p219_shape,
    .strides = p219_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p22_shape[4] = {1, 1, 1, 8};
ANT_ML_STATIC int64_t p22_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p22 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p22_shape,
    .strides = p22_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p220_shape[4] = {3, 3, 56, 1};
ANT_ML_STATIC int64_t p220_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p220 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p220_shape,
    .strides = p220_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p221_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p221_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p221 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p221_shape,
    .strides = p221_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p222_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p222_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p222 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p222_shape,
    .strides = p222_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p223_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p223_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p223 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p223_shape,
    .strides = p223_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p224_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p224_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p224 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p224_shape,
    .strides = p224_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p225_shape[4] = {1, 1, 56, 28};
ANT_ML_STATIC int64_t p225_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p225 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p225_shape,
    .strides = p225_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p226_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p226_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p226 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p226_shape,
    .strides = p226_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p227_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p227_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p227 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p227_shape,
    .strides = p227_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p228_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p228_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p228 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p228_shape,
    .strides = p228_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p229_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p229_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p229 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p229_shape,
    .strides = p229_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p23_shape[0] = {};
ANT_ML_STATIC int64_t p23_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p23 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p23_shape,
    .strides = p23_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p230_shape[1] = {1};
ANT_ML_STATIC int64_t p230_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p230 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p230_shape,
    .strides = p230_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p231_shape[4] = {1, 1, 28, 56};
ANT_ML_STATIC int64_t p231_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p231 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p231_shape,
    .strides = p231_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p232_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p232_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p232 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p232_shape,
    .strides = p232_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p233_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p233_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p233 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p233_shape,
    .strides = p233_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p234_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p234_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p234 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p234_shape,
    .strides = p234_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p235_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p235_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p235 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p235_shape,
    .strides = p235_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p236_shape[4] = {3, 3, 56, 1};
ANT_ML_STATIC int64_t p236_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p236 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p236_shape,
    .strides = p236_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p237_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p237_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p237 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p237_shape,
    .strides = p237_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p238_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p238_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p238 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p238_shape,
    .strides = p238_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p239_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p239_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p239 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p239_shape,
    .strides = p239_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p24_shape[4] = {1, 1, 8, 28};
ANT_ML_STATIC int64_t p24_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p24 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p24_shape,
    .strides = p24_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p240_shape[4] = {1, 1, 1, 56};
ANT_ML_STATIC int64_t p240_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p240 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p240_shape,
    .strides = p240_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p241_shape[4] = {1, 1, 56, 16};
ANT_ML_STATIC int64_t p241_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p241 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p241_shape,
    .strides = p241_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p242_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p242_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p242 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p242_shape,
    .strides = p242_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p243_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p243_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p243 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p243_shape,
    .strides = p243_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p244_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p244_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p244 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p244_shape,
    .strides = p244_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p245_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p245_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p245 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p245_shape,
    .strides = p245_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p246_shape[1] = {1};
ANT_ML_STATIC int64_t p246_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p246 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p246_shape,
    .strides = p246_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p247_shape[4] = {1, 1, 16, 96};
ANT_ML_STATIC int64_t p247_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p247 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p247_shape,
    .strides = p247_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p248_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p248_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p248 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p248_shape,
    .strides = p248_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p249_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p249_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p249 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p249_shape,
    .strides = p249_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p25_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p25_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p25 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p25_shape,
    .strides = p25_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p250_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p250_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p250 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p250_shape,
    .strides = p250_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p251_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p251_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p251 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p251_shape,
    .strides = p251_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p252_shape[4] = {3, 3, 96, 1};
ANT_ML_STATIC int64_t p252_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p252 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p252_shape,
    .strides = p252_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p253_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p253_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p253 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p253_shape,
    .strides = p253_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p254_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p254_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p254 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p254_shape,
    .strides = p254_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p255_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p255_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p255 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p255_shape,
    .strides = p255_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p256_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p256_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p256 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p256_shape,
    .strides = p256_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p257_shape[4] = {1, 1, 96, 16};
ANT_ML_STATIC int64_t p257_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p257 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p257_shape,
    .strides = p257_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p258_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p258_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p258 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p258_shape,
    .strides = p258_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p259_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p259_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p259 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p259_shape,
    .strides = p259_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p26_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p26_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p26 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p26_shape,
    .strides = p26_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p260_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p260_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p260 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p260_shape,
    .strides = p260_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p261_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p261_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p261 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p261_shape,
    .strides = p261_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p262_shape[0] = {};
ANT_ML_STATIC int64_t p262_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p262 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 32, 1},
    .shape = p262_shape,
    .strides = p262_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p263_shape[1] = {1};
ANT_ML_STATIC int64_t p263_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p263 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p263_shape,
    .strides = p263_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p264_shape[4] = {1, 1, 16, 96};
ANT_ML_STATIC int64_t p264_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p264 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p264_shape,
    .strides = p264_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p265_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p265_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p265 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p265_shape,
    .strides = p265_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p266_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p266_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p266 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p266_shape,
    .strides = p266_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p267_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p267_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p267 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p267_shape,
    .strides = p267_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p268_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p268_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p268 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p268_shape,
    .strides = p268_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p269_shape[4] = {3, 3, 96, 1};
ANT_ML_STATIC int64_t p269_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p269 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p269_shape,
    .strides = p269_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p27_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p27_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p27 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p27_shape,
    .strides = p27_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p270_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p270_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p270 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p270_shape,
    .strides = p270_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p271_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p271_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p271 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p271_shape,
    .strides = p271_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p272_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p272_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p272 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p272_shape,
    .strides = p272_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p273_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p273_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p273 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p273_shape,
    .strides = p273_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p274_shape[4] = {1, 1, 96, 16};
ANT_ML_STATIC int64_t p274_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p274 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p274_shape,
    .strides = p274_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p275_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p275_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p275 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p275_shape,
    .strides = p275_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p276_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p276_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p276 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p276_shape,
    .strides = p276_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p277_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p277_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p277 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p277_shape,
    .strides = p277_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p278_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p278_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p278 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p278_shape,
    .strides = p278_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p279_shape[4] = {1, 1, 16, 96};
ANT_ML_STATIC int64_t p279_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p279 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p279_shape,
    .strides = p279_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p28_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p28_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p28 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p28_shape,
    .strides = p28_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p280_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p280_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p280 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p280_shape,
    .strides = p280_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p281_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p281_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p281 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p281_shape,
    .strides = p281_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p282_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p282_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p282 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p282_shape,
    .strides = p282_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p283_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p283_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p283 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p283_shape,
    .strides = p283_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p284_shape[4] = {3, 3, 96, 1};
ANT_ML_STATIC int64_t p284_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p284 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p284_shape,
    .strides = p284_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p285_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p285_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p285 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p285_shape,
    .strides = p285_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p286_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p286_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p286 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p286_shape,
    .strides = p286_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p287_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p287_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p287 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p287_shape,
    .strides = p287_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p288_shape[4] = {1, 1, 1, 96};
ANT_ML_STATIC int64_t p288_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p288 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p288_shape,
    .strides = p288_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p289_shape[4] = {1, 1, 96, 32};
ANT_ML_STATIC int64_t p289_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p289 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p289_shape,
    .strides = p289_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p29_shape[4] = {3, 3, 28, 1};
ANT_ML_STATIC int64_t p29_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p29 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p29_shape,
    .strides = p29_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p290_shape[4] = {1, 1, 1, 32};
ANT_ML_STATIC int64_t p290_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p290 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p290_shape,
    .strides = p290_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p291_shape[4] = {1, 1, 1, 32};
ANT_ML_STATIC int64_t p291_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p291 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p291_shape,
    .strides = p291_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p292_shape[4] = {1, 1, 1, 32};
ANT_ML_STATIC int64_t p292_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p292 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p292_shape,
    .strides = p292_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p293_shape[4] = {1, 1, 1, 32};
ANT_ML_STATIC int64_t p293_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p293 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p293_shape,
    .strides = p293_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p294_shape[0] = {};
ANT_ML_STATIC int64_t p294_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p294 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p294_shape,
    .strides = p294_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p295_shape[4] = {1, 1, 32, 128};
ANT_ML_STATIC int64_t p295_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p295 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p295_shape,
    .strides = p295_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p296_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p296_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p296 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p296_shape,
    .strides = p296_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p297_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p297_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p297 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p297_shape,
    .strides = p297_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p298_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p298_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p298 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p298_shape,
    .strides = p298_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p299_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p299_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p299 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p299_shape,
    .strides = p299_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p3_shape[4] = {1, 1, 1, 12};
ANT_ML_STATIC int64_t p3_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p3 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p3_shape,
    .strides = p3_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p30_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p30_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p30 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p30_shape,
    .strides = p30_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p300_shape[4] = {1, 1, 128, 2};
ANT_ML_STATIC int64_t p300_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p300 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p300_shape,
    .strides = p300_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p301_shape[4] = {1, 1, 1, 2};
ANT_ML_STATIC int64_t p301_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p301 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p301_shape,
    .strides = p301_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p302_shape[4] = {1, 1, 1, 2};
ANT_ML_STATIC int64_t p302_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p302 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p302_shape,
    .strides = p302_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p303_shape[4] = {1, 1, 1, 2};
ANT_ML_STATIC int64_t p303_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p303 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p303_shape,
    .strides = p303_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p304_shape[4] = {1, 1, 1, 2};
ANT_ML_STATIC int64_t p304_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p304 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p304_shape,
    .strides = p304_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p31_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p31_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p31 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p31_shape,
    .strides = p31_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p32_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p32_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p32 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p32_shape,
    .strides = p32_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p33_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p33_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p33 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p33_shape,
    .strides = p33_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p34_shape[4] = {1, 1, 28, 24};
ANT_ML_STATIC int64_t p34_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p34 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p34_shape,
    .strides = p34_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p35_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p35_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p35 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p35_shape,
    .strides = p35_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p36_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p36_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p36 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p36_shape,
    .strides = p36_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p37_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p37_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p37 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p37_shape,
    .strides = p37_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p38_shape[4] = {1, 1, 1, 24};
ANT_ML_STATIC int64_t p38_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p38 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p38_shape,
    .strides = p38_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p39_shape[0] = {};
ANT_ML_STATIC int64_t p39_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p39 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 32, 1},
    .shape = p39_shape,
    .strides = p39_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p4_shape[4] = {1, 1, 1, 12};
ANT_ML_STATIC int64_t p4_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p4 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p4_shape,
    .strides = p4_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p40_shape[1] = {1};
ANT_ML_STATIC int64_t p40_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p40 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p40_shape,
    .strides = p40_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p41_shape[0] = {};
ANT_ML_STATIC int64_t p41_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p41 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p41_shape,
    .strides = p41_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p42_shape[4] = {1, 1, 24, 28};
ANT_ML_STATIC int64_t p42_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p42 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p42_shape,
    .strides = p42_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p43_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p43_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p43 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p43_shape,
    .strides = p43_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p44_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p44_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p44 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p44_shape,
    .strides = p44_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p45_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p45_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p45 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p45_shape,
    .strides = p45_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p46_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p46_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p46 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p46_shape,
    .strides = p46_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p47_shape[4] = {3, 3, 28, 1};
ANT_ML_STATIC int64_t p47_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p47 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p47_shape,
    .strides = p47_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p48_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p48_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p48 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p48_shape,
    .strides = p48_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p49_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p49_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p49 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p49_shape,
    .strides = p49_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p5_shape[4] = {1, 1, 1, 12};
ANT_ML_STATIC int64_t p5_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p5 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p5_shape,
    .strides = p5_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p50_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p50_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p50 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p50_shape,
    .strides = p50_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p51_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p51_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p51 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p51_shape,
    .strides = p51_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p52_shape[4] = {1, 1, 28, 20};
ANT_ML_STATIC int64_t p52_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p52 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p52_shape,
    .strides = p52_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p53_shape[4] = {1, 1, 1, 20};
ANT_ML_STATIC int64_t p53_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p53 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p53_shape,
    .strides = p53_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p54_shape[4] = {1, 1, 1, 20};
ANT_ML_STATIC int64_t p54_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p54 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p54_shape,
    .strides = p54_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p55_shape[4] = {1, 1, 1, 20};
ANT_ML_STATIC int64_t p55_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p55 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p55_shape,
    .strides = p55_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p56_shape[4] = {1, 1, 1, 20};
ANT_ML_STATIC int64_t p56_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p56 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p56_shape,
    .strides = p56_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p57_shape[0] = {};
ANT_ML_STATIC int64_t p57_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p57 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 32, 1},
    .shape = p57_shape,
    .strides = p57_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p58_shape[1] = {1};
ANT_ML_STATIC int64_t p58_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p58 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p58_shape,
    .strides = p58_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p59_shape[0] = {};
ANT_ML_STATIC int64_t p59_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p59 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p59_shape,
    .strides = p59_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p6_shape[0] = {};
ANT_ML_STATIC int64_t p6_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p6 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 32, 1},
    .shape = p6_shape,
    .strides = p6_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p60_shape[4] = {1, 1, 24, 128};
ANT_ML_STATIC int64_t p60_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p60 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p60_shape,
    .strides = p60_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p61_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p61_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p61 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p61_shape,
    .strides = p61_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p62_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p62_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p62 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p62_shape,
    .strides = p62_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p63_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p63_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p63 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p63_shape,
    .strides = p63_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p64_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p64_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p64 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p64_shape,
    .strides = p64_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p65_shape[4] = {3, 3, 128, 1};
ANT_ML_STATIC int64_t p65_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p65 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p65_shape,
    .strides = p65_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p66_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p66_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p66 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p66_shape,
    .strides = p66_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p67_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p67_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p67 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p67_shape,
    .strides = p67_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p68_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p68_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p68 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p68_shape,
    .strides = p68_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p69_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t p69_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p69 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p69_shape,
    .strides = p69_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p7_shape[0] = {};
ANT_ML_STATIC int64_t p7_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p7 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p7_shape,
    .strides = p7_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p70_shape[4] = {1, 1, 128, 28};
ANT_ML_STATIC int64_t p70_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p70 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p70_shape,
    .strides = p70_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p71_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p71_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p71 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p71_shape,
    .strides = p71_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p72_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p72_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p72 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p72_shape,
    .strides = p72_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p73_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p73_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p73 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p73_shape,
    .strides = p73_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p74_shape[4] = {1, 1, 1, 28};
ANT_ML_STATIC int64_t p74_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p74 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p74_shape,
    .strides = p74_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p75_shape[0] = {};
ANT_ML_STATIC int64_t p75_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p75 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 32, 1},
    .shape = p75_shape,
    .strides = p75_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p76_shape[1] = {1};
ANT_ML_STATIC int64_t p76_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p76 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p76_shape,
    .strides = p76_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p77_shape[0] = {};
ANT_ML_STATIC int64_t p77_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p77 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 32, 1},
    .shape = p77_shape,
    .strides = p77_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p78_shape[0] = {};
ANT_ML_STATIC int64_t p78_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p78 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p78_shape,
    .strides = p78_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p79_shape[4] = {1, 1, 28, 152};
ANT_ML_STATIC int64_t p79_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p79 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p79_shape,
    .strides = p79_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p8_shape[4] = {1, 1, 12, 4};
ANT_ML_STATIC int64_t p8_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p8 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p8_shape,
    .strides = p8_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p80_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p80_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p80 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p80_shape,
    .strides = p80_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p81_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p81_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p81 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p81_shape,
    .strides = p81_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p82_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p82_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p82 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p82_shape,
    .strides = p82_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p83_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p83_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p83 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p83_shape,
    .strides = p83_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p84_shape[4] = {3, 3, 152, 1};
ANT_ML_STATIC int64_t p84_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p84 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p84_shape,
    .strides = p84_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p85_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p85_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p85 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p85_shape,
    .strides = p85_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p86_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p86_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p86 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p86_shape,
    .strides = p86_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p87_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p87_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p87 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p87_shape,
    .strides = p87_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p88_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p88_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p88 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p88_shape,
    .strides = p88_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p89_shape[4] = {1, 1, 152, 16};
ANT_ML_STATIC int64_t p89_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p89 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p89_shape,
    .strides = p89_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p9_shape[4] = {1, 1, 1, 4};
ANT_ML_STATIC int64_t p9_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p9 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p9_shape,
    .strides = p9_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p90_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p90_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p90 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p90_shape,
    .strides = p90_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p91_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p91_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p91 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p91_shape,
    .strides = p91_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p92_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p92_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p92 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p92_shape,
    .strides = p92_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p93_shape[4] = {1, 1, 1, 16};
ANT_ML_STATIC int64_t p93_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p93 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p93_shape,
    .strides = p93_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p94_shape[1] = {1};
ANT_ML_STATIC int64_t p94_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p94 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p94_shape,
    .strides = p94_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p95_shape[1] = {1};
ANT_ML_STATIC int64_t p95_strides[1] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p95 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 1,
    .dtype = {kDLInt, 32, 1},
    .shape = p95_shape,
    .strides = p95_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p96_shape[0] = {};
ANT_ML_STATIC int64_t p96_strides[0] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p96 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 0,
    .dtype = {kDLInt, 16, 1},
    .shape = p96_shape,
    .strides = p96_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p97_shape[4] = {1, 1, 28, 152};
ANT_ML_STATIC int64_t p97_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p97 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = p97_shape,
    .strides = p97_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p98_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p98_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p98 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = p98_shape,
    .strides = p98_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t p99_shape[4] = {1, 1, 1, 152};
ANT_ML_STATIC int64_t p99_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor p99 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 64, 1},
    .shape = p99_shape,
    .strides = p99_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 

//
// Inputs:
//
ANT_ML_STATIC int64_t input_shape[4] = {1, 50, 50, 1};
ANT_ML_STATIC int64_t input_strides[4] = {};
ANT_ML_STATIC float input_quant_scale[1] = { 0.007549178320914507, };
ANT_ML_STATIC int32_t input_quant_zero_point[1] = { -72, };
ANT_ML_STATIC ant_ml_quantization_info input_quant = {
  .scale = input_quant_scale,
  .zero_point = input_quant_zero_point,
  .dim = -1
};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor input = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = input_shape,
    .strides = input_strides,
    .byte_offset = 0
  },
  .quant = &input_quant 
}; 


ANT_ML_STATIC ant_ml_tensor * _InputsList[] = {
  &input, // [0]
}; 

//
// Activations:
//
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_0_shape[4] = {1, 50, 50, 1};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__0_shape[4] = {1, 25, 25, 12};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_0_shape[4] = {1, 25, 25, 4};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_0_shape[4] = {1, 25, 25, 4};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__0_shape[4] = {1, 25, 25, 8};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_0_shape[4] = {1, 25, 25, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_0_shape[4] = {1, 13, 13, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_0_shape[4] = {1, 13, 13, 24};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_1_0_shape[4] = {1, 13, 13, 24};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_1_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_1_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_1_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_1_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_0_shape[4] = {1, 13, 13, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_0_shape[4] = {1, 13, 13, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_0_shape[4] = {1, 13, 13, 20};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__0_shape[4] = {1, 13, 13, 24};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__0_shape,
    .strides = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_0_shape[4] = {1, 13, 13, 128};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_0_shape[4] = {1, 7, 7, 128};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_0_shape[4] = {1, 7, 7, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_2_0_shape[4] = {1, 7, 7, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_2_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_2_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_2_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_2_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_0_shape[4] = {1, 7, 7, 152};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_0_shape[4] = {1, 7, 7, 152};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_0_shape[4] = {1, 7, 7, 16};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__0_shape[4] = {1, 7, 7, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__0_shape,
    .strides = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_21_0_shape[4] = {1, 7, 7, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_21_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_21_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_21_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_21_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_0_shape[4] = {1, 7, 7, 152};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_0_shape[4] = {1, 7, 7, 152};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__0_shape[4] = {1, 7, 7, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__0_shape,
    .strides = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_0_shape[4] = {1, 7, 7, 152};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_0_shape[4] = {1, 4, 4, 152};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_0_shape[4] = {1, 4, 4, 24};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_3_0_shape[4] = {1, 4, 4, 24};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_3_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_3_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_3_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_3_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_0_shape[4] = {1, 4, 4, 112};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_0_shape[4] = {1, 4, 4, 112};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__0_shape[4] = {1, 4, 4, 24};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__0_shape,
    .strides = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_31_0_shape[4] = {1, 4, 4, 24};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_31_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_31_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_31_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_31_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_0_shape[4] = {1, 4, 4, 112};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_0_shape[4] = {1, 4, 4, 112};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_0_shape[4] = {1, 4, 4, 12};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_0_shape[4] = {1, 4, 4, 24};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_pad_0_shape[4] = {1, 4, 4, 32};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_pad_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_pad_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_nn_pad_0_shape,
    .strides = tvmgen_default_fused_nn_pad_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_32_0_shape[4] = {1, 4, 4, 24};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_32_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_32_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_32_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_32_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_0_shape[4] = {1, 4, 4, 76};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_0_shape[4] = {1, 4, 4, 76};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_0_shape[4] = {1, 4, 4, 32};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_0_shape[4] = {1, 4, 4, 76};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_0_shape[4] = {1, 4, 4, 76};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_0_shape[4] = {1, 4, 4, 16};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_pad_1_0_shape[4] = {1, 4, 4, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_pad_1_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_pad_1_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_nn_pad_1_0_shape,
    .strides = tvmgen_default_fused_nn_pad_1_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_4_0_shape[4] = {1, 4, 4, 16};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_4_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_4_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_4_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_4_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_0_shape[4] = {1, 4, 4, 56};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_0_shape[4] = {1, 4, 4, 56};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_0_shape[4] = {1, 4, 4, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_5_0_shape[4] = {1, 4, 4, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_5_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_5_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_5_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_5_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_0_shape[4] = {1, 4, 4, 56};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_0_shape[4] = {1, 4, 4, 56};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_0_shape[4] = {1, 4, 4, 28};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_0_shape[4] = {1, 4, 4, 56};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_0_shape[4] = {1, 2, 2, 56};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_0_shape[4] = {1, 2, 2, 16};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_6_0_shape[4] = {1, 2, 2, 16};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_6_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_6_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_6_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_6_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_0_shape[4] = {1, 2, 2, 96};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_0_shape[4] = {1, 2, 2, 96};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_0_shape[4] = {1, 2, 2, 16};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_61_0_shape[4] = {1, 2, 2, 16};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_61_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_61_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_61_0_shape,
    .strides = tvmgen_default_fused_cast_subtract_61_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_0_shape[4] = {1, 2, 2, 96};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_0_shape[4] = {1, 2, 2, 96};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__0_shape[4] = {1, 2, 2, 16};
ANT_ML_STATIC int64_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__0_shape,
    .strides = tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_0_shape[4] = {1, 2, 2, 96};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_0_shape[4] = {1, 2, 2, 96};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_0_shape[4] = {1, 2, 2, 32};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_0_shape[4] = {1, 2, 2, 128};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 32, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_0_shape[4] = {1, 1, 1, 128};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 16, 1},
    .shape = tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_0_shape,
    .strides = tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_0_shape[4] = {1, 1, 1, 2};
ANT_ML_STATIC int64_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_0_strides[4] = {};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 4,
    .dtype = {kDLInt, 8, 1},
    .shape = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_0_shape,
    .strides = tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_0_strides,
    .byte_offset = 0
  },
  .quant = NULL 
}; 

//
// Outputs:
//
ANT_ML_STATIC int64_t reshape_nop_0_shape[2] = {1, 2};
ANT_ML_STATIC int64_t reshape_nop_0_strides[2] = {};
ANT_ML_STATIC float reshape_nop_0_quant_scale[1] = { 0.05457817763090134, };
ANT_ML_STATIC int32_t reshape_nop_0_quant_zero_point[1] = { 34, };
ANT_ML_STATIC ant_ml_quantization_info reshape_nop_0_quant = {
  .scale = reshape_nop_0_quant_scale,
  .zero_point = reshape_nop_0_quant_zero_point,
  .dim = -1
};

ANT_ML_ALIGNED(8) ANT_ML_STATIC ant_ml_tensor reshape_nop_0 = {
  .dltensor = {
    .data = (ant_ml_ptr)(NULL),
    .device = {kDLCPU,0},
    .ndim = 2,
    .dtype = {kDLInt, 8, 1},
    .shape = reshape_nop_0_shape,
    .strides = reshape_nop_0_strides,
    .byte_offset = 0
  },
  .quant = &reshape_nop_0_quant 
}; 


ANT_ML_STATIC ant_ml_tensor * _OutputsList[] = {
  &reshape_nop_0, // [0]
}; 


//
// Network: network
//
TVM_DLL int32_t tvmgen_default_fused_cast_subtract(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245_(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1_(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_1(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5_(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_2(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b_(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_2(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4_(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_3(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9_(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_3(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_pad(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_3(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_pad_1(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_4(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_5(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_6(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_6(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078_(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract(void * args, void * arg_type_ids, int32_t num_args); 
TVM_DLL int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8(void * args, void * arg_type_ids, int32_t num_args); 


// ============================================================
//   network_configure_activations
// ============================================================
ANT_ML_STATIC ANT_ML_INLINE
ant_ml_status network_configure_activations (
  const ant_ml_ptr activations
)
{
  if (activations == NULL) {
    TVMAPISetLastError ("Non-null activations arena is required for this model.");
    return ANT_ML_STATUS_ERROR;
  }

  //
  // input: storage_id:0
  //
  input.dltensor.data = (ant_ml_ptr)(activations + 75506);

  //
  // tvmgen_default_fused_cast_subtract_0: storage_id:2
  //
  tvmgen_default_fused_cast_subtract_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__0: storage_id:10
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__0.dltensor.data = (ant_ml_ptr)(activations + 14896);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_0: storage_id:10
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_0.dltensor.data = (ant_ml_ptr)(activations + 14896);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_0: storage_id:10
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_0.dltensor.data = (ant_ml_ptr)(activations + 14896);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_0: storage_id:10
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_0.dltensor.data = (ant_ml_ptr)(activations + 14896);

  //
  // tvmgen_default_fused_cast_subtract_1_0: storage_id:2
  //
  tvmgen_default_fused_cast_subtract_1_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__0: storage_id:2
  //
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_0: storage_id:10
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_0.dltensor.data = (ant_ml_ptr)(activations + 14896);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_cast_subtract_2_0: storage_id:2
  //
  tvmgen_default_fused_cast_subtract_2_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_0: storage_id:10
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_0.dltensor.data = (ant_ml_ptr)(activations + 14896);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_0: storage_id:98
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__0: storage_id:2
  //
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_cast_subtract_21_0: storage_id:50
  //
  tvmgen_default_fused_cast_subtract_21_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_0: storage_id:10
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_0.dltensor.data = (ant_ml_ptr)(activations + 14896);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__0: storage_id:10
  //
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__0.dltensor.data = (ant_ml_ptr)(activations + 14896);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_0: storage_id:98
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_cast_subtract_3_0: storage_id:136
  //
  tvmgen_default_fused_cast_subtract_3_0.dltensor.data = (ant_ml_ptr)(activations + 73840);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__0: storage_id:136
  //
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__0.dltensor.data = (ant_ml_ptr)(activations + 73840);

  //
  // tvmgen_default_fused_cast_subtract_31_0: storage_id:98
  //
  tvmgen_default_fused_cast_subtract_31_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_0: storage_id:98
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_0: storage_id:169
  //
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_0.dltensor.data = (ant_ml_ptr)(activations + 74736);

  //
  // tvmgen_default_fused_nn_pad_0: storage_id:136
  //
  tvmgen_default_fused_nn_pad_0.dltensor.data = (ant_ml_ptr)(activations + 73840);

  //
  // tvmgen_default_fused_cast_subtract_32_0: storage_id:98
  //
  tvmgen_default_fused_cast_subtract_32_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_0: storage_id:2
  //
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_0: storage_id:169
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_0.dltensor.data = (ant_ml_ptr)(activations + 74736);

  //
  // tvmgen_default_fused_nn_pad_1_0: storage_id:136
  //
  tvmgen_default_fused_nn_pad_1_0.dltensor.data = (ant_ml_ptr)(activations + 73840);

  //
  // tvmgen_default_fused_cast_subtract_4_0: storage_id:98
  //
  tvmgen_default_fused_cast_subtract_4_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_0: storage_id:98
  //
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_cast_subtract_5_0: storage_id:136
  //
  tvmgen_default_fused_cast_subtract_5_0.dltensor.data = (ant_ml_ptr)(activations + 73840);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_0: storage_id:136
  //
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_0.dltensor.data = (ant_ml_ptr)(activations + 73840);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_0: storage_id:50
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_0.dltensor.data = (ant_ml_ptr)(activations + 58160);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_0: storage_id:98
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_0: storage_id:169
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_0.dltensor.data = (ant_ml_ptr)(activations + 74736);

  //
  // tvmgen_default_fused_cast_subtract_6_0: storage_id:98
  //
  tvmgen_default_fused_cast_subtract_6_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_0: storage_id:136
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_0.dltensor.data = (ant_ml_ptr)(activations + 73840);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_0: storage_id:98
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_0: storage_id:136
  //
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_0.dltensor.data = (ant_ml_ptr)(activations + 73840);

  //
  // tvmgen_default_fused_cast_subtract_61_0: storage_id:169
  //
  tvmgen_default_fused_cast_subtract_61_0.dltensor.data = (ant_ml_ptr)(activations + 74736);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_0: storage_id:98
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_0: storage_id:169
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_0.dltensor.data = (ant_ml_ptr)(activations + 74736);

  //
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__0: storage_id:98
  //
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_0: storage_id:169
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_0.dltensor.data = (ant_ml_ptr)(activations + 74736);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_0: storage_id:98
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_0.dltensor.data = (ant_ml_ptr)(activations + 73056);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_0: storage_id:169
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_0.dltensor.data = (ant_ml_ptr)(activations + 74736);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_0: storage_id:2
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_0.dltensor.data = (ant_ml_ptr)(activations + 0);

  //
  // tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_0: storage_id:169
  //
  tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_0.dltensor.data = (ant_ml_ptr)(activations + 74736);

  //
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_0: storage_id:312
  //
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_0.dltensor.data = (ant_ml_ptr)(activations + 75504);

  //
  // reshape_nop_0: storage_id:312
  //
  reshape_nop_0.dltensor.data = (ant_ml_ptr)(activations + 75504);

  return ANT_ML_STATUS_OK;
}

// ============================================================
//   network_configure_weights
// ============================================================
ANT_ML_STATIC ANT_ML_INLINE
ant_ml_status network_configure_weights (
  const ant_ml_ptr weights
)
{
  if (weights == NULL) {
    TVMAPISetLastError("Non-null weights arena is required for this model.");
    return ANT_ML_STATUS_ERROR;
  }
  //
  //  p0
  //
  p0.dltensor.data = (ant_ml_ptr)(weights + 0);
  //
  //  p1
  //
  p1.dltensor.data = (ant_ml_ptr)(weights + 2);
  //
  //  p10
  //
  p10.dltensor.data = (ant_ml_ptr)(weights + 224);
  //
  //  p100
  //
  p100.dltensor.data = (ant_ml_ptr)(weights + 256);
  //
  //  p101
  //
  p101.dltensor.data = (ant_ml_ptr)(weights + 1472);
  //
  //  p102
  //
  p102.dltensor.data = (ant_ml_ptr)(weights + 2688);
  //
  //  p103
  //
  p103.dltensor.data = (ant_ml_ptr)(weights + 5424);
  //
  //  p104
  //
  p104.dltensor.data = (ant_ml_ptr)(weights + 6032);
  //
  //  p105
  //
  p105.dltensor.data = (ant_ml_ptr)(weights + 7248);
  //
  //  p106
  //
  p106.dltensor.data = (ant_ml_ptr)(weights + 8464);
  //
  //  p107
  //
  p107.dltensor.data = (ant_ml_ptr)(weights + 9680);
  //
  //  p108
  //
  p108.dltensor.data = (ant_ml_ptr)(weights + 18192);
  //
  //  p109
  //
  p109.dltensor.data = (ant_ml_ptr)(weights + 18304);
  //
  //  p11
  //
  p11.dltensor.data = (ant_ml_ptr)(weights + 18528);
  //
  //  p110
  //
  p110.dltensor.data = (ant_ml_ptr)(weights + 18560);
  //
  //  p111
  //
  p111.dltensor.data = (ant_ml_ptr)(weights + 18784);
  //
  //  p112
  //
  p112.dltensor.data = (ant_ml_ptr)(weights + 19008);
  //
  //  p113
  //
  p113.dltensor.data = (ant_ml_ptr)(weights + 19010);
  //
  //  p114
  //
  p114.dltensor.data = (ant_ml_ptr)(weights + 27524);
  //
  //  p115
  //
  p115.dltensor.data = (ant_ml_ptr)(weights + 28136);
  //
  //  p116
  //
  p116.dltensor.data = (ant_ml_ptr)(weights + 29352);
  //
  //  p117
  //
  p117.dltensor.data = (ant_ml_ptr)(weights + 30568);
  //
  //  p118
  //
  p118.dltensor.data = (ant_ml_ptr)(weights + 31784);
  //
  //  p119
  //
  p119.dltensor.data = (ant_ml_ptr)(weights + 34520);
  //
  //  p12
  //
  p12.dltensor.data = (ant_ml_ptr)(weights + 35128);
  //
  //  p120
  //
  p120.dltensor.data = (ant_ml_ptr)(weights + 35160);
  //
  //  p121
  //
  p121.dltensor.data = (ant_ml_ptr)(weights + 36376);
  //
  //  p122
  //
  p122.dltensor.data = (ant_ml_ptr)(weights + 37592);
  //
  //  p123
  //
  p123.dltensor.data = (ant_ml_ptr)(weights + 38808);
  //
  //  p124
  //
  p124.dltensor.data = (ant_ml_ptr)(weights + 46104);
  //
  //  p125
  //
  p125.dltensor.data = (ant_ml_ptr)(weights + 46200);
  //
  //  p126
  //
  p126.dltensor.data = (ant_ml_ptr)(weights + 46392);
  //
  //  p127
  //
  p127.dltensor.data = (ant_ml_ptr)(weights + 46584);
  //
  //  p128
  //
  p128.dltensor.data = (ant_ml_ptr)(weights + 46776);
  //
  //  p129
  //
  p129.dltensor.data = (ant_ml_ptr)(weights + 46780);
  //
  //  p13
  //
  p13.dltensor.data = (ant_ml_ptr)(weights + 46784);
  //
  //  p130
  //
  p130.dltensor.data = (ant_ml_ptr)(weights + 46856);
  //
  //  p131
  //
  p131.dltensor.data = (ant_ml_ptr)(weights + 46858);
  //
  //  p132
  //
  p132.dltensor.data = (ant_ml_ptr)(weights + 52236);
  //
  //  p133
  //
  p133.dltensor.data = (ant_ml_ptr)(weights + 52688);
  //
  //  p134
  //
  p134.dltensor.data = (ant_ml_ptr)(weights + 53584);
  //
  //  p135
  //
  p135.dltensor.data = (ant_ml_ptr)(weights + 54480);
  //
  //  p136
  //
  p136.dltensor.data = (ant_ml_ptr)(weights + 55376);
  //
  //  p137
  //
  p137.dltensor.data = (ant_ml_ptr)(weights + 57392);
  //
  //  p138
  //
  p138.dltensor.data = (ant_ml_ptr)(weights + 57840);
  //
  //  p139
  //
  p139.dltensor.data = (ant_ml_ptr)(weights + 58736);
  //
  //  p14
  //
  p14.dltensor.data = (ant_ml_ptr)(weights + 59632);
  //
  //  p140
  //
  p140.dltensor.data = (ant_ml_ptr)(weights + 59648);
  //
  //  p141
  //
  p141.dltensor.data = (ant_ml_ptr)(weights + 60544);
  //
  //  p142
  //
  p142.dltensor.data = (ant_ml_ptr)(weights + 65920);
  //
  //  p143
  //
  p143.dltensor.data = (ant_ml_ptr)(weights + 66016);
  //
  //  p144
  //
  p144.dltensor.data = (ant_ml_ptr)(weights + 66208);
  //
  //  p145
  //
  p145.dltensor.data = (ant_ml_ptr)(weights + 66400);
  //
  //  p146
  //
  p146.dltensor.data = (ant_ml_ptr)(weights + 66592);
  //
  //  p147
  //
  p147.dltensor.data = (ant_ml_ptr)(weights + 66596);
  //
  //  p148
  //
  p148.dltensor.data = (ant_ml_ptr)(weights + 66598);
  //
  //  p149
  //
  p149.dltensor.data = (ant_ml_ptr)(weights + 71976);
  //
  //  p15
  //
  p15.dltensor.data = (ant_ml_ptr)(weights + 72424);
  //
  //  p150
  //
  p150.dltensor.data = (ant_ml_ptr)(weights + 72456);
  //
  //  p151
  //
  p151.dltensor.data = (ant_ml_ptr)(weights + 73352);
  //
  //  p152
  //
  p152.dltensor.data = (ant_ml_ptr)(weights + 74248);
  //
  //  p153
  //
  p153.dltensor.data = (ant_ml_ptr)(weights + 75144);
  //
  //  p154
  //
  p154.dltensor.data = (ant_ml_ptr)(weights + 77160);
  //
  //  p155
  //
  p155.dltensor.data = (ant_ml_ptr)(weights + 77608);
  //
  //  p156
  //
  p156.dltensor.data = (ant_ml_ptr)(weights + 78504);
  //
  //  p157
  //
  p157.dltensor.data = (ant_ml_ptr)(weights + 79400);
  //
  //  p158
  //
  p158.dltensor.data = (ant_ml_ptr)(weights + 80296);
  //
  //  p159
  //
  p159.dltensor.data = (ant_ml_ptr)(weights + 82984);
  //
  //  p16
  //
  p16.dltensor.data = (ant_ml_ptr)(weights + 83032);
  //
  //  p160
  //
  p160.dltensor.data = (ant_ml_ptr)(weights + 83064);
  //
  //  p161
  //
  p161.dltensor.data = (ant_ml_ptr)(weights + 83160);
  //
  //  p162
  //
  p162.dltensor.data = (ant_ml_ptr)(weights + 83256);
  //
  //  p163
  //
  p163.dltensor.data = (ant_ml_ptr)(weights + 83352);
  //
  //  p164
  //
  p164.dltensor.data = (ant_ml_ptr)(weights + 83356);
  //
  //  p165
  //
  p165.dltensor.data = (ant_ml_ptr)(weights + 87004);
  //
  //  p166
  //
  p166.dltensor.data = (ant_ml_ptr)(weights + 87312);
  //
  //  p167
  //
  p167.dltensor.data = (ant_ml_ptr)(weights + 87920);
  //
  //  p168
  //
  p168.dltensor.data = (ant_ml_ptr)(weights + 88528);
  //
  //  p169
  //
  p169.dltensor.data = (ant_ml_ptr)(weights + 89136);
  //
  //  p17
  //
  p17.dltensor.data = (ant_ml_ptr)(weights + 90504);
  //
  //  p170
  //
  p170.dltensor.data = (ant_ml_ptr)(weights + 90536);
  //
  //  p171
  //
  p171.dltensor.data = (ant_ml_ptr)(weights + 90840);
  //
  //  p172
  //
  p172.dltensor.data = (ant_ml_ptr)(weights + 91448);
  //
  //  p173
  //
  p173.dltensor.data = (ant_ml_ptr)(weights + 92056);
  //
  //  p174
  //
  p174.dltensor.data = (ant_ml_ptr)(weights + 92664);
  //
  //  p175
  //
  p175.dltensor.data = (ant_ml_ptr)(weights + 97528);
  //
  //  p176
  //
  p176.dltensor.data = (ant_ml_ptr)(weights + 97656);
  //
  //  p177
  //
  p177.dltensor.data = (ant_ml_ptr)(weights + 97912);
  //
  //  p178
  //
  p178.dltensor.data = (ant_ml_ptr)(weights + 98168);
  //
  //  p179
  //
  p179.dltensor.data = (ant_ml_ptr)(weights + 98424);
  //
  //  p18
  //
  p18.dltensor.data = (ant_ml_ptr)(weights + 98428);
  //
  //  p180
  //
  p180.dltensor.data = (ant_ml_ptr)(weights + 98492);
  //
  //  p181
  //
  p181.dltensor.data = (ant_ml_ptr)(weights + 98494);
  //
  //  p182
  //
  p182.dltensor.data = (ant_ml_ptr)(weights + 103360);
  //
  //  p183
  //
  p183.dltensor.data = (ant_ml_ptr)(weights + 103664);
  //
  //  p184
  //
  p184.dltensor.data = (ant_ml_ptr)(weights + 104272);
  //
  //  p185
  //
  p185.dltensor.data = (ant_ml_ptr)(weights + 104880);
  //
  //  p186
  //
  p186.dltensor.data = (ant_ml_ptr)(weights + 105488);
  //
  //  p187
  //
  p187.dltensor.data = (ant_ml_ptr)(weights + 106856);
  //
  //  p188
  //
  p188.dltensor.data = (ant_ml_ptr)(weights + 107160);
  //
  //  p189
  //
  p189.dltensor.data = (ant_ml_ptr)(weights + 107768);
  //
  //  p19
  //
  p19.dltensor.data = (ant_ml_ptr)(weights + 108376);
  //
  //  p190
  //
  p190.dltensor.data = (ant_ml_ptr)(weights + 108408);
  //
  //  p191
  //
  p191.dltensor.data = (ant_ml_ptr)(weights + 109016);
  //
  //  p192
  //
  p192.dltensor.data = (ant_ml_ptr)(weights + 111448);
  //
  //  p193
  //
  p193.dltensor.data = (ant_ml_ptr)(weights + 111512);
  //
  //  p194
  //
  p194.dltensor.data = (ant_ml_ptr)(weights + 111640);
  //
  //  p195
  //
  p195.dltensor.data = (ant_ml_ptr)(weights + 111768);
  //
  //  p196
  //
  p196.dltensor.data = (ant_ml_ptr)(weights + 111896);
  //
  //  p197
  //
  p197.dltensor.data = (ant_ml_ptr)(weights + 111900);
  //
  //  p198
  //
  p198.dltensor.data = (ant_ml_ptr)(weights + 111902);
  //
  //  p199
  //
  p199.dltensor.data = (ant_ml_ptr)(weights + 113696);
  //
  //  p2
  //
  p2.dltensor.data = (ant_ml_ptr)(weights + 113920);
  //
  //  p20
  //
  p20.dltensor.data = (ant_ml_ptr)(weights + 113968);
  //
  //  p200
  //
  p200.dltensor.data = (ant_ml_ptr)(weights + 114032);
  //
  //  p201
  //
  p201.dltensor.data = (ant_ml_ptr)(weights + 114480);
  //
  //  p202
  //
  p202.dltensor.data = (ant_ml_ptr)(weights + 114928);
  //
  //  p203
  //
  p203.dltensor.data = (ant_ml_ptr)(weights + 115376);
  //
  //  p204
  //
  p204.dltensor.data = (ant_ml_ptr)(weights + 116384);
  //
  //  p205
  //
  p205.dltensor.data = (ant_ml_ptr)(weights + 116608);
  //
  //  p206
  //
  p206.dltensor.data = (ant_ml_ptr)(weights + 117056);
  //
  //  p207
  //
  p207.dltensor.data = (ant_ml_ptr)(weights + 117504);
  //
  //  p208
  //
  p208.dltensor.data = (ant_ml_ptr)(weights + 117952);
  //
  //  p209
  //
  p209.dltensor.data = (ant_ml_ptr)(weights + 121088);
  //
  //  p21
  //
  p21.dltensor.data = (ant_ml_ptr)(weights + 121200);
  //
  //  p210
  //
  p210.dltensor.data = (ant_ml_ptr)(weights + 121264);
  //
  //  p211
  //
  p211.dltensor.data = (ant_ml_ptr)(weights + 121488);
  //
  //  p212
  //
  p212.dltensor.data = (ant_ml_ptr)(weights + 121712);
  //
  //  p213
  //
  p213.dltensor.data = (ant_ml_ptr)(weights + 121936);
  //
  //  p214
  //
  p214.dltensor.data = (ant_ml_ptr)(weights + 121940);
  //
  //  p215
  //
  p215.dltensor.data = (ant_ml_ptr)(weights + 121942);
  //
  //  p216
  //
  p216.dltensor.data = (ant_ml_ptr)(weights + 125080);
  //
  //  p217
  //
  p217.dltensor.data = (ant_ml_ptr)(weights + 125304);
  //
  //  p218
  //
  p218.dltensor.data = (ant_ml_ptr)(weights + 125752);
  //
  //  p219
  //
  p219.dltensor.data = (ant_ml_ptr)(weights + 126200);
  //
  //  p22
  //
  p22.dltensor.data = (ant_ml_ptr)(weights + 126648);
  //
  //  p220
  //
  p220.dltensor.data = (ant_ml_ptr)(weights + 126712);
  //
  //  p221
  //
  p221.dltensor.data = (ant_ml_ptr)(weights + 127720);
  //
  //  p222
  //
  p222.dltensor.data = (ant_ml_ptr)(weights + 127944);
  //
  //  p223
  //
  p223.dltensor.data = (ant_ml_ptr)(weights + 128392);
  //
  //  p224
  //
  p224.dltensor.data = (ant_ml_ptr)(weights + 128840);
  //
  //  p225
  //
  p225.dltensor.data = (ant_ml_ptr)(weights + 129288);
  //
  //  p226
  //
  p226.dltensor.data = (ant_ml_ptr)(weights + 132424);
  //
  //  p227
  //
  p227.dltensor.data = (ant_ml_ptr)(weights + 132536);
  //
  //  p228
  //
  p228.dltensor.data = (ant_ml_ptr)(weights + 132760);
  //
  //  p229
  //
  p229.dltensor.data = (ant_ml_ptr)(weights + 132984);
  //
  //  p23
  //
  p23.dltensor.data = (ant_ml_ptr)(weights + 133208);
  //
  //  p230
  //
  p230.dltensor.data = (ant_ml_ptr)(weights + 133212);
  //
  //  p231
  //
  p231.dltensor.data = (ant_ml_ptr)(weights + 133216);
  //
  //  p232
  //
  p232.dltensor.data = (ant_ml_ptr)(weights + 136352);
  //
  //  p233
  //
  p233.dltensor.data = (ant_ml_ptr)(weights + 136576);
  //
  //  p234
  //
  p234.dltensor.data = (ant_ml_ptr)(weights + 137024);
  //
  //  p235
  //
  p235.dltensor.data = (ant_ml_ptr)(weights + 137472);
  //
  //  p236
  //
  p236.dltensor.data = (ant_ml_ptr)(weights + 137920);
  //
  //  p237
  //
  p237.dltensor.data = (ant_ml_ptr)(weights + 138928);
  //
  //  p238
  //
  p238.dltensor.data = (ant_ml_ptr)(weights + 139152);
  //
  //  p239
  //
  p239.dltensor.data = (ant_ml_ptr)(weights + 139600);
  //
  //  p24
  //
  p24.dltensor.data = (ant_ml_ptr)(weights + 140048);
  //
  //  p240
  //
  p240.dltensor.data = (ant_ml_ptr)(weights + 140496);
  //
  //  p241
  //
  p241.dltensor.data = (ant_ml_ptr)(weights + 140944);
  //
  //  p242
  //
  p242.dltensor.data = (ant_ml_ptr)(weights + 142736);
  //
  //  p243
  //
  p243.dltensor.data = (ant_ml_ptr)(weights + 142800);
  //
  //  p244
  //
  p244.dltensor.data = (ant_ml_ptr)(weights + 142928);
  //
  //  p245
  //
  p245.dltensor.data = (ant_ml_ptr)(weights + 143056);
  //
  //  p246
  //
  p246.dltensor.data = (ant_ml_ptr)(weights + 143184);
  //
  //  p247
  //
  p247.dltensor.data = (ant_ml_ptr)(weights + 143188);
  //
  //  p248
  //
  p248.dltensor.data = (ant_ml_ptr)(weights + 146260);
  //
  //  p249
  //
  p249.dltensor.data = (ant_ml_ptr)(weights + 146648);
  //
  //  p25
  //
  p25.dltensor.data = (ant_ml_ptr)(weights + 147416);
  //
  //  p250
  //
  p250.dltensor.data = (ant_ml_ptr)(weights + 147528);
  //
  //  p251
  //
  p251.dltensor.data = (ant_ml_ptr)(weights + 148296);
  //
  //  p252
  //
  p252.dltensor.data = (ant_ml_ptr)(weights + 149064);
  //
  //  p253
  //
  p253.dltensor.data = (ant_ml_ptr)(weights + 150792);
  //
  //  p254
  //
  p254.dltensor.data = (ant_ml_ptr)(weights + 151176);
  //
  //  p255
  //
  p255.dltensor.data = (ant_ml_ptr)(weights + 151944);
  //
  //  p256
  //
  p256.dltensor.data = (ant_ml_ptr)(weights + 152712);
  //
  //  p257
  //
  p257.dltensor.data = (ant_ml_ptr)(weights + 153480);
  //
  //  p258
  //
  p258.dltensor.data = (ant_ml_ptr)(weights + 156552);
  //
  //  p259
  //
  p259.dltensor.data = (ant_ml_ptr)(weights + 156616);
  //
  //  p26
  //
  p26.dltensor.data = (ant_ml_ptr)(weights + 156744);
  //
  //  p260
  //
  p260.dltensor.data = (ant_ml_ptr)(weights + 156968);
  //
  //  p261
  //
  p261.dltensor.data = (ant_ml_ptr)(weights + 157096);
  //
  //  p262
  //
  p262.dltensor.data = (ant_ml_ptr)(weights + 157224);
  //
  //  p263
  //
  p263.dltensor.data = (ant_ml_ptr)(weights + 157228);
  //
  //  p264
  //
  p264.dltensor.data = (ant_ml_ptr)(weights + 157232);
  //
  //  p265
  //
  p265.dltensor.data = (ant_ml_ptr)(weights + 160304);
  //
  //  p266
  //
  p266.dltensor.data = (ant_ml_ptr)(weights + 160688);
  //
  //  p267
  //
  p267.dltensor.data = (ant_ml_ptr)(weights + 161456);
  //
  //  p268
  //
  p268.dltensor.data = (ant_ml_ptr)(weights + 162224);
  //
  //  p269
  //
  p269.dltensor.data = (ant_ml_ptr)(weights + 162992);
  //
  //  p27
  //
  p27.dltensor.data = (ant_ml_ptr)(weights + 164720);
  //
  //  p270
  //
  p270.dltensor.data = (ant_ml_ptr)(weights + 164944);
  //
  //  p271
  //
  p271.dltensor.data = (ant_ml_ptr)(weights + 165328);
  //
  //  p272
  //
  p272.dltensor.data = (ant_ml_ptr)(weights + 166096);
  //
  //  p273
  //
  p273.dltensor.data = (ant_ml_ptr)(weights + 166864);
  //
  //  p274
  //
  p274.dltensor.data = (ant_ml_ptr)(weights + 167632);
  //
  //  p275
  //
  p275.dltensor.data = (ant_ml_ptr)(weights + 170704);
  //
  //  p276
  //
  p276.dltensor.data = (ant_ml_ptr)(weights + 170768);
  //
  //  p277
  //
  p277.dltensor.data = (ant_ml_ptr)(weights + 170896);
  //
  //  p278
  //
  p278.dltensor.data = (ant_ml_ptr)(weights + 171024);
  //
  //  p279
  //
  p279.dltensor.data = (ant_ml_ptr)(weights + 171152);
  //
  //  p28
  //
  p28.dltensor.data = (ant_ml_ptr)(weights + 174224);
  //
  //  p280
  //
  p280.dltensor.data = (ant_ml_ptr)(weights + 174448);
  //
  //  p281
  //
  p281.dltensor.data = (ant_ml_ptr)(weights + 174832);
  //
  //  p282
  //
  p282.dltensor.data = (ant_ml_ptr)(weights + 175600);
  //
  //  p283
  //
  p283.dltensor.data = (ant_ml_ptr)(weights + 176368);
  //
  //  p284
  //
  p284.dltensor.data = (ant_ml_ptr)(weights + 177136);
  //
  //  p285
  //
  p285.dltensor.data = (ant_ml_ptr)(weights + 178864);
  //
  //  p286
  //
  p286.dltensor.data = (ant_ml_ptr)(weights + 179248);
  //
  //  p287
  //
  p287.dltensor.data = (ant_ml_ptr)(weights + 180016);
  //
  //  p288
  //
  p288.dltensor.data = (ant_ml_ptr)(weights + 180784);
  //
  //  p289
  //
  p289.dltensor.data = (ant_ml_ptr)(weights + 181552);
  //
  //  p29
  //
  p29.dltensor.data = (ant_ml_ptr)(weights + 187696);
  //
  //  p290
  //
  p290.dltensor.data = (ant_ml_ptr)(weights + 188200);
  //
  //  p291
  //
  p291.dltensor.data = (ant_ml_ptr)(weights + 188328);
  //
  //  p292
  //
  p292.dltensor.data = (ant_ml_ptr)(weights + 188584);
  //
  //  p293
  //
  p293.dltensor.data = (ant_ml_ptr)(weights + 188840);
  //
  //  p294
  //
  p294.dltensor.data = (ant_ml_ptr)(weights + 189096);
  //
  //  p295
  //
  p295.dltensor.data = (ant_ml_ptr)(weights + 189098);
  //
  //  p296
  //
  p296.dltensor.data = (ant_ml_ptr)(weights + 197292);
  //
  //  p297
  //
  p297.dltensor.data = (ant_ml_ptr)(weights + 197808);
  //
  //  p298
  //
  p298.dltensor.data = (ant_ml_ptr)(weights + 198832);
  //
  //  p299
  //
  p299.dltensor.data = (ant_ml_ptr)(weights + 199856);
  //
  //  p3
  //
  p3.dltensor.data = (ant_ml_ptr)(weights + 200880);
  //
  //  p30
  //
  p30.dltensor.data = (ant_ml_ptr)(weights + 200976);
  //
  //  p300
  //
  p300.dltensor.data = (ant_ml_ptr)(weights + 201088);
  //
  //  p301
  //
  p301.dltensor.data = (ant_ml_ptr)(weights + 201600);
  //
  //  p302
  //
  p302.dltensor.data = (ant_ml_ptr)(weights + 201608);
  //
  //  p303
  //
  p303.dltensor.data = (ant_ml_ptr)(weights + 201624);
  //
  //  p304
  //
  p304.dltensor.data = (ant_ml_ptr)(weights + 201640);
  //
  //  p31
  //
  p31.dltensor.data = (ant_ml_ptr)(weights + 201656);
  //
  //  p32
  //
  p32.dltensor.data = (ant_ml_ptr)(weights + 201880);
  //
  //  p33
  //
  p33.dltensor.data = (ant_ml_ptr)(weights + 202104);
  //
  //  p34
  //
  p34.dltensor.data = (ant_ml_ptr)(weights + 202328);
  //
  //  p35
  //
  p35.dltensor.data = (ant_ml_ptr)(weights + 203672);
  //
  //  p36
  //
  p36.dltensor.data = (ant_ml_ptr)(weights + 203768);
  //
  //  p37
  //
  p37.dltensor.data = (ant_ml_ptr)(weights + 203960);
  //
  //  p38
  //
  p38.dltensor.data = (ant_ml_ptr)(weights + 204152);
  //
  //  p39
  //
  p39.dltensor.data = (ant_ml_ptr)(weights + 204344);
  //
  //  p4
  //
  p4.dltensor.data = (ant_ml_ptr)(weights + 204352);
  //
  //  p40
  //
  p40.dltensor.data = (ant_ml_ptr)(weights + 204448);
  //
  //  p41
  //
  p41.dltensor.data = (ant_ml_ptr)(weights + 204452);
  //
  //  p42
  //
  p42.dltensor.data = (ant_ml_ptr)(weights + 204454);
  //
  //  p43
  //
  p43.dltensor.data = (ant_ml_ptr)(weights + 205800);
  //
  //  p44
  //
  p44.dltensor.data = (ant_ml_ptr)(weights + 205912);
  //
  //  p45
  //
  p45.dltensor.data = (ant_ml_ptr)(weights + 206136);
  //
  //  p46
  //
  p46.dltensor.data = (ant_ml_ptr)(weights + 206360);
  //
  //  p47
  //
  p47.dltensor.data = (ant_ml_ptr)(weights + 206584);
  //
  //  p48
  //
  p48.dltensor.data = (ant_ml_ptr)(weights + 207088);
  //
  //  p49
  //
  p49.dltensor.data = (ant_ml_ptr)(weights + 207200);
  //
  //  p5
  //
  p5.dltensor.data = (ant_ml_ptr)(weights + 207424);
  //
  //  p50
  //
  p50.dltensor.data = (ant_ml_ptr)(weights + 207520);
  //
  //  p51
  //
  p51.dltensor.data = (ant_ml_ptr)(weights + 207744);
  //
  //  p52
  //
  p52.dltensor.data = (ant_ml_ptr)(weights + 207968);
  //
  //  p53
  //
  p53.dltensor.data = (ant_ml_ptr)(weights + 209088);
  //
  //  p54
  //
  p54.dltensor.data = (ant_ml_ptr)(weights + 209168);
  //
  //  p55
  //
  p55.dltensor.data = (ant_ml_ptr)(weights + 209328);
  //
  //  p56
  //
  p56.dltensor.data = (ant_ml_ptr)(weights + 209488);
  //
  //  p57
  //
  p57.dltensor.data = (ant_ml_ptr)(weights + 209648);
  //
  //  p58
  //
  p58.dltensor.data = (ant_ml_ptr)(weights + 209652);
  //
  //  p59
  //
  p59.dltensor.data = (ant_ml_ptr)(weights + 209656);
  //
  //  p6
  //
  p6.dltensor.data = (ant_ml_ptr)(weights + 209660);
  //
  //  p60
  //
  p60.dltensor.data = (ant_ml_ptr)(weights + 209664);
  //
  //  p61
  //
  p61.dltensor.data = (ant_ml_ptr)(weights + 215808);
  //
  //  p62
  //
  p62.dltensor.data = (ant_ml_ptr)(weights + 216320);
  //
  //  p63
  //
  p63.dltensor.data = (ant_ml_ptr)(weights + 217344);
  //
  //  p64
  //
  p64.dltensor.data = (ant_ml_ptr)(weights + 218368);
  //
  //  p65
  //
  p65.dltensor.data = (ant_ml_ptr)(weights + 219392);
  //
  //  p66
  //
  p66.dltensor.data = (ant_ml_ptr)(weights + 221696);
  //
  //  p67
  //
  p67.dltensor.data = (ant_ml_ptr)(weights + 222208);
  //
  //  p68
  //
  p68.dltensor.data = (ant_ml_ptr)(weights + 223232);
  //
  //  p69
  //
  p69.dltensor.data = (ant_ml_ptr)(weights + 224256);
  //
  //  p7
  //
  p7.dltensor.data = (ant_ml_ptr)(weights + 225280);
  //
  //  p70
  //
  p70.dltensor.data = (ant_ml_ptr)(weights + 225282);
  //
  //  p71
  //
  p71.dltensor.data = (ant_ml_ptr)(weights + 232452);
  //
  //  p72
  //
  p72.dltensor.data = (ant_ml_ptr)(weights + 232568);
  //
  //  p73
  //
  p73.dltensor.data = (ant_ml_ptr)(weights + 232792);
  //
  //  p74
  //
  p74.dltensor.data = (ant_ml_ptr)(weights + 233016);
  //
  //  p75
  //
  p75.dltensor.data = (ant_ml_ptr)(weights + 233240);
  //
  //  p76
  //
  p76.dltensor.data = (ant_ml_ptr)(weights + 233244);
  //
  //  p77
  //
  p77.dltensor.data = (ant_ml_ptr)(weights + 233248);
  //
  //  p78
  //
  p78.dltensor.data = (ant_ml_ptr)(weights + 233252);
  //
  //  p79
  //
  p79.dltensor.data = (ant_ml_ptr)(weights + 233254);
  //
  //  p8
  //
  p8.dltensor.data = (ant_ml_ptr)(weights + 241766);
  //
  //  p80
  //
  p80.dltensor.data = (ant_ml_ptr)(weights + 241864);
  //
  //  p81
  //
  p81.dltensor.data = (ant_ml_ptr)(weights + 242472);
  //
  //  p82
  //
  p82.dltensor.data = (ant_ml_ptr)(weights + 243688);
  //
  //  p83
  //
  p83.dltensor.data = (ant_ml_ptr)(weights + 244904);
  //
  //  p84
  //
  p84.dltensor.data = (ant_ml_ptr)(weights + 246120);
  //
  //  p85
  //
  p85.dltensor.data = (ant_ml_ptr)(weights + 248856);
  //
  //  p86
  //
  p86.dltensor.data = (ant_ml_ptr)(weights + 249464);
  //
  //  p87
  //
  p87.dltensor.data = (ant_ml_ptr)(weights + 250680);
  //
  //  p88
  //
  p88.dltensor.data = (ant_ml_ptr)(weights + 251896);
  //
  //  p89
  //
  p89.dltensor.data = (ant_ml_ptr)(weights + 253112);
  //
  //  p9
  //
  p9.dltensor.data = (ant_ml_ptr)(weights + 257976);
  //
  //  p90
  //
  p90.dltensor.data = (ant_ml_ptr)(weights + 257992);
  //
  //  p91
  //
  p91.dltensor.data = (ant_ml_ptr)(weights + 258056);
  //
  //  p92
  //
  p92.dltensor.data = (ant_ml_ptr)(weights + 258184);
  //
  //  p93
  //
  p93.dltensor.data = (ant_ml_ptr)(weights + 258312);
  //
  //  p94
  //
  p94.dltensor.data = (ant_ml_ptr)(weights + 258440);
  //
  //  p95
  //
  p95.dltensor.data = (ant_ml_ptr)(weights + 258444);
  //
  //  p96
  //
  p96.dltensor.data = (ant_ml_ptr)(weights + 258448);
  //
  //  p97
  //
  p97.dltensor.data = (ant_ml_ptr)(weights + 258450);
  //
  //  p98
  //
  p98.dltensor.data = (ant_ml_ptr)(weights + 266964);
  //
  //  p99
  //
  p99.dltensor.data = (ant_ml_ptr)(weights + 267576);

  return ANT_ML_STATUS_OK;
}

// ============================================================
//   ant_ml_network_create
// ============================================================
ANT_ML_API_ENTRY
ant_ml_status ant_ml_network_create(
  const ant_ml_ptr weights,
  const ant_ml_ptr activations
)
{
  ant_ml_status status = ANT_ML_STATUS_OK;
  status = network_configure_weights (weights);
  if (status != ANT_ML_STATUS_OK) {
    return status;
  }
  status = network_configure_activations (activations);
  if (status != ANT_ML_STATUS_OK) {
    return status;
  }
  return ANT_ML_STATUS_OK;
}

// ============================================================
//   ant_ml_network_destroy
// ============================================================
ANT_ML_API_ENTRY
ant_ml_status ant_ml_network_destroy ()
{
  return ANT_ML_STATUS_OK;
}

// ============================================================
//   ant_ml_network_run
// ============================================================
ANT_ML_API_ENTRY
ant_ml_status ant_ml_network_run (
  ant_ml_tensor *inputs[],
  ant_ml_tensor *outputs[]
)
{
  // 
  // tvmgen_default_fused_cast_subtract
  // 
  TVMValue tvmgen_default_fused_cast_subtract_args[3]; 
  int32_t tvmgen_default_fused_cast_subtract_arg_type_ids[3]; 
  tvmgen_default_fused_cast_subtract_args[0].v_handle = &inputs[0]->dltensor; 
  tvmgen_default_fused_cast_subtract_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_args[1].v_handle = &p0.dltensor; 
  tvmgen_default_fused_cast_subtract_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_args[2].v_handle = &tvmgen_default_fused_cast_subtract_0.dltensor; 
  tvmgen_default_fused_cast_subtract_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract (tvmgen_default_fused_cast_subtract_args, tvmgen_default_fused_cast_subtract_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245_
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__args[0].v_handle = &tvmgen_default_fused_cast_subtract_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__args[1].v_handle = &p1.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__args[2].v_handle = &p2.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__args[3].v_handle = &p3.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__args[4].v_handle = &p4.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__args[5].v_handle = &p5.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245_  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245_ (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245_  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_args[1].v_handle = &p8.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_args[2].v_handle = &p9.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_args[3].v_handle = &p10.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_args[4].v_handle = &p11.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_args[5].v_handle = &p12.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__1_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_args[1].v_handle = &p13.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_args[2].v_handle = &p14.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_args[3].v_handle = &p15.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_args[4].v_handle = &p16.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_args[5].v_handle = &p17.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1_
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__args[8]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__arg_type_ids[8]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__2_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__args[1].v_handle = &p18.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__args[2].v_handle = &p19.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__args[3].v_handle = &p20.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__args[4].v_handle = &p21.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__args[5].v_handle = &p22.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__args[6].v_handle = &p23.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__args[7].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__arg_type_ids[7] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1_  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1_ (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__arg_type_ids, 8)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1_  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_args[1].v_handle = &p24.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_args[2].v_handle = &p25.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_args[3].v_handle = &p26.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_args[4].v_handle = &p27.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_args[5].v_handle = &p28.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__3_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_args[1].v_handle = &p29.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_args[2].v_handle = &p30.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_args[3].v_handle = &p31.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_args[4].v_handle = &p32.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_args[5].v_handle = &p33.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_args[8]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_arg_type_ids[8]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__4_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_args[1].v_handle = &p34.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_args[2].v_handle = &p35.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_args[3].v_handle = &p36.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_args[4].v_handle = &p37.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_args[5].v_handle = &p38.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_args[6].v_handle = &p39.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_args[7].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_arg_type_ids[7] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_arg_type_ids, 8)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_1
  // 
  TVMValue tvmgen_default_fused_cast_subtract_1_args[3]; 
  int32_t tvmgen_default_fused_cast_subtract_1_arg_type_ids[3]; 
  tvmgen_default_fused_cast_subtract_1_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_0.dltensor; 
  tvmgen_default_fused_cast_subtract_1_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_1_args[1].v_handle = &p41.dltensor; 
  tvmgen_default_fused_cast_subtract_1_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_1_args[2].v_handle = &tvmgen_default_fused_cast_subtract_1_0.dltensor; 
  tvmgen_default_fused_cast_subtract_1_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_1  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_1 (tvmgen_default_fused_cast_subtract_1_args, tvmgen_default_fused_cast_subtract_1_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_1  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_args[0].v_handle = &tvmgen_default_fused_cast_subtract_1_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_args[1].v_handle = &p42.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_args[2].v_handle = &p43.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_args[3].v_handle = &p44.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_args[4].v_handle = &p45.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_args[5].v_handle = &p46.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__5_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_args[1].v_handle = &p47.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_args[2].v_handle = &p48.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_args[3].v_handle = &p49.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_args[4].v_handle = &p50.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_args[5].v_handle = &p51.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_args[8]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_arg_type_ids[8]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__6_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_args[1].v_handle = &p52.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_args[2].v_handle = &p53.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_args[3].v_handle = &p54.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_args[4].v_handle = &p55.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_args[5].v_handle = &p56.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_args[6].v_handle = &p57.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_args[7].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_arg_type_ids[7] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_arg_type_ids, 8)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5_
  // 
  TVMValue tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__args[6]; 
  int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__arg_type_ids[6]; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__args[1].v_handle = &p40.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__args[2].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_1_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__args[3].v_handle = &p58.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__args[4].v_handle = &p59.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__args[5].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__arg_type_ids[5] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5_  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5_ (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__args, tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__arg_type_ids, 6)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5_  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_a436fff4d148b5d5__0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_args[1].v_handle = &p60.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_args[2].v_handle = &p61.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_args[3].v_handle = &p62.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_args[4].v_handle = &p63.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_args[5].v_handle = &p64.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__7_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_args[1].v_handle = &p65.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_args[2].v_handle = &p66.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_args[3].v_handle = &p67.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_args[4].v_handle = &p68.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_args[5].v_handle = &p69.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_args[8]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_arg_type_ids[8]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__8_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_args[1].v_handle = &p70.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_args[2].v_handle = &p71.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_args[3].v_handle = &p72.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_args[4].v_handle = &p73.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_args[5].v_handle = &p74.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_args[6].v_handle = &p75.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_args[7].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_arg_type_ids[7] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_arg_type_ids, 8)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_2
  // 
  TVMValue tvmgen_default_fused_cast_subtract_2_args[3]; 
  int32_t tvmgen_default_fused_cast_subtract_2_arg_type_ids[3]; 
  tvmgen_default_fused_cast_subtract_2_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_0.dltensor; 
  tvmgen_default_fused_cast_subtract_2_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_2_args[1].v_handle = &p78.dltensor; 
  tvmgen_default_fused_cast_subtract_2_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_2_args[2].v_handle = &tvmgen_default_fused_cast_subtract_2_0.dltensor; 
  tvmgen_default_fused_cast_subtract_2_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_2  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_2 (tvmgen_default_fused_cast_subtract_2_args, tvmgen_default_fused_cast_subtract_2_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_2  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_args[0].v_handle = &tvmgen_default_fused_cast_subtract_2_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_args[1].v_handle = &p79.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_args[2].v_handle = &p80.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_args[3].v_handle = &p81.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_args[4].v_handle = &p82.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_args[5].v_handle = &p83.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_args[1].v_handle = &p84.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_args[2].v_handle = &p85.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_args[3].v_handle = &p86.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_args[4].v_handle = &p87.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_args[5].v_handle = &p88.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_args[7]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_arg_type_ids[7]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_args[1].v_handle = &p89.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_args[2].v_handle = &p90.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_args[3].v_handle = &p91.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_args[4].v_handle = &p92.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_args[5].v_handle = &p93.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_args[6].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_arg_type_ids[6] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_arg_type_ids, 7)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b_
  // 
  TVMValue tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__args[6]; 
  int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__arg_type_ids[6]; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_2_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__args[1].v_handle = &p76.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__args[2].v_handle = &p77.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__args[3].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_3_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__args[4].v_handle = &p94.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__args[5].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__arg_type_ids[5] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b_  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b_ (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__args, tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__arg_type_ids, 6)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b_  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_2
  // 
  TVMValue tvmgen_default_fused_cast_subtract_21_args[3]; 
  int32_t tvmgen_default_fused_cast_subtract_21_arg_type_ids[3]; 
  tvmgen_default_fused_cast_subtract_21_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__0.dltensor; 
  tvmgen_default_fused_cast_subtract_21_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_21_args[1].v_handle = &p96.dltensor; 
  tvmgen_default_fused_cast_subtract_21_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_21_args[2].v_handle = &tvmgen_default_fused_cast_subtract_21_0.dltensor; 
  tvmgen_default_fused_cast_subtract_21_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_2  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_2 (tvmgen_default_fused_cast_subtract_21_args, tvmgen_default_fused_cast_subtract_21_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_2  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_args[0].v_handle = &tvmgen_default_fused_cast_subtract_21_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_args[1].v_handle = &p97.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_args[2].v_handle = &p98.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_args[3].v_handle = &p99.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_args[4].v_handle = &p100.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_args[5].v_handle = &p101.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__91_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_args[1].v_handle = &p102.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_args[2].v_handle = &p103.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_args[3].v_handle = &p104.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_args[4].v_handle = &p105.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_args[5].v_handle = &p106.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__10  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4_
  // 
  TVMValue tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[12]; 
  int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[12]; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[1].v_handle = &p95.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[2].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__101_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[3].v_handle = &p107.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[4].v_handle = &p108.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[5].v_handle = &p109.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[6].v_handle = &p110.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[7].v_handle = &p111.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[8].v_handle = &p57.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[8] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[9].v_handle = &p58.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[9] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[10].v_handle = &p112.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[10] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args[11].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids[11] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4_  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4_ (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__args, tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__arg_type_ids, 12)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4_  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_args[1].v_handle = &p113.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_args[2].v_handle = &p114.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_args[3].v_handle = &p115.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_args[4].v_handle = &p116.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_args[5].v_handle = &p117.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__9  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__92_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_args[1].v_handle = &p118.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_args[2].v_handle = &p119.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_args[3].v_handle = &p120.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_args[4].v_handle = &p121.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_args[5].v_handle = &p122.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_args[8]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_arg_type_ids[8]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__11_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_args[1].v_handle = &p123.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_args[2].v_handle = &p124.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_args[3].v_handle = &p125.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_args[4].v_handle = &p126.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_args[5].v_handle = &p127.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_args[6].v_handle = &p128.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_args[7].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_arg_type_ids[7] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_arg_type_ids, 8)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_3
  // 
  TVMValue tvmgen_default_fused_cast_subtract_3_args[3]; 
  int32_t tvmgen_default_fused_cast_subtract_3_arg_type_ids[3]; 
  tvmgen_default_fused_cast_subtract_3_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_0.dltensor; 
  tvmgen_default_fused_cast_subtract_3_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_3_args[1].v_handle = &p130.dltensor; 
  tvmgen_default_fused_cast_subtract_3_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_3_args[2].v_handle = &tvmgen_default_fused_cast_subtract_3_0.dltensor; 
  tvmgen_default_fused_cast_subtract_3_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_3  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_3 (tvmgen_default_fused_cast_subtract_3_args, tvmgen_default_fused_cast_subtract_3_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_3  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_args[0].v_handle = &tvmgen_default_fused_cast_subtract_3_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_args[1].v_handle = &p131.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_args[2].v_handle = &p132.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_args[3].v_handle = &p133.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_args[4].v_handle = &p134.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_args[5].v_handle = &p135.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_args[1].v_handle = &p136.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_args[2].v_handle = &p137.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_args[3].v_handle = &p138.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_args[4].v_handle = &p139.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_args[5].v_handle = &p140.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9_
  // 
  TVMValue tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[11]; 
  int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[11]; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_4_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[1].v_handle = &p129.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[2].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[3].v_handle = &p141.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[4].v_handle = &p142.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[5].v_handle = &p143.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[6].v_handle = &p144.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[7].v_handle = &p145.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[8].v_handle = &p39.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[8] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[9].v_handle = &p40.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[9] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args[10].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids[10] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9_  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9_ (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__args, tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__arg_type_ids, 11)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9_  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_3
  // 
  TVMValue tvmgen_default_fused_cast_subtract_31_args[3]; 
  int32_t tvmgen_default_fused_cast_subtract_31_arg_type_ids[3]; 
  tvmgen_default_fused_cast_subtract_31_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__0.dltensor; 
  tvmgen_default_fused_cast_subtract_31_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_31_args[1].v_handle = &p147.dltensor; 
  tvmgen_default_fused_cast_subtract_31_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_31_args[2].v_handle = &tvmgen_default_fused_cast_subtract_31_0.dltensor; 
  tvmgen_default_fused_cast_subtract_31_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_3  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_3 (tvmgen_default_fused_cast_subtract_31_args, tvmgen_default_fused_cast_subtract_31_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_3  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_args[0].v_handle = &tvmgen_default_fused_cast_subtract_31_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_args[1].v_handle = &p148.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_args[2].v_handle = &p149.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_args[3].v_handle = &p150.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_args[4].v_handle = &p151.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_args[5].v_handle = &p152.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__12  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__121_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_args[1].v_handle = &p153.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_args[2].v_handle = &p154.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_args[3].v_handle = &p155.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_args[4].v_handle = &p156.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_args[5].v_handle = &p157.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__13  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_args[8]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_arg_type_ids[8]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__131_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_args[1].v_handle = &p158.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_args[2].v_handle = &p159.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_args[3].v_handle = &p160.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_args[4].v_handle = &p161.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_args[5].v_handle = &p162.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_args[6].v_handle = &p75.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_args[7].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_arg_type_ids[7] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_arg_type_ids, 8)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1
  // 
  TVMValue tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_args[6]; 
  int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_arg_type_ids[6]; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_args[1].v_handle = &p146.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_args[2].v_handle = &p75.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_args[3].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_5_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_args[4].v_handle = &p76.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_args[5].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_arg_type_ids[5] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1 (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_args, tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_arg_type_ids, 6)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_pad
  // 
  TVMValue tvmgen_default_fused_nn_pad_args[2]; 
  int32_t tvmgen_default_fused_nn_pad_arg_type_ids[2]; 
  tvmgen_default_fused_nn_pad_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_0.dltensor; 
  tvmgen_default_fused_nn_pad_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_pad_args[1].v_handle = &tvmgen_default_fused_nn_pad_0.dltensor; 
  tvmgen_default_fused_nn_pad_arg_type_ids[1] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_pad  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_pad (tvmgen_default_fused_nn_pad_args, tvmgen_default_fused_nn_pad_arg_type_ids, 2)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_pad  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_3
  // 
  TVMValue tvmgen_default_fused_cast_subtract_32_args[3]; 
  int32_t tvmgen_default_fused_cast_subtract_32_arg_type_ids[3]; 
  tvmgen_default_fused_cast_subtract_32_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_pad_cast_subtract_fixed_point_mu_63fc278acef3cb2b__1_0.dltensor; 
  tvmgen_default_fused_cast_subtract_32_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_32_args[1].v_handle = &p78.dltensor; 
  tvmgen_default_fused_cast_subtract_32_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_32_args[2].v_handle = &tvmgen_default_fused_cast_subtract_32_0.dltensor; 
  tvmgen_default_fused_cast_subtract_32_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_3  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_3 (tvmgen_default_fused_cast_subtract_32_args, tvmgen_default_fused_cast_subtract_32_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_3  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_args[0].v_handle = &tvmgen_default_fused_cast_subtract_32_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_args[1].v_handle = &p164.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_args[2].v_handle = &p165.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_args[3].v_handle = &p166.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_args[4].v_handle = &p167.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_args[5].v_handle = &p168.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__14_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_args[1].v_handle = &p169.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_args[2].v_handle = &p170.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_args[3].v_handle = &p171.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_args[4].v_handle = &p172.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_args[5].v_handle = &p173.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1
  // 
  TVMValue tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[12]; 
  int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[12]; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[0].v_handle = &tvmgen_default_fused_nn_pad_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[1].v_handle = &p76.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[2].v_handle = &p163.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[3].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[4].v_handle = &p174.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[5].v_handle = &p175.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[6].v_handle = &p176.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[7].v_handle = &p177.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[8].v_handle = &p178.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[8] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[9].v_handle = &p179.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[9] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[10].v_handle = &p180.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[10] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args[11].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids[11] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1 (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_args, tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_arg_type_ids, 12)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__1_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_args[1].v_handle = &p181.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_args[2].v_handle = &p182.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_args[3].v_handle = &p183.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_args[4].v_handle = &p184.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_args[5].v_handle = &p185.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__16_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_args[1].v_handle = &p186.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_args[2].v_handle = &p187.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_args[3].v_handle = &p188.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_args[4].v_handle = &p189.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_args[5].v_handle = &p190.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__15  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_args[7]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_arg_type_ids[7]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__151_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_args[1].v_handle = &p191.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_args[2].v_handle = &p192.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_args[3].v_handle = &p193.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_args[4].v_handle = &p194.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_args[5].v_handle = &p195.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_args[6].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_arg_type_ids[6] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_arg_type_ids, 7)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_pad_1
  // 
  TVMValue tvmgen_default_fused_nn_pad_1_args[2]; 
  int32_t tvmgen_default_fused_nn_pad_1_arg_type_ids[2]; 
  tvmgen_default_fused_nn_pad_1_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_0.dltensor; 
  tvmgen_default_fused_nn_pad_1_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_pad_1_args[1].v_handle = &tvmgen_default_fused_nn_pad_1_0.dltensor; 
  tvmgen_default_fused_nn_pad_1_arg_type_ids[1] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_pad_1  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_pad_1 (tvmgen_default_fused_nn_pad_1_args, tvmgen_default_fused_nn_pad_1_arg_type_ids, 2)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_pad_1  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_4
  // 
  TVMValue tvmgen_default_fused_cast_subtract_4_args[3]; 
  int32_t tvmgen_default_fused_cast_subtract_4_arg_type_ids[3]; 
  tvmgen_default_fused_cast_subtract_4_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_6_0.dltensor; 
  tvmgen_default_fused_cast_subtract_4_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_4_args[1].v_handle = &p197.dltensor; 
  tvmgen_default_fused_cast_subtract_4_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_4_args[2].v_handle = &tvmgen_default_fused_cast_subtract_4_0.dltensor; 
  tvmgen_default_fused_cast_subtract_4_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_4  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_4 (tvmgen_default_fused_cast_subtract_4_args, tvmgen_default_fused_cast_subtract_4_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_4  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_args[0].v_handle = &tvmgen_default_fused_cast_subtract_4_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_args[1].v_handle = &p198.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_args[2].v_handle = &p199.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_args[3].v_handle = &p200.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_args[4].v_handle = &p201.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_args[5].v_handle = &p202.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__17_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_args[1].v_handle = &p203.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_args[2].v_handle = &p204.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_args[3].v_handle = &p205.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_args[4].v_handle = &p206.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_args[5].v_handle = &p207.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1
  // 
  TVMValue tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[11]; 
  int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[11]; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[0].v_handle = &tvmgen_default_fused_nn_pad_1_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[1].v_handle = &p196.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[2].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[3].v_handle = &p208.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[4].v_handle = &p209.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[5].v_handle = &p210.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[6].v_handle = &p211.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[7].v_handle = &p212.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[8].v_handle = &p39.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[8] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[9].v_handle = &p40.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[9] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args[10].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids[10] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1 (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_args, tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_arg_type_ids, 11)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_5
  // 
  TVMValue tvmgen_default_fused_cast_subtract_5_args[3]; 
  int32_t tvmgen_default_fused_cast_subtract_5_arg_type_ids[3]; 
  tvmgen_default_fused_cast_subtract_5_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_0.dltensor; 
  tvmgen_default_fused_cast_subtract_5_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_5_args[1].v_handle = &p214.dltensor; 
  tvmgen_default_fused_cast_subtract_5_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_5_args[2].v_handle = &tvmgen_default_fused_cast_subtract_5_0.dltensor; 
  tvmgen_default_fused_cast_subtract_5_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_5  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_5 (tvmgen_default_fused_cast_subtract_5_args, tvmgen_default_fused_cast_subtract_5_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_5  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_args[0].v_handle = &tvmgen_default_fused_cast_subtract_5_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_args[1].v_handle = &p215.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_args[2].v_handle = &p216.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_args[3].v_handle = &p217.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_args[4].v_handle = &p218.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_args[5].v_handle = &p219.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_args[1].v_handle = &p220.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_args[2].v_handle = &p221.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_args[3].v_handle = &p222.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_args[4].v_handle = &p223.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_args[5].v_handle = &p224.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__18  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2
  // 
  TVMValue tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[12]; 
  int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[12]; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__1_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[1].v_handle = &p213.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[2].v_handle = &p77.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[3].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__181_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[4].v_handle = &p225.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[5].v_handle = &p226.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[6].v_handle = &p227.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[7].v_handle = &p228.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[8].v_handle = &p229.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[8] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[9].v_handle = &p230.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[9] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[10].v_handle = &p96.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[10] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args[11].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids[11] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2 (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_args, tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_arg_type_ids, 12)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_681645e755b6cbd4__2_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_args[1].v_handle = &p231.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_args[2].v_handle = &p232.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_args[3].v_handle = &p233.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_args[4].v_handle = &p234.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_args[5].v_handle = &p235.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__19  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__191_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_args[1].v_handle = &p236.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_args[2].v_handle = &p237.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_args[3].v_handle = &p238.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_args[4].v_handle = &p239.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_args[5].v_handle = &p240.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_args[8]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_arg_type_ids[8]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__20_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_args[1].v_handle = &p241.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_args[2].v_handle = &p242.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_args[3].v_handle = &p243.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_args[4].v_handle = &p244.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_args[5].v_handle = &p245.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_args[6].v_handle = &p163.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_args[7].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_arg_type_ids[7] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_arg_type_ids, 8)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_6
  // 
  TVMValue tvmgen_default_fused_cast_subtract_6_args[3]; 
  int32_t tvmgen_default_fused_cast_subtract_6_arg_type_ids[3]; 
  tvmgen_default_fused_cast_subtract_6_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_0.dltensor; 
  tvmgen_default_fused_cast_subtract_6_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_6_args[1].v_handle = &p180.dltensor; 
  tvmgen_default_fused_cast_subtract_6_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_6_args[2].v_handle = &tvmgen_default_fused_cast_subtract_6_0.dltensor; 
  tvmgen_default_fused_cast_subtract_6_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_6  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_6 (tvmgen_default_fused_cast_subtract_6_args, tvmgen_default_fused_cast_subtract_6_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_6  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_args[0].v_handle = &tvmgen_default_fused_cast_subtract_6_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_args[1].v_handle = &p247.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_args[2].v_handle = &p248.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_args[3].v_handle = &p249.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_args[4].v_handle = &p250.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_args[5].v_handle = &p251.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_args[1].v_handle = &p252.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_args[2].v_handle = &p253.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_args[3].v_handle = &p254.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_args[4].v_handle = &p255.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_args[5].v_handle = &p256.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2
  // 
  TVMValue tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[12]; 
  int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[12]; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_7_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[1].v_handle = &p246.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[2].v_handle = &p128.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[3].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[4].v_handle = &p257.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[5].v_handle = &p258.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[6].v_handle = &p259.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[7].v_handle = &p260.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[8].v_handle = &p261.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[8] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[9].v_handle = &p262.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[9] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[10].v_handle = &p263.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[10] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args[11].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids[11] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2 (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_args, tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_arg_type_ids, 12)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_6
  // 
  TVMValue tvmgen_default_fused_cast_subtract_61_args[3]; 
  int32_t tvmgen_default_fused_cast_subtract_61_arg_type_ids[3]; 
  tvmgen_default_fused_cast_subtract_61_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_0.dltensor; 
  tvmgen_default_fused_cast_subtract_61_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_61_args[1].v_handle = &p130.dltensor; 
  tvmgen_default_fused_cast_subtract_61_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_61_args[2].v_handle = &tvmgen_default_fused_cast_subtract_61_0.dltensor; 
  tvmgen_default_fused_cast_subtract_61_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_6  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_6 (tvmgen_default_fused_cast_subtract_61_args, tvmgen_default_fused_cast_subtract_61_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_6  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_args[0].v_handle = &tvmgen_default_fused_cast_subtract_61_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_args[1].v_handle = &p264.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_args[2].v_handle = &p265.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_args[3].v_handle = &p266.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_args[4].v_handle = &p267.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_args[5].v_handle = &p268.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__211_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_args[1].v_handle = &p269.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_args[2].v_handle = &p270.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_args[3].v_handle = &p271.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_args[4].v_handle = &p272.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_args[5].v_handle = &p273.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078_
  // 
  TVMValue tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[11]; 
  int32_t tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[11]; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_3787dbfb71edc6d9__2_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[1].v_handle = &p129.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[2].v_handle = &p77.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[3].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__221_0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[4].v_handle = &p274.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[5].v_handle = &p275.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[6].v_handle = &p276.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[7].v_handle = &p277.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[8].v_handle = &p278.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[8] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[9].v_handle = &p96.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[9] = kTVMNDArrayHandle; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args[10].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__0.dltensor; 
  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids[10] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078_  ... \r\n");
#endif
  if (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078_ (tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__args, tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__arg_type_ids, 11)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078_  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_args[0].v_handle = &tvmgen_default_fused_cast_subtract_fixed_point_multiply_add_nn_conv2d_add_cast_multiply_add_rig_8e03b6e513bd0078__0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_args[1].v_handle = &p279.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_args[2].v_handle = &p280.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_args[3].v_handle = &p281.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_args[4].v_handle = &p282.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_args[5].v_handle = &p283.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__21  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__212_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_args[1].v_handle = &p284.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_args[2].v_handle = &p285.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_args[3].v_handle = &p286.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_args[4].v_handle = &p287.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_args[5].v_handle = &p288.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_args[7].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__22  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_args[9]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_arg_type_ids[9]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_s_8a376065fd35c245__222_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_args[1].v_handle = &p289.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_args[2].v_handle = &p290.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_args[3].v_handle = &p291.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_args[4].v_handle = &p292.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_args[5].v_handle = &p293.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_args[6].v_handle = &p262.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_args[7].v_handle = &p294.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_arg_type_ids[7] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_args[8].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_arg_type_ids[8] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_arg_type_ids, 9)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_args[8]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_arg_type_ids[8]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_cast_subtra_19fb8299102985a1__1_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_args[1].v_handle = &p295.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_args[2].v_handle = &p296.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_args[3].v_handle = &p297.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_args[4].v_handle = &p298.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_args[5].v_handle = &p299.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_args[6].v_handle = &p6.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_arg_type_ids[6] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_args[7].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_arg_type_ids[7] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_arg_type_ids, 8)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract
  // 
  TVMValue tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_args[3]; 
  int32_t tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_arg_type_ids[3]; 
  tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_args[0].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_clip_cast_0.dltensor; 
  tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_args[1].v_handle = &p7.dltensor; 
  tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_args[2].v_handle = &tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_0.dltensor; 
  tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_arg_type_ids[2] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract (tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_args, tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_arg_type_ids, 3)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract  Done.\r\n");
#endif
  // 
  // tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8
  // 
  TVMValue tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_args[7]; 
  int32_t tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_arg_type_ids[7]; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_args[0].v_handle = &tvmgen_default_fused_nn_avg_pool2d_cast_cast_subtract_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_arg_type_ids[0] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_args[1].v_handle = &p300.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_arg_type_ids[1] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_args[2].v_handle = &p301.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_arg_type_ids[2] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_args[3].v_handle = &p302.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_arg_type_ids[3] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_args[4].v_handle = &p303.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_arg_type_ids[4] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_args[5].v_handle = &p304.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_arg_type_ids[5] = kTVMNDArrayHandle; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_args[6].v_handle = &tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_0.dltensor; 
  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_arg_type_ids[6] = kTVMNDArrayHandle; 


#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8  ... \r\n");
#endif
  if (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8 (tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_args, tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8_arg_type_ids, 7)) {
    TVMAPISetLastError("Invalid handle");
    return ANT_ML_STATUS_ERROR;
  }
#if (_VERBOSE_ > 0)
  printf ("  tvmgen_default_fused_nn_conv2d_add_cast_multiply_add_right_shift_cast_add_clip_cast_8  Done.\r\n");
#endif


  return ANT_ML_STATUS_OK;
}

ANT_ML_ALIGNED(8) __attribute__ ((section(".network.nn_data_act"))) uint8_t network_activations[78006];

ANT_ML_API_ENTRY  ant_ml_model_info network_network = {
  .name = "network",
  .datetime = "25/11/2022 06:06:59",
  .revision = "v1",
  .tool_version = "0.9.0",
  .api_version = "1.0.0.0",
  .n_nodes = 71,
  .n_inputs = 1,
  .n_outputs = 1,
  .activations_size = 78006,
  .params_size = 268792,
  .activations = network_activations,
  .inputs = _InputsList,
  .outputs = _OutputsList,
  .ant_ml_get_params = &ant_ml_network_data_weights_get,
  .ant_ml_create = &ant_ml_network_create,
  .ant_ml_destroy = &ant_ml_network_destroy,
  .ant_ml_run = &ant_ml_network_run
};
