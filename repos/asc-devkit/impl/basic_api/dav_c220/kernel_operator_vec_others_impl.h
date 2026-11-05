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
 * \file kernel_operator_vec_others_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_c220/kernel_operator_vec_others_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_OTHERS_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_OTHERS_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_OTHERS_IMPL_H

#ifndef ASCENDC_CPU_DEBUG
namespace AscendC {
template <typename T>
__aicore__ inline void AddRelu(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint64_t config)
{
    vaddrelu(dst, src0, src1, config);
}

template <typename T>
__aicore__ inline void AddRelu(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, uint8_t dstBlockStride,
    uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, uint8_t src0RepeatStride,
    uint8_t src1RepeatStride)
{
    vaddrelu(
        dst, src0, src1, repeatTime, dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride,
        src0RepeatStride, src1RepeatStride);
}

template <typename T>
__aicore__ inline void CmpvsEq(__ubuf__ uint8_t* dst, __ubuf__ T* src0, T src1, uint64_t config)
{
    vcmpvs_eq(dst, src0, src1, config);
}

template <typename T>
__aicore__ inline void CmpvsEq(
    __ubuf__ uint8_t* dst, __ubuf__ T* src0, T src1, uint8_t repeatTime, uint16_t dstBlockStride,
    uint16_t srcBlockStride, uint16_t dstRepeatStride, uint16_t srcRepeatStride)
{
    vcmpvs_eq(dst, src0, src1, repeatTime, dstBlockStride, srcBlockStride, dstRepeatStride, srcRepeatStride);
}

template <typename T>
__aicore__ inline void Gather(__ubuf__ T* dst, __ubuf__ uint32_t* src, uint64_t config)
{
    vgather(dst, src, config);
}

template <typename T>
__aicore__ inline void Madd(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint64_t config)
{
    vmadd(dst, src0, src1, config);
}

template <typename T>
__aicore__ inline void Madd(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, uint8_t dstBlockStride,
    uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, uint8_t src0RepeatStride,
    uint8_t src1RepeatStride)
{
    vmadd(
        dst, src0, src1, repeatTime, dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride,
        src0RepeatStride, src1RepeatStride);
}

template <typename T>
__aicore__ inline void MaddRelu(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint64_t config)
{
    vmaddrelu(dst, src0, src1, config);
}

template <typename T>
__aicore__ inline void MaddRelu(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, uint8_t dstBlockStride,
    uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, uint8_t src0RepeatStride,
    uint8_t src1RepeatStride)
{
    vmaddrelu(
        dst, src0, src1, repeatTime, dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride,
        src0RepeatStride, src1RepeatStride);
}

template <typename T>
__aicore__ inline void Mla(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint64_t config)
{
    vmla(dst, src0, src1, config);
}

template <typename T>
__aicore__ inline void Mla(__ubuf__ T* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint64_t config)
{
    vmla(dst, src0, src1, config);
}

template <typename T>
__aicore__ inline void Mla(
    __ubuf__ T* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeatTime, uint8_t dstBlockStride,
    uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, uint8_t src0RepeatStride,
    uint8_t src1RepeatStride, bool repeatStrideMode, bool strideSizeMode)
{
    vmla(
        dst, src0, src1, repeatTime, dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride,
        src0RepeatStride, src1RepeatStride, repeatStrideMode, strideSizeMode);
}

template <typename T>
__aicore__ inline void Mla(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, uint8_t dstBlockStride,
    uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, uint8_t src0RepeatStride,
    uint8_t src1RepeatStride, bool repeatStrideMode, bool strideSizeMode)
{
    vmla(
        dst, src0, src1, repeatTime, dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride,
        src0RepeatStride, src1RepeatStride, repeatStrideMode, strideSizeMode);
}

template <typename T>
__aicore__ inline void Mla(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, uint8_t dstBlockStride,
    uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, uint8_t src0RepeatStride,
    uint8_t src1RepeatStride)
{
    vmla(
        dst, src0, src1, repeatTime, dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride,
        src0RepeatStride, src1RepeatStride);
}

template <typename T>
__aicore__ inline void SubRelu(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint64_t config)
{
    vsubrelu(dst, src0, src1, config);
}

template <typename T>
__aicore__ inline void SubRelu(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, uint8_t dstBlockStride,
    uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, uint8_t src0RepeatStride,
    uint8_t src1RepeatStride)
{
    vsubrelu(
        dst, src0, src1, repeatTime, dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride,
        src0RepeatStride, src1RepeatStride);
}

__aicore__ inline void SubReluConvF162s8(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint64_t config, bool h)
{
    vsubreluconv_f162s8(dst, src0, src1, config, h);
}

__aicore__ inline void SubReluConvF162s8(
    __ubuf__ int8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeatTime, uint8_t dstBlockStride,
    uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, uint8_t src0RepeatStride,
    uint8_t src1RepeatStride, bool h)
{
    vsubreluconv_f162s8(
        dst, src0, src1, repeatTime, dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride,
        src0RepeatStride, src1RepeatStride, h);
}

template <typename T>
__aicore__ inline void AddReluConvVdeqs162b8(
    __ubuf__ T* dst, __ubuf__ int16_t* src0, __ubuf__ int16_t* src1, uint8_t repeatTime, uint8_t dstBlockStride,
    uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, uint8_t src0RepeatStride,
    uint8_t src1RepeatStride, bool h)
{
    vaddreluconv_vdeqs162b8(
        dst, src0, src1, repeatTime, dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride,
        src0RepeatStride, src1RepeatStride, h);
}

template <typename T>
__aicore__ inline void MulAndCast(__ubuf__ T* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint64_t config)
{
    vmulconv_f162s8(dst, src0, src1, config);
}

template <typename T>
__aicore__ inline void MulAndCast(
    __ubuf__ T* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeatTime, uint8_t dstBlockStride,
    uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, uint8_t src0RepeatStride,
    uint8_t src1RepeatStride)
{
    vmulconv_f162s8(
        dst, src0, src1, repeatTime, dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride,
        src0RepeatStride, src1RepeatStride);
}

template <typename T>
__aicore__ inline void MulAndCast(
    __ubuf__ T* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeatTime, uint8_t dstBlockStride,
    uint8_t src0BlockStride, uint8_t src1BlockStride, uint8_t dstRepeatStride, uint8_t src0RepeatStride,
    uint8_t src1RepeatStride, bool repeatStrideMode, bool strideSizeMode)
{
    vmulconv_f162s8(
        dst, src0, src1, repeatTime, dstBlockStride, src0BlockStride, src1BlockStride, dstRepeatStride,
        src0RepeatStride, src1RepeatStride, repeatStrideMode, strideSizeMode);
}
} // namespace AscendC
#endif
#endif // ASCENDC_MODULE_OPERATOR_VEC_OTHERS_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_OTHERS_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_OTHERS_IMPL_H__
#endif
