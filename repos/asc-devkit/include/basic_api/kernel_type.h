/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_type.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_TYPE_H__
#endif

#ifndef KERNEL_TYPE_H
#define KERNEL_TYPE_H

#if !defined(__NPU_HOST__) && !defined(__NPU_DEVICE__) && !defined(__ASCC_HOST__) && !defined(__ASCC_DEVICE__)
#define DT_FLOAT 0           // float32
#define DT_FLOAT16 1         // half
#define DT_INT8 2            // int8
#define DT_INT32 3           // int32
#define DT_UINT8 4           // u8
#define DT_INT16 6           // int16
#define DT_UINT16 7          // u16
#define DT_INT64 9           // int64
#define DT_UINT32 8          // u32
#define DT_UINT64 10         // u64
#define DT_DOUBLE 11         // double
#define DT_BOOL 12           // bool
#define DT_STRING 13         // string
#define DT_DUAL_SUB_INT8 14  // dual output int8
#define DT_DUAL_SUB_UINT8 15 // dual output u8
#define DT_COMPLEX64 16      // complex64
#define DT_COMPLEX128 17     // complex128
#define DT_QINT8 18          // qint8
#define DT_QINT16 19         // qint16
#define DT_QINT32 20         // qint32
#define DT_QUINT8 21         // quint8
#define DT_QUINT16 22        // quint16
#define DT_RESOURCE 23       // resource
#define DT_STRING_REF 24     // string ref
#define DT_DUAL 25           // dual output
#define DT_VARIANT 26        // dt_variant
#define DT_BF16 27           // bf16
#define DT_UNDEFINED 28      // Indicate a DataType field has not been set.
#define DT_INT4 29           // int4
#define DT_UINT1 30          // u1
#define DT_INT2 31           // int2
#define DT_UINT2 32          // u2
#define DT_COMPLEX32 33      // complex32 type
#define DT_HIFLOAT8 34       // hif8 type
#define DT_FLOAT8_E5M2 35    // fp8_e5m2 type
#define DT_FLOAT8_E4M3FN 36  // fp8_e4m3 type
#define DT_FLOAT8_E8M0 37    // fp8_e8m0 type
#define DT_FLOAT6_E3M2 38    // fp6_e3m2 type
#define DT_FLOAT6_E2M3 39    // fp6_e2m3 type
#define DT_FLOAT4_E2M1 40    // fp4_e2m1 type
#define DT_FLOAT4_E1M2 41    // fp4_e1m2 type
#define DT_HIFLOAT4 42       // hif4 type
#define DT_HIFLOAT4_SCALE 43 // hif4_scale type
#define DT_MAX 44            // Mark the boundaries of AscendCData type

