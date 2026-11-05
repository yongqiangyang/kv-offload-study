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
 * \file kernel_operator_vec_duplicate_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m200/kernel_operator_vec_duplicate_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_IMPL_H

#include <type_traits>

namespace AscendC {
template <typename T>
constexpr __aicore__ inline void CheckDuplicateSupportedType()
{
    static_assert(
        SupportType<T, half, int16_t, uint16_t, int32_t, uint32_t, float>(),
        "Failed to check dtype in "
        "Duplicate, current api support dtype combination is src and dst both: half / int16_t / uint16_t / int32_t / "
        "uint32_t / float.");
}

template <typename T>
__aicore__ inline void DuplicateIntrinsicsImpl(
    __ubuf__ T* dstLocal, T scalarValue, const uint8_t repeatTime, const uint16_t dstBlockStride,
    const uint8_t dstRepeatStride)
{
    vector_dup(dstLocal, scalarValue, repeatTime, dstBlockStride, 1, dstRepeatStride, 0);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ T* dstLocal, const T& scalarValue, uint64_t mask, const uint8_t repeatTime, const uint16_t dstBlockStride,
    const uint8_t dstRepeatStride)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    DuplicateIntrinsicsImpl(dstLocal, scalarValue, repeatTime, dstBlockStride, dstRepeatStride);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ T* dstLocal, const T& scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    DuplicateIntrinsicsImpl(dstLocal, scalarValue, repeatTime, dstBlockStride, dstRepeatStride);
}

template <typename T>
__aicore__ inline void DuplicateImpl(__ubuf__ T* dstLocal, const T& scalarValue, const int32_t& count)
{
    SetMaskCount();
    AscendCUtils::SetMask<T>(0, count);
    DuplicateIntrinsicsImpl(dstLocal, scalarValue, 1, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE);
    ResetMask();
    SetMaskNorm();
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_IMPL_H__
#endif
