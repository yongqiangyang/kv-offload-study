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
 * \file inner_kernel_operator_hscb_intf.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/inner_kernel_operator_hscb_intf.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/inner_kernel_operator_hscb_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_INNER_KERNEL_OPERATOR_HSCB_H__
#endif

#ifndef ASCENDC_MODULE_INNER_OPERATOR_HSCB_INTERFACE_H
#define ASCENDC_MODULE_INNER_OPERATOR_HSCB_INTERFACE_H

#if !defined(ASCENDC_CPU_DEBUG)
#if (__NPU_ARCH__ == 9201)
#include "dav_9201/kernel_operator_hscb_impl.h"
#elif (__NPU_ARCH__ == 9202)
#include "dav_9202/kernel_operator_hscb_impl.h"
#endif

namespace AscendC {

namespace Hscb {

enum class AtomicOp { Add, Max, Min, Exch, Cas };

template <typename T, bool async = true>
__aicore__ inline void Store(uint64_t phyAddr, T value)
{
    StoreImpl<T, async>(phyAddr, value);
}

template <typename T>
__aicore__ inline T Load(uint64_t phyAddr)
{
    return LoadImpl<T>(phyAddr);
}

template <pipe_t pipe, typename T, bool async = true>
__aicore__ inline void Sync(uint64_t phyAddr, T value)
{
    SyncImpl<pipe, T, async>(phyAddr, value);
}

template <pipe_t pipe>
__aicore__ inline void WaitSpr(uint8_t sprID, uint16_t expectedValue)
{
    return WaitSprImpl<pipe>(sprID, expectedValue);
};

template <class T, AtomicOp op>
__aicore__ inline T Atomic(uint64_t phyAddr, T value, T compare = T())
{
    if constexpr (op == AtomicOp::Add) {
        return AtomicAddImpl<T>(phyAddr, value);
    } else if constexpr (op == AtomicOp::Max) {
        return AtomicMaxImpl<T>(phyAddr, value);
    } else if constexpr (op == AtomicOp::Min) {
        return AtomicMinImpl<T>(phyAddr, value);
    } else if constexpr (op == AtomicOp::Exch) {
        return AtomicExchImpl<T>(phyAddr, value);
    } else if constexpr (op == AtomicOp::Cas) {
        return AtomicCasImpl<T>(phyAddr, compare, value);
    }
}

template <class T, AtomicOp op>
__aicore__ inline void RedAtomic(uint64_t phyAddr, T value)
{
    static_assert(
        (op == AtomicOp::Add || op == AtomicOp::Min || op == AtomicOp::Max),
        "inlpace atomic support add, max, min only");
    if constexpr (op == AtomicOp::Add) {
        AtomicAddImpl(phyAddr, value);
    } else if constexpr (op == AtomicOp::Max) {
        AtomicMaxImpl(phyAddr, value);
    } else if constexpr (op == AtomicOp::Min) {
        AtomicMinImpl(phyAddr, value);
    }
}
} // namespace Hscb
} // namespace AscendC

#endif // ASCENDC_CPU_DEBUG

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_INNER_KERNEL_OPERATOR_HSCB_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_INNER_KERNEL_OPERATOR_HSCB_H__
#endif
#endif // ASCENDC_MODULE_INNER_OPERATOR_HSCB_INTERFACE_H
