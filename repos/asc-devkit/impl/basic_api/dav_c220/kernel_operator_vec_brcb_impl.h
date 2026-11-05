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
 * \file kernel_operator_vec_brcb_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_c220/kernel_operator_vec_brcb_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BRCB_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_BRCB_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_BRCB_IMPL_H
#include "../../../include/basic_api/kernel_struct_brcb.h"

namespace AscendC {
/* **************************************************************************************************
 * Brcb                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void BrcbImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, const uint8_t repeatTime, const BrcbRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        ASCENDC_DEBUG_ASSERT(
            (SupportType<T, int16_t, uint16_t, int32_t, uint32_t, half, bfloat16_t, float>()),
            KERNEL_LOG_INTERNAL(
                KERNEL_ERROR,
                "Failed to check dtype in Brcb, current api support dtype combination is "
                "src and dst both: int16_t / uint16_t / int32_t / uint32_t / half / bfloat16_t / float.\n"));
        ResetMask();
        if constexpr (sizeof(T) == B16_BYTE_SIZE) {
            vbrcb(
                (__ubuf__ uint16_t*)dst, (__ubuf__ uint16_t*)src0, repeatParams.dstBlkStride, repeatParams.dstRepStride,
                repeatTime);
        } else if constexpr (sizeof(T) == B32_BYTE_SIZE) {
            vbrcb(
                (__ubuf__ uint32_t*)dst, (__ubuf__ uint32_t*)src0, repeatParams.dstBlkStride, repeatParams.dstRepStride,
                repeatTime);
        }
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_BRCB_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BRCB_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BRCB_IMPL_H__
#endif
