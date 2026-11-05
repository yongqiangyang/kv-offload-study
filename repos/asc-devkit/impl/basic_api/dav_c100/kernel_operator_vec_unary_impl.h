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
 * \file kernel_operator_vec_unary_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_c100/kernel_operator_vec_unary_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_UNARY_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_UNARY_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_UNARY_IMPL_H
#include "../../../include/basic_api/kernel_struct_unary.h"

namespace AscendC {
/* **************************************** Relu ****************************************** */
__aicore__ inline void ReluIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vrelu(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void ReluIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vrelu(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void ReluIntrinsicsImpl(
    __ubuf__ int32_t* dst, __ubuf__ int32_t* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vrelu(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Exp ****************************************** */
__aicore__ inline void ExpIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vexp(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void ExpIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vexp(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Ln ****************************************** */
__aicore__ inline void LnIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vln(dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void LnIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vln(dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Abs ****************************************** */
__aicore__ inline void AbsIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vabs(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void AbsIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vabs(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void AbsIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vabs(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint16_t>(repeatParams.dstRepStride),
        static_cast<uint16_t>(repeatParams.srcRepStride));
}

/* **************************************** Reciprocal ****************************************** */
__aicore__ inline void ReciprocalIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vrec(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void ReciprocalIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vrec(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Rsqrt ****************************************** */
__aicore__ inline void RsqrtIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vrsqrt(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void RsqrtIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vrsqrt(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Sqrt ****************************************** */
__aicore__ inline void SqrtIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vsqrt(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void SqrtIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vsqrt(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Not ****************************************** */
__aicore__ inline void NotIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ int16_t* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vnot(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

__aicore__ inline void NotIntrinsicsImpl(
    __ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vnot(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

template <typename T>
__aicore__ inline void VecUnaryCompute(
    __ubuf__ T* dst, __ubuf__ T* src, const int32_t& count,
    void (*func)(__ubuf__ T*, __ubuf__ T*, uint64_t, uint8_t, const UnaryRepeatParams&))
{
    struct UnaryRepeatParams repeatParams;

    IntriInfo intriInfo = AscendCUtils::CalIntriInfo(sizeof(T), count);

    int32_t dstOffset = 0, srcOffset = 0;
    const auto dstOffsetCount = MAX_REPEAT_TIMES * repeatParams.dstRepStride * intriInfo.c0Count;
    const auto srcOffsetCount = MAX_REPEAT_TIMES * repeatParams.srcRepStride * intriInfo.c0Count;
    const int32_t fullMask = intriInfo.c0Count * DEFAULT_BLK_NUM;
    for (int32_t i = 0; i < intriInfo.repeatRounding; i++) {
        func((__ubuf__ T*)(dst + dstOffset), (__ubuf__ T*)(src + srcOffset), fullMask, MAX_REPEAT_TIMES, repeatParams);
        dstOffset += dstOffsetCount;
        srcOffset += srcOffsetCount;
    }

    dstOffset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.dstRepStride * intriInfo.c0Count;
    srcOffset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.srcRepStride * intriInfo.c0Count;

    if (intriInfo.repeatRemaining != 0) {
        func(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ T*)(src + srcOffset), fullMask, intriInfo.repeatRemaining,
            repeatParams);
    }

    if (intriInfo.tail != 0) {
        dstOffset = intriInfo.repeat * repeatParams.dstRepStride * intriInfo.c0Count;
        srcOffset = intriInfo.repeat * repeatParams.srcRepStride * intriInfo.c0Count;
        func((__ubuf__ T*)(dst + dstOffset), (__ubuf__ T*)(src + srcOffset), intriInfo.tail, 1, repeatParams);
    }
}

/* **************************************** Relu ****************************************** */
// Relu::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    ReluIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    ReluIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

// Relu::Level 2
template <typename T>
__aicore__ inline void ReluImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    VecUnaryCompute(dst, src, count, ReluImpl<T>);
}

/* **************************************** Exp ****************************************** */
// Exp::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ExpImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    ExpIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ExpImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    ExpIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

// Exp::Level 2
template <typename T>
__aicore__ inline void ExpImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    VecUnaryCompute(dst, src, count, ExpImpl<T>);
}

/* **************************************** Ln ****************************************** */
// Ln::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void LnImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    LnIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void LnImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    LnIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

// Ln::Level 2
template <typename T>
__aicore__ inline void LnImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    VecUnaryCompute(dst, src, count, LnImpl<T>);
}

/* **************************************** Abs ****************************************** */
// Abs::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void AbsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    AbsIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AbsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    AbsIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

// Ln::Level 2
template <typename T>
__aicore__ inline void AbsImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    VecUnaryCompute(dst, src, count, AbsImpl<T>);
}

/* **************************************** Reciprocal ****************************************** */
// Reciprocal::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ReciprocalImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    ReciprocalIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ReciprocalImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    ReciprocalIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

// Reciprocal::Level 2
template <typename T>
__aicore__ inline void ReciprocalImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    VecUnaryCompute(dst, src, count, ReciprocalImpl<T>);
}

/* **************************************** Rsqrt ****************************************** */
// Rsqrt::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void RsqrtImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    RsqrtIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void RsqrtImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    RsqrtIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

// Rsqrt::Level 2
template <typename T>
__aicore__ inline void RsqrtImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    VecUnaryCompute(dst, src, count, RsqrtImpl<T>);
}

/* **************************************** Sqrt ****************************************** */
// Sqrt::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void SqrtImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    SqrtIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void SqrtImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    SqrtIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

// Rsqrt::Level 2
template <typename T>
__aicore__ inline void SqrtImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    VecUnaryCompute(dst, src, count, SqrtImpl<T>);
}

/* **************************************** Not ****************************************** */
// Not::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void NotImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    NotIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void NotImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    NotIntrinsicsImpl(dst, src, repeatTime, repeatParams);
}

// Not::Level 2
template <typename T>
__aicore__ inline void NotImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    VecUnaryCompute(dst, src, count, NotImpl<T>);
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_UNARY_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_UNARY_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_UNARY_IMPL_H__
#endif
