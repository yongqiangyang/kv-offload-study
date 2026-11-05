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
    "impl/basic_api/dav_l311/kernel_operator_vec_scatter_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_SCATTER_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_SCATTER_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_SCATTER_IMPL_H
#include "kernel_operator_common_impl.h"

namespace AscendC {
/* **************************************************************************************************
 * scatter                                             *
 * ************************************************************************************************* */
constexpr uint32_t mulsScalar = 2;
constexpr uint32_t addsScalar = 1;
constexpr int16_t b32ShiftVal = 2;
constexpr int16_t b64ShiftVal = 3;
constexpr int16_t b16ShiftVal = 1;
constexpr uint32_t b32BlkElems = 8;
constexpr uint32_t b16BlkElems = 16;
constexpr uint32_t b8BlkElems = 32;
constexpr uint32_t indexRepElems = 64;
constexpr uint32_t srcRepElems = 64;
constexpr uint32_t srcRep128 = 128;
constexpr uint32_t b64RepElems = 32;
constexpr Reg::CastTrait castTraitEven = {Reg::RegLayout::ZERO, Reg::SatMode::SAT, Reg::MaskMergeMode::ZEROING};

constexpr Reg::CastTrait castTraitOdd = {Reg::RegLayout::ONE, Reg::SatMode::SAT, Reg::MaskMergeMode::ZEROING};

template <typename T>
__aicore__ inline void ScatterImplB16(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstBaseOffset,
    const uint32_t count)
{
    __VEC_SCOPE__
    {
        Reg::RegTensor<T> srcReg;
        Reg::RegTensor<uint32_t> indexReg;
        Reg::RegTensor<uint16_t> indexU16;
        Reg::RegTensor<uint32_t> indexRegSec;
        Reg::RegTensor<uint16_t> lowerU16Reg;
        Reg::RegTensor<uint16_t> highU16Reg;
        Reg::MaskReg preg;
        uint32_t sregPlt = static_cast<uint32_t>(count);
        Reg::MaskReg indexMask = Reg::CreateMask<uint8_t>();
        Reg::MaskReg selectMask = Reg::CreateMask<uint16_t, Reg::MaskPattern::H>();
        uint16_t repeatTime = CeilDivision(count, srcRep128);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = Reg::UpdateMask<T>(sregPlt);
            Reg::DataCopy<uint32_t>(indexReg, dstOffsetLocal + 2 * i * indexRepElems);
            Reg::DataCopy<uint32_t>(indexRegSec, dstOffsetLocal + (2 * i + 1) * indexRepElems);
            Reg::ShiftRights<uint32_t, int16_t>(indexReg, indexReg, b16ShiftVal, indexMask);
            Reg::ShiftRights<uint32_t, int16_t>(indexRegSec, indexRegSec, b16ShiftVal, indexMask);
            Reg::Cast<uint16_t, uint32_t, castTraitEven>(lowerU16Reg, indexReg, indexMask);
            Reg::Cast<uint16_t, uint32_t, castTraitOdd>(highU16Reg, indexRegSec, indexMask);
            Reg::DeInterleave(lowerU16Reg, highU16Reg, lowerU16Reg, highU16Reg);
            Reg::Select(indexU16, lowerU16Reg, highU16Reg, selectMask);
            Reg::DataCopy<T>(srcReg, srcLocal + i * srcRep128);
            Reg::DataCopyScatter<T, uint16_t>(dstLocal + dstBaseOffset, srcReg, indexU16, preg);
        }
    }
}