#define FORMAT_NCHW 0                      // NCHW Tensor
#define FORMAT_NHWC 1                      // NHWC Tensor
#define FORMAT_ND 2                        // ND Tensor
#define FORMAT_NC1HWC0 3                   // NC1HWC0
#define FORMAT_FRACTAL_Z 4                 // FRACTAL_Z for cube
#define FORMAT_NC1C0HWPAD 5                // NC1C0HWPAD
#define FORMAT_NHWC1C0 6                   // NHWC1C0 Tensor
#define FORMAT_FSR_NCHW 7                  // FSR NCHW Tensor
#define FORMAT_FRACTAL_DECONV 8            // DECONV
#define FORMAT_C1HWNC0 9                   // C1HWNC0
#define FORMAT_FRACTAL_DECONV_TRANSPOSE 10 // TRANSPOSE
#define FORMAT_FRACTAL_DECONV_SP_STRIDE_TRANS 11
#define FORMAT_NC1HWC0_C04 12   // NC1HWC0 C0 is 4
#define FORMAT_FRACTAL_Z_C04 13 // FRACZ C0 is 4
#define FORMAT_CHWN 14          // CHWN
#define FORMAT_FRACTAL_DECONV_SP_STRIDE8_TRANS 15
#define FORMAT_HWCN 16        // HWCN
#define FORMAT_NC1KHKWHWC0 17 // KHKW kernel h& kernel w maxpooling max output
#define FORMAT_BN_WEIGHT 18   // Batch Normalization layer
#define FORMAT_FILTER_HWCK 19 // filter input tensor
#define FORMAT_HASHTABLE_LOOKUP_LOOKUPS 20
#define FORMAT_HASHTABLE_LOOKUP_KEYS 21
#define FORMAT_HASHTABLE_LOOKUP_VALUE 22
#define FORMAT_HASHTABLE_LOOKUP_OUTPUT 23
#define FORMAT_HASHTABLE_LOOKUP_HITS 24
#define FORMAT_C1HWNCoC0 25 // C1HWNCoC0
#define FORMAT_MD 26
#define FORMAT_NDHWC 27                  // NDHWC
#define FORMAT_FRACTAL_ZZ 28             // ZZ for cube
#define FORMAT_FRACTAL_NZ 29             // NZ for cube
#define FORMAT_NCDHW 30                  // NCDHW
#define FORMAT_DHWCN 31                  // 3D filter input tensor
#define FORMAT_NDC1HWC0 32               // NDC1HWC0
#define FORMAT_FRACTAL_Z_3D 33           // 05jgfd9
#define FORMAT_CN 34                     // CN
#define FORMAT_NC 35                     // NC
#define FORMAT_DHWNC 36                  // DHWNCX
#define FORMAT_FRACTAL_Z_3D_TRANSPOSE 37 // 3D filter(transpose) input tensor
#define FORMAT_FRACTAL_ZN_LSTM 38        // For LSTM Net
#define FORMAT_FRACTAL_Z_G 39
#define FORMAT_RESERVED 40
#define FORMAT_ALL 41
#define FORMAT_NULL 42
#define FORMAT_ND_RNN_BIAS 43    // Bias Format for RNN
#define FORMAT_FRACTAL_ZN_RNN 44 // ZN for RNN
#define FORMAT_NYUV 45
#define FORMAT_NYUV_A 46
#define FORMAT_NCL 47
// Add new formats here

