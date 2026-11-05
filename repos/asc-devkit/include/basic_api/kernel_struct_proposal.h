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
 * \file kernel_struct_proposal.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_struct_proposal.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_PROPOSAL_H__
#endif

#ifndef ASCENDC_MODULE_STRUCT_PROPOSAL_H
#define ASCENDC_MODULE_STRUCT_PROPOSAL_H

#include "../../impl/basic_api/utils/kernel_utils_constants.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {

template <typename T>
class LocalTensor;

struct MrgSort4Info {
    __aicore__ MrgSort4Info() {}

    __aicore__ MrgSort4Info(
        const uint16_t elementLengthsIn[MRG_SORT_ELEMENT_LEN], const bool ifExhaustedSuspensionIn,
        const uint16_t validBitIn, const uint16_t repeatTimesIn)
        : ifExhaustedSuspension(ifExhaustedSuspensionIn), validBit(validBitIn), repeatTimes(repeatTimesIn)
    {
        for (int32_t i = 0; i < MRG_SORT_ELEMENT_LEN; ++i) {
            elementLengths[i] = elementLengthsIn[i];
        }
    }

    uint16_t elementLengths[MRG_SORT_ELEMENT_LEN] = {0};
    bool ifExhaustedSuspension = false;
    uint16_t validBit = 0;
    uint8_t repeatTimes = 1;
};

template <typename T>
struct MrgSortSrcList {
    __aicore__ MrgSortSrcList() {}

    __aicore__ MrgSortSrcList(
        const LocalTensor<T>& src1In, const LocalTensor<T>& src2In, const LocalTensor<T>& src3In,
        const LocalTensor<T>& src4In)
    {
        src1 = src1In[0];
        src2 = src2In[0];
        src3 = src3In[0];
        src4 = src4In[0];
    }

    LocalTensor<T> src1;
    LocalTensor<T> src2;
    LocalTensor<T> src3;
    LocalTensor<T> src4;
};
} // namespace AscendC
#endif // ASCENDC_MODULE_STRUCT_PROPOSAL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_PROPOSAL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_PROPOSAL_H__
#endif
