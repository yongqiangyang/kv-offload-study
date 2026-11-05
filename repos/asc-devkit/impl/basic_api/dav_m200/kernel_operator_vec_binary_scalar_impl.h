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
    "impl/basic_api/dav_m200/kernel_operator_vec_binary_scalar_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#include "../../../include/basic_api/kernel_struct_unary.h"

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
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    MaxsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MaxsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    MaxsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}
// Maxs::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MaxsImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
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
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    MinsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    MinsIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}

// Mins::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
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
template <typename T>
__aicore__ inline void LeakyReluIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float>(), "Failed to check dtype in LeakyRelu, current api support dtype "
                                       "combination is src and dst both: half / float.");
    vlrelu(
        dst, src, scalarValue, repeatTime, static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.srcBlkStride), static_cast<uint8_t>(repeatParams.dstRepStride),
        static_cast<uint8_t>(repeatParams.srcRepStride), false, false);
}

// LeakyRelu::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    LeakyReluIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    LeakyReluIntrinsicsImpl(dst, src, scalarValue, repeatTime, repeatParams);
}

// LeakyRelu::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(__ubuf__ T* dst, __ubuf__ T* src, const T& scalarValue, const int32_t& count)
{
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
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__
#endif
