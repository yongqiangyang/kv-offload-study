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
 * \file kernel_operator_set_atomic_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m200/kernel_operator_set_atomic_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SET_ATOMIC_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_SET_ATOMIC_IMPL_H
#define ASCENDC_MODULE_OPERATOR_SET_ATOMIC_IMPL_H
namespace AscendC {
__aicore__ inline void SetAtomicNoneImpl() { set_atomic_none(); }
template <typename T>
__aicore__ inline void SetAtomicAddImpl()
{}
template <>
__aicore__ inline void SetAtomicAddImpl<float>()
{
    set_atomic_f32();
}
template <>
__aicore__ inline void SetAtomicAddImpl<half>()
{
    set_atomic_f16();
}
template <>
__aicore__ inline void SetAtomicAddImpl<int16_t>()
{
    set_atomic_s16();
}
template <>
__aicore__ inline void SetAtomicAddImpl<int32_t>()
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetAtomicAdd with dtype int32_t");
}
template <>
__aicore__ inline void SetAtomicAddImpl<int8_t>()
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetAtomicAdd with dtype int8_t");
}

template <typename T>
__aicore__ inline void SetAtomicTypeImpl()
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetAtomicType");
}

template <>
__aicore__ inline void SetAtomicTypeImpl<float>()
{
    set_atomic_f32();
}

template <>
__aicore__ inline void SetAtomicTypeImpl<half>()
{
    set_atomic_f16();
}

template <>
__aicore__ inline void SetAtomicTypeImpl<int16_t>()
{
    set_atomic_s16();
}

template <typename T>
__aicore__ inline void SetAtomicMaxImpl()
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetAtomicMax");
}

template <typename T>
__aicore__ inline void SetAtomicMinImpl()
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetAtomicMin");
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_SET_ATOMIC_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SET_ATOMIC_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SET_ATOMIC_IMPL_H__
#endif
