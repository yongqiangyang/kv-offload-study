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
 * \file kernel_struct_vdeq.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_struct_vdeq.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_VDEQ_H__
#endif

#ifndef ASCENDC_MODULE_STRUCT_VDEQ_H
#define ASCENDC_MODULE_STRUCT_VDEQ_H

#include "../../impl/basic_api/kernel_macros.h"
#include "../../impl/basic_api/utils/kernel_utils_constants.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
struct VdeqInfo {
    __aicore__ VdeqInfo() {}

    __aicore__ VdeqInfo(
        const float vdeqScaleIn[VDEQ_TENSOR_SIZE], const int16_t vdeqOffsetIn[VDEQ_TENSOR_SIZE],
        const bool vdeqSignModeIn[VDEQ_TENSOR_SIZE])
    {
        for (int32_t i = 0; i < VDEQ_TENSOR_SIZE; ++i) {
            vdeqScale[i] = vdeqScaleIn[i];
            vdeqOffset[i] = vdeqOffsetIn[i];
            vdeqSignMode[i] = vdeqSignModeIn[i];
        }
    }

    float vdeqScale[VDEQ_TENSOR_SIZE] = {0};
    int16_t vdeqOffset[VDEQ_TENSOR_SIZE] = {0};
    bool vdeqSignMode[VDEQ_TENSOR_SIZE] = {0};
};
} // namespace AscendC
#endif // ASCENDC_MODULE_STRUCT_VDEQ_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_VDEQ_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_STRUCT_VDEQ_H__
#endif
