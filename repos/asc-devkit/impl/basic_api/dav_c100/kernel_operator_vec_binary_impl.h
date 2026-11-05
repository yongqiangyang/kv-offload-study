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
 * \file kernel_operator_vec_binary_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_c100/kernel_operator_vec_binary_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_BINARY_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_BINARY_IMPL_H
#include "../kernel_utils.h"
#include "../../../include/basic_api/kernel_struct_binary.h"

namespace AscendC {
template <typename T>
__aicore__ inline void BinaryCompute(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count,
    void (*func)(__ubuf__ T*, __ubuf__ T*, __ubuf__ T*, const uint64_t, const uint8_t, const BinaryRepeatParams&))
{
    struct BinaryRepeatParams repeatParams;
    IntriInfo intriInfo = AscendCUtils::CalIntriInfo(sizeof(T), count);

    uint32_t dstOffset = 0;
    uint32_t src0Offset = 0;
    uint32_t src1Offset = 0;
    const auto dstOffsetCount = MAX_REPEAT_TIMES * repeatParams.dstRepStride * intriInfo.c0Count;
    const auto src0OffsetCount = MAX_REPEAT_TIMES * repeatParams.src0RepStride * intriInfo.c0Count;
    const auto src1OffsetCount = MAX_REPEAT_TIMES * repeatParams.src1RepStride * intriInfo.c0Count;

    const int32_t fullMask = intriInfo.c0Count * DEFAULT_BLK_NUM;
    for (uint32_t i = 0; i < intriInfo.repeatRounding; i++) {
        func(dst + dstOffset, src0 + src0Offset, src1 + src1Offset, fullMask, MAX_REPEAT_TIMES, repeatParams);
        dstOffset += dstOffsetCount;
        src0Offset += src0OffsetCount;
        src1Offset += src1OffsetCount;
    }

    dstOffset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.dstRepStride * intriInfo.c0Count;
    src0Offset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.src0RepStride * intriInfo.c0Count;
    src1Offset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.src1RepStride * intriInfo.c0Count;

    if (intriInfo.repeatRemaining != 0) {
        func(dst + dstOffset, src0 + src0Offset, src1 + src1Offset, fullMask, intriInfo.repeatRemaining, repeatParams);
    }

    if (intriInfo.tail != 0) {
        dstOffset = intriInfo.repeat * repeatParams.dstRepStride * intriInfo.c0Count;
        src0Offset = intriInfo.repeat * repeatParams.src0RepStride * intriInfo.c0Count;
        src1Offset = intriInfo.repeat * repeatParams.src1RepStride * intriInfo.c0Count;
        func(dst + dstOffset, src0 + src0Offset, src1 + src1Offset, intriInfo.tail, 1, repeatParams);
    }
}
/* **************************************************************************************************
 * Add                                             *
 * ************************************************************************************************* */
__aicore__ inline void AddIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vadd(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void AddIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vadd(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void AddIntrinsicsImpl(
    const __ubuf__ int16_t* dst, const __ubuf__ int16_t* src0, const __ubuf__ int16_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    (void)(dst);
    (void)(src0);
    (void)(src1);
    (void)(repeatTime);
    (void)(repeatParams);
    ASCENDC_REPORT_NOT_SUPPORT(false, "Add with type int16_t");
}

__aicore__ inline void AddIntrinsicsImpl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vadd(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}
// Add::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void AddImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    AddIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AddImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    AddIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Add::Level 2
template <typename T>
__aicore__ inline void AddImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    BinaryCompute(dst, src0, src1, count, AddImpl<T>);
}

/* **************************************************************************************************
 * Sub                                             *
 * ************************************************************************************************* */
__aicore__ inline void SubIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vsub(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void SubIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vsub(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void SubIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vsub(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void SubIntrinsicsImpl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vsub(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}
// Sub::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void SubImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    SubIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void SubImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    SubIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Sub::Level 2
template <typename T>
__aicore__ inline void SubImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    BinaryCompute(dst, src0, src1, count, SubImpl<T>);
}
/* **************************************************************************************************
 * Mul                                             *
 * ************************************************************************************************* */
__aicore__ inline void MulIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmul(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void MulIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmul(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void MulIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmul(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void MulIntrinsicsImpl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmul(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}
// Mul::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MulImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    MulIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MulImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    MulIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Mul::Level 2
template <typename T>
__aicore__ inline void MulImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    BinaryCompute(dst, src0, src1, count, MulImpl<T>);
}
/* **************************************************************************************************
 * Div                                             *
 * ************************************************************************************************* */
__aicore__ inline void DivIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vdiv(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void DivIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vdiv(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// Div::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void DivImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    DivIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void DivImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    DivIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Div::Level 2
template <typename T>
__aicore__ inline void DivImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    BinaryCompute(dst, src0, src1, count, DivImpl<T>);
}

/* **************************************************************************************************
 * Max                                             *
 * ************************************************************************************************* */
__aicore__ inline void MaxIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmax(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void MaxIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmax(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void MaxIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmax(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void MaxIntrinsicsImpl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmax(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}
// Max::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MaxImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    MaxIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MaxImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    MaxIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Max::Level 2
template <typename T>
__aicore__ inline void MaxImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    BinaryCompute(dst, src0, src1, count, MaxImpl<T>);
}

/* **************************************************************************************************
 * Min                                             *
 * ************************************************************************************************* */
__aicore__ inline void MinIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmin(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void MinIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmin(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void MinIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmin(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void MinIntrinsicsImpl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vmin(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}
// Min::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MinImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    MinIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MinImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    MinIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Min::Level 2
template <typename T>
__aicore__ inline void MinImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    BinaryCompute(dst, src0, src1, count, MinImpl<T>);
}

/* **************************************************************************************************
 * And                                             *
 * ************************************************************************************************* */
__aicore__ inline void AndIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vand(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}
__aicore__ inline void AndIntrinsicsImpl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vand(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// And::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void AndImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    AndIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AndImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    AndIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// And::Level 2
template <typename T>
__aicore__ inline void AndImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    BinaryCompute(dst, src0, src1, count, AndImpl<T>);
}

/* **************************************************************************************************
 * Or                                             *
 * ************************************************************************************************* */
__aicore__ inline void OrIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vor(dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

__aicore__ inline void OrIntrinsicsImpl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src0, __ubuf__ uint16_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vor(dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}
// Or::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void OrImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    OrIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void OrImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    OrIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Or::Level 2
template <typename T>
__aicore__ inline void OrImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    BinaryCompute(dst, src0, src1, count, OrImpl<T>);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AddReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "AddRelu");
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AddReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "AddRelu");
}

/* **************************************************************************************************
 * FusedMulAdd                                             *
 * ************************************************************************************************* */
// FusedMulAdd::Level 2
template <typename T>
__aicore__ inline void FusedMulAddImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "FusedMulAdd");
}

// FusedMulAdd::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void FusedMulAddImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "FusedMulAdd");
}

template <typename T, bool isSetMask = true>
__aicore__ inline void FusedMulAddImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "FusedMulAdd");
}

/* **************************************************************************************************
 * FusedMulAddRelu                                             *
 * ************************************************************************************************* */
// FusedMulAddRelu::Level 2
template <typename T>
__aicore__ inline void FusedMulAddReluImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "FusedMulAddRelu");
}

// FusedMulAddRelu::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void FusedMulAddReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "FusedMulAddRelu");
}

template <typename T, bool isSetMask = true>
__aicore__ inline void FusedMulAddReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "FusedMulAddRelu");
}

