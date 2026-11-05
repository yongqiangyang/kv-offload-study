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
 * \file kernel_operator_vec_scatter_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m300/kernel_operator_vec_scatter_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_SCATTER_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_SCATTER_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_SCATTER_IMPL_H

#include "../../../include/basic_api/kernel_common.h"

namespace AscendC {
/* **************************************************************************************************
 * Scatter                                             *
 * ************************************************************************************************* */
constexpr int16_t b32ShiftVal = 2;
constexpr int16_t b16ShiftVal = 1;
constexpr uint32_t b32BlkElems = 8;
constexpr uint32_t b16BlkElems = 16;
constexpr uint32_t b8BlkElems = 32;
constexpr uint32_t indexRepElems = 64;
constexpr uint32_t srcRepElems = 64;
constexpr uint32_t srcRep128 = 128;

template <typename T>
__aicore__ inline void ScatterImplB32(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstBaseOffset,
    const uint32_t count)
{
    __VEC_SCOPE__
    {
        vector_u32 indexReg;
        vector_f32 srcReg;
        uint32_t sregPlt = static_cast<uint32_t>(count);
        vector_bool indexMask = pset_b32(PAT_ALL);
        vector_bool preg;
        uint16_t repeatTime = CeilDivision(count, srcRepElems);

        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sregPlt, POST_UPDATE);
            vlds(indexReg, dstOffsetLocal, i * srcRepElems, NORM);
            vshrs(indexReg, indexReg, b32ShiftVal, indexMask, MODE_ZEROING);
            vlds(srcReg, (__ubuf__ float*)srcLocal, i * srcRepElems, NORM);
            vscatter(srcReg, (__ubuf__ float*)(dstLocal + dstBaseOffset), indexReg, preg);
        }
    }
}

template <typename T>
__aicore__ inline void ScatterImplB32(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseOffset, const uint64_t mask, const uint8_t repeatTime, const uint8_t srcRepStride)
{
    __VEC_SCOPE__
    {
        vector_u32 indexReg;
        vector_f32 srcReg;
        vector_bool indexMask = pset_b32(PAT_ALL);
        uint32_t maskV = static_cast<uint32_t>(mask);
        vector_bool srcMask;
        srcMask = plt_b32(maskV, POST_UPDATE);

        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); i++) {
            vlds(indexReg, dstOffsetLocal, i * srcRepElems, NORM);
            vshrs(indexReg, indexReg, b32ShiftVal, indexMask, MODE_ZEROING);
            vlds(srcReg, (__ubuf__ float*)srcLocal, i * srcRepStride * b32BlkElems, NORM);
            vscatter(srcReg, (__ubuf__ float*)(dstLocal + dstBaseOffset), indexReg, srcMask);
        }
    }
}

template <typename T>
__aicore__ inline void ScatterImplB32(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseOffset, const uint64_t mask[], const uint8_t repeatTime, const uint8_t srcRepStride)
{
    SetVectorMask<T>(mask[1], mask[0]);
    __VEC_SCOPE__
    {
        vector_u32 indexReg;
        vector_f32 srcReg;
        vector_bool indexMask = pset_b32(PAT_ALL);
        vector_bool srcPreg;
        srcPreg = movp_b32();

        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); i++) {
            vlds(indexReg, dstOffsetLocal, i * srcRepElems, NORM);
            vshrs(indexReg, indexReg, b32ShiftVal, indexMask, MODE_ZEROING);
            vlds(srcReg, (__ubuf__ float*)srcLocal, i * srcRepStride * b32BlkElems, NORM);
            vscatter(srcReg, (__ubuf__ float*)(dstLocal + dstBaseOffset), indexReg, srcPreg);
        }
    }
}

template <typename T>
__aicore__ inline void ScatterImplB16(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstBaseOffset,
    const uint32_t count)
{
    __VEC_SCOPE__
    {
        vector_u32 indexReg;
        vector_u32 indexRegSec;
        vector_f16 srcReg;
        uint32_t sregPlt = static_cast<uint32_t>(count);
        vector_bool indexMask = pset_b32(PAT_ALL);
        vector_bool preg;
        uint16_t repeatTime = CeilDivision(count, srcRep128);

        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sregPlt, POST_UPDATE);
            vlds(indexReg, dstOffsetLocal, 2 * i * indexRepElems, NORM);
            vlds(indexRegSec, dstOffsetLocal, (2 * i + 1) * indexRepElems, NORM);
            vshrs(indexReg, indexReg, b16ShiftVal, indexMask, MODE_ZEROING);
            vshrs(indexRegSec, indexRegSec, b16ShiftVal, indexMask, MODE_ZEROING);
            vdintlv((vector_u16&)indexReg, (vector_u16&)indexRegSec, (vector_u16&)indexReg, (vector_u16&)indexRegSec);
            vlds(srcReg, (__ubuf__ half*)srcLocal, i * srcRep128, NORM);
            vscatter(srcReg, (__ubuf__ half*)(dstLocal + dstBaseOffset), (vector_u16&)indexReg, preg);
        }
    }
}

