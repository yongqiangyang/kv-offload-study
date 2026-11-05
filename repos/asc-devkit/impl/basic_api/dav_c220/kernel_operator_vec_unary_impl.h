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
    "impl/basic_api/dav_c220/kernel_operator_vec_unary_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_UNARY_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_UNARY_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_UNARY_IMPL_H
#include "../../../include/basic_api/kernel_struct_unary.h"

namespace AscendC {
/* **************************************** Relu ****************************************** */
template <typename T>
__aicore__ inline void ReluIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float, int32_t>(), "Failed to check dtype in Relu, current api support dtype "
                                                "combination is src and dst both: half / float / int32_t.");
    vrelu(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Exp ****************************************** */
template <typename T>
__aicore__ inline void ExpIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float>(), "Failed to check dtype in Exp, current api support dtype combination "
                                       "is src and dst both: half / float.");
    vexp(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Ln ****************************************** */
template <typename T>
__aicore__ inline void LnIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float>(), "Failed to check dtype in Ln, current api support dtype combination "
                                       "is src and dst both: half / float.");
    vln(dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Abs ****************************************** */
template <typename T>
__aicore__ inline void AbsIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float>(), "Failed to check dtype in Abs, current api support dtype combination "
                                       "is src and dst both: half / float.");
    vabs(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Reciprocal ****************************************** */
template <typename T>
__aicore__ inline void ReciprocalIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float>(), "Failed to check dtype in Reciprocal, current api support dtype "
                                       "combination is src and dst both: half / float.");
    vrec(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Rsqrt ****************************************** */
template <typename T>
__aicore__ inline void RsqrtIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float>(), "Failed to check dtype in Rsqrt, current api support dtype "
                                       "combination is src and dst both: half / float.");
    vrsqrt(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Sqrt ****************************************** */
template <typename T>
__aicore__ inline void SqrtIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float>(), "Failed to check dtype in Sqrt, current api support dtype "
                                       "combination is src and dst both: half / float.");
    vsqrt(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Not ****************************************** */
template <typename T>
__aicore__ inline void NotIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, int16_t, uint16_t>(), "Failed to check dtype in Not, current api support dtype "
                                             "combination is src and dst both: int16_t / uint16_t.");
    vnot(
        dst, src, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride));
}

/* **************************************** Relu ****************************************** */
// Relu::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        ReluIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        ReluIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

// Relu::Level 2
template <typename T>
__aicore__ inline void ReluImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, half, float, int32_t>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR,
                "Failed to check dtype in Relu, current api support dtype combination is src and dst both: half / "
                "float / int32_t.\n"));
        set_mask_count();
        set_vector_mask(0, count);
        vrelu(
            dst, src, 1, static_cast<uint16_t>(DEFAULT_BLK_STRIDE), static_cast<uint16_t>(DEFAULT_BLK_STRIDE),
            static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE), static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE));
        set_mask_norm();
        set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
    }
}

/* **************************************** Exp ****************************************** */
// Exp::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ExpImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        ExpIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ExpImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        ExpIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

// Exp::Level 2
template <typename T>
__aicore__ inline void ExpImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, half, float>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR,
                "Failed to check "
                "dtype in Exp, current api support dtype combination is src and dst both: half / float.\n"));
        set_mask_count();
        set_vector_mask(0, count);
        vexp(
            dst, src, 1, static_cast<uint16_t>(DEFAULT_BLK_STRIDE), static_cast<uint16_t>(DEFAULT_BLK_STRIDE),
            static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE), static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE));
        set_mask_norm();
        set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
    }
}

/* **************************************** Ln ****************************************** */
// Ln::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void LnImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        LnIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void LnImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        LnIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

// Ln::Level 2
template <typename T>
__aicore__ inline void LnImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, half, float>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR,
                "Failed to check "
                "dtype in Ln, current api support dtype combination is src and dst both: half / float.\n"));
        set_mask_count();
        set_vector_mask(0, count);
        vln(dst, src, 1, static_cast<uint16_t>(DEFAULT_BLK_STRIDE), static_cast<uint16_t>(DEFAULT_BLK_STRIDE),
            static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE), static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE));
        set_mask_norm();
        set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
    }
}

