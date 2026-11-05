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
    "impl/basic_api/dav_c220/kernel_operator_vec_binary_scalar_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#include "../../../include/basic_api/kernel_struct_unary.h"
#include "../kernel_npu_debug.h"

namespace AscendC {
/* **************************************************************************************************
 * Adds                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void AddsIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float, int16_t, int32_t>(),
        "Failed to check dtype in Adds, current api support "
        "dtype combination is src and dst both: half / float / int16_t / int32_t.");
    vadds(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint16_t>(repeatParams.dstRepStride),
        static_cast<uint16_t>(repeatParams.srcRepStride));
}

// Adds::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void AddsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        AddsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AddsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        AddsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}

// Adds::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void AddsImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    if ASCEND_IS_AIV {
        if constexpr (!isSetMask) {
            AddsIntrinsicsImpl(
                dst, src, scalarValue, 1,
                {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
            return;
        }
        SetMaskCount();
        AscendCUtils::SetMask<T>(0, count);
        AddsIntrinsicsImpl(
            dst, src, scalarValue, 1,
            {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
        ResetMask();
        SetMaskNorm();
    }
}

/* **************************************************************************************************
 * Muls                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void MulsIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float, int16_t, int32_t>(),
        "Failed to check dtype in Muls, current api support "
        "dtype combination is src and dst both: half / float / int16_t / int32_t.");
    vmuls(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint16_t>(repeatParams.dstRepStride),
        static_cast<uint16_t>(repeatParams.srcRepStride));
}

// Muls::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MulsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        MulsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MulsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        MulsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}

// Muls::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MulsImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    if ASCEND_IS_AIV {
        if constexpr (!isSetMask) {
            MulsIntrinsicsImpl(
                dst, src, scalarValue, 1,
                {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
            return;
        }
        SetMaskCount();
        AscendCUtils::SetMask<T>(0, count);
        MulsIntrinsicsImpl(
            dst, src, scalarValue, 1,
            {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
        ResetMask();
        SetMaskNorm();
    }
}

/* **************************************************************************************************
 * Maxs                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void MaxsIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float, int16_t, int32_t>(),
        "Failed to check dtype in Maxs, current api support "
        "dtype combination is src and dst both: half / float / int16_t / int32_t.");
    vmaxs(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride), false, false);
}

// Maxs::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MaxsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        MaxsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MaxsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        MaxsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}
// Maxs::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MaxsImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    if ASCEND_IS_AIV {
        if constexpr (!isSetMask) {
            MaxsIntrinsicsImpl(
                dst, src, scalarValue, 1,
                {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
            return;
        }
        SetMaskCount();
        AscendCUtils::SetMask<T>(0, count);
        MaxsIntrinsicsImpl(
            dst, src, scalarValue, 1,
            {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
        ResetMask();
        SetMaskNorm();
    }
}

/* **************************************************************************************************
 * Mins                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void MinsIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float, int16_t, int32_t>(),
        "Failed to check dtype in Mins, current api support "
        "dtype combination is src and dst both: half / float / int16_t / int32_t.");
    vmins(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride), false, false);
}

// Mins::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        MinsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        MinsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}

// Mins::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    if ASCEND_IS_AIV {
        if constexpr (!isSetMask) {
            MinsIntrinsicsImpl(
                dst, src, scalarValue, 1,
                {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
            return;
        }
        SetMaskCount();
        AscendCUtils::SetMask<T>(0, count);
        MinsIntrinsicsImpl(
            dst, src, scalarValue, 1,
            {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
        ResetMask();
        SetMaskNorm();
    }
}

/* **************************************************************************************************
 * ShiftLeft                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void ShiftLeftIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, uint16_t, uint32_t, int16_t, int32_t>(),
        "Failed to check dtype in ShiftLeft, current "
        "api support dtype combination is src and dst both: uint16_t / uint32_t / int16_t / int32_t.");
    // B16 must be in range [0, 16]. B32 must be in range [0, 32].
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckValueRange<T>(scalarValue, 0, sizeof(T) * 8, "scalarValue", "ShiftLeft");
#endif
    vshl(
        dst, src, static_cast<uint32_t>(scalarValue), repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint16_t>(repeatParams.dstRepStride),
        static_cast<uint16_t>(repeatParams.srcRepStride));
}

// ShiftLeft::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftLeftImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        ShiftLeftIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftLeftImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        ShiftLeftIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}

// ShiftLeft::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftLeftImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    if ASCEND_IS_AIV {
        if constexpr (!isSetMask) {
            ShiftLeftIntrinsicsImpl(
                dst, src, scalarValue, 1,
                {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
            return;
        }
        SetMaskCount();
        AscendCUtils::SetMask<T>(0, count);
        ShiftLeftIntrinsicsImpl(
            dst, src, scalarValue, 1,
            {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
        ResetMask();
        SetMaskNorm();
    }
}

/* **************************************************************************************************
 * ShiftRight                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void ShiftRightIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, uint8_t repeatTime, const UnaryRepeatParams& repeatParams,
    bool roundEn)
{
    ASCENDC_DEBUG_ASSERT(
        (SupportType<T, int16_t, uint16_t, int32_t, uint32_t>()),
        KERNEL_LOG_INTERNAL(
            KERNEL_ERROR,
            "Failed to check dtype in ShiftRight, current api support dtype combination is src and dst both: int16_t / "
            "uint16_t / int32_t / uint32_t.\n"));
    ASCENDC_DEBUG_WARNING(
        (!(SupportType<T, uint16_t, uint32_t>() && roundEn)),
        KERNEL_LOG_INTERNAL(
            KERNEL_WARN, "roundEn does not take effect in ShiftRight when dtype is uint16_t / uint32_t.\n"));
    // B16 must be in range [0, 16]. B32 must be in range [0, 32].
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckValueRange<T>(scalarValue, 0, sizeof(T) * 8, "scalarValue", "ShiftRight");
#endif
    if (roundEn) {
        if constexpr (SupportType<T, int16_t, int32_t>()) {
            vshr(
                dst, src, (int32_t)scalarValue, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride,
                static_cast<uint16_t>(repeatParams.dstRepStride), static_cast<uint16_t>(repeatParams.srcRepStride),
                true);
        } else {
            vshr(
                dst, src, static_cast<uint32_t>(scalarValue), repeatTime, repeatParams.dstBlkStride,
                repeatParams.srcBlkStride, static_cast<uint16_t>(repeatParams.dstRepStride),
                static_cast<uint16_t>(repeatParams.srcRepStride), true);
        }
    } else {
        if constexpr (SupportType<T, int16_t, int32_t>()) {
            vshr(
                dst, src, (int32_t)scalarValue, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride,
                static_cast<uint16_t>(repeatParams.dstRepStride), static_cast<uint16_t>(repeatParams.srcRepStride),
                false);
        } else {
            vshr(
                dst, src, static_cast<uint32_t>(scalarValue), repeatTime, repeatParams.dstBlkStride,
                repeatParams.srcBlkStride, static_cast<uint16_t>(repeatParams.dstRepStride),
                static_cast<uint16_t>(repeatParams.srcRepStride), false);
        }
    }
}

// ShiftRight::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftRightImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn = false)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        ShiftRightIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams, roundEn);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftRightImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams, bool roundEn = false)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        ShiftRightIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams, roundEn);
    }
}

// ShiftRight::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void ShiftRightImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    if ASCEND_IS_AIV {
        if constexpr (!isSetMask) {
            ShiftRightIntrinsicsImpl(
                dst, src, scalarValue, 1,
                {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE}, false);
            return;
        }
        SetMaskCount();
        AscendCUtils::SetMask<T>(0, count);
        ShiftRightIntrinsicsImpl(
            dst, src, scalarValue, 1,
            {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE}, false);
        ResetMask();
        SetMaskNorm();
    }
}

/* **************************************************************************************************
 * LeakyRelu                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void LeakyReluIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float>(), "Failed to check dtype in LeakyRelu, current api support dtype "
                                       "combination is src and dst both: half / float.");
    vlrelu(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint16_t>(repeatParams.dstRepStride),
        static_cast<uint16_t>(repeatParams.srcRepStride));
}

// LeakyRelu::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
        LeakyReluIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<T, isSetMask>(mask);
        LeakyReluIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
    }
}

// LeakyRelu::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
    if ASCEND_IS_AIV {
        if constexpr (!isSetMask) {
            LeakyReluIntrinsicsImpl(
                dst, src, scalarValue, 1,
                {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
            return;
        }
        SetMaskCount();
        AscendCUtils::SetMask<T>(0, count);
        LeakyReluIntrinsicsImpl(
            dst, src, scalarValue, 1,
            {DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
        ResetMask();
        SetMaskNorm();
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__
#endif
