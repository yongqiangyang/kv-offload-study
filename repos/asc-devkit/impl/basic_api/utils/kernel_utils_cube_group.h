/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_check_conv2d.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_CONV2D_H__
#endif

#ifndef ASCENDC_MODULE_UTILS_CUBE_GROUP_H
#define ASCENDC_MODULE_UTILS_CUBE_GROUP_H

namespace AscendC {
// method to update arrive / wait counter
enum class PipeMode : uint8_t { SCALAR_MODE = 0, MTE3_MODE = 1, MAX };

enum class CubeMsgState : uint8_t {
    FREE = 0, // current CubeMsg is empty, allow to AllocMessage
    VALID,    // current CubeMsg needs aic to read and execute
    QUIT,     // tell aic that one aiv has ended service
    FAKE      // current CubeMsg is fake, need aic to FREE that msg when reading msg with skipCnt!=0
    // ex: aic read aiv0 skipCnt = 4, then aiv1~aiv4 need to be set FAKE first, then aic set to FREE.
};

struct BarrierInfo {
    volatile uint32_t head; // counter value for Arrive / Wait
    uint32_t buffer[15];    // guarantee 64B aligned
};
} // namespace AscendC
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CUBE_GROUP_INFO_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CUBE_GROUP_INFO_H__
#endif
