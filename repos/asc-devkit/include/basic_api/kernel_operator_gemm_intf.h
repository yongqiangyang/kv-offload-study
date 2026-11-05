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
 * \file kernel_operator_gemm_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_gemm_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GEMM_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_GEMM_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_GEMM_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "kernel_tensor.h"
#include "kernel_struct_conv2d.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
// T should be left matrix dtype
template <typename T>
__aicore__ inline GemmTiling GetGemmTiling(uint32_t m, uint32_t k, uint32_t n);

/*
 * @ingroup Gemm
 * @brief Multiply two matrices
 * @param [out] dst output LocalTensor
 * @param [in] src0 input GlobalTensor
 * @param [in] src1 input GlobalTensor
 * @param [in] m Number of rows of src0
 * @param [in] n Number of rows of src1
 * @param [in] k Number of columns of src1
 * @param [in] tiling.blockSize size of block
 * @param [in] tiling.mNum args of m
 * @param [in] tiling.nNum args of n
 * @param [in] tiling.kNum args of k
 * @param [in] tiling.roundM/N/K Rounding parameter
 * @param [in] tiling.c0Size The byte length of a block
 * @param [in] tiling.dtypeSize Byte length of the incoming data type
 * @param [in] tiling.m/n/kBlockNum Number of blocks of m/n/k axis
 * @param [in] tiling.m/n/kIterNum Number of traversal dimensions
 * @param [in] tiling.m/k/nTileBlock Number of M/N/K axis cutting blocks
 * @param [in] tiling.m/n/kHasTailNumber of tail blocks of M/K/N axis
 * @param [in] tiling.kHasTileEle Judge whether the tail block exists
 * @param [in] tiling.KtailEle K-axis tail block element
 * @param [in] tiling.kThreadNum K-axis passes
 * @param [in] partialsum judge whether the calculation result is moved out
 * @param [in] initValue Initialization parameters
 */
template <typename T, typename U, typename S>
__aicore__ inline __inout_pipe__(V) void Gemm(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<S>& src1, const uint32_t m,
    const uint32_t k, const uint32_t n, GemmTiling tiling, bool partialsum = true, int32_t initValue = 0);
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_gemm_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_GEMM_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GEMM_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GEMM_INTF_H__
#endif
