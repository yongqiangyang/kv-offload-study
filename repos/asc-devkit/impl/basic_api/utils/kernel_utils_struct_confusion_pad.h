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
 * \file kernel_utils_struct_confusion_pad.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_utils_struct_confusion_pad.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_STRUCT_CONFUSION_PAD_H__
#endif
#ifndef ASCENDC_MODULE_UTILS_STRUCT_CONFUSION_PAD_H
#define ASCENDC_MODULE_UTILS_STRUCT_CONFUSION_PAD_H
#include "kernel_utils_mode.h"

namespace AscendC {
struct ConfusionTranspose2NZ012NTiling {
    __aicore__ ConfusionTranspose2NZ012NTiling()
    {
        blockSize = 0;
        shapeB = 0;
        shapeN = 0;
        hnDiv = 0;
        blockNum = 0;
        shapeH = 0;
        hBlockNum = 0;
        sBlockNum = 0;
        alignH = 0;
        alignS = 0;
        hnDivBlockNum = 0;
        alignHnDiv = 0;
        gap = 0;
        alignsBlockCube = 0;
        prehBlockNum = 0;
        dstBatchOffset = 0;
        srcBatchOffset = 0;
    }

    __aicore__ ConfusionTranspose2NZ012NTiling(
        uint32_t blockSizeIn, uint32_t shapeBIn, uint32_t shapeNIn, uint32_t hnDivIn, uint32_t blockNumIn,
        uint32_t shapeHIn, uint32_t hBlockNumIn, uint32_t sBlockNumIn, uint32_t alignHIn, uint32_t alignSIn,
        uint32_t hnDivBlockNumIn, uint32_t alignHnDivIn, uint32_t gapIn, uint32_t alignsBlockCubeIn,
        uint32_t prehBlockNumIn, uint32_t dstBatchOffsetIn, uint32_t srcBatchOffsetIn)
    {
        blockSize = blockSizeIn;
        shapeB = shapeBIn;
        shapeN = shapeNIn;
        hnDiv = hnDivIn;
        blockNum = blockNumIn;
        shapeH = shapeHIn;
        hBlockNum = hBlockNumIn;
        sBlockNum = sBlockNumIn;
        alignH = alignHIn;
        alignS = alignSIn;
        hnDivBlockNum = hnDivBlockNumIn;
        alignHnDiv = alignHnDivIn;
        gap = gapIn;
        alignsBlockCube = alignsBlockCubeIn;
        prehBlockNum = prehBlockNumIn;
        dstBatchOffset = dstBatchOffsetIn;
        srcBatchOffset = srcBatchOffsetIn;
    }
    uint32_t blockSize = 0;
    uint32_t shapeB = 0;
    uint32_t shapeN = 0;
    uint32_t hnDiv = 0;
    uint32_t blockNum = 0;
    uint32_t shapeH = 0;
    uint32_t hBlockNum = 0;
    uint32_t sBlockNum = 0;
    uint32_t alignH = 0;
    uint32_t alignS = 0;
    uint32_t hnDivBlockNum = 0;
    uint32_t alignHnDiv = 0;
    uint32_t gap = 0;
    uint32_t alignsBlockCube = 0;
    uint32_t prehBlockNum = 0;
    uint32_t dstBatchOffset = 0;
    uint32_t srcBatchOffset = 0;
};

struct ConfusionTranspose2ND012NTiling {
    __aicore__ ConfusionTranspose2ND012NTiling()
    {
        blockSize = 0;
        shapeB = 0;
        shapeN = 0;
        hnDiv = 0;
        shapeH = 0;
        hBlockNum = 0;
        sBlockNum = 0;
        hnDivBlockNum = 0;
        alignHnDiv = 0;
        gap = 0;
        alignsCube = 0;
        prehBlockNum = 0;
        alignsMulAlignHnDiv = 0;
        alignHnDivCube = 0;
        alignHnDivBlockSize = 0;
        dstBatchOffset = 0;
        srcBatchOffset = 0;
        blockNum = 0;
    }

