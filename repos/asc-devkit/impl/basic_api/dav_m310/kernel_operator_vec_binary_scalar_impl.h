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
 * \file kernel_operator_vec_binary_scalar_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m310/kernel_operator_vec_binary_scalar_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tpipe.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#include "../kernel_utils.h"
#include "kernel_operator_common_impl.h"
#include "../../../include/basic_api/kernel_struct_unary.h"
#include "../../../include/basic_api/kernel_tpipe.h"
#include "../../../include/basic_api/kernel_common.h"

namespace AscendC {
/* **************************************************************************************************
 * Adds                                             *
 * ************************************************************************************************* */
// Adds::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void AddsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void AddsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vadds(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void AddsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vadds(vreg1, vreg0, scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void AddsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vadds(vreg1, vreg0, scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void AddsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vadds(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AddsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void AddsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vadds(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void AddsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vadds(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void AddsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vadds(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void AddsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vadds(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

// Adds::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void AddsImpl(__ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const int32_t& count)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void AddsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vadds(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void AddsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vadds(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

template <>
__aicore__ inline void AddsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vadds(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void AddsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vadds(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

/* **************************************************************************************************
 * Muls                                             *
 * ************************************************************************************************* */
// Muls::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MulsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void MulsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmuls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void MulsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vmuls(vreg1, vreg0, scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void MulsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vmuls(vreg1, vreg0, scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void MulsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmuls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MulsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void MulsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmuls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void MulsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmuls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void MulsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vmuls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void MulsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vmuls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

// Muls::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MulsImpl(__ubuf__ T* dst, __ubuf__ T* src, const T scalarValue, const int32_t& count)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void MulsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmuls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void MulsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmuls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

template <>
__aicore__ inline void MulsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmuls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void MulsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmuls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

/* **************************************************************************************************
 * Maxs                                             *
 * ************************************************************************************************* */
// Maxs::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MaxsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void MaxsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmaxs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void MaxsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vmaxs(vreg1, vreg0, scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void MaxsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vmaxs(vreg1, vreg0, scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void MaxsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmaxs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MaxsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void MaxsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmaxs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void MaxsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmaxs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void MaxsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vmaxs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void MaxsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vmaxs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

// Maxs::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MaxsImpl(__ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const int32_t& count)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void MaxsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmaxs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void MaxsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmaxs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

template <>
__aicore__ inline void MaxsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmaxs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void MaxsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmaxs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

/* **************************************************************************************************
 * Mins                                             *
 * ************************************************************************************************* */
// Mins::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void MinsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmins(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void MinsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vmins(vreg1, vreg0, scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void MinsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vmins(vreg1, vreg0, scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void MinsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmins(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void MinsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmins(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void MinsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vmins(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void MinsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vmins(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void MinsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vmins(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

// Mins::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(__ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const int32_t& count)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void MinsImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmins(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void MinsImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmins(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

template <>
__aicore__ inline void MinsImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmins(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void MinsImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vmins(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

/* **************************************************************************************************
 * ShiftLeft                                             *
 * ************************************************************************************************* */
// ShiftLeft::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftLeftImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void ShiftLeftImpl<uint16_t, true>(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t scalarValue, const uint64_t mask[],
    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_u16 vreg0;
        vector_u16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vshls(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void ShiftLeftImpl<uint32_t, true>(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t scalarValue, const uint64_t mask[],
    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_u32 vreg0;
        vector_u32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vshls(vreg1, vreg0, (int16_t)scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void ShiftLeftImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vshls(vreg1, vreg0, (int16_t)scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void ShiftLeftImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vshls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftLeftImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void ShiftLeftImpl<uint16_t, true>(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_u16 vreg0;
        vector_u16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vshls(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftLeftImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vshls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftLeftImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vshls(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftLeftImpl<uint32_t, true>(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_u32 vreg0;
        vector_u32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vshls(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

// ShiftLeft::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftLeftImpl(__ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const int32_t& count)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void ShiftLeftImpl<uint16_t, true>(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_u16 vreg0;
        vector_u16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vshls(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftLeftImpl<uint32_t, true>(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_u32 vreg0;
        vector_u32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vshls(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftLeftImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vshls(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftLeftImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vshls(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

/* **************************************************************************************************
 * ShiftRight                                             *
 * ************************************************************************************************* */
// ShiftRight::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftRightImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void ShiftRightImpl<uint16_t, true>(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t scalarValue, const uint64_t mask[],
    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams, bool roundEn)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_u16 vreg0;
        vector_u16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vshrs(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void ShiftRightImpl<uint32_t, true>(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t scalarValue, const uint64_t mask[],
    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams, bool roundEn)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_u32 vreg0;
        vector_u32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vshrs(vreg1, vreg0, (int16_t)scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void ShiftRightImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vshrs(vreg1, vreg0, (int16_t)scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void ShiftRightImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vshrs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftRightImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void ShiftRightImpl<uint16_t, true>(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn)
{
    __VEC_SCOPE__
    {
        vector_u16 vreg0;
        vector_u16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vshrs(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftRightImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vshrs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftRightImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vshrs(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftRightImpl<uint32_t, true>(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn)
{
    __VEC_SCOPE__
    {
        vector_u32 vreg0;
        vector_u32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vshrs(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

// ShiftLeft::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftRightImpl(__ubuf__ T* dst, __ubuf__ T* src, const T scalarValue, const int32_t& count)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void ShiftRightImpl<uint16_t, true>(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_u16 vreg0;
        vector_u16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vshrs(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftRightImpl<uint32_t, true>(
    __ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_u32 vreg0;
        vector_u32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vshrs(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftRightImpl<int16_t, true>(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_s16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vshrs(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}

template <>
__aicore__ inline void ShiftRightImpl<int32_t, true>(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_s32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vshrs(vreg1, vreg0, (int16_t)scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

/* **************************************************************************************************
 * LeakyRelu                                             *
 * ************************************************************************************************* */
// LeakyRelu::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void LeakyReluImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        vector_bool preg;
        vector_bool preg1;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        punpack(preg1, preg, LOWER);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg1);
            vlrelu(vreg1, vreg0, scalarValue, preg1, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg1);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <>
__aicore__ inline void LeakyReluImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint8_t* tmpBuf = AscendCUtils::GetTemporaryBufferAddr<uint8_t>(TMP_UB_OFFSET, 16);
    *((__ubuf__ uint64_t*)tmpBuf) = mask[0];
    *((__ubuf__ uint64_t*)tmpBuf + 1) = mask[1];
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)tmpBuf), 0, US);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vlrelu(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
    AscendCUtils::FreeTemporaryBuffer<uint8_t>(tmpBuf);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void LeakyReluImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 8, strideConfig0, preg);
            vlrelu(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 8, strideConfig1, preg);
        }
    }
}

template <>
__aicore__ inline void LeakyReluImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig0 = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << 16);
        uint32_t strideConfig1 = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsldb(vreg0, src + i * repeatParams.srcRepStride * 16, strideConfig0, preg);
            vlrelu(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg1, dst + i * repeatParams.dstRepStride * 16, strideConfig1, preg);
        }
    }
}

// LeakyRelu::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(__ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const int32_t& count)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device"); });
}

template <>
__aicore__ inline void LeakyReluImpl<float, true>(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_f32 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(64);
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vlrelu(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B32, preg);
        }
    }
}

template <>
__aicore__ inline void LeakyReluImpl<half, true>(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_f16 vreg1;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint32_t sregLower = static_cast<uint32_t>(128);
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vlds(vreg0, src, i * sregLower, NORM);
            vlrelu(vreg1, vreg0, scalarValue, preg, MODE_ZEROING);
            vsts(vreg1, dst, i * sregLower, NORM_B16, preg);
        }
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__
#endif
