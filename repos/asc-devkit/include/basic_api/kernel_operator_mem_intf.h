/*
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 *\file kernel_operator_mem_intf.h
 *\brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MEM_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_MEM_MNG_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_MEM_MNG_INTERFACE_H

namespace AscendC {

using MemoryType = Hardware;
template <size_t v>
using Int = Std::integral_constant<size_t, v>;

namespace __LocalMemPoolAux {
template <class T>
struct __AuxStruct {};
template <>
struct __AuxStruct<Int<size_t(-1)>> {
    uint32_t addr; // Starting Address
    uint8_t head;
    uint8_t sliceMax;
};
} // namespace __LocalMemPoolAux

namespace LocalMemPool {
template <MemoryType hardType, int offset_, pipe_t srcPipe_, pipe_t dstPipe_>
struct MemPool {
    constexpr static pipe_t srcPipe = srcPipe_;
    constexpr static pipe_t dstPipe = dstPipe_;
    constexpr static int segmentOffset = offset_;

    __LocalMemPoolAux::__AuxStruct<Int<segmentOffset>> loopQueue;

    __aicore__ inline MemPool(int32_t size);

    template <class T, pipe_t pipe = srcPipe>
    __aicore__ inline LocalTensor<T> Alloc(int32_t bufLen);

    template <class T, pipe_t pipe = srcPipe>
    __aicore__ inline void Alloc(LocalTensor<T>& tensor, int32_t bufLen);

    template <class T, pipe_t pipe = srcPipe>
    __aicore__ inline void AllocBulk(LocalTensor<T>& tensor, int32_t bulkNumber);

    template <pipe_t pipe = dstPipe, class T>
    __aicore__ inline void Free(LocalTensor<T>& tensor);
};

template <MemoryType hardType, class Arg0, class... Args>
__aicore__ inline static auto Init(Arg0 memPool0, Args... memPools);
}; // namespace LocalMemPool

// UB Tail Address Global Management
__BLOCK_LOCAL__ __inline__ int32_t g_ubMaxAddr; // Set the maximum value for each resource record [Compiler opens the
                                                // init function of Block_Local]

} // namespace AscendC

#if ((__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202)) && !defined(ASCENDC_CPU_DEBUG)
#include "../../impl/basic_api/dav_9201/kernel_operator_mem_impl.h"
#endif

#endif // ASCENDC_MODULE_INNER_OPERATOR_ATOMIC_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MEM_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_MEM_INTF_H__
#endif