/* **************************************** Abs ****************************************** */
// Abs::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void AbsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        AbsIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AbsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        AbsIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

// Abs::Level 2
template <typename T>
__aicore__ inline void AbsImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, half, float>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR,
                "Failed to check "
                "dtype in Abs, current api support dtype combination is src and dst both: half / float.\n"));
        set_mask_count();
        set_vector_mask(0, count);
        vabs(
            dst, src, 1, static_cast<uint16_t>(DEFAULT_BLK_STRIDE), static_cast<uint16_t>(DEFAULT_BLK_STRIDE),
            static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE), static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE));
        set_mask_norm();
        set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
    }
}

/* **************************************** Reciprocal ****************************************** */
// Reciprocal::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ReciprocalImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        ReciprocalIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ReciprocalImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        ReciprocalIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

// Reciprocal::Level 2
template <typename T>
__aicore__ inline void ReciprocalImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, half, float>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR,
                "Failed to check "
                "dtype in Reciprocal, current api support dtype combination is src and dst both: half / float.\n"));
        set_mask_count();
        set_vector_mask(0, count);
        vrec(
            dst, src, 1, static_cast<uint16_t>(DEFAULT_BLK_STRIDE), static_cast<uint16_t>(DEFAULT_BLK_STRIDE),
            static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE), static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE));
        set_mask_norm();
        set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
    }
}

/* **************************************** Rsqrt ****************************************** */
// Rsqrt::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void RsqrtImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        RsqrtIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void RsqrtImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        RsqrtIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

// Rsqrt::Level 2
template <typename T>
__aicore__ inline void RsqrtImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, half, float>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR,
                "Failed to check "
                "dtype in Rsqrt, current api support dtype combination is src and dst both: half / float.\n"));
        set_mask_count();
        set_vector_mask(0, count);
        vrsqrt(
            dst, src, 1, static_cast<uint16_t>(DEFAULT_BLK_STRIDE), static_cast<uint16_t>(DEFAULT_BLK_STRIDE),
            static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE), static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE));
        set_mask_norm();
        set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
    }
}

/* **************************************** Sqrt ****************************************** */
// Sqrt::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void SqrtImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        SqrtIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void SqrtImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        SqrtIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

// Sqrt::Level 2
template <typename T>
__aicore__ inline void SqrtImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, half, float>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR,
                "Failed to check "
                "dtype in Sqrt, current api support dtype combination is src and dst both: half / float.\n"));
        set_mask_count();
        set_vector_mask(0, count);
        vsqrt(
            dst, src, 1, static_cast<uint16_t>(DEFAULT_BLK_STRIDE), static_cast<uint16_t>(DEFAULT_BLK_STRIDE),
            static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE), static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE));
        set_mask_norm();
        set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
    }
}

/* **************************************** Not ****************************************** */
// Not::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void NotImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        NotIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void NotImpl(
    __ubuf__ T* dst, __ubuf__ T* src, uint64_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        NotIntrinsicsImpl(dst, src, repeatTime, repeatParams);
    }
}

// Not::Level 2
template <typename T>
__aicore__ inline void NotImpl(__ubuf__ T* dst, __ubuf__ T* src, const int32_t& count)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, int16_t, uint16_t>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR, "Failed to "
                              "check dtype in Not, current api support dtype combination is src and dst both: int16_t "
                              "/ uint16_t.\n"));
        set_mask_count();
        set_vector_mask(0, count);
        vnot(
            dst, src, 1, static_cast<uint16_t>(DEFAULT_BLK_STRIDE), static_cast<uint16_t>(DEFAULT_BLK_STRIDE),
            static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE), static_cast<uint8_t>(DEFAULT_REPEAT_STRIDE));
        set_mask_norm();
        set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_UNARY_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_UNARY_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_UNARY_IMPL_H__
#endif
