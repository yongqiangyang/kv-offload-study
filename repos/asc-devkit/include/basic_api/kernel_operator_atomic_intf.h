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
 * \file kernel_operator_atomic_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_atomic_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_ATOMIC_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_ATOMIC_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_ATOMIC_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include "stub_def.h"
#endif

namespace AscendC {
template <typename T>
__aicore__ inline T AtomicAdd(__gm__ T* address, T value);

template <typename T>
__aicore__ inline T AtomicMax(__gm__ T* address, T value);

template <typename T>
__aicore__ inline T AtomicMin(__gm__ T* address, T value);

template <typename T>
__aicore__ inline T AtomicCas(__gm__ T* address, T value1, T value2);

template <typename T>
__aicore__ inline T AtomicExch(__gm__ T* address, T value);
} // namespace AscendC
#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_atomic_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_ATOMIC_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_ATOMIC_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_ATOMIC_INTF_H__
#endif
