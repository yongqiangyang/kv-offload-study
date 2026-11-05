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
 * \file kernel_prof_trace_intf_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_prof_trace_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_prof_trace_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PROF_TRACE_INTF_IMPL_H__
#endif
#ifndef ASCENDC_KERNEL_PROF_TRACE_INTERFACE_IMPL_H
#define ASCENDC_KERNEL_PROF_TRACE_INTERFACE_IMPL_H
#include "kernel_prof_trace.h"

namespace AscendC {
__aicore__ inline void MetricsProfStart() { ProfStartImpl(); }

__aicore__ inline void MetricsProfStop() { ProfStopImpl(); }

#if defined(__NPU_ARCH__) && ((__NPU_ARCH__ == 3510) || (__NPU_ARCH__ == 9201) || (__NPU_ARCH__ == 9202))
template <pipe_t pipe, uint16_t index>
__aicore__ inline void MarkStamp()
{
    MarkStampImpl<pipe, index>();
}

template <pipe_t pipe>
__aicore__ inline void MarkStamp(uint16_t index)
{
    MarkStampImpl<pipe>(index);
}
#endif
} // namespace AscendC
#endif // ASCENDC_KERNEL_PROF_TRACE_INTERFACE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PROF_TRACE_INTF_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_PROF_TRACE_INTF_IMPL_H__
#endif
