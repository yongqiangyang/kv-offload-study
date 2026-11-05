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
    "impl/basic_api/dav_c100/kernel_operator_vec_gather_mask_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_MASK_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_REDUCEV2_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_REDUCEV2_IMPL_H
#include "../../../include/basic_api/kernel_struct_gather.h"

namespace AscendC {
template <typename T, typename U, GatherMaskMode mode = defaultGatherMaskMode>
__aicore__ inline void GatherMaskCal(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ U* src1, const bool reduceMode, const uint32_t mask,
    const GatherMaskParams& gatherMaskParams, uint64_t& rsvdCnt)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "GatherMask");
}

template <typename T, GatherMaskMode mode = defaultGatherMaskMode>
__aicore__ inline void GatherMaskCal(
    __ubuf__ T* dst, __ubuf__ T* src0, const uint8_t src1Pattern, const bool reduceMode, const uint32_t mask,
    const GatherMaskParams& gatherMaskParams, uint64_t& rsvdCnt)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "GatherMask");
}

__aicore__ inline int64_t GetGatherMaskRemainCountImpl()
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "GetGatherMaskRemainCount");
    return 0;
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_REDUCEV2_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_MASK_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_GATHER_MASK_IMPL_H__
#endif