    __aicore__ ConfusionTranspose2ND012NTiling(
        uint32_t blockSizeIn, uint32_t shapeBIn, uint32_t shapeNIn, uint32_t hnDivIn, uint32_t shapeHIn,
        uint32_t hBlockNumIn, uint32_t sBlockNumIn, uint32_t hnDivBlockNumIn, uint32_t alignHnDivIn, uint32_t gapIn,
        uint32_t alignsCubeIn, uint32_t prehBlockNumIn, uint32_t alignsMulAlignHnDivIn, uint32_t alignHnDivCubeIn,
        uint32_t alignHnDivBlockSizeIn, uint32_t dstBatchOffsetIn, uint32_t srcBatchOffsetIn, uint32_t blockNumIn)
    {
        blockSize = blockSizeIn;
        shapeB = shapeBIn;
        shapeN = shapeNIn;
        hnDiv = hnDivIn;
        shapeH = shapeHIn;
        hBlockNum = hBlockNumIn;
        sBlockNum = sBlockNumIn;
        hnDivBlockNum = hnDivBlockNumIn;
        alignHnDiv = alignHnDivIn;
        gap = gapIn;
        alignsCube = alignsCubeIn;
        prehBlockNum = prehBlockNumIn;
        alignsMulAlignHnDiv = alignsMulAlignHnDivIn;
        alignHnDivCube = alignHnDivCubeIn;
        alignHnDivBlockSize = alignHnDivBlockSizeIn;
        dstBatchOffset = dstBatchOffsetIn;
        srcBatchOffset = srcBatchOffsetIn;
        blockNum = blockNumIn;
    }
    uint32_t blockSize = 0;
    uint32_t shapeB = 0;
    uint32_t shapeN = 0;
    uint32_t hnDiv = 0;
    uint32_t shapeH = 0;
    uint32_t hBlockNum = 0;
    uint32_t sBlockNum = 0;
    uint32_t hnDivBlockNum = 0;
    uint32_t alignHnDiv = 0;
    uint32_t gap = 0;
    uint32_t alignsCube = 0;
    uint32_t prehBlockNum = 0;
    uint32_t alignsMulAlignHnDiv = 0;
    uint32_t alignHnDivCube = 0;
    uint32_t alignHnDivBlockSize = 0;
    uint32_t dstBatchOffset = 0;
    uint32_t srcBatchOffset = 0;
    uint32_t blockNum = 0;
};

struct ConfusionTranspose012Tiling {
    __aicore__ ConfusionTranspose012Tiling()
    {
        blockSize = 0;
        shapeB = 0;
        shapeN = 0;
        hnDiv = 0;
        shapeH = 0;
        hBlockNum = 0;
        sBlockNum = 0;
        hnDivBlockNum = 0;
        alignH = 0;
        alignsCube = 0;
        alignhBlockCube = 0;
        blockSizeMulAlignH = 0;
        srcBatchOffset = 0;
        dstBatchOffset = 0;
        blockNum = 0;
    }

    __aicore__ ConfusionTranspose012Tiling(
        uint32_t blockSizeIn, uint32_t shapeBIn, uint32_t shapeNIn, uint32_t hnDivIn, uint32_t shapeHIn,
        uint32_t hBlockNumIn, uint32_t sBlockNumIn, uint32_t hnDivBlockNumIn, uint32_t alignHIn, uint32_t alignsCubeIn,
        uint32_t alignhBlockCubeIn, uint32_t blockSizeMulAlignHIn, uint32_t srcBatchOffsetIn, uint32_t dstBatchOffsetIn,
        uint32_t blockNumIn)
    {
        blockSize = blockSizeIn;
        shapeB = shapeBIn;
        shapeN = shapeNIn;
        hnDiv = hnDivIn;
        shapeH = shapeHIn;
        hBlockNum = hBlockNumIn;
        sBlockNum = sBlockNumIn;
        hnDivBlockNum = hnDivBlockNumIn;
        alignH = alignHIn;
        alignsCube = alignsCubeIn;
        alignhBlockCube = alignhBlockCubeIn;
        blockSizeMulAlignH = blockSizeMulAlignHIn;
        srcBatchOffset = srcBatchOffsetIn;
        dstBatchOffset = dstBatchOffsetIn;
        blockNum = blockNumIn;
    }
    uint32_t blockSize = 0;
    uint32_t shapeB = 0;
    uint32_t shapeN = 0;
    uint32_t hnDiv = 0;
    uint32_t shapeH = 0;
    uint32_t hBlockNum = 0;
    uint32_t sBlockNum = 0;
    uint32_t hnDivBlockNum = 0;
    uint32_t alignH = 0;
    uint32_t alignsCube = 0;
    uint32_t alignhBlockCube = 0;
    uint32_t blockSizeMulAlignH = 0;
    uint32_t srcBatchOffset = 0;
    uint32_t dstBatchOffset = 0;
    uint32_t blockNum = 0;
};

struct ConfusionTransposeOnlyTiling {
    __aicore__ ConfusionTransposeOnlyTiling()
    {
        blockSize = 0;
        height = 0;
        width = 0;
        highBlock = 0;
        stride = 0;
        repeat = 0;
    }

