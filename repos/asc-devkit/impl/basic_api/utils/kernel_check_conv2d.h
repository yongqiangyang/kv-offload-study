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
 * \file kernel_check_conv2d.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_check_conv2d.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_CONV2D_H__
#endif
#ifndef ASCENDC_MODULE_CHECK_CONV2D_H
#define ASCENDC_MODULE_CHECK_CONV2D_H

#if ASCENDC_CPU_DEBUG
#include <unordered_set>
#include "../../../include/basic_api/kernel_tensor.h"
#include "../../../include/basic_api/kernel_struct_conv2d.h"

namespace AscendC {

const std::unordered_set<std::string> CONV2D_SUPPORT_TYPE{"s8s8s32", "f16f16f32", "f16f16f16"};

__aicore__ inline bool CheckConv2DRange(const uint32_t* rangePair, const uint32_t num)
{
    if ((num >= rangePair[0]) && (num <= rangePair[1])) {
        return true;
    }
    return false;
}

__aicore__ inline bool CheckConv2DParamsRange(Conv2dParams& conv2dParams, Conv2dTilling& tilling)
{
    uint32_t cinRange[] = {tilling.c0Size, 4096};
    uint32_t coutRange[] = {16, 4096};

    uint32_t loopTime[] = {CONV2D_IMG_SIZE, CONV2D_KERNEL_SIZE, CONV2D_STRIDE, CONV2D_PAD, CONV2D_DILATION};
    // imgSizeRange, kernelSizeRange, strideRange, padRange, dilationRange
    uint32_t rangePair[][2] = {{1, 4096}, {1, 255}, {1, 63}, {0, 255}, {1, 255}};

    uint32_t* arrayPtr[] = {
        conv2dParams.imgShape, conv2dParams.kernelShape, conv2dParams.stride, conv2dParams.padList,
        conv2dParams.dilation};

    if (conv2dParams.cout % BLOCK_CUBE != 0) {
        return false;
    }

    if ((CheckConv2DRange(cinRange, conv2dParams.cin) == false) ||
        CheckConv2DRange(coutRange, conv2dParams.cout) == false) {
        return false;
    }

    for (size_t i = 0; i < sizeof(loopTime) / sizeof(loopTime[0]); i++) {
        for (size_t j = 0; j < loopTime[i]; j++) {
            if (CheckConv2DRange(rangePair[i], arrayPtr[i][j]) == false) {
                return false;
            }
        }
    }
    return true;
}

template <typename T, typename U>
__aicore__ inline bool CheckConv2DOverflow(
    const LocalTensor<T>& dst, const LocalTensor<U>& featureMap, const LocalTensor<U>& weight,
    Conv2dParams& conv2dParams, Conv2dTilling& tilling)
{
    // check l0c
    uint32_t roundM = tilling.roundM;
    uint32_t roundN = tilling.roundN;
    uint32_t roundK = tilling.roundK;

    uint32_t kTileBlock = tilling.kTileBlock;

    uint32_t needElementLoc = roundM * roundN * sizeof(PrimT<T>);
    if (needElementLoc > TOTAL_L0C_SIZE) {
        return false;
    }

    // check l0a:
    uint32_t needElementL0a = roundM * roundK * sizeof(PrimT<U>);

    // check l0b:
    uint32_t needElementL0b = roundN * roundK * sizeof(PrimT<U>);
    if ((needElementL0b + needElementL0a) > TOTAL_L1_SIZE) {
        return false;
    }

    uint32_t minElementL0a = roundM * kTileBlock * tilling.c0Size * sizeof(PrimT<U>);
    uint32_t minElementL0b = roundN * kTileBlock * tilling.c0Size * sizeof(PrimT<U>);

    if (minElementL0a > TOTAL_L0A_SIZE) {
        return false;
    }

    if (minElementL0b > TOTAL_L0B_SIZE) {
        return false;
    }

    return true;
}

template <typename T, typename U>
__aicore__ inline bool CheckConv2DParams(
    const LocalTensor<T>& dst, const LocalTensor<T>& bias, const LocalTensor<U>& featureMap,
    const LocalTensor<U>& weight, Conv2dParams& conv2dParams, Conv2dTilling& tilling)
{
    if (tilling.c0Size == 0) {
        return false;
    }

    // check scope
    const Hardware src0Scope = GetPhyType((TPosition)featureMap.GetPosition());
    const Hardware src1Scope = GetPhyType((TPosition)weight.GetPosition());
    const Hardware dstScope = GetPhyType((TPosition)dst.GetPosition());
    const Hardware biasScope = GetPhyType((TPosition)bias.GetPosition());

    if (src0Scope != Hardware::L1 || src1Scope != Hardware::L1) {
        return false;
    }

    if ((dstScope != Hardware::UB) && (dstScope != Hardware::L0C)) {
        return false;
    }

    // check dtype
    std::string dtypeStr = GetTypeStr(featureMap) + GetTypeStr(weight) + GetTypeStr(dst);
    if (CONV2D_SUPPORT_TYPE.find(dtypeStr) == CONV2D_SUPPORT_TYPE.end()) {
        return false;
    }

    // check range
    bool check = CheckConv2DParamsRange(conv2dParams, tilling);
    if (check == false) {
        return false;
    }

    // check overflow
    if (!CheckConv2DOverflow(dst, featureMap, weight, conv2dParams, tilling)) {
        return false;
    }

    return true;
}
} // namespace AscendC
#endif

#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_CONV2D_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_CONV2D_H__
#endif
