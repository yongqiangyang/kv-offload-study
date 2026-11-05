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
 * \file kernel_operator_vec_gather_mask_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_c220/kernel_operator_vec_gather_mask_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_MASK_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_REDUCEV2_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_REDUCEV2_IMPL_H
#include "../../../include/basic_api/kernel_struct_gather.h"
#include "../kernel_npu_debug.h"

namespace AscendC {

template <typename T>
__aicore__ inline void GatherMaskImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint8_t patternMode, const bool reduceMode,
    const uint32_t mask, const GatherMaskParams& gatherMaskParams, uint64_t& rsvdCnt)
{
    if ASCEND_IS_AIV {
        if (reduceMode) {
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
            CheckValueRange<uint32_t>(mask, 1, UINT32_MAX, "mask", "GatherMask when reduceMode = true.");
#endif
            SetMaskCount();
        } else {
            SetMaskNorm();
        }
#if ASCENDC_CPU_DEBUG
        if (reduceMode) {
            set_vector_mask(0, mask);
        } else {
            AscendCUtils::SetMask<T>(ONE_REPEAT_BYTE_SIZE / sizeof(T));
        }
#else
        set_vector_mask(0, mask);
#endif
        vreducev2(
            dst, src0, src1, gatherMaskParams.repeatTimes, gatherMaskParams.src0BlockStride, patternMode,
            gatherMaskParams.src0RepeatStride, gatherMaskParams.src1RepeatStride);
        rsvdCnt = AscendCUtils::GetRsvdCnt();
        SetMaskNorm();
    }
}

template <typename T>
__aicore__ inline void GatherMaskImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint8_t patternMode,
    const GatherMaskParams& gatherMaskParams)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, half, bfloat16_t, uint16_t, int16_t, float, uint32_t, int32_t>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR, "Failed to check dtype in GatherMask, current api support dtype "
                              "combination is src0 and dst both: half / bfloat16_t / uint16_t / int16_t / float / "
                              "uint32_t / int32_t.\n"));
        if (sizeof(T) == sizeof(uint16_t)) {
            vreducev2(
                reinterpret_cast<__ubuf__ uint16_t*>(dst), reinterpret_cast<__ubuf__ uint16_t*>(src0),
                reinterpret_cast<__ubuf__ uint16_t*>(src1), gatherMaskParams.repeatTimes,
                gatherMaskParams.src0BlockStride, patternMode, gatherMaskParams.src0RepeatStride,
                gatherMaskParams.src1RepeatStride);
        } else {
            vreducev2(
                reinterpret_cast<__ubuf__ uint32_t*>(dst), reinterpret_cast<__ubuf__ uint32_t*>(src0),
                reinterpret_cast<__ubuf__ uint32_t*>(src1), gatherMaskParams.repeatTimes,
                gatherMaskParams.src0BlockStride, patternMode, gatherMaskParams.src0RepeatStride,
                gatherMaskParams.src1RepeatStride);
        }
    }
}

template <typename T>
__aicore__ inline void GatherMaskImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, const uint8_t patternMode, const GatherMaskParams& gatherMaskParams)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckValueRange<uint8_t>(patternMode, 1, 7, "src1Pattern", "GatherMask");
#endif
    __ubuf__ T* nullsrc1 = ONE_REPEAT_BYTE_SIZE * sizeof(T) + src0;
    GatherMaskImpl(dst, src0, nullsrc1, patternMode, gatherMaskParams);
}

template <typename T, typename U, GatherMaskMode mode = defaultGatherMaskMode>
__aicore__ inline void GatherMaskCal(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ U* src1, const bool reduceMode, const uint32_t mask,
    const GatherMaskParams& gatherMaskParams, uint64_t& rsvdCnt)
{
    static_assert(
        SupportType<U, uint16_t, uint32_t>(), "Failed to check src1Pattern dtype in GatherMask, current api "
                                              "support src1Pattern dtype to be uint16_t / uint32_t.");
    if constexpr (SupportType<U, uint16_t>()) {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, half, bfloat16_t, uint16_t, int16_t>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR,
                "Failed to check dtype in GatherMask when src1Pattern is uint16_t tensor, current api support dtype "
                "combination is src0 and dst both: half / bfloat16_t / uint16_t / int16_t.\n"));
    } else {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, float, uint32_t, int32_t>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR,
                "Failed to check dtype in GatherMask when src1Pattern is uint32_t tensor, current api support dtype "
                "combination is src0 and dst both: float / uint32_t / int32_t.\n"));
    }
    GatherMaskImpl(
        reinterpret_cast<__ubuf__ U*>(dst), reinterpret_cast<__ubuf__ U*>(src0), src1, 0, reduceMode, mask,
        gatherMaskParams, rsvdCnt);
}

template <typename T, GatherMaskMode mode = defaultGatherMaskMode>
__aicore__ inline void GatherMaskCal(
    __ubuf__ T* dst, __ubuf__ T* src0, const uint8_t src1Pattern, const bool reduceMode, const uint32_t mask,
    const GatherMaskParams& gatherMaskParams, uint64_t& rsvdCnt)
{
#if defined(ASCENDC_DEBUG) || defined(ASCENDC_CPU_DEBUG)
    CheckValueRange<uint8_t>(src1Pattern, 1, 7, "src1Pattern", "GatherMask");
#endif
    ASCENDC_DEBUG_ASSERT(
        (SupportType<T, half, bfloat16_t, uint16_t, int16_t, float, uint32_t, int32_t>()),
        KERNEL_LOG_INTERNAL(
            KERNEL_ERROR,
            "Failed to check dtype in GatherMask, current api support dtype combination "
            "is src0 and dst both: half / bfloat16_t / uint16_t / int16_t / float / uint32_t / int32_t.\n"));

    __ubuf__ T* nullsrc1 = ONE_REPEAT_BYTE_SIZE * sizeof(T) + src0;
    if (sizeof(T) == sizeof(uint16_t)) {
        GatherMaskImpl(
            reinterpret_cast<__ubuf__ uint16_t*>(dst), reinterpret_cast<__ubuf__ uint16_t*>(src0),
            reinterpret_cast<__ubuf__ uint16_t*>(nullsrc1), src1Pattern, reduceMode, mask, gatherMaskParams, rsvdCnt);
    } else {
        GatherMaskImpl(
            reinterpret_cast<__ubuf__ uint32_t*>(dst), reinterpret_cast<__ubuf__ uint32_t*>(src0),
            reinterpret_cast<__ubuf__ uint32_t*>(nullsrc1), src1Pattern, reduceMode, mask, gatherMaskParams, rsvdCnt);
    }
}

__aicore__ inline int64_t GetGatherMaskRemainCountImpl() { return get_rsvd_cnt(); }
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_REDUCEV2_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_MASK_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_MASK_IMPL_H__
#endif