    __aicore__ ConfusionTransposeOnlyTiling(
        uint32_t blockSizeIn, uint32_t heightIn, uint32_t widthIn, uint32_t highBlockIn, uint32_t strideIn,
        uint32_t repeatIn)
    {
        blockSize = blockSizeIn;
        height = heightIn;
        width = widthIn;
        highBlock = highBlockIn;
        stride = strideIn;
        repeat = repeatIn;
    }
    uint32_t blockSize = 0;
    uint32_t height = 0;
    uint32_t width = 0;
    uint32_t highBlock = 0;
    uint32_t stride = 0;
    uint32_t repeat = 0;
};

struct ConfusionTranspose0213Tiling {
    __aicore__ ConfusionTranspose0213Tiling()
    {
        blockSize = 0;
        shapeB = 0;
        shapeA1 = 0;
        alignA3 = 0;
        alignA2 = 0;
        widthTiling = 0;
        newPopSize = 0;
        newPopH = 0;
        needSize = 0;
        mainBlocks = 0;
        tailSize = 0;
        alignA2MulAlignA3 = 0;
        batchOffset = 0;
        alignA3MulA1 = 0;
        shapeA1BlockCube = 0;
        mainOffset = 0;
    }

    __aicore__ ConfusionTranspose0213Tiling(
        uint32_t blockSizeIn, uint32_t shapeBIn, uint32_t shapeA1In, uint32_t alignA3In, uint32_t alignA2In,
        uint32_t widthTilingIn, uint32_t newPopSizeIn, uint32_t newPopHIn, uint32_t needSizeIn, uint32_t mainBlocksIn,
        uint32_t tailSizeIn, uint32_t alignA2MulAlignA3In, uint32_t batchOffsetIn, uint32_t alignA3MulA1In,
        uint32_t shapeA1BlockCubeIn, uint32_t mainOffsetIn)
    {
        blockSize = blockSizeIn;
        shapeB = shapeBIn;
        shapeA1 = shapeA1In;
        alignA3 = alignA3In;
        alignA2 = alignA2In;
        widthTiling = widthTilingIn;
        newPopSize = newPopSizeIn;
        newPopH = newPopHIn;
        needSize = needSizeIn;
        mainBlocks = mainBlocksIn;
        tailSize = tailSizeIn;
        alignA2MulAlignA3 = alignA2MulAlignA3In;
        batchOffset = batchOffsetIn;
        alignA3MulA1 = alignA3MulA1In;
        shapeA1BlockCube = shapeA1BlockCubeIn;
        mainOffset = mainOffsetIn;
    }
    uint32_t blockSize = 0;
    uint32_t shapeB = 0;
    uint32_t shapeA1 = 0;
    uint32_t alignA3 = 0;
    uint32_t alignA2 = 0;
    uint32_t widthTiling = 0;
    uint32_t newPopSize = 0;
    uint32_t newPopH = 0;
    uint32_t needSize = 0;
    uint32_t mainBlocks = 0;
    uint32_t tailSize = 0;
    uint32_t alignA2MulAlignA3 = 0;
    uint32_t batchOffset = 0;
    uint32_t alignA3MulA1 = 0;
    uint32_t shapeA1BlockCube = 0;
    uint32_t mainOffset = 0;
};

struct ConfusionTranspose021Tiling {
    __aicore__ ConfusionTranspose021Tiling()
    {
        dim0 = 0;
        dim1 = 0;
        dim2 = 0;
    }