template <typename T>
__aicore__ inline void ScatterImplB16(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseOffset, const uint64_t mask, const uint8_t repeatTime, const uint8_t srcRepStride)
{
    __VEC_SCOPE__
    {
        vector_u32 indexReg;
        vector_u32 indexRegSec;
        vector_f16 srcReg;
        vector_bool indexMask = pset_b32(PAT_ALL);
        uint32_t sreg = static_cast<uint32_t>(mask);
        vector_bool preg = plt_b16(sreg, POST_UPDATE);

        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vlds(indexReg, dstOffsetLocal, 2 * i * indexRepElems, NORM);
            vlds(indexRegSec, dstOffsetLocal, (2 * i + 1) * indexRepElems, NORM);
            vshrs(indexReg, indexReg, b16ShiftVal, indexMask, MODE_ZEROING);
            vshrs(indexRegSec, indexRegSec, b16ShiftVal, indexMask, MODE_ZEROING);
            vdintlv((vector_u16&)indexReg, (vector_u16&)indexRegSec, (vector_u16&)indexReg, (vector_u16&)indexRegSec);
            vlds(srcReg, (__ubuf__ half*)srcLocal, i * srcRepStride * b16BlkElems, NORM);
            vscatter(srcReg, (__ubuf__ half*)(dstLocal + dstBaseOffset), (vector_u16&)indexReg, preg);
        }
    }
}

template <typename T>
__aicore__ inline void ScatterImplB16(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseOffset, const uint64_t mask[], const uint8_t repeatTime, const uint8_t srcRepStride)
{
    SetVectorMask<T>(mask[1], mask[0]);
    __VEC_SCOPE__
    {
        vector_u32 indexReg;
        vector_u32 indexRegSec;
        vector_bool indexMask = pset_b32(PAT_ALL);
        vector_f16 srcReg;
        vector_bool srcPreg;
        srcPreg = movp_b16();

        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); i++) {
            vlds(indexReg, dstOffsetLocal, 2 * i * indexRepElems, NORM);
            vlds(indexRegSec, dstOffsetLocal, (2 * i + 1) * indexRepElems, NORM);
            vshrs(indexReg, indexReg, b16ShiftVal, indexMask, MODE_ZEROING);
            vshrs(indexRegSec, indexRegSec, b16ShiftVal, indexMask, MODE_ZEROING);
            vdintlv((vector_u16&)indexReg, (vector_u16&)indexRegSec, (vector_u16&)indexReg, (vector_u16&)indexRegSec);
            vlds(srcReg, (__ubuf__ half*)srcLocal, i * srcRepStride * b16BlkElems, NORM);
            vscatter(srcReg, (__ubuf__ half*)(dstLocal + dstBaseOffset), (vector_u16&)indexReg, srcPreg);
        }
    }
}

template <typename T>
__aicore__ inline void ScatterImplB8(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstBaseOffset,
    const uint32_t count)
{
    __VEC_SCOPE__
    {
        vector_u32 indexReg;
        vector_u32 indexRegSec;
        vector_s8 srcReg;
        uint32_t sregPlt = static_cast<uint32_t>(count);
        vector_bool indexMask = pset_b32(PAT_ALL);
        vector_bool preg;
        uint16_t repeatTime = CeilDivision(count, srcRep128);

        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b16(sregPlt, POST_UPDATE);
            vlds(indexReg, dstOffsetLocal, 2 * i * indexRepElems, NORM);
            vlds(indexRegSec, dstOffsetLocal, (2 * i + 1) * indexRepElems, NORM);
            vdintlv((vector_u16&)indexReg, (vector_u16&)indexRegSec, (vector_u16&)indexReg, (vector_u16&)indexRegSec);
            vlds(srcReg, (__ubuf__ int8_t*)srcLocal, i * srcRep128, UNPK_B8);
            vscatter(srcReg, (__ubuf__ int8_t*)dstLocal + dstBaseOffset, (vector_u16&)indexReg, preg);
        }
    }
}

