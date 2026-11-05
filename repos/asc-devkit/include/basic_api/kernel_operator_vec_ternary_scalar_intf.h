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
 * \file kernel_operator_vec_ternary_scalar_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_vec_ternary_scalar_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TERNARY_SCALAR_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_VEC_TERNARY_SCALAR_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_VEC_TERNARY_SCALAR_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "kernel_tensor.h"
#include "kernel_struct_unary.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

#pragma begin_pipe(V)
namespace AscendC {
/*
 * @ingroup Axpy Level 0
 * @brief dst[i] = src[i]*scalar + dst[i]
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] scalarValue input scalar number
 * @param [in] mask[]/mask mask array/count
 * @param [in] repeatTime repeat times
 * @param [in] repeatParams.dstBlkStride dst block stride
 * @param [in] repeatParams.srcBlkStride src block stride
 * @param [in] repeatParams.dstRepStride dst repeat stride
 * @param [in] repeatParams.src0RepStride src repeat stride
 */
template <typename T, typename U, bool isSetMask = true>
__ASC_USE_RESERVED_UBUF__(3510, "Axpy is forbidden when compile option --cce-disable-asc-reserved-ubuf is enabled")
__aicore__ inline void Axpy(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const U& scalarValue, uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams);

template <typename T, typename U, bool isSetMask = true>
__ASC_USE_RESERVED_UBUF__(3510, "Axpy is forbidden when compile option --cce-disable-asc-reserved-ubuf is enabled")
__aicore__ inline void Axpy(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const U& scalarValue, uint64_t mask[],
    const uint8_t repeatTime, const UnaryRepeatParams& repeatParams);

/*
 * @ingroup Axpy Level 2
 * @brief dst[i] = src[i]*scalar + dst[i]
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] scalarValue input scalar number
 * @param [in] count number Number of data involved in calculation
 */
template <typename T, typename U>
__aicore__ inline void Axpy(
    const LocalTensor<T>& dst, const LocalTensor<U>& src, const U& scalarValue, const int32_t& count);
} // namespace AscendC
#pragma end_pipe

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_vec_ternary_scalar_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_VEC_TERNARY_SCALAR_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TERNARY_SCALAR_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TERNARY_SCALAR_INTF_H__
#endif