    __aicore__ ConfusionTranspose021Tiling(uint32_t dim0In, uint32_t dim1In, uint32_t dim2In)
    {
        dim0 = dim0In;
        dim1 = dim1In;
        dim2 = dim2In;
    }
    uint32_t dim0 = 0;
    uint32_t dim1 = 0;
    uint32_t dim2 = 0;
};

struct ConfusionTranspose102Tiling {
    __aicore__ ConfusionTranspose102Tiling()
    {
        dim0 = 0;
        dim1 = 0;
        dim2 = 0;
    }

    __aicore__ ConfusionTranspose102Tiling(uint32_t dim0In, uint32_t dim1In, uint32_t dim2In)
    {
        dim0 = dim0In;
        dim1 = dim1In;
        dim2 = dim2In;
    }
    uint32_t dim0 = 0;
    uint32_t dim1 = 0;
    uint32_t dim2 = 0;
};

struct ConfusionTranspose210Tiling {
    __aicore__ ConfusionTranspose210Tiling()
    {
        dim0 = 0;
        dim1 = 0;
        dim2 = 0;
    }

    __aicore__ ConfusionTranspose210Tiling(uint32_t dim0In, uint32_t dim1In, uint32_t dim2In)
    {
        dim0 = dim0In;
        dim1 = dim1In;
        dim2 = dim2In;
    }
    uint32_t dim0 = 0;
    uint32_t dim1 = 0;
    uint32_t dim2 = 0;
};

struct IntriInfo {
    uint32_t c0Count{0};
    uint32_t repeat{0};
    uint32_t repeatRounding{0};
    uint32_t repeatRemaining{0};
    uint32_t tail{0};
};

enum class PaddingMode : uint8_t {
    Normal = 0,
    Compact,
};

struct PadParams {
    __aicore__ PadParams()
    {
        leftPad = 0;
        rightPad = 0;
        padValue = 0;
    }

    __aicore__ PadParams(const uint16_t leftPadIn, const uint16_t rightPadIn, const int32_t padValueIn)
    {
        leftPad = leftPadIn;
        rightPad = rightPadIn;
        padValue = padValueIn;
    }

    uint16_t leftPad = 0;
    uint16_t rightPad = 0;
    int32_t padValue = 0;
};

struct UnPadParams {
    __aicore__ UnPadParams()
    {
        leftPad = 0;
        rightPad = 0;
    }

    __aicore__ UnPadParams(const uint16_t leftPadIn, const uint16_t rightPadIn)
    {
        leftPad = leftPadIn;
        rightPad = rightPadIn;
    }