#define FORMAT_MAX 0xff
#else  // defined(__NPU_HOST__) || defined(__NPU_DEVICE__) || defined(__ASCC_HOST__) || defined(__ASCC_DEVICE__)
namespace AscendC {
enum DataType {
    DT_FLOAT = 0, // float32
    DT_FLOAT16,   // half
    DT_INT8,      // int8
    DT_INT32,     // int32
    DT_UINT8,     // u8
    DT_INT16,     // int16
    DT_UINT16,    // u16
    DT_INT64,     // int64
    DT_UINT32,    // u32
    DT_UINT64,    // u64
    DT_BOOL,
    DT_DOUBLE,
    DT_STRING,         // string
    DT_DUAL_SUB_INT8,  // dual output int8
    DT_DUAL_SUB_UINT8, // dual output u8
    DT_COMPLEX64,      // complex64
    DT_COMPLEX128,     // complex128
    DT_QINT8,          // qint8
    DT_QINT16,         // qint16
    DT_QINT32,         // qint32
    DT_QUINT8,         // quint8
    DT_QUINT16,        // quint16
    DT_RESOURCE,       // resource
    DT_STRING_REF,     // string ref
    DT_DUAL,           // dual output
    DT_VARIANT,        // dt_variant
    DT_BF16,           // bf16
    DT_UNDEFINED,      // Indicate a DataType field has not been set.
    DT_INT4,           // int4
    DT_UINT1,          // u1
    DT_INT2,           // int2
    DT_UINT2,          // u2
    DT_COMPLEX32,      // complex32 type
    DT_HIFLOAT8,       // hif8 type
    DT_FLOAT8_E5M2,    // fp8_e5m2 type
    DT_FLOAT8_E4M3FN,  // fp8_e4m3 type
    DT_FLOAT8_E8M0,    // fp8_e8m0 type
    DT_FLOAT6_E3M2,    // fp6_e3m2 type
    DT_FLOAT6_E2M3,    // fp6_e2m3 type
    DT_FLOAT4_E2M1,    // fp4_e2m1 type
    DT_FLOAT4_E1M2,    // fp4_e1m2 type
    DT_HIFLOAT4,       // hif4 type
    DT_HIFLOAT4_SCALE, // hif4_scale type
    DT_MAX,            // Mark the boundaries of AscendCData type
};
enum Format {
    FORMAT_NCHW,                     // NCHW Tensor
    FORMAT_NHWC,                     // NHWC Tensor
    FORMAT_ND,                       // ND Tensor
    FORMAT_NC1HWC0,                  // NC1HWC0
    FORMAT_FRACTAL_Z,                // FRACTAL_Z for cube
    FORMAT_NC1C0HWPAD,               // NC1C0HWPAD
    FORMAT_NHWC1C0,                  // NHWC1C0 Tensor
    FORMAT_FSR_NCHW,                 // FSR NCHW Tensor
    FORMAT_FRACTAL_DECONV,           // DECONV
    FORMAT_C1HWNC0,                  // C1HWNC0
    FORMAT_FRACTAL_DECONV_TRANSPOSE, // TRANSPOSE
    FORMAT_FRACTAL_DECONV_SP_STRIDE_TRANS,
    FORMAT_NC1HWC0_C04,   // NC1HWC0 C0 is 4
    FORMAT_FRACTAL_Z_C04, // FRACZ C0 is 4
    FORMAT_CHWN,          // CHWN
    FORMAT_FRACTAL_DECONV_SP_STRIDE8_TRANS,
    FORMAT_HWCN,        // HWCN
    FORMAT_NC1KHKWHWC0, // KHKW kernel h& kernel w maxpooling max output
    FORMAT_BN_WEIGHT,   // Batch Normalization layer
    FORMAT_FILTER_HWCK, // filter input tensor
    FORMAT_HASHTABLE_LOOKUP_LOOKUPS,
    FORMAT_HASHTABLE_LOOKUP_KEYS,
    FORMAT_HASHTABLE_LOOKUP_VALUE,
    FORMAT_HASHTABLE_LOOKUP_OUTPUT,
    FORMAT_HASHTABLE_LOOKUP_HITS,
    FORMAT_C1HWNCoC0, // C1HWNCoC0
    FORMAT_MD,
    FORMAT_NDHWC,                  // NDHWC
    FORMAT_FRACTAL_ZZ,             // ZZ for cube
    FORMAT_FRACTAL_NZ,             // NZ for cube
    FORMAT_NCDHW,                  // NCDHW
    FORMAT_DHWCN,                  // 3D filter input tensor
    FORMAT_NDC1HWC0,               // NDC1HWC0
    FORMAT_FRACTAL_Z_3D,           // 05jgfd9
    FORMAT_CN,                     // CN
    FORMAT_NC,                     // NC
    FORMAT_DHWNC,                  // DHWNCX
    FORMAT_FRACTAL_Z_3D_TRANSPOSE, // 3D filter(transpose) input tensor
    FORMAT_FRACTAL_ZN_LSTM,        // For LSTM Net
    FORMAT_FRACTAL_Z_G,
    FORMAT_RESERVED,
    FORMAT_ALL,
    FORMAT_NULL,
    FORMAT_ND_RNN_BIAS,    // Bias Format for RNN
    FORMAT_FRACTAL_ZN_RNN, // ZN for RNN
    FORMAT_NYUV,
    FORMAT_NYUV_A,
    FORMAT_NCL,
    FORMAT_FRACTAL_Z_WINO,
    FORMAT_C1HWC0,
    FORMAT_FRACTAL_NZ_C0_16, // NZ for cube
    FORMAT_FRACTAL_NZ_C0_32, // NZ for cube
    FORMAT_FRACTAL_NZ_C0_2,  // NZ for cube
    FORMAT_FRACTAL_NZ_C0_4,  // NZ for cube
    FORMAT_FRACTAL_NZ_C0_8,  // NZ for cube
    FORMAT_MAX,
};
} // namespace AscendC
#endif // !defined(__NPU_HOST__) && !defined(__NPU_DEVICE__) && !defined(__ASCC_HOST__) && !defined(__ASCC_DEVICE__)
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_TYPE_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_TYPE_H__
#endif
