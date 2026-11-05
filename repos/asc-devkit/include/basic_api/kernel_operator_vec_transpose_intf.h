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
 * \file kernel_operator_vec_transpose_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_vec_transpose_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TRANSPOSE_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_VEC_TRANSPOSE_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_VEC_TRANSPOSE_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "kernel_tensor.h"
#include "kernel_struct_transpose.h"
#include "../../impl/basic_api/utils/kernel_utils_constants.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
#pragma begin_pipe(V)
/* **************************************************************************************************
 * Transpose                                            *
 * ************************************************************************************************* */
/*
 * @ingroup Transpose
 * @brief dst[i][j] = src[j][i]
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 */
template <typename T>
__aicore__ inline void Transpose(const LocalTensor<T>& dst, const LocalTensor<T>& src);

/* **************************************************************************************************
 * TransDataTo5HD                                            *
 * ************************************************************************************************* */
/*
 * @ingroup Nchwconv
 * @brief NCHW to NC1HWC0 format
 * @param [out] dstList output LocalTensor list
 * @param [in] srcList input LocalTensor list
 * @param [in] nchwconvParams.dstHighHalf Specify dst data is stored in the upper half or lower half of the block
 * @param [in] nchwconvParams.srcHighHalf Specify src data is stored in the upper half or lower half of the block
 * @param [in] nchwconvParams.repeatTimes repeat times
 * @param [in] nchwconvParams.dstRepStride dst repeat stride
 * @param [in] nchwconvParams.srcRepStride src repeat stride
 */
template <typename T>
__aicore__ inline __check_sync_alias__ void TransDataTo5HD(
    const LocalTensor<T> (&dstList)[NCHW_CONV_ADDR_LIST_SIZE],
    const LocalTensor<T> (&srcList)[NCHW_CONV_ADDR_LIST_SIZE], const TransDataTo5HDParams& nchwconvParams);

template <typename T>
__aicore__ inline __check_sync_alias__ void TransDataTo5HD(
    uint64_t dstList[NCHW_CONV_ADDR_LIST_SIZE], uint64_t srcList[NCHW_CONV_ADDR_LIST_SIZE],
    const TransDataTo5HDParams& nchwconvParams);

template <typename T>
__aicore__ inline void Transpose(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const TransposeParamsExt& transposeParams);
#pragma end_pipe
template <typename T>
__aicore__ inline __check_sync_alias__ __in_pipe__(S) __out_pipe__(V) void TransDataTo5HD(
    const LocalTensor<uint64_t>& dst, const LocalTensor<uint64_t>& src, const TransDataTo5HDParams& nchwconvParams);
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_vec_transpose_intf_impl.h"
#endif
#endif // ASCENDC_MODULE_OPERATOR_VEC_TRANSPOSE_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TRANSPOSE_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TRANSPOSE_INTF_H__
#endif