template <typename T, bool isNormalMode = true, bool isMaskBitMode = true>
__aicore__ inline void ScatterImplB16(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseOffset, const uint64_t mask, const uint8_t repeatTime, const uint8_t srcRepStride)
{
    __VEC_SCOPE__
    {
        Reg::RegTensor<uint32_t> indexReg;
        Reg::RegTensor<uint32_t> indexRegSec;
        Reg::RegTensor<uint16_t> indexU16;
        Reg::RegTensor<uint16_t> lowerU16;
        Reg::RegTensor<uint16_t> highU16;
        Reg::RegTensor<T> srcReg;
        Reg::MaskReg selectMask = Reg::CreateMask<uint16_t, Reg::MaskPattern::H>();
        Reg::MaskReg indexMask = Reg::CreateMask<uint8_t>();
        Reg::MaskReg b16SrcMask;
        uint32_t maskV = static_cast<uint32_t>(mask);
        if constexpr (isNormalMode) {
            if constexpr (isMaskBitMode) {
                b16SrcMask = Reg::MoveMask<T>();
                Reg::MaskPack(b16SrcMask, b16SrcMask);
                Reg::MaskUnPack(b16SrcMask, b16SrcMask);
            } else {
                b16SrcMask = Reg::UpdateMask<T>(maskV);
            }
        }
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); i++) {
            if constexpr (!isNormalMode) {
                b16SrcMask = Reg::UpdateMask<T>(maskV);
            }
            Reg::DataCopy<uint32_t>(indexReg, dstOffsetLocal + 2 * i * indexRepElems);
            Reg::DataCopy<uint32_t>(indexRegSec, dstOffsetLocal + (2 * i + 1) * indexRepElems);
            Reg::ShiftRights<uint32_t, int16_t>(indexReg, indexReg, b16ShiftVal, indexMask);
            Reg::ShiftRights<uint32_t, int16_t>(indexRegSec, indexRegSec, b16ShiftVal, indexMask);
            Reg::Cast<uint16_t, uint32_t, castTraitEven>(lowerU16, indexReg, indexMask);
            Reg::Cast<uint16_t, uint32_t, castTraitOdd>(highU16, indexRegSec, indexMask);
            Reg::DeInterleave(lowerU16, highU16, lowerU16, highU16);
            Reg::Select(indexU16, lowerU16, highU16, selectMask);
            Reg::DataCopy<T>(srcReg, srcLocal + i * srcRepStride * b16BlkElems);
            Reg::DataCopyScatter<T, uint16_t>(dstLocal + dstBaseOffset, srcReg, indexU16, b16SrcMask);
        }
    }
}

template <typename T>
__aicore__ inline void ScatterImplB32(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstBaseOffset,
    const uint32_t count)
{
    __VEC_SCOPE__
    {
        Reg::RegTensor<uint32_t> indexReg;
        Reg::RegTensor<T> srcReg;
        uint32_t sregPlt = static_cast<uint32_t>(count);
        Reg::MaskReg indexMask = Reg::CreateMask<uint8_t>();
        Reg::MaskReg preg;
        uint16_t repeatTime = CeilDivision(count, srcRepElems);

        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = Reg::UpdateMask<T>(sregPlt);
            Reg::DataCopy<uint32_t>(indexReg, dstOffsetLocal + i * srcRepElems);
            Reg::ShiftRights<uint32_t, int16_t>(indexReg, indexReg, b32ShiftVal, indexMask);
            Reg::DataCopy<T>(srcReg, srcLocal + i * srcRepElems);
            Reg::DataCopyScatter<T, uint32_t>(dstLocal + dstBaseOffset, srcReg, indexReg, preg);
        }
    }
}

