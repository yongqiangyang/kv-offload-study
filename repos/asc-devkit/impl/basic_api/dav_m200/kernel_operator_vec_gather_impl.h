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
 * \file kernel_operator_vec_gather_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m200/kernel_operator_vec_gather_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_GATHER_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_GATHER_IMPL_H
#include "../../../include/basic_api/kernel_struct_gather.h"

namespace AscendC {
/* **************************************************************************************************
 * Gather                                             *
 * ************************************************************************************************* */
template <typename T>
[[deprecated("NOTICE: Gatherb is not deprecated. Currently, Gatherb is an unsupported API on current device. "
             "Please check your code!")]] __aicore__ inline void
GatherbImpl(
    __ubuf__ T* dst, __ubuf__ T* src, __ubuf__ uint32_t* offset, const uint32_t srcLength, const uint8_t repeatTime,
    const GatherRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Gatherb");
}

template <typename T>
__aicore__ inline void GatherImpl(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* srcOffsetLocal, const uint32_t srcLength,
    const uint32_t srcBaseOffset, const uint64_t mask, const uint8_t repeatTime, const uint16_t dstRepStride)
{
    ASCENDC_ASSERT((SupportType<T, half, uint16_t, int16_t, float, uint32_t, int32_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR, "Failed to check dtype in Gather, current api support dtype combination is src and "
                          "dst both: half / uint16_t / int16_t / float / uint32_t / int32_t");
    });
    uint32_t offsetAddr = (uint64_t)srcLocal + srcBaseOffset;
#if ASCENDC_CPU_DEBUG
    uint64_t cpuAddr = (uint64_t)srcLocal + srcBaseOffset;
    SetModelGatherbSrc0Tensor(cpuAddr, srcLength);
#endif
    uint64_t config = 0;
    config |= (uint64_t(offsetAddr & 0xFFFFFFFF));
    config |= (uint64_t(repeatTime & 0xFF) << 56);
    config |= (uint64_t(dstRepStride & 0xFF) << 32);

    AscendCUtils::SetMask<T>(mask);
    if constexpr (sizeof(T) == sizeof(uint16_t)) {
        vgather((__ubuf__ uint16_t*)dstLocal, srcOffsetLocal, config);
    } else {
        vgather((__ubuf__ uint32_t*)dstLocal, srcOffsetLocal, config);
    }
}

template <typename T>
__aicore__ inline void GatherImpl(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* srcOffsetLocal, const uint32_t srcLength,
    const uint32_t srcBaseOffset, const uint64_t mask[], const uint8_t repeatTime, const uint16_t dstRepStride)
{
    ASCENDC_ASSERT((SupportType<T, half, uint16_t, int16_t, float, uint32_t, int32_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR, "Failed to check dtype in Gather, current api support dtype combination is src and "
                          "dst both: half / uint16_t / int16_t / float / uint32_t / int32_t");
    });
    uint32_t offsetAddr = (uint64_t)srcLocal + srcBaseOffset;
#if ASCENDC_CPU_DEBUG
    uint64_t cpuAddr = (uint64_t)srcLocal + srcBaseOffset;
    SetModelGatherbSrc0Tensor(cpuAddr, srcLength);
#endif
    uint64_t config = 0;
    config |= (uint64_t(offsetAddr & 0xFFFFFFFF));
    config |= (uint64_t(repeatTime & 0xFF) << 56);
    config |= (uint64_t(dstRepStride & 0xFF) << 32);

    AscendCUtils::SetMask<T>(mask[1], mask[0]);
    if constexpr (sizeof(T) == sizeof(uint16_t)) {
        vgather((__ubuf__ uint16_t*)dstLocal, srcOffsetLocal, config);
    } else {
        vgather((__ubuf__ uint32_t*)dstLocal, srcOffsetLocal, config);
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_GATHER_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_IMPL_H__
#endif