// MulAddDst::Level 2
template <typename T, typename U>
__aicore__ inline void MulAddDstImpl(__ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const int32_t& count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "MulAddDst");
}

// MulAddDst::Level 0
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void MulAddDstImpl(
    __ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "MulAddDst");
}

template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void MulAddDstImpl(
    __ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "MulAddDst");
}

/* **************************************************************************************************
 * SubRelu                                             *
 * ************************************************************************************************* */
// SubRelu::Level 2
template <typename T>
__aicore__ inline void SubReluImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SubRelu");
}

// SubRelu::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void SubReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SubRelu");
}

template <typename T, bool isSetMask = true>
__aicore__ inline void SubReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SubRelu");
}
/* **************************************************************************************************
 * AddDeqRelu                                             *
 * ************************************************************************************************* */
__aicore__ inline void AddDeqReluImpl(
    __ubuf__ half* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const int32_t& count)
{
    (void)dst;
    (void)src0;
    (void)src1;
    (void)count;
    ASCENDC_REPORT_NOT_SUPPORT(false, "AddDeqRelu");
}

// AddDeqRelu::Level 0
template <bool isSetMask = true>
__aicore__ inline void AddDeqReluImpl(
    __ubuf__ half* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    (void)dst;
    (void)src0;
    (void)src1;
    (void)mask;
    (void)repeatTime;
    (void)repeatParams;
    ASCENDC_REPORT_NOT_SUPPORT(false, "AddDeqRelu");
}

template <bool isSetMask = true>
__aicore__ inline void AddDeqReluImpl(
    __ubuf__ half* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    (void)dst;
    (void)src0;
    (void)src1;
    (void)mask;
    (void)repeatTime;
    (void)repeatParams;
    ASCENDC_REPORT_NOT_SUPPORT(false, "AddDeqRelu");
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_BINARY_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_IMPL_H__
#endif
