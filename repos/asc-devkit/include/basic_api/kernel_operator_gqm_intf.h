/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_gqm_intf.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GQM_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_GQM_INTF_H
#define ASCENDC_MODULE_OPERATOR_GQM_INTF_H
#include "kernel_tensor.h"

#if ((__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202)) && !defined(ASCENDC_CPU_DEBUG)
namespace AscendC {
class Gqm {
public:
    __aicore__ inline Gqm() = default;
    __aicore__ inline uint64_t Init(uint64_t addr, int msgCount);
    __aicore__ inline uint64_t EnQue(uint64_t data);
    __aicore__ inline uint64_t DeQue(uint64_t& retValue);
    __aicore__ inline uint64_t GetFreeSpace();

private:
    __aicore__ inline Gqm(const Gqm&) = delete;
    __aicore__ inline Gqm& operator=(const Gqm&) = delete;

    uint64_t addr_ = 0; // GQM queue addr
};
} // namespace AscendC
#endif

#include "../../impl/basic_api/kernel_operator_gqm_intf_impl.h"

#endif // ASCENDC_MODULE_OPERATOR_GQM_INTF_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GQM_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GQM_INTF_H__
#endif
