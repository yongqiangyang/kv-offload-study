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
 * \file kernel_struct_aipp.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_AIPP_H__
#endif

#ifndef ASCENDC_MODULE_STRUCT_AIPP_H
#define ASCENDC_MODULE_STRUCT_AIPP_H

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "kernel_fp16.h"
#endif

namespace AscendC {
enum class AippInputFormat : uint8_t {
    YUV420SP_U8 = 0,
    XRGB8888_U8 = 1,
    RGB888_U8 = 4,
    YUV400_U8 = 9,
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5162)
    NC1HWC0_DI_FP16 = 2,
    NC1HWC0_DI_U8 = 3,
    ARGB8888_U8 = 5,
    YUYV_PACKED_U8 = 6,
    YUV422SP_U8 = 7,
    AYUV444_U8 = 8,
    RAW8 = 27,
    RAW10 = 10,
    RAW12 = 11,
    RAW16 = 12,
    ARGB16_U16 = 13,
    XRGB16_U16 = 14,
    RAW24 = 15,
    YUV420SP_U10 = 16,
    RGB8_PLUS_IR = 21,
    RGB16_PLUS_IR = 22,
    R11G11B10_FLOAT = 26,
    R10G10B10A2_UNORM = 28,
    YUV444SP_U8 = 29
#endif
};

enum class AippPaddingMode : uint32_t { CONSTANT = 0, SINGLE_REPLICATION = 1, MULTI_REPLICATION = 2, MIRROR = 3 };

template <typename T>
struct AippPaddingParams {
    uint32_t paddingMode{0};
    T paddingValueCh0{0};
    T paddingValueCh1{0};
    T paddingValueCh2{0};
    T paddingValueCh3{0};
};

struct AippSwapParams {
    bool isSwapRB{false};
    bool isSwapUV{false};
    bool isSwapAX{false};
};

struct AippFlipParams {
    bool isHorizontalFlip{false};
    bool isVerticalFlip{false};
};

enum class AippScalingMethod : uint8_t { BILINEAR = 0, NEAREST = 1 };

struct AippScalingParams {
    bool isVertScaling{false};
    bool isHorizScaling{false};

    bool vertFirst{false};
    /* 0: non alpha channel, 1: alpha channel */
    AippScalingMethod vertScalingMethod[2]{AippScalingMethod::BILINEAR, AippScalingMethod::BILINEAR};
    AippScalingMethod horizScalingMethod[2]{AippScalingMethod::BILINEAR, AippScalingMethod::BILINEAR};

    bool raw12Square{false};
    uint8_t raw12SquareRShift{0}; /* valid: 0~9 */

    uint32_t outputWidth{0};
    uint32_t outputHeight{0};

    uint32_t horizInitialPhase{0};
    uint32_t vertInitialPhase{0};

    uint32_t horizScalingFactor{0};
    uint32_t vertScalingFactor{0};
};

enum class AippChannelTransformMode : uint8_t {
    NONE = 0,
    SUBPIXEL_2 = 1,
    CANVAS_TRANSFORM = 2,
    SUBPIXEL_4 = 3,
    G_TO_2G = 4,
    SPACE_TO_DEPTH_2x2 = 5,
    SPACE_TO_DEPTH_4x4 = 6,
    SPACE_TO_DEPTH_6x6 = 7
};

enum class AippConcatMode : uint8_t { NONE = 0, CONCAT_2FRAME = 1, CONCAT_4FRAME = 2, CONCAT_3FRAME = 3 };

struct AippSingleLineParams {
    bool isSingleLineCopy{false};
};

enum class AippDtcPadMode : uint32_t { PAD_AS_4ZERO_U12 = 0, PAD_AS_1ZERO_U12_3ZERO };

/* for RAW input_format */
enum class AippPatternIndicator : uint8_t { BAYER_PATTERN = 0, QUADRA_PATTERN };

enum class AippStartComponent : uint8_t { R_COMPONENT = 0, Gr_COMPONENT = 1, Gb_COMPONENT = 2, B_COMPONENT = 3 };

struct AippDataTypeConvParams {
    uint8_t dtcMeanCh0{0};
    uint8_t dtcMeanCh1{0};
    uint8_t dtcMeanCh2{0};
    half dtcMinCh0{0};
    half dtcMinCh1{0};
    half dtcMinCh2{0};
    half dtcVarCh0{1.0};
    half dtcVarCh1{1.0};
    half dtcVarCh2{1.0};
    uint32_t dtcRoundMode{0};
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5162)
    bool isDtcBypass{true};
    AippDtcPadMode dtcPadMode{AippDtcPadMode::PAD_AS_4ZERO_U12};

    uint16_t dtcGainCh0{0}; /* unsigned fixed value, higher 8bit integer part and lower 8bit decimal part */
    uint16_t dtcGainCh1{0}; /* unsigned fixed value, higher 8bit integer part and lower 8bit decimal part */
    uint16_t dtcGainCh2{0}; /* unsigned fixed value, higher 8bit integer part and lower 8bit decimal part */
    uint16_t dtcGainCh3{0}; /* unsigned fixed value, higher 8bit integer part and lower 8bit decimal part */

    int32_t dtcOffsetChn0{0};
    int32_t dtcOffsetChn1{0};
    int32_t dtcOffsetChn2{0};
    int32_t dtcOffsetChn3{0};
#endif
};

template <typename T>
struct AippChannelPaddingParams {
    uint32_t cPaddingMode;
    T cPaddingValue;
};

struct AippColorSpaceConvParams {
    bool isEnableCsc{false};
    int16_t cscMatrixR0C0{0};
    int16_t cscMatrixR0C1{0};
    int16_t cscMatrixR0C2{0};
    int16_t cscMatrixR1C0{0};
    int16_t cscMatrixR1C1{0};
    int16_t cscMatrixR1C2{0};
    int16_t cscMatrixR2C0{0};
    int16_t cscMatrixR2C1{0};
    int16_t cscMatrixR2C2{0};
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5162)
    int16_t cscBiasIn0{0}; /* use low 10bit */
    int16_t cscBiasIn1{0}; /* use low 10bit */
    int16_t cscBiasIn2{0}; /* use low 10bit */
#else
    uint8_t cscBiasIn0{0};
    uint8_t cscBiasIn1{0};
    uint8_t cscBiasIn2{0};
#endif
    uint8_t cscBiasOut0{0};
    uint8_t cscBiasOut1{0};
    uint8_t cscBiasOut2{0};
};

template <typename T>
struct AippParams {
    AippPaddingParams<T> paddingParams;
    AippSwapParams swapParams;
    AippSingleLineParams singleLineParams;
    AippDataTypeConvParams dtcParams;
    AippChannelPaddingParams<T> cPaddingParams;
    AippColorSpaceConvParams cscParams;
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 5162)
    AippFlipParams flipParams;
    AippChannelTransformMode channelTransformMode{AippChannelTransformMode::NONE};
    AippPatternIndicator patternIndicator{AippPatternIndicator::BAYER_PATTERN};
    AippStartComponent startComponent{AippStartComponent::R_COMPONENT};
    AippScalingParams scalingParams;
    AippConcatMode concatMode{AippConcatMode::NONE};
    uint32_t outputStride{0};
#endif
};
} // namespace AscendC
#endif // ASCENDC_MODULE_STRUCT_AIPP_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_AIPP_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_AIPP_H__
#endif