template <typename T, bool isNormalMode = true, bool isMaskBitMode = true>
__aicore__ inline void ScatterImplB32(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseOffset, const uint64_t mask, const uint8_t repeatTime, const uint8_t srcRepStride)
{
    __VEC_SCOPE__
    {
        Reg::RegTensor<uint32_t> indexReg;
        uint32_t maskV = static_cast<uint32_t>(mask);
        Reg::RegTensor<T> srcReg;
        Reg::MaskReg indexMask = Reg::CreateMask<uint8_t>();
        Reg::MaskReg b32SrcMask;
        if constexpr (isNormalMode) {
            if constexpr (isMaskBitMode) {
                b32SrcMask = Reg::MoveMask<T>();
                Reg::MaskPack(b32SrcMask, b32SrcMask);
                Reg::MaskUnPack(b32SrcMask, b32SrcMask);
            } else {
                b32SrcMask = Reg::UpdateMask<T>(maskV);
            }
        }
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); i++) {
            if constexpr (!isNormalMode) {
                b32SrcMask = Reg::UpdateMask<T>(maskV);
            }
            Reg::DataCopy<uint32_t>(indexReg, dstOffsetLocal + i * srcRepElems);
            Reg::ShiftRights<uint32_t, int16_t>(indexReg, indexReg, b32ShiftVal, indexMask);
            Reg::DataCopy<T>(srcReg, srcLocal + i * srcRepStride * b32BlkElems);
            Reg::DataCopyScatter<T, uint32_t>(dstLocal + dstBaseOffset, srcReg, indexReg, b32SrcMask);
        }
    }
}
template <typename T>
__aicore__ inline void ScatterImpl(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseAddr, const uint64_t mask, const uint8_t repeatTime, const uint8_t srcRepStride)
{
    static_assert(SupportBytes<T, 2, 4>(), "Scatter only support type b16/b32 on current device");
    bool isNormalMode = !Internal::IsCounterMode();
    uint32_t dstBaseOffset = dstBaseAddr / sizeof(T);
    if (isNormalMode) {
        if constexpr (sizeof(T) == 2) {
            ScatterImplB16<T, true, false>(
                dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask, repeatTime, srcRepStride);
        } else if constexpr (sizeof(T) == 4) {
            ScatterImplB32<T, true, false>(
                dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask, repeatTime, srcRepStride);
        }
    } else {
        uint8_t newRepeatTimes = static_cast<uint8_t>(Internal::VecMicroGetRepeatTimes<T, false>(mask, repeatTime));
        if constexpr (sizeof(T) == 2) {
            ScatterImplB16<T, false, false>(
                dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask, newRepeatTimes, srcRepStride);
        } else if constexpr (sizeof(T) == 4) {
            ScatterImplB32<T, false, false>(
                dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask, newRepeatTimes, srcRepStride);
        }
    }
}

template <typename T>
__aicore__ inline void ScatterImpl(
    __ubuf__ T* dstLocal, __ubuf__ T* srcLocal, __ubuf__ uint32_t* dstOffsetLocal, const uint32_t dstLength,
    const uint32_t dstBaseAddr, const uint64_t mask[], const uint8_t repeatTime, const uint8_t srcRepStride)
{
    static_assert(SupportBytes<T, 2, 4>(), "Scatter only support type b16/b32 on current device");
    bool isNormalMode = !Internal::IsCounterMode();
    uint32_t dstBaseOffset = dstBaseAddr / sizeof(T);
    if (isNormalMode) {
        SetVectorMask<T>(mask[1], mask[0]);

        if constexpr (sizeof(T) == 2) {
            ScatterImplB16<T, true, true>(
                dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask[0], repeatTime, srcRepStride);
        } else if constexpr (sizeof(T) == 4) {
            ScatterImplB32<T, true, true>(
                dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask[0], repeatTime, srcRepStride);
        }
    } else {
        uint8_t newRepeatTimes = static_cast<uint8_t>(Internal::VecMicroGetRepeatTimes<T, false>(mask[0], repeatTime));
        if constexpr (sizeof(T) == 2) {
            ScatterImplB16<T, false, true>(
                dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask[0], newRepeatTimes, srcRepStride);
        } else if constexpr (sizeof(T) == 4) {
            ScatterImplB32<T, false, true>(
                dstLocal, srcLocal, dstOffsetLocal, dstLength, dstBaseOffset, mask[0], newRepeatTimes, srcRepStride);
        }
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_SCATTER_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_SCATTER_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_SCATTER_IMPL_H__
#endif
