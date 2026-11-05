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
 * \file kernel_operator_proposal_base_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_proposal_base_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_proposal_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_BASE_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_PROPOSAL_BASE_H
#define ASCENDC_MODULE_OPERATOR_PROPOSAL_BASE_H

namespace AscendC {
constexpr uint32_t SORT_LEN = 4;
constexpr uint32_t SORT_NUM_TWO = 2;
constexpr uint32_t SORT_NUM_THREE = 3;

__aicore__ inline void ComSortInnerLoopTail(
    uint32_t& offset0Tail, uint32_t& offset1Tail, uint32_t& offset2Tail, uint32_t& offset3Tail, uint16_t& validBitTail,
    uint16_t (&elementCountListTail)[SORT_LEN], const uint32_t baseOffset, const uint32_t elementCountTail,
    int32_t mergeTmpTailQueNum)
{
    if (mergeTmpTailQueNum == SORT_NUM_TWO) {
        offset1Tail = offset0Tail + baseOffset;
        elementCountListTail[1] = elementCountTail;
        offset2Tail = 0;
        elementCountListTail[2] = 0;
        offset3Tail = 0;
        elementCountListTail[3] = 0;
        validBitTail = 0b0011;
    } else if (mergeTmpTailQueNum == SORT_NUM_THREE) {
        offset1Tail = offset0Tail + baseOffset;
        offset2Tail = offset0Tail + SORT_NUM_TWO * baseOffset;
        elementCountListTail[2] = elementCountTail;
        offset3Tail = 0;
        elementCountListTail[3] = 0;
        validBitTail = 0b0111;
    } else {
        offset1Tail = offset0Tail + baseOffset;
        offset2Tail = offset0Tail + SORT_NUM_TWO * baseOffset;
        offset3Tail = offset0Tail + SORT_NUM_THREE * baseOffset;
        elementCountListTail[3] = elementCountTail;
        validBitTail = 0b1111;
    }
}

} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_PROPOSAL_BASE_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_BASE_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_BASE_IMPL_H__
#endif
