/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_swap_mem_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

/**
 * \file kernel_operator_swap_mem_intf.h
 * \brief Interface for memory swap and workspace management
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SWAP_MEM_INTF_H__
#endif

#ifndef ASCENDC_MODULE_SWAP_MEM_INTF_H
#define ASCENDC_MODULE_SWAP_MEM_INTF_H

#include "../../impl/basic_api/kernel_macros.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

#ifndef WORKSPACE_PARAM_OFFSET
#define WORKSPACE_PARAM_OFFSET 0xffffffff
#endif

__BLOCK_LOCAL__ __inline__ __gm__ uint8_t* g_sysWorkspaceReserved;

#if defined(ASCENDC_CPU_DEBUG)
__aicore__ __gm__ uint8_t* __gm__ GetSysWorkSpacePtr();
#else
__aicore__ inline __gm__ uint8_t* __gm__ GetSysWorkSpacePtr()
{
    // kernel launch
#if defined(__NPU_DEVICE__) && defined(__NPU_ARCH__)
    if constexpr (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 9201 || __NPU_ARCH__ == 9202) {
        return __get_kfc_workspace_addr();
    } else {
        return g_sysWorkspaceReserved;
    }
#else
    // framework launch
#if (WORKSPACE_PARAM_OFFSET != 0xffffffff)
    return ((GM_ADDR*)get_para_base())[WORKSPACE_PARAM_OFFSET];
#else
    return g_sysWorkspaceReserved;
#endif
#endif
}
#endif

#if defined(ASCENDC_CPU_DEBUG)
__aicore__ void SetSysWorkSpacePtr(__gm__ uint8_t* workspace);
#else
[[deprecated("NOTICE: SetSysWorkSpacePtr has been deprecated and will be removed in the next version.")]]
__aicore__ inline void SetSysWorkSpacePtr(__gm__ uint8_t* workspace)
{
    g_sysWorkspaceReserved = workspace;
}
#endif
#endif // ASCENDC_KERNEL_SWAP_MEM_INTF_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SWAP_MEM_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SWAP_MEM_INTF_H__
#endif
