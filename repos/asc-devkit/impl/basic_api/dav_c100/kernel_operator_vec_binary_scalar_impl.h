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
    "impl/basic_api/dav_c100/kernel_operator_vec_binary_scalar_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#include "../../../include/basic_api/kernel_struct_unary.h"

namespace AscendC {
template <typename T>
__aicore__ inline void VecBinaryScalarCompute(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count,
    void (*func)(__ubuf__ T*, __ubuf__ T*, const T&, const uint64_t, const uint8_t, const UnaryRepeatParams&))
{
    struct UnaryRepeatParams repeatParams;
    IntriInfo intriInfo = AscendCUtils::CalIntriInfo(sizeof(T), count);

    uint32_t dstOffset = 0;
    uint32_t srcOffset = 0;
    const auto dstOffsetCount = MAX_REPEAT_TIMES * repeatParams.dstRepStride * intriInfo.c0Count;
    const auto srcOffsetCount = MAX_REPEAT_TIMES * repeatParams.srcRepStride * intriInfo.c0Count;

    const int32_t fullMask = intriInfo.c0Count * DEFAULT_BLK_NUM;
    for (int32_t i = 0; i < intriInfo.repeatRounding; i++) {
        func(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ T*)(src + srcOffset), scalarValue, fullMask, MAX_REPEAT_TIMES,
            repeatParams);
        dstOffset += dstOffsetCount;
        srcOffset += srcOffsetCount;
    }

    dstOffset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.dstRepStride * intriInfo.c0Count;
    srcOffset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.srcRepStride * intriInfo.c0Count;

    if (intriInfo.repeatRemaining != 0) {
        func(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ T*)(src + srcOffset), scalarValue, fullMask,
            intriInfo.repeatRemaining, repeatParams);
    }

    if (intriInfo.tail != 0) {
        dstOffset = intriInfo.repeat * repeatParams.dstRepStride * intriInfo.c0Count;
        srcOffset = intriInfo.repeat * repeatParams.srcRepStride * intriInfo.c0Count;
        func(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ T*)(src + srcOffset), scalarValue, intriInfo.tail, 1,
            repeatParams);
    }
}
template <typename T>
__aicore__ inline void ShiftRightCompute(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count,
    void (*func)(__ubuf__ T*, __ubuf__ T*, const T&, const uint64_t, const uint8_t, const UnaryRepeatParams&, bool),
    bool roundEn)
{
    struct UnaryRepeatParams repeatParams;
    IntriInfo intriInfo = AscendCUtils::CalIntriInfo(sizeof(T), count);

    uint32_t dstOffset = 0;
    uint32_t srcOffset = 0;
    const auto dstOffsetCount = MAX_REPEAT_TIMES * repeatParams.dstRepStride * intriInfo.c0Count;
    const auto srcOffsetCount = MAX_REPEAT_TIMES * repeatParams.srcRepStride * intriInfo.c0Count;

    const int32_t fullMask = intriInfo.c0Count * DEFAULT_BLK_NUM;
    for (int32_t i = 0; i < intriInfo.repeatRounding; i++) {
        func(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ T*)(src + srcOffset), scalarValue, fullMask, MAX_REPEAT_TIMES,
            repeatParams, roundEn);
        dstOffset += dstOffsetCount;
        srcOffset += srcOffsetCount;
    }

    dstOffset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.dstRepStride * intriInfo.c0Count;
    srcOffset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.srcRepStride * intriInfo.c0Count;

    if (intriInfo.repeatRemaining != 0) {
        func(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ T*)(src + srcOffset), scalarValue, fullMask,
            intriInfo.repeatRemaining, repeatParams, roundEn);
    }

    if (intriInfo.tail != 0) {
        dstOffset = intriInfo.repeat * repeatParams.dstRepStride * intriInfo.c0Count;
        srcOffset = intriInfo.repeat * repeatParams.srcRepStride * intriInfo.c0Count;
        func(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ T*)(src + srcOffset), scalarValue, intriInfo.tail, 1,
            repeatParams, roundEn);
    }
}
/* **************************************************************************************************
 * Adds                                             *
 * ************************************************************************************************* */
__aicore__ inline void AddsIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vadds(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void AddsIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    vadds(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void AddsIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    vadds(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void AddsIntrinsicsImpl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    vadds(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}
// Adds::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void AddsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    AddsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AddsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    AddsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}

// Adds::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void AddsImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    VecBinaryScalarCompute(dst, src, scalarValue, count, AddsImpl<T, isSetMask>);
}

/* **************************************************************************************************
 * Muls                                             *
 * ************************************************************************************************* */
__aicore__ inline void MulsIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vmuls(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void MulsIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    vmuls(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void MulsIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, int16_t scalarValue, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    vmuls(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void MulsIntrinsicsImpl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, int32_t scalarValue, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    vmuls(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

// Muls::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MulsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    MulsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MulsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    MulsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}

// Muls::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MulsImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    VecBinaryScalarCompute(dst, src, scalarValue, count, MulsImpl<T>);
}

/* **************************************************************************************************
 * Maxs                                             *
 * ************************************************************************************************* */
// Maxs::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MaxsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Maxs");
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MaxsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Maxs");
}
// Maxs::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MaxsImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Maxs");
}

/* **************************************************************************************************
 * Mins                                             *
 * ************************************************************************************************* */
// Mins::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Mins");
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Mins");
}

// Mins::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Mins");
}

/* **************************************************************************************************
 * ShiftLeft                                             *
 * ************************************************************************************************* */
// ShiftLeft::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftLeftImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "ShiftLeft");
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftLeftImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "ShiftLeft");
}

// ShiftLeft::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftLeftImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "ShiftLeft");
}

/* **************************************************************************************************
 * ShiftRight                                             *
 * ************************************************************************************************* */
// ShiftRight::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftRightImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn = false)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "ShiftRight");
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftRightImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn = false)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "ShiftRight");
}

// ShiftRight::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftRightImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "ShiftRight");
}

/* **************************************************************************************************
 * LeakyRelu                                             *
 * ************************************************************************************************* */
// LeakyRelu::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "LeakyRelu");
}

template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "LeakyRelu");
}

// LeakyRelu::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "LeakyRelu");
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__
#endif
