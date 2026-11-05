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
    "impl/basic_api/dav_m300/kernel_operator_vec_duplicate_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_IMPL_H
#include <type_traits>
#include "kernel_operator_common_impl.h"
#include "../../../include/basic_api/kernel_common.h"

namespace AscendC {
template <typename T>
constexpr __aicore__ inline void CheckDuplicateSupportedType()
{
    static_assert(
        std::is_same<T, half>::value || std::is_same<T, int16_t>::value || std::is_same<T, uint16_t>::value ||
            std::is_same<T, int32_t>::value || std::is_same<T, uint32_t>::value || std::is_same<T, float>::value,
        "Duplicate instr only support half/int16_t/uint16_t/int32_t/uint32_t/float type on current device");
}

template <typename T, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ T* dstLocal, const T& scalarValue, uint64_t mask, const uint8_t repeatTime, const uint16_t dstBlockStride,
    const uint8_t dstRepeatStride)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device!"); });
}

template <typename T = half, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ half* dstLocal, const half& scalarValue, uint64_t mask, const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 16, strideConfig, preg);
        }
    }
}

template <typename T = float, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ float* dstLocal, const float& scalarValue, uint64_t mask, const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 8, strideConfig, preg);
        }
    }
}

template <typename T = int16_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ int16_t* dstLocal, const int16_t& scalarValue, uint64_t mask, const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 16, strideConfig, preg);
        }
    }
}

template <typename T = uint16_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ uint16_t* dstLocal, const uint16_t& scalarValue, uint64_t mask, const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    __VEC_SCOPE__
    {
        vector_u16 vreg;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 16, strideConfig, preg);
        }
    }
}

template <typename T = int32_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ int32_t* dstLocal, const int32_t& scalarValue, uint64_t mask, const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 8, strideConfig, preg);
        }
    }
}

template <typename T = uint32_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ uint32_t* dstLocal, const uint32_t& scalarValue, uint64_t mask, const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    __VEC_SCOPE__
    {
        vector_u32 vreg;
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 8, strideConfig, preg);
        }
    }
}

template <typename T, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ T* dstLocal, const T& scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device!"); });
}

template <typename T = half, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ half* dstLocal, const half& scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    if constexpr (isSetMask) {
        SetVectorMask<T>(mask[1], mask[0]);
    }

    __VEC_SCOPE__
    {
        vector_f16 vreg;
        vector_bool preg;
        preg = movp_b16();
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 16, strideConfig, preg);
        }
    }
}

template <typename T = float, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ float* dstLocal, const float& scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    if constexpr (isSetMask) {
        SetVectorMask<T>(mask[1], mask[0]);
    }

    __VEC_SCOPE__
    {
        vector_f32 vreg;
        vector_bool preg;
        preg = movp_b32();
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 8, strideConfig, preg);
        }
    }
}

template <typename T = int16_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ int16_t* dstLocal, const int16_t& scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    if constexpr (isSetMask) {
        SetVectorMask<T>(mask[1], mask[0]);
    }

    __VEC_SCOPE__
    {
        vector_s16 vreg;
        vector_bool preg;
        preg = movp_b16();
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 16, strideConfig, preg);
        }
    }
}

template <typename T = uint16_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ uint16_t* dstLocal, const uint16_t& scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    if constexpr (isSetMask) {
        SetVectorMask<T>(mask[1], mask[0]);
    }

    __VEC_SCOPE__
    {
        vector_u16 vreg;
        vector_bool preg;
        preg = movp_b16();
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 16, strideConfig, preg);
        }
    }
}

template <typename T = int32_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ int32_t* dstLocal, const int32_t& scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    if constexpr (isSetMask) {
        SetVectorMask<T>(mask[1], mask[0]);
    }

    __VEC_SCOPE__
    {
        vector_s32 vreg;
        vector_bool preg;
        preg = movp_b32();
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 8, strideConfig, preg);
        }
    }
}

template <typename T = uint32_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(
    __ubuf__ uint32_t* dstLocal, const uint32_t& scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const uint16_t dstBlockStride, const uint8_t dstRepeatStride)
{
    if constexpr (isSetMask) {
        SetVectorMask<T>(mask[1], mask[0]);
    }

    __VEC_SCOPE__
    {
        vector_u32 vreg;
        vector_bool preg;
        preg = movp_b32();
        uint32_t strideConfig = ((static_cast<uint32_t>(dstBlockStride)) << 16);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsstb(vreg, dstLocal + i * dstRepeatStride * 8, strideConfig, preg);
        }
    }
}

template <typename T>
__aicore__ inline void DuplicateImpl(__ubuf__ T* dstLocal, const T& scalarValue, const int32_t& count)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported on current device!"); });
}

template <typename T = half, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(__ubuf__ half* dstLocal, const half& scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f16 vreg;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsts(vreg, dstLocal, i * 128, NORM_B16, preg);
        }
    }
}

template <typename T = float, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(__ubuf__ float* dstLocal, const float& scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f32 vreg;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsts(vreg, dstLocal, i * 64, NORM_B32, preg);
        }
    }
}

template <typename T = int16_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(__ubuf__ int16_t* dstLocal, const int16_t& scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s16 vreg;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsts(vreg, dstLocal, i * 128, NORM_B16, preg);
        }
    }
}

template <typename T = uint16_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(__ubuf__ uint16_t* dstLocal, const uint16_t& scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_u16 vreg;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint16_t repeatTime = CeilDivision(count, 128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sreg, POST_UPDATE);
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsts(vreg, dstLocal, i * 128, NORM_B16, preg);
        }
    }
}

template <typename T = int32_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(__ubuf__ int32_t* dstLocal, const int32_t& scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_s32 vreg;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsts(vreg, dstLocal, i * 64, NORM_B32, preg);
        }
    }
}

template <typename T = uint32_t, bool isSetMask = true>
__aicore__ inline void DuplicateImpl(__ubuf__ uint32_t* dstLocal, const uint32_t& scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_u32 vreg;
        uint32_t sreg = static_cast<uint32_t>(count);
        vector_bool preg;
        uint16_t repeatTime = CeilDivision(count, 64);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vdup(vreg, scalarValue, preg, MODE_ZEROING);
            vsts(vreg, dstLocal, i * 64, NORM_B32, preg);
        }
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_IMPL_H__
#endif