template <typename T>
__aicore__ inline void ScatterImplB8(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseOffset, const uint64_t mask, const uint8_t repeatTime, const uint8_t srcRepStride)
{
    __VEC_SCOPE__
    {
        vector_u32 indexReg;
        vector_u32 indexRegSec;
        uint32_t selNum = indexRepElems;
        vector_bool indexMask = pset_b32(PAT_ALL);
        vector_s8 srcReg;
        vector_bool srcPreg;
        uint32_t srcMask = static_cast<uint32_t>(mask);
        if (srcMask > srcRep128) {
            srcMask = srcRep128;
        }
        srcPreg = plt_b16(srcMask, POST_UPDATE);

        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vlds(indexReg, dstOffsetLocal, 2 * i * indexRepElems, NORM);
            vlds(indexRegSec, dstOffsetLocal, (2 * i + 1) * indexRepElems, NORM);
            vdintlv((vector_u16&)indexReg, (vector_u16&)indexRegSec, (vector_u16&)indexReg, (vector_u16&)indexRegSec);
            vlds(srcReg, (__ubuf__ int8_t*)srcLocal, i * srcRepStride * b8BlkElems, UNPK_B8);
            vscatter(srcReg, (__ubuf__ int8_t*)dstLocal + dstBaseOffset, (vector_u16&)indexReg, srcPreg);
        }
    }
}

template <typename T>
__aicore__ inline void ScatterImplB8(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseOffset, const uint64_t mask[], const uint8_t repeatTime, const uint8_t srcRepStride)
{
    SetVectorMask<T>(mask[1], mask[0]);
    __VEC_SCOPE__
    {
        vector_u32 indexReg;
        vector_u32 indexRegSec;
        vector_bool indexMask = pset_b32(PAT_ALL);
        vector_s8 srcReg;
        vector_bool srcPreg;
        srcPreg = movp_b16();
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vlds(indexReg, dstOffsetLocal, 2 * i * indexRepElems, NORM);
            vlds(indexRegSec, dstOffsetLocal, (2 * i + 1) * indexRepElems, NORM);
            vdintlv((vector_u16&)indexReg, (vector_u16&)indexRegSec, (vector_u16&)indexReg, (vector_u16&)indexRegSec);
            vlds(srcReg, (__ubuf__ int8_t*)srcLocal, i * srcRepStride * b8BlkElems, UNPK_B8);
            vscatter(srcReg, (__ubuf__ int8_t*)dstLocal + dstBaseOffset, (vector_u16&)indexReg, srcPreg);
        }
    }
}

// norm mode
template <typename T>
__aicore__ inline void ScatterImpl(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseAddr, const uint64_t mask, const uint8_t repeatTime, const uint8_t srcRepStride)
{
    static_assert(SupportBytes<T, 1, 2, 4>(), "Scatter only support type b8/b16/b32 on current device");
    uint32_t dstBaseOffset = dstBaseAddr / sizeof(T);
    if constexpr (sizeof(T) == 1) {
        ScatterImplB8(dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask, repeatTime, srcRepStride);
    } else if constexpr (sizeof(T) == 2) {
        ScatterImplB16(dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask, repeatTime, srcRepStride);
    } else if constexpr (sizeof(T) == 4) {
        ScatterImplB32(dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask, repeatTime, srcRepStride);
    }
}

// bit mode
template <typename T>
__aicore__ inline void ScatterImpl(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseAddr, const uint64_t mask[], const uint8_t repeatTime, const uint8_t srcRepStride)
{
    static_assert(SupportBytes<T, 1, 2, 4>(), "Scatter only support type b8/b16/b32 on current device");
    uint32_t dstBaseOffset = dstBaseAddr / sizeof(T);
    if constexpr (sizeof(T) == 1) {
        ScatterImplB8(dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask, repeatTime, srcRepStride);
    } else if constexpr (sizeof(T) == 2) {
        ScatterImplB16(dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask, repeatTime, srcRepStride);
    } else if constexpr (sizeof(T) == 4) {
        ScatterImplB32(dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask, repeatTime, srcRepStride);
    }
}

// counter mode
template <typename T>
__aicore__ inline void ScatterImpl(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstBaseAddr,
    const uint32_t count)
{
    static_assert(SupportBytes<T, 1, 2, 4>(), "Scatter only support type b8/b16/b32 on current device");
    uint32_t dstBaseOffset = dstBaseAddr / sizeof(T);
    if constexpr (sizeof(T) == 1) {
        ScatterImplB8(dstLocal, srcLocal, dstOffsetLocal, dstBaseOffset, count);
    } else if constexpr (sizeof(T) == 2) {
        ScatterImplB16(dstLocal, srcLocal, dstOffsetLocal, dstBaseOffset, count);
    } else if constexpr (sizeof(T) == 4) {
        ScatterImplB32(dstLocal, srcLocal, dstOffsetLocal, dstBaseOffset, count);
    }
}

} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_SCATTER_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_SCATTER_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_SCATTER_IMPL_H__
#endif
