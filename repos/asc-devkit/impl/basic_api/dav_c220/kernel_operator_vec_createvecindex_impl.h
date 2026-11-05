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
 * \file kernel_operator_vec_createvecindex_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_c220/kernel_operator_vec_createvecindex_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tpipe.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CREATEVECINDEX_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_CREATEVECINDEX_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_CREATEVECINDEX_IMPL_H
#include "../../../include/basic_api/kernel_tensor.h"
#include "../../../include/basic_api/kernel_struct_unary.h"
#include "../../../include/basic_api/kernel_tpipe.h"
#include "../../../include/basic_api/kernel_operator_vec_binary_scalar_intf.h"
#if ASCENDC_CPU_DEBUG
#include "../kernel_check.h"
#endif

namespace AscendC {
constexpr int32_t maskBitNum = 64;

template <typename T>
__aicore__ inline T GetCreateVecIndexValue(const T& firstValue, int32_t offset)
{
    if constexpr (SupportType<T, int8_t, int16_t, int32_t, int64_t>()) {
        return static_cast<T>(static_cast<int64_t>(firstValue) + static_cast<int64_t>(offset));
    } else if constexpr (SupportType<T, uint8_t, uint16_t, uint32_t, uint64_t>()) {
        return static_cast<T>(static_cast<uint64_t>(firstValue) + static_cast<uint64_t>(offset));
    } else {
        return static_cast<T>(static_cast<float>(firstValue) + static_cast<float>(offset));
    }
}

template <typename T>
__aicore__ inline void CreateVecIndexOneBlk(const LocalTensor<T>& dst, const T& firstValue, uint32_t count)
{
    for (int32_t i = 0; i < static_cast<int32_t>(count); i++) {
        dst.SetValue(i, GetCreateVecIndexValue(firstValue, i));
    }
    auto eventIdSToV = GetTPipePtr()->FetchEventID(HardEvent::S_V);
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
}

template <typename T>
__aicore__ inline void CreateVecIndexOneRep(
    const LocalTensor<T>& dst, const T& firstValue, uint64_t mask[], uint16_t dstBlkStride)
{
    constexpr int32_t eleCntOfOneBlk = (ONE_BLK_SIZE / sizeof(T));
    constexpr int32_t eleCntOfOneRep = (ONE_BLK_SIZE * DEFAULT_REPEAT_STRIDE / sizeof(T));
    if constexpr (sizeof(T) == sizeof(half)) {
        for (int i = 0; i < 2; i++) {
            uint64_t maskValue = 1;
            for (int j = 0; j < maskBitNum; j++) {
                if (mask[i] & maskValue) {
                    uint32_t index = i * maskBitNum + j;
                    uint32_t blkIndex = index / eleCntOfOneBlk;
                    uint32_t eleIndex = blkIndex * eleCntOfOneBlk * dstBlkStride + index % eleCntOfOneBlk;
                    dst.SetValue(eleIndex, GetCreateVecIndexValue(firstValue, i * maskBitNum + j));
                }
                maskValue <<= 1;
            }
        }
    } else {
        uint64_t maskValue = 1;
        for (int j = 0; j < maskBitNum; j++) {
            if (mask[0] & maskValue) {
                uint32_t blkIndex = j / eleCntOfOneBlk;
                uint32_t eleIndex = blkIndex * eleCntOfOneBlk * dstBlkStride + j % eleCntOfOneBlk;
                dst.SetValue(eleIndex, GetCreateVecIndexValue(firstValue, j));
            }
            maskValue <<= 1;
        }
    }
    auto eventIdSToV = GetTPipePtr()->FetchEventID(HardEvent::S_V);
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
}

template <typename T>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<T>& dst, const T& firstValue, uint64_t mask, uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    // 1st block
    constexpr int32_t eleCntOfOneBlk = (ONE_BLK_SIZE / sizeof(T));
    if (mask < eleCntOfOneBlk) {
        CreateVecIndexOneBlk(dst, firstValue, mask);
    } else {
        CreateVecIndexOneBlk(dst, firstValue, eleCntOfOneBlk);
    }
    constexpr int32_t eleCntOfOneRep = (ONE_BLK_SIZE * DEFAULT_REPEAT_STRIDE / sizeof(T));
    UnaryRepeatParams addsParams;
    // 2~8 block
    int32_t loopN = mask / eleCntOfOneBlk - 1;
    int32_t tailSize = mask % eleCntOfOneBlk;
    int32_t blkEleStride = dstBlkStride * eleCntOfOneBlk;
    int32_t repEleStride = dstRepStride * eleCntOfOneBlk;
    for (int i = 0; i < loopN; i++) {
        Adds(dst[(i + 1) * blkEleStride], dst[i * blkEleStride], (T)(eleCntOfOneBlk), eleCntOfOneBlk, 1, addsParams);
        PipeBarrier<PIPE_V>();
    }
    addsParams.dstBlkStride = dstBlkStride;
    addsParams.srcBlkStride = dstBlkStride;
    int32_t offsetTailDst = mask / eleCntOfOneBlk * eleCntOfOneBlk * dstBlkStride;
    int32_t offsetTailSrc = offsetTailDst - eleCntOfOneBlk * dstBlkStride;
    if (tailSize > 0) {
        Adds(dst[offsetTailDst], dst[offsetTailSrc], (T)eleCntOfOneBlk, tailSize, 1, addsParams);
        PipeBarrier<PIPE_V>();
    }

