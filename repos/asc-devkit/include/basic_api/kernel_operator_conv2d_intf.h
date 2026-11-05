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
 * \file kernel_operator_conv2d_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_conv2d_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CONV2D_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_CONV2D_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_CONV2D_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "kernel_struct_conv2d.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include "stub_def.h"
#endif

namespace AscendC {
template <typename T>
class LocalTensor;

// T should be featureMap matrix dtype
template <typename T>
__aicore__ inline Conv2dTilling GetConv2dTiling(Conv2dParams& conv2dParams);

/*
 * @ingroup：Conv2D
 * @brief：Given an input tensor and a weight tensor, perform the convolution 2-D operation, and output the
 * result tensor
 * @param [out] dst output LocalTensor
 * @param [in] bias input LocalTensor
 * @param [in] featureMap input LocalTensor
 * @param [in] weight input LocalTensor
 * @param [in] conv2dParams.imgShape Shape of "featureMap"
 * @param [in] conv2dParams.kernel Shape shape of "weight"
 * @param [in] conv2dParams.stride Stride of convolution
 * @param [in] conv2dParams.cin Fractal layout parameter,cin = c1 * c0
 * @param [in] conv2dParams.cout Fractal layout parameter
 * @param [in] conv2dParams.padList Padding rows/columns
 * @param [in] conv2dParams.dilation Void convolution parameter
 * @param [in] conv2dParams.initY dst initialization parameters
 * @param [in] conv2dParams.partialSum Judge whether the result is moved out
 */
template <typename T, typename U>
__aicore__ inline __in_pipe__(MTE2) __out_pipe__(MTE3) void Conv2D(
    const LocalTensor<T>& dst, const LocalTensor<U>& featureMap, const LocalTensor<U>& weight,
    Conv2dParams& conv2dParams, Conv2dTilling& tilling);

template <typename T, typename U>
__aicore__ inline __in_pipe__(MTE2) __out_pipe__(MTE3) void Conv2D(
    const LocalTensor<T>& dst, const LocalTensor<T>& bias, const LocalTensor<U>& featureMap,
    const LocalTensor<U>& weight, Conv2dParams& conv2dParams, Conv2dTilling& tilling);
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_conv2d_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_CONV2D_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CONV2D_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CONV2D_INTF_H__
#endif