    uint16_t leftPad = 0;
    uint16_t rightPad = 0;
};

// aipp config offset
constexpr int32_t AIPP_OFFSET_CSC_ENABLE = 63;
constexpr int32_t AIPP_OFFSET_CH1 = 16;
constexpr int32_t AIPP_OFFSET_CH2 = 32;
constexpr int32_t AIPP_OFFSET_CH3 = 48;
constexpr int32_t AIPP_OFFSET_SWAP_RB = 16;
constexpr int32_t AIPP_OFFSET_SWAP_UV = 17;
constexpr int32_t AIPP_OFFSET_SWAP_AX = 18;
constexpr int32_t AIPP_OFFSET_FORMAT = 19;
constexpr int32_t AIPP_OFFSET_SINGLE_LINE = 24;
constexpr int32_t AIPP_OFFSET_PADDING_MODE = 27;
constexpr int32_t AIPP_OFFSET_CPADDING_MODE = 40;
constexpr int32_t AIPP_OFFSET_CSC_OUT_CH0 = 16;
constexpr int32_t AIPP_OFFSET_CSC_OUT_CH1 = 24;
constexpr int32_t AIPP_OFFSET_CSC_OUT_CH2 = 32;
constexpr int32_t AIPP_OFFSET_CSC_IN_CH0 = 40;
constexpr int32_t AIPP_OFFSET_CSC_IN_CH1 = 48;
constexpr int32_t AIPP_OFFSET_CSC_IN_CH2 = 56;
constexpr int32_t AIPP_OFFSET_DTC_CH1 = 32;
constexpr int32_t AIPP_OFFSET_DTC_ROUND_MODE = 34;
// aipp config offset for DAV_5162
constexpr int32_t AIPP_OFFSET_CH0 = 0;
constexpr int32_t AIPP_OFFSET_HORIZ_FLIP = 25;
constexpr int32_t AIPP_OFFSET_VERT_FLIP = 26;
constexpr int32_t AIPP_OFFSET_FORMAT_EXT = 35;
constexpr int32_t AIPP_OFFSET_START_COMPONENT = 38;
constexpr int32_t AIPP_OFFSET_PATTERN_INDICATOR = 41;
constexpr int32_t AIPP_OFFSET_CONCAT_MODE = 42;
constexpr int32_t AIPP_OFFSET_CHANNEL_TRANSFORM_MODE = 46;
constexpr int32_t AIPP_OFFSET_OUTPUT_STRIDE = 48;
constexpr int32_t AIPP_OFFSET_SCALE_OUT_HEIGHT = 0;
constexpr int32_t AIPP_OFFSET_SCALE_OUT_WIDTH = 16;
constexpr int32_t AIPP_OFFSET_VERT_SCALING_EN = 0;
constexpr int32_t AIPP_OFFSET_HORIZ_SCALING_EN = 2;
constexpr int32_t AIPP_OFFSET_VERT_FIRST = 7;
constexpr int32_t AIPP_OFFSET_VERT_SCALE_METHOD_CH0 = 8;
constexpr int32_t AIPP_OFFSET_VERT_SCALE_METHOD_CH1 = 9;
constexpr int32_t AIPP_OFFSET_HORIZ_SCALE_METHOD_CH0 = 10;
constexpr int32_t AIPP_OFFSET_HORIZ_SCALE_METHOD_CH1 = 11;
constexpr int32_t AIPP_OFFSET_RAW12_SQUARE = 48;
constexpr int32_t AIPP_OFFSET_RAW12_SQUARE_RSHIFT = 50;
constexpr int32_t AIPP_OFFSET_VERT_INITIAL_PHASE = 0;
constexpr int32_t AIPP_OFFSET_HORIZ_INITIAL_PHASE = 32;
constexpr int32_t AIPP_OFFSET_VERT_SCALING_FACTOR = 0;
constexpr int32_t AIPP_OFFSET_HORIZ_SCALING_FACTOR = 32;
constexpr int32_t AIPP_OFFSET_DTC_BYPASS = 60;
constexpr int32_t AIPP_OFFSET_DTC_OFFSET_CHN0 = 0;
constexpr int32_t AIPP_OFFSET_DTC_OFFSET_CHN1 = 20;
constexpr int32_t AIPP_OFFSET_DTC_OFFSET_CHN2 = 40;
constexpr int32_t AIPP_OFFSET_DTC_PAD_MODE = 61;
constexpr int32_t AIPP_OFFSET_CSC_BIAS_IN0 = 0;
constexpr int32_t AIPP_OFFSET_CSC_BIAS_IN1 = 8;
constexpr int32_t AIPP_OFFSET_CSC_BIAS_IN2 = 16;
constexpr int32_t AIPP_OFFSET_DTC_OFFSET_CHN3 = 24;
constexpr int32_t AIPP_OUTPUT_CHANNEL_NUM = 3;
constexpr int32_t AIPP_YUV400_OUTPUT_CHANNEL_NUM = 1;
constexpr int32_t AIPP_YUV420_SRC0_CHANNEL_NUM = 1;
constexpr int32_t AIPP_XRGB8888_OUTPUT_CHANNEL_NUM = 4;
constexpr int32_t AIPP_OUTPUT_PADDING_MODE0_CHANNEL_NUM = 4;

} // namespace AscendC
#endif // ASCENDC_MODULE_UTILS_STRUCT_CONFUSION_PAD_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_STRUCT_CONFUSION_PAD_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_UTILS_STRUCT_CONFUSION_PAD_H__
#endif
