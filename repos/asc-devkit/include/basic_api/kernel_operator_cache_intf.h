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
 * \file kernel_operator_cache_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_cache_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CACHE_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_CACHE_INTF_H
#define ASCENDC_MODULE_OPERATOR_CACHE_INTF_H

#include "../../impl/basic_api/kernel_macros.h"
#include "../../impl/basic_api/kernel_reg.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
template <typename T>
class GlobalTensor;

template <typename T>
class LocalTensor;

template <typename T>
__aicore__ inline void DataCachePreload(const GlobalTensor<uint64_t>& src, const T cacheOffset);

template <typename T>
__aicore__ inline void MultiDataCachelinePreload(const GlobalTensor<T>& src, const uint32_t cachelineNum);

template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst);

template <typename T, CacheLine entireType, DcciDst dcciDst>
__aicore__ inline void DataCacheCleanAndInvalid(const LocalTensor<T>& dst);

template <typename T, CacheLine entireType>
__aicore__ inline void DataCacheCleanAndInvalid(const GlobalTensor<T>& dst);

enum class UnitSize {
    UNIT_2048_BYTES,
#if (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202)
    UNIT_512_BYTES
#endif
};

template <UnitSize size = UnitSize::UNIT_2048_BYTES>
__aicore__ inline void ICachePreLoad(const int64_t preFetchLen);

__aicore__ inline int64_t GetICachePreloadStatus();

__aicore__ inline void PrefetchStop();

enum class PrefetchLayout : uint8_t {
    ROW_MAJOR,   // ND
    COLUMN_MAJOR // DN
};

struct PrefetchParams {
    __aicore__ PrefetchParams() {}

    __aicore__ PrefetchParams(const uint32_t burstNumIn, const uint32_t burstLenIn, const int64_t srcStrideIn)
        : burstNum(burstNumIn), burstLen(burstLenIn), srcStride(srcStrideIn)
    {}

    uint32_t burstNum = 0;
    uint32_t burstLen = 0;
    int64_t srcStride = 0;
};

template <PrefetchLayout layout>
struct PrefetchToNZParams {
    __aicore__ PrefetchToNZParams() {}

    __aicore__ PrefetchToNZParams(
        const uint16_t matrixNumIn, const uint16_t nValueIn, const uint32_t dValueIn, const uint64_t srcMatrixStrideIn,
        const uint64_t srcDValueIn)
        : matrixNum(matrixNumIn),
          nValue(nValueIn),
          dValue(dValueIn),
          srcMatrixStride(srcMatrixStrideIn),
          srcDValue(srcDValueIn)
    {}

    uint16_t matrixNum = 0;
    uint16_t nValue = 0;
    uint32_t dValue = 0;
    uint64_t srcMatrixStride = 0;
    uint64_t srcDValue = 0;
};

template <typename T>
__aicore__ inline void Prefetch(const GlobalTensor<T>& src, const PrefetchParams& prefetchParams);

template <typename T, PrefetchLayout layout>
__aicore__ inline void Prefetch(const GlobalTensor<T>& src, const PrefetchToNZParams<layout>& prefetchToNZParams);

} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_cache_intf_impl.h"
#endif
#endif // KERNEL_CACHE_INTF_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CACHE_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CACHE_INTF_H__
#endif
