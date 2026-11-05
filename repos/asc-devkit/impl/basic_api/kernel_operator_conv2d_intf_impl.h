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
 * \file kernel_operator_conv2d_intf_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_conv2d_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_conv2d_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CONV2D_INTF_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_CONV2D_INTERFACE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_CONV2D_INTERFACE_IMPL_H
#include "../../include/basic_api/kernel_tensor.h"
#include "kernel_operator_conv2d_base_impl.h"
#include "kernel_operator_data_copy_intf_impl.h"
#include "../../include/basic_api/kernel_struct_data_copy.h"

namespace AscendC {
// T should be featureMap matrix dtype
template <typename T>
[[deprecated("NOTICE: GetConv2dTiling has been deprecated and will be removed in the next version. "
             "Please do not use it!")]] __aicore__ inline Conv2dTilling
GetConv2dTiling(Conv2dParams& conv2dParams)
{
    Conv2dTilling tilling;
    GetTypeforC0<T>(conv2dParams, tilling);

    tilling.loopMode = LoopMode::MODE_NM;

    tilling.strideH = conv2dParams.stride[0];
    tilling.strideW = conv2dParams.stride[1];
    tilling.dilationH = conv2dParams.dilation[0];
    tilling.dilationW = conv2dParams.dilation[1];
    tilling.hi = conv2dParams.imgShape[0];
    tilling.wi = conv2dParams.imgShape[1];
    tilling.ho = (tilling.hi + conv2dParams.padList[2] + conv2dParams.padList[3] -
                  tilling.dilationH * (conv2dParams.kernelShape[0] - 1) - 1) /
                     tilling.strideH +
                 1;
    tilling.wo = (tilling.wi + conv2dParams.padList[0] + conv2dParams.padList[1] -
                  tilling.dilationW * (conv2dParams.kernelShape[1] - 1) - 1) /
                     tilling.strideW +
                 1;

    tilling.height = conv2dParams.kernelShape[0];
    tilling.width = conv2dParams.kernelShape[1];

    tilling.howo = tilling.ho * tilling.wo;

    tilling.mNum = tilling.howo;
    tilling.nNum = conv2dParams.cout;
    tilling.kNum = conv2dParams.cin * tilling.height * tilling.width;

    CalculateConv2dTiling(tilling);

    return tilling;
}

/*
 * @ingroup：Conv2D
 * @brief：Given an input tensor and a weight tensor, perform the convolution 2-D operation, and output the
 * result tensor
 * @param [out] dst output LocalTensor
 * @param [in] bias input LocalTensor
 * @param [in] featureMap input LocalTensor
 * @param [in] weight input LocalTensor
 * @param [in] intriParams.imgShape Shape of "featureMap"
 * @param [in] intriParams.kernel Shape shape of "weight"
 * @param [in] intriParams.stride Stride of convolution
 * @param [in] intriParams.cin Fractal layout parameter,cin = c1 * c0
 * @param [in] intriParams.cout Fractal layout parameter
 * @param [in] intriParams.padList Padding rows/columns
 * @param [in] intriParams.dilation Void convolution parameter
 * @param [in] intriParams.initY dst initialization parameters
 * @param [in] intriParams.partialSum Judge whether the result is moved out
 */
template <typename T, typename U>
[[deprecated("NOTICE: Conv2D has been deprecated and will be removed in the next version. "
             "Please do not use it!")]] __aicore__ inline __in_pipe__(MTE2)
    __out_pipe__(MTE3) void Conv2D(
        const LocalTensor<T>& dst, const LocalTensor<U>& featureMap, const LocalTensor<U>& weight,
        Conv2dParams& conv2dParams, Conv2dTilling& tilling)
{
    if (conv2dParams.initY == 2) {
        return;
    }

    Conv2D(dst, dst, featureMap, weight, conv2dParams, tilling);
}

template <typename T, typename U>
[[deprecated("NOTICE: Conv2D has been deprecated and will be removed in the next version. "
             "Please do not use it!")]] __aicore__ inline __in_pipe__(MTE2)
    __out_pipe__(MTE3) void Conv2D(
        const LocalTensor<T>& dst, const LocalTensor<T>& bias, const LocalTensor<U>& featureMap,
        const LocalTensor<U>& weight, Conv2dParams& conv2dParams, Conv2dTilling& tilling)
{
#if ASCENDC_CPU_DEBUG
    if (!CheckConv2DParams(dst, bias, featureMap, weight, conv2dParams, tilling)) {
        return;
    }
#endif

#if (__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 2002)
    DataCopyParams dataCopyParams;
    dataCopyParams.blockCount = 1;
    DataCopyEnhancedParams enhancedParams;
    enhancedParams.blockMode = BlockMode::BLOCK_MODE_MATRIX;
#endif

    const Hardware dstScope = GetPhyType((TPosition)dst.GetPosition());
    LocalTensor<T> l0c;
    if (dstScope == Hardware::L0C) {
        l0c = dst[0];
    } else {
#if (__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 2002)
        TBuffAddr tbufc;
        tbufc.logicPos = static_cast<uint8_t>(TPosition::C2);
        l0c.SetAddr(tbufc);
        l0c.InitBuffer(0, TOTAL_L0C_SIZE / sizeof(PrimT<T>));

        dataCopyParams.blockLen = dst.GetSize() * sizeof(PrimT<T>) / 1024;
        DataCopy(l0c, dst, dataCopyParams, enhancedParams);
#endif
    }

    if (tilling.loopMode == LoopMode::MODE_NM) {
        Conv2DExecNm(l0c, bias, featureMap, weight, conv2dParams, tilling);
    } else if (tilling.loopMode == LoopMode::MODE_MN) {
        Conv2DExecMn(l0c, bias, featureMap, weight, conv2dParams, tilling);
    } else {
        // other mode are not supported
    }

#if (__NPU_ARCH__ == 1001) || (__NPU_ARCH__ == 2002)
    if ((!conv2dParams.partialSum) && (dstScope == Hardware::UB)) {
        pipe_barrier(PIPE_ALL);
        dataCopyParams.blockLen = tilling.roundM * tilling.roundN * sizeof(PrimT<T>) / 1024;
        DataCopy(dst, l0c, dataCopyParams, enhancedParams);
    }
#endif
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_CONV2D_INTERFACE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CONV2D_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CONV2D_INTF_IMPL_H__
#endif
