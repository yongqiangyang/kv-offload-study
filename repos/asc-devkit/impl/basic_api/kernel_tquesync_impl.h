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
 * \file kernel_tquesync_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_tquesync_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tpipe.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_TQUESYNC_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_TQUESYNC_IMPL_H
#define ASCENDC_MODULE_TQUESYNC_IMPL_H

namespace AscendC {
template <pipe_t src, pipe_t dst>
__aicore__ inline void TQueSync<src, dst>::SetFlag(TEventID id)
{
    static_assert((src != dst), "src/dst pipe cannot be same.");
    static_assert(IsSupportedPipe(src), "src pipe not supported");
    static_assert(IsSupportedPipe(dst), "dst pipe not supported");
    ASCENDC_DEBUG_ASSERT(
        (id < QUE_MAX_EVENT),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "event id input is %d, which should be less than %d", id, QUE_MAX_EVENT));
    set_flag(src, dst, id);
}

template <pipe_t src, pipe_t dst>
__aicore__ inline void TQueSync<src, dst>::WaitFlag(TEventID id)
{
    static_assert((src != dst), "src/dst pipe cannot be same.");
    static_assert(IsSupportedPipe(src), "src pipe not supported");
    static_assert(IsSupportedPipe(dst), "dst pipe not supported");
    ASCENDC_DEBUG_ASSERT(
        (id < QUE_MAX_EVENT),
        KERNEL_LOG_INTERNAL(KERNEL_ERROR, "event id input is %d, which should be less than %d", id, QUE_MAX_EVENT));
    wait_flag(src, dst, id);
}
} // namespace AscendC
#endif // ASCENDC_MODULE_TQUESYNC_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_TQUESYNC_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_TQUESYNC_IMPL_H__
#endif