    // 2~n repeats
    for (int i = 0; i < repeatTime - 1; i++) {
        Adds(dst[(i + 1) * repEleStride], dst[i * repEleStride], (T)(eleCntOfOneRep), mask, 1, addsParams);
        PipeBarrier<PIPE_V>();
    }
}

template <typename T>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<T>& dst, const T& firstValue, uint64_t mask[], uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    // first rep
    CreateVecIndexOneRep(dst, firstValue, mask, dstBlkStride);
    // 2~n repeats
    UnaryRepeatParams addsParams;
    addsParams.dstBlkStride = dstBlkStride;
    addsParams.srcBlkStride = dstBlkStride;
    constexpr int32_t eleCntOfOneBlk = (ONE_BLK_SIZE / sizeof(T));
    constexpr int32_t eleCntOfOneRep = (ONE_BLK_SIZE * DEFAULT_REPEAT_STRIDE / sizeof(T));
    int32_t blkEleStride = dstBlkStride * eleCntOfOneBlk;
    int32_t repEleStride = dstRepStride * eleCntOfOneBlk;
    for (int i = 0; i < repeatTime - 1; i++) {
        Adds(dst[(i + 1) * repEleStride], dst[i * repEleStride], (T)(eleCntOfOneRep), mask, 1, addsParams);
        PipeBarrier<PIPE_V>();
    }
}

template <typename T>
__aicore__ inline void CreateVecIndexCalc(LocalTensor<T> dst, const T& firstValue, uint32_t count)
{
    ASCENDC_ASSERT((SupportType<T, half, int16_t, float, int32_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to check "
            "dtype in CreateVecIndex, current api support dtype combination is dst: half / int16_t / float / int32_t");
    });
    // first block
    constexpr int32_t eleCntOfOneBlk = (ONE_BLK_SIZE / sizeof(T));
    if (count <= eleCntOfOneBlk) {
        CreateVecIndexOneBlk(dst, firstValue, count);
        return;
    }
    CreateVecIndexOneBlk(dst, firstValue, static_cast<uint32_t>(eleCntOfOneBlk));

    UnaryRepeatParams addsParams;
    constexpr int32_t eleCntOfOneRep = (ONE_BLK_SIZE * DEFAULT_REPEAT_STRIDE / sizeof(T));
    // 2~8 block
    int32_t loopN = 0, tailSize = 0, offsetTailDst, offsetTailSrc;
    if (count >= eleCntOfOneRep) {
        loopN = DEFAULT_REPEAT_STRIDE - 1;
    } else {
        loopN = count / eleCntOfOneBlk - 1;
        tailSize = count % eleCntOfOneBlk;
    }
    for (int i = 0; i < loopN; i++) {
        Adds(dst[(i + 1) * eleCntOfOneBlk], dst[i * eleCntOfOneBlk], (T)eleCntOfOneBlk, eleCntOfOneBlk, 1, addsParams);
        PipeBarrier<PIPE_V>();
    }
    offsetTailDst = count / eleCntOfOneBlk * eleCntOfOneBlk;
    offsetTailSrc = offsetTailDst - eleCntOfOneBlk;
    if (tailSize > 0) {
        Adds(dst[offsetTailDst], dst[offsetTailSrc], (T)eleCntOfOneBlk, tailSize, 1, addsParams);
        PipeBarrier<PIPE_V>();
    }
    if (count <= eleCntOfOneRep) {
        return;
    }
    // 2~n repeats
    loopN = count / eleCntOfOneRep - 1;
    tailSize = count % eleCntOfOneRep;
    for (int i = 0; i < loopN; i++) {
        Adds(
            dst[(i + 1) * eleCntOfOneRep], dst[i * eleCntOfOneRep], (T)(eleCntOfOneRep), eleCntOfOneRep, 1, addsParams);
        PipeBarrier<PIPE_V>();
    }
    offsetTailDst = count / eleCntOfOneRep * eleCntOfOneRep;
    offsetTailSrc = offsetTailDst - eleCntOfOneRep;
    if (tailSize > 0) {
        Adds(dst[offsetTailDst], dst[offsetTailSrc], (T)(eleCntOfOneRep), tailSize, 1, addsParams);
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_CREATEVECINDEX_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CREATEVECINDEX_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CREATEVECINDEX_IMPL_H__
#endif
