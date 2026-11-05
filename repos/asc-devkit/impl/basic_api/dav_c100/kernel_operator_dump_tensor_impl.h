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
 * \file kernel_operator_dump_tensor_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_c100/kernel_operator_dump_tensor_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_IMPL_H

#include "../kernel_utils.h"
#include "../../../include/basic_api/kernel_tensor.h"
#include "kernel_operator_common_impl.h"

namespace AscendC {
/* **************************************************************************************************
 * DumpTensorImpl                                             *
 * ************************************************************************************************* */
__aicore__ inline void InitDumpImpl(bool mixFlag, uint32_t gmLen) { ASCENDC_REPORT_NOT_SUPPORT(false, "InitDump"); }

template <typename T>
__aicore__ void DumpTensorLocal2GMImpl(const LocalTensor<T>& src, uint32_t desc, uint32_t size)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Dump tensor");
}

__aicore__ inline void DumpShapeImpl(const ShapeInfo& shapeInfo) { ASCENDC_REPORT_NOT_SUPPORT(false, "Dump shape"); }

template <typename T>
__aicore__ void DumpTensorGM2GMImpl(const GlobalTensor<T>& src, uint32_t desc, uint32_t size)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Dump tensor");
}

template <class... Args>
__aicore__ inline void PrintfImpl(DumpType printType, __gm__ const char* fmt, Args&&... args)
{
#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0)
    ASCENDC_REPORT_NOT_SUPPORT(false, "Dump scalar");
#endif
}

__aicore__ inline void InitDump(bool mixFlag, uint32_t gmLen)
{
    (void)gmLen;
    (void)mixFlag;
    return;
}
__aicore__ inline void InitDump(bool mixFlag, GM_ADDR dumpStartAddr, uint32_t gmLen)
{
    (void)dumpStartAddr;
    (void)gmLen;
    (void)mixFlag;
    return;
}

__aicore__ inline void DumpTimeStampImpl(uint32_t descId) { return; }
__aicore__ inline void AscendCTimeStamp(uint32_t descId, uint64_t pcPtr = 0) { return; }
} // namespace AscendC
#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_DUMP_TENSOR_IMPL_H__
#endif
