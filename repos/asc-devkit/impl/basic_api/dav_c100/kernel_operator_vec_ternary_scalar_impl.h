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
 * \file kernel_operator_vec_ternary_scalar_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_c100/kernel_operator_vec_ternary_scalar_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H
#include "../../../include/basic_api/kernel_struct_unary.h"

namespace AscendC {
template <typename T, typename U>
__aicore__ inline void AxpyIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ U* src, half scalarValue, const uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    vaxpy(
        dst, src, scalarValue, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride,
        repeatParams.dstRepStride, repeatParams.srcRepStride);
}
template <typename T, typename U>
__aicore__ inline void AxpyIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ U* src, float scalarValue, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    vaxpy(
        dst, src, scalarValue, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride,
        repeatParams.dstRepStride, repeatParams.srcRepStride);
}

// Axpy::Level 0 - mask count mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void AxpyImpl(
    __ubuf__ T* dst, __ubuf__ U* src, const U& scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        if (sizeof(T) > sizeof(U)) {
            AscendCUtils::SetMask<T>(mask);
        } else {
            AscendCUtils::SetMask<U>(mask);
        }
    }
    AxpyIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}

// Axpy::Level 0 - mask bit mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void AxpyImpl(
    __ubuf__ T* dst, __ubuf__ U* src, const U& scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    AxpyIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}

// Axpy::Level 2
template <typename T, typename U>
__aicore__ inline void AxpyImpl(__ubuf__ T* dst, __ubuf__ U* src, const U& scalarValue, const int32_t& count)
{
    const int32_t typeLen = (sizeof(T) > sizeof(U)) ? sizeof(T) : sizeof(U);
    const int32_t oneRepeatNum = DEFAULT_BLOCK_SIZE / typeLen;
    IntriInfo intriInfo = AscendCUtils::CalIntriInfo(typeLen, count);

    struct UnaryRepeatParams repeatParams;
    repeatParams.dstBlkStride = 1;
    repeatParams.srcBlkStride = 1;
    repeatParams.dstRepStride = oneRepeatNum / (ONE_BLK_SIZE / sizeof(T));
    repeatParams.srcRepStride = oneRepeatNum / (ONE_BLK_SIZE / sizeof(U));
    int32_t dstOffset = 0, srcOffset = 0;
    const int32_t dstOffsetCount = MAX_REPEAT_TIMES * oneRepeatNum;
    const int32_t srcOffsetCount = MAX_REPEAT_TIMES * oneRepeatNum;
    for (int32_t i = 0; i < intriInfo.repeatRounding; i++) {
        AxpyImpl(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ U*)(src + srcOffset), scalarValue, oneRepeatNum, MAX_REPEAT_TIMES,
            repeatParams);
        dstOffset += i * dstOffsetCount;
        srcOffset += i * srcOffsetCount;
    }

    dstOffset = intriInfo.repeatRounding * MAX_REPEAT_TIMES * oneRepeatNum;
    srcOffset = intriInfo.repeatRounding * MAX_REPEAT_TIMES * oneRepeatNum;

    if (intriInfo.repeatRemaining != 0) {
        AxpyImpl(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ U*)(src + srcOffset), scalarValue, oneRepeatNum,
            intriInfo.repeatRemaining, repeatParams);
    }

    if (intriInfo.tail != 0) {
        dstOffset += intriInfo.repeatRemaining * oneRepeatNum;
        srcOffset += intriInfo.repeatRemaining * oneRepeatNum;
        AxpyImpl(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ U*)(src + srcOffset), scalarValue, intriInfo.tail, 1,
            repeatParams);
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H__
#endif
