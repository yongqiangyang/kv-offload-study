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
 * \file kernel_operator_group_barrier_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_group_barrier_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GROUP_BARRIER_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_GROUP_BARRIER_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_GROUP_BARRIER_INTERFACE_H
#include "kernel_tensor.h"
#if __NPU_ARCH__ == 2201
#include "../../../../impl/basic_api/dav_c220/core_mng/roc/kernel_operator_cube_group_info.h"
#elif __NPU_ARCH__ == 3510
#include "../../../../impl/basic_api/dav_3510/core_mng/roc/kernel_operator_cube_group_info.h"
#elif __NPU_ARCH__ == 9201
#include "../../../../impl/basic_api/dav_9201/core_mng/roc/kernel_operator_cube_group_info.h"
#elif __NPU_ARCH__ == 9202
#include "../../../../impl/basic_api/dav_9202/core_mng/roc/kernel_operator_cube_group_info.h"
#endif
namespace AscendC {
template <PipeMode pipeMode>
class GroupBarrier {
public:
    __aicore__ inline GroupBarrier(GM_ADDR groupWorkspace, uint32_t arriveSizeIn, uint32_t waitSizeIn);
    __aicore__ inline void Arrive(uint32_t arriveIndex);
    // stuck in while loop until all aiv has arrived, then update wait counter
    __aicore__ inline void Wait(uint32_t waitIndex);
    __aicore__ inline uint64_t GetWorkspaceLen();

private:
    __aicore__ inline void __WriteCurrentValue(__gm__ BarrierInfo* barrierInfoAddr);
    __aicore__ inline GroupBarrier() = delete;
    __gm__ BarrierInfo* barrierInfoArrive; // 64B in GM for storing current arrive counter
    __gm__ BarrierInfo* barrierInfoWait;   // 64B in GM for storing current wait counter
    uint32_t arriveSize;
    uint32_t waitSize;
    uint32_t counter; // in which round
    bool hasArrive;   // whether current aiv has called arrive function in this round
};
} // namespace AscendC

#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GROUP_BARRIER_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GROUP_BARRIER_INTF_H__
#endif
