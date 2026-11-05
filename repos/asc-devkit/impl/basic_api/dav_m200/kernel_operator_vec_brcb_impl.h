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
 * \file kernel_operator_vec_brcb_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m200/kernel_operator_vec_brcb_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tpipe.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BRCB_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_BRCB_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_BRCB_IMPL_H
#include "../../../include/basic_api/kernel_struct_brcb.h"
#include "../../../include/basic_api/kernel_tpipe.h"

namespace AscendC {
/* **************************************************************************************************
 * Brcb                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void BrcbImplB32Impl(
    __ubuf__ T* dst, __ubuf__ T* src0, const uint8_t repeatTime, const BrcbRepeatParams& repeatParams)
{
    // only support for dstblockStride = 1
    constexpr uint16_t oneRepeatNum = 8;
    constexpr int32_t defaultTmpSize = 256;
    constexpr uint16_t defaultLen = 8;
    constexpr uint8_t defaultStride = 8;
    uint16_t dstRptEle = ONE_BLK_SIZE / sizeof(uint32_t) * repeatParams.dstRepStride;

    __ubuf__ int32_t* tempBuf = AscendCUtils::GetTemporaryBufferAddr<int32_t>(TMP_UB_OFFSET, defaultTmpSize);
    uint16_t srcOffset = 0;
    uint16_t dstOffset = 0;
    AscendCUtils::SetMask<uint32_t>(0, FULL_MASK);
    for (uint8_t i = 0; i < repeatTime; i++) {
        // padding data to 8 * 8
        vadds(
            (__ubuf__ int32_t*)tempBuf, (__ubuf__ int32_t*)src0 + srcOffset, static_cast<int32_t>(0),
            static_cast<uint8_t>(1), static_cast<uint16_t>(1), static_cast<uint16_t>(0), defaultStride,
            static_cast<uint8_t>(0));
        PipeBarrier<PIPE_V>();
        // transpose
        v4dtrans((__ubuf__ uint32_t*)dst + dstOffset, (__ubuf__ uint32_t*)tempBuf, defaultLen, defaultLen, true);
        PipeBarrier<PIPE_V>();
        srcOffset += oneRepeatNum;
        dstOffset += dstRptEle;
    }
}

template <typename T>
__aicore__ inline void BrcbImplB16Impl(
    __ubuf__ T* dst, __ubuf__ T* src0, const uint8_t repeatTime, const BrcbRepeatParams& repeatParams)
{
    // only support for dstblockStride = 1
    constexpr uint32_t tmpUbSize = 1024;
    constexpr uint16_t residualOffset = 128;
    constexpr uint8_t defaultStride = 8;
    constexpr int32_t defaultTmpSize = 512;
    constexpr uint64_t maskValue = 0x00ff00ff00ff00ff;
    constexpr uint16_t oneRepeatNum = 8;
    uint16_t dstRptEle = ONE_BLK_SIZE / sizeof(uint16_t) * repeatParams.dstRepStride;

    __ubuf__ int16_t* tempBuf =
        AscendCUtils::GetTemporaryBufferAddr<int16_t>(TMP_UB_OFFSET + tmpUbSize, defaultTmpSize);
    __ubuf__ int16_t* tempBuf1 = (__ubuf__ int16_t*)tempBuf + residualOffset * 2;

    uint16_t srcOffset = 0;
    uint16_t dstOffset = 0;
    AscendCUtils::SetMask<int16_t>(FULL_MASK, FULL_MASK);
    for (uint8_t i = 0; i < repeatTime; i++) {
        // save the rear data(128) in tmp buffer
        vadds(
            (__ubuf__ int16_t*)tempBuf1, (__ubuf__ int16_t*)dst + dstOffset + residualOffset, static_cast<int16_t>(0),
            static_cast<uint8_t>(1), static_cast<uint16_t>(1), static_cast<uint16_t>(1), defaultStride, defaultStride);
        // padding to 16 * 16
        AscendCUtils::SetMask<int16_t>(maskValue, maskValue);
        vadds(
            (__ubuf__ int16_t*)tempBuf, (__ubuf__ int16_t*)src0 + srcOffset, static_cast<int16_t>(0),
            static_cast<uint8_t>(2), static_cast<uint16_t>(1), static_cast<uint16_t>(0), defaultStride,
            static_cast<uint8_t>(0));
        PipeBarrier<PIPE_V>();
        vtranspose((__ubuf__ uint16_t*)dst + dstOffset, (__ubuf__ uint16_t*)tempBuf);
        PipeBarrier<PIPE_V>();
        // restore the rear data to dst from tmp buffer
        AscendCUtils::SetMask<uint16_t>(FULL_MASK, FULL_MASK);
        vadds(
            (__ubuf__ int16_t*)dst + dstOffset + residualOffset, (__ubuf__ int16_t*)(tempBuf1), static_cast<int16_t>(0),
            static_cast<uint8_t>(1), static_cast<uint16_t>(1), static_cast<uint16_t>(1), defaultStride, defaultStride);
        PipeBarrier<PIPE_V>();
        srcOffset += oneRepeatNum;
        dstOffset += dstRptEle;
    }
}

template <typename T>
__aicore__ inline void BrcbImplStrideOne(
    __ubuf__ T* dst, __ubuf__ T* src0, const uint8_t repeatTime, const BrcbRepeatParams& repeatParams)
{
    if constexpr (sizeof(T) == sizeof(uint16_t)) {
        BrcbImplB16Impl(dst, src0, repeatTime, repeatParams);
    } else {
        BrcbImplB32Impl(dst, src0, repeatTime, repeatParams);
    }
}

template <typename T>
__aicore__ inline void BrcbImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, const uint8_t repeatTime, const BrcbRepeatParams& repeatParams)
{
    SetMaskNorm();
    if constexpr (sizeof(T) != B16_BYTE_SIZE && sizeof(T) != B32_BYTE_SIZE) {
        ASCENDC_ASSERT(false, {
            KERNEL_LOG(
                KERNEL_ERROR,
                "Failed to check dtype in Brcb, current api support dtype "
                "combination is src and dst both: half / int16_t / uint16_t / float / int32_t / uint32_t.");
        });
        return;
    }
    if (likely((repeatParams.dstBlkStride == 1) || (repeatParams.dstBlkStride == 0))) {
        BrcbImplStrideOne(dst, src0, repeatTime, repeatParams);
    } else {
        constexpr uint8_t blockNum = 8;
        uint16_t dstblkEle = ONE_BLK_SIZE / sizeof(T) * repeatParams.dstBlkStride;
        uint16_t dstRptEle = ONE_BLK_SIZE / sizeof(T) * repeatParams.dstRepStride;
        uint64_t mask = ONE_BLK_SIZE / sizeof(T);
        AscendCUtils::SetMask<T>(mask);
        event_t eventID0 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
        event_t eventID1 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE2_S));
        event_t eventID2 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_S));
        event_t eventID3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE3_S));
        SetFlag<HardEvent::MTE2_S>(eventID1);
        WaitFlag<HardEvent::MTE2_S>(eventID1);
        SetFlag<HardEvent::V_S>(eventID2);
        WaitFlag<HardEvent::V_S>(eventID2);
        SetFlag<HardEvent::MTE3_S>(eventID3);
        WaitFlag<HardEvent::MTE3_S>(eventID3);
        for (uint8_t i = 0; i < repeatTime; i++) {
            for (uint8_t j = 0; j < blockNum; j++) {
                T scalarValue = *((__ubuf__ T*)src0 + i * blockNum + j);
                SetFlag<HardEvent::S_V>(eventID0);
                WaitFlag<HardEvent::S_V>(eventID0);
                vector_dup(
                    (__ubuf__ T*)(dst) + i * dstRptEle + j * dstblkEle, scalarValue, static_cast<uint8_t>(1),
                    static_cast<uint16_t>(1), static_cast<uint16_t>(1), blockNum, static_cast<uint8_t>(0));
            }
        }
    }
    ResetMask();
}

} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_BRCB_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BRCB_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BRCB_IMPL_H__
#endif
