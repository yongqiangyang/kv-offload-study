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
 * \file kernel_struct_conv2d.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_struct_conv2d.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_CONV2D_H__
#endif

#ifndef ASCENDC_MODULE_STRUCT_CONV2D_H
#define ASCENDC_MODULE_STRUCT_CONV2D_H

#include "../../impl/basic_api/kernel_macros.h"
#include "../../impl/basic_api/utils/kernel_utils_constants.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
enum class LoopMode : uint8_t { MODE_NM = 0, MODE_MN = 1, MODE_KM = 2, MODE_KN = 3 };

struct Conv2dTilling {
    const uint32_t blockSize = 16; // # M block size is always 16
    LoopMode loopMode = LoopMode::MODE_NM;

    uint32_t c0Size = 32;
    uint32_t dTypeSize = 1;

    uint32_t strideH = 0;
    uint32_t strideW = 0;
    uint32_t dilationH = 0;
    uint32_t dilationW = 0;
    uint32_t hi = 0;
    uint32_t wi = 0;
    uint32_t ho = 0;
    uint32_t wo = 0;

    uint32_t height = 0;
    uint32_t width = 0;

    uint32_t howo = 0;

    uint32_t mNum = 0;
    uint32_t nNum = 0;
    uint32_t kNum = 0;

    uint32_t mBlockNum = 0;
    uint32_t kBlockNum = 0;
    uint32_t nBlockNum = 0;

    uint32_t roundM = 0;
    uint32_t roundN = 0;
    uint32_t roundK = 0;

    uint32_t mTileBlock = 0;
    uint32_t nTileBlock = 0;
    uint32_t kTileBlock = 0;

    uint32_t mIterNum = 0;
    uint32_t nIterNum = 0;
    uint32_t kIterNum = 0;

    uint32_t mTileNums = 0;

    bool mHasTail = false;
    bool nHasTail = false;
    bool kHasTail = false;

    uint32_t kTailBlock = 0;
    uint32_t mTailBlock = 0;
    uint32_t nTailBlock = 0;

    uint32_t mTailNums = 0;
};

struct Conv2dParams {
    __aicore__ Conv2dParams() {}

    __aicore__ Conv2dParams(
        const uint32_t imgShapeIn[CONV2D_IMG_SIZE], const uint32_t kernelShapeIn[CONV2D_KERNEL_SIZE],
        const uint32_t strideIn[CONV2D_STRIDE], const uint32_t cinIn, const uint32_t coutIn,
        const uint32_t padListIn[CONV2D_PAD], const uint32_t dilationIn[CONV2D_DILATION], const uint32_t initYIn,
        const bool partialSumIn)
    {
        for (int32_t i = 0; i < CONV2D_IMG_SIZE; ++i) {
            imgShape[i] = imgShapeIn[i];
        }
        for (int32_t i = 0; i < CONV2D_KERNEL_SIZE; ++i) {
            kernelShape[i] = kernelShapeIn[i];
        }
        for (int32_t i = 0; i < CONV2D_STRIDE; ++i) {
            stride[i] = strideIn[i];
        }
        cin = cinIn;
        cout = coutIn;
        for (int32_t i = 0; i < CONV2D_PAD; ++i) {
            padList[i] = padListIn[i];
        }
        for (int32_t i = 0; i < CONV2D_DILATION; ++i) {
            dilation[i] = dilationIn[i];
        }
        initY = initYIn;
        partialSum = partialSumIn;
    }

    uint32_t imgShape[CONV2D_IMG_SIZE] = {0};       // [H, W]
    uint32_t kernelShape[CONV2D_KERNEL_SIZE] = {0}; // [Kh, Kw]
    uint32_t stride[CONV2D_STRIDE] = {0};           // [stride_h, stride_w]
    uint32_t cin = 0;                               // cin = C0 * C1;
    uint32_t cout = 0;
    uint32_t padList[CONV2D_PAD] = {0};       // [pad_left, pad_right, pad_top, pad_bottom]
    uint32_t dilation[CONV2D_DILATION] = {0}; // [dilation_h, dilation_w]
    uint32_t initY = 0;
    bool partialSum = false;
};

struct GemmTiling {
    __aicore__ GemmTiling()
    {
        mIterNum = 1;
        nIterNum = 1;
        kIterNum = 1;
        loopMode = LoopMode::MODE_NM;
    }

    const uint32_t blockSize = 16;
    LoopMode loopMode = LoopMode::MODE_NM;
    uint32_t mNum = 0;
    uint32_t nNum = 0;
    uint32_t kNum = 0;
    uint32_t roundM = 0;
    uint32_t roundN = 0;
    uint32_t roundK = 0;
    uint32_t c0Size = 32;
    uint32_t dtypeSize = 1;
    uint32_t mBlockNum = 0;
    uint32_t nBlockNum = 0;
    uint32_t kBlockNum = 0;
    uint32_t mIterNum = 0;
    uint32_t nIterNum = 0;
    uint32_t kIterNum = 0;
    uint32_t mTileBlock = 0;
    uint32_t nTileBlock = 0;
    uint32_t kTileBlock = 0;
    uint32_t kTailBlock = 0;
    uint32_t mTailBlock = 0;
    uint32_t nTailBlock = 0;
    bool kHasTail = false;
    bool mHasTail = false;
    bool nHasTail = false;
    bool kHasTailEle = false;
    uint32_t kTailEle = 0;
};
} // namespace AscendC
#endif // ASCENDC_MODULE_STRUCT_CONV2D_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_CONV2D_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_CONV2D_H__
#endif
