/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file kernel_reg_compute_vec_binary_impl.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic/reg_compute/dav_5162/kernel_reg_compute_vec_binary_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use \"#include \"reg_compute/kernel_reg_compute_vec_binary_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_VEC_BINARY_IMPL__
#endif

#ifndef ASCENDC_MODULE_REG_COMPUTE_VEC_BINARY_IMPL_H
#define ASCENDC_MODULE_REG_COMPUTE_VEC_BINARY_IMPL_H

#include "../../../../include/basic_api/reg_compute/kernel_reg_compute_vec_cmpsel_intf.h"
#include "../../../../include/basic_api/reg_compute/kernel_reg_compute_vec_duplicate_intf.h"

namespace AscendC {
namespace Reg {
namespace Internal {
__aicore__ inline constexpr DivSpecificMode GetDivSpecificMode(MaskMergeMode mrgMode)
{
    return {.mrgMode = mrgMode, .precisionMode = false, .algo = DivAlgo::INTRINSIC};
}

__aicore__ inline constexpr DivSpecificMode GetDivSpecificMode(const DivSpecificMode* sprMode)
{
    return {.mrgMode = sprMode->mrgMode, .precisionMode = sprMode->precisionMode, .algo = sprMode->algo};
}
} // namespace Internal
template <typename T, MaskMergeMode mode, typename U>
__simd_callee__ inline void AddComplexTwoRegImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "U should be RegTraitNumTwo");
    Add((RegTensor<typename ActualT::EleType>&)dstReg.reg[0], (RegTensor<typename ActualT::EleType>&)srcReg0.reg[0],
        (RegTensor<typename ActualT::EleType>&)srcReg1.reg[0], mask);
    Add((RegTensor<typename ActualT::EleType>&)dstReg.reg[1], (RegTensor<typename ActualT::EleType>&)srcReg0.reg[1],
        (RegTensor<typename ActualT::EleType>&)srcReg1.reg[1], mask);
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void AddB64Impl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 8, "data type should be B64");
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "U should be RegTraitNumTwo");
    if constexpr (SupportType<ActualT, complex64>()) {
        AddComplexTwoRegImpl<T, mode, U>(dstReg, srcReg0, srcReg1, mask);
    } else {
        MaskReg carryMask;
        MaskReg carrySrcMask;
        if constexpr (Std::is_same_v<ActualT, uint64_t>) {
            Add(carryMask, (RegTensor<uint32_t>&)dstReg.reg[0], (RegTensor<uint32_t>&)srcReg0.reg[0],
                (RegTensor<uint32_t>&)srcReg1.reg[0], mask);
            AddC(
                carrySrcMask, (RegTensor<uint32_t>&)dstReg.reg[1], (RegTensor<uint32_t>&)srcReg0.reg[1],
                (RegTensor<uint32_t>&)srcReg1.reg[1], carryMask, mask);
        } else if constexpr (Std::is_same_v<ActualT, int64_t>) {
            Add(carryMask, (RegTensor<int32_t>&)dstReg.reg[0], (RegTensor<int32_t>&)srcReg0.reg[0],
                (RegTensor<int32_t>&)srcReg1.reg[0], mask);
            AddC(
                carrySrcMask, (RegTensor<int32_t>&)dstReg.reg[1], (RegTensor<int32_t>&)srcReg0.reg[1],
                (RegTensor<int32_t>&)srcReg1.reg[1], carryMask, mask);
        }
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void AddComplex32OnetraitImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 4, "data type should be B32");
    static_assert(CheckRegTrait<U, RegTraitNumOne>(), "U should be RegTraitNumOne");
    MaskReg maskTrait2;
    MaskPack(maskTrait2, mask);
    RegTensor<ActualT, RegTraitNumTwo> addTraitTwoSrcReg0;
    RegTensor<ActualT, RegTraitNumTwo> addTraitTwoSrcReg1;
    RegTensor<ActualT, RegTraitNumTwo> addTraitTwoDstReg;
    B32TraitOneToTraitTwo(addTraitTwoSrcReg0, srcReg0);
    B32TraitOneToTraitTwo(addTraitTwoSrcReg1, srcReg1);
    AddComplexTwoRegImpl<T, mode, RegTensor<ActualT, RegTraitNumTwo>>(
        addTraitTwoDstReg, addTraitTwoSrcReg0, addTraitTwoSrcReg1, maskTrait2);
    B32TraitTwoToTraitOne(dstReg, addTraitTwoDstReg);
}

template <typename T, auto mode, typename U, auto func>
__simd_callee__ inline void CalTraitOneByTransToTraitTwo(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    MaskReg maskTrait2;
    MaskPack(maskTrait2, mask);
    RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg0;
    RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg1;
    RegTensor<ActualT, RegTraitNumTwo> traitTwoDstReg;
    B64TraitOneToTraitTwo(traitTwoSrcReg0, srcReg0);
    B64TraitOneToTraitTwo(traitTwoSrcReg1, srcReg1);
    func(traitTwoDstReg, traitTwoSrcReg0, traitTwoSrcReg1, maskTrait2);
    B64TraitTwoToTraitOne(dstReg, traitTwoDstReg);
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void AddOperator(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    if constexpr (SupportType<ActualT, complex32>()) {
        if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            AddComplexTwoRegImpl<T, mode, U>(dstReg, srcReg0, srcReg1, mask);
        } else {
            AddComplex32OnetraitImpl(dstReg, srcReg0, srcReg1, mask);
        }
    } else if constexpr (SupportBytes<ActualT, 8>()) {
        if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            U dstTemp;
            AddB64Impl(dstTemp, srcReg0, srcReg1, mask);
            dstReg = dstTemp;
        } else {
            CalTraitOneByTransToTraitTwo<T, mode, U, AddB64Impl<T, mode, Reg::RegTensor<ActualT, Reg::RegTraitNumTwo>>>(
                dstReg, srcReg0, srcReg1, mask);
        }
    } else {
        constexpr auto modeValue = GetMaskMergeMode<mode>();
        vadd(dstReg, srcReg0, srcReg1, mask, modeValue);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void AddImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(
        SupportType<
            ActualT, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, half, float, bfloat16_t, uint64_t, int64_t,
            complex32, complex64>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING, MaskMergeMode::MERGING>(),
        "current Add api only supported Mode ZEROING/MERGING on current device!");
    if constexpr (mode == MaskMergeMode::ZEROING) {
        AddOperator<T, mode, U>(dstReg, srcReg0, srcReg1, mask);
    } else if constexpr (mode == MaskMergeMode::MERGING) {
        U dstCopyReg;
        AddOperator<T, MaskMergeMode::ZEROING, U>(dstCopyReg, srcReg0, srcReg1, mask);
        CopyMerging(dstReg, dstCopyReg, mask);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void SubB64Impl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 8, "data type should be B64");
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "U should be RegTraitNumTwo");
    MaskReg carryMask;
    MaskReg carrySrcMask;
    if constexpr (SupportType<ActualT, complex64>()) {
        SubComplex64Impl(dstReg, srcReg0, srcReg1, mask);
    } else {
        if constexpr (Std::is_same_v<ActualT, uint64_t>) {
            Sub(carryMask, (RegTensor<uint32_t>&)dstReg.reg[0], (RegTensor<uint32_t>&)srcReg0.reg[0],
                (RegTensor<uint32_t>&)srcReg1.reg[0], mask);
            SubC(
                carrySrcMask, (RegTensor<uint32_t>&)dstReg.reg[1], (RegTensor<uint32_t>&)srcReg0.reg[1],
                (RegTensor<uint32_t>&)srcReg1.reg[1], carryMask, mask);
        } else if constexpr (Std::is_same_v<ActualT, int64_t>) {
            Sub(carryMask, (RegTensor<int32_t>&)dstReg.reg[0], (RegTensor<int32_t>&)srcReg0.reg[0],
                (RegTensor<int32_t>&)srcReg1.reg[0], mask);
            SubC(
                carrySrcMask, (RegTensor<int32_t>&)dstReg.reg[1], (RegTensor<int32_t>&)srcReg0.reg[1],
                (RegTensor<int32_t>&)srcReg1.reg[1], carryMask, mask);
        }
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void SubComplex64Impl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 8, "data type should be B64");
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "U should be RegTraitNumTwo");
    Sub((RegTensor<float>&)dstReg.reg[0], (RegTensor<float>&)srcReg0.reg[0], (RegTensor<float>&)srcReg1.reg[0], mask);
    Sub((RegTensor<float>&)dstReg.reg[1], (RegTensor<float>&)srcReg0.reg[1], (RegTensor<float>&)srcReg1.reg[1], mask);
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void SubComplex32TwoImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 4, "data type should be B32");
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "U should be RegTraitNumTwo");
    Sub((RegTensor<half>&)dstReg.reg[0], (RegTensor<half>&)srcReg0.reg[0], (RegTensor<half>&)srcReg1.reg[0], mask);
    Sub((RegTensor<half>&)dstReg.reg[1], (RegTensor<half>&)srcReg0.reg[1], (RegTensor<half>&)srcReg1.reg[1], mask);
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void SubComplex32OnetraitImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 4, "data type should be B32");
    static_assert(CheckRegTrait<U, RegTraitNumOne>(), "U should be RegTraitNumOne");
    MaskReg maskTrait2;
    MaskPack(maskTrait2, mask);
    RegTensor<ActualT, RegTraitNumTwo> subTraitTwoSrcReg0;
    RegTensor<ActualT, RegTraitNumTwo> subTraitTwoSrcReg1;
    RegTensor<ActualT, RegTraitNumTwo> subTraitTwoDstReg;
    B32TraitOneToTraitTwo(subTraitTwoSrcReg0, srcReg0);
    B32TraitOneToTraitTwo(subTraitTwoSrcReg1, srcReg1);
    SubComplex32TwoImpl<T, mode, RegTensor<ActualT, RegTraitNumTwo>>(
        subTraitTwoDstReg, subTraitTwoSrcReg0, subTraitTwoSrcReg1, maskTrait2);
    B32TraitTwoToTraitOne(dstReg, subTraitTwoDstReg);
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void SubImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(
        SupportType<
            ActualT, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, half, float, bfloat16_t, uint64_t, int64_t,
            complex32, complex64>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING>(), "current Sub api only supported Mode ZEROING on current device!");
    if constexpr (SupportType<ActualT, complex32>()) {
        if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            SubComplex32TwoImpl(dstReg, srcReg0, srcReg1, mask);
        } else {
            SubComplex32OnetraitImpl(dstReg, srcReg0, srcReg1, mask);
        }
    } else if constexpr (SupportBytes<ActualT, 8>()) {
        if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            U dstTemp;
            SubB64Impl(dstTemp, srcReg0, srcReg1, mask);
            dstReg = dstTemp;
        } else {
            CalTraitOneByTransToTraitTwo<T, mode, U, SubB64Impl<T, mode, Reg::RegTensor<ActualT, Reg::RegTraitNumTwo>>>(
                dstReg, srcReg0, srcReg1, mask);
        }
    } else {
        constexpr auto modeValue = GetMaskMergeMode<mode>();
        vsub(dstReg, srcReg0, srcReg1, mask, modeValue);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void MulB64Impl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 8, "MulB64Impl data type should be B64");
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "MulB64Impl U should be RegTraitNumTwo");
    static_assert(SupportEnum<mode, MaskMergeMode::ZEROING>(), "MulB64Impl only support Mode ZEROING");
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (SupportType<ActualT, complex64>()) {
        MulComplex64Impl(dstReg, srcReg0, srcReg1, mask);
    } else {
        if constexpr (Std::is_same_v<ActualT, uint64_t>) {
            Mull(
                (RegTensor<uint32_t>&)dstReg.reg[0], (RegTensor<uint32_t>&)dstReg.reg[1],
                (RegTensor<uint32_t>&)srcReg0.reg[0], (RegTensor<uint32_t>&)srcReg1.reg[0], mask);
            vmula(
                (RegTensor<uint32_t>&)dstReg.reg[1], (RegTensor<uint32_t>&)srcReg0.reg[0],
                (RegTensor<uint32_t>&)srcReg1.reg[1], mask, modeValue);
            vmula(
                (RegTensor<uint32_t>&)dstReg.reg[1], (RegTensor<uint32_t>&)srcReg0.reg[1],
                (RegTensor<uint32_t>&)srcReg1.reg[0], mask, modeValue);
        } else if constexpr (Std::is_same_v<ActualT, int64_t>) {
            Mull(
                (RegTensor<uint32_t>&)dstReg.reg[0], (RegTensor<uint32_t>&)dstReg.reg[1],
                (RegTensor<uint32_t>&)srcReg0.reg[0], (RegTensor<uint32_t>&)srcReg1.reg[0], mask);
            vmula(
                (RegTensor<int32_t>&)dstReg.reg[1], (RegTensor<int32_t>&)srcReg0.reg[0],
                (RegTensor<int32_t>&)srcReg1.reg[1], mask, modeValue);
            vmula(
                (RegTensor<int32_t>&)dstReg.reg[1], (RegTensor<int32_t>&)srcReg0.reg[1],
                (RegTensor<int32_t>&)srcReg1.reg[0], mask, modeValue);
        }
    }
}

template <typename T, MaskMergeMode mode, typename U>
__simd_callee__ inline void ComplexMulKernel(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "U should be RegTraitNumTwo");
    RegTensor<typename ActualT::EleType>& src0Real = (RegTensor<typename ActualT::EleType>&)srcReg0.reg[0];
    RegTensor<typename ActualT::EleType>& src0Imag = (RegTensor<typename ActualT::EleType>&)srcReg0.reg[1];
    RegTensor<typename ActualT::EleType>& src1Real = (RegTensor<typename ActualT::EleType>&)srcReg1.reg[0];
    RegTensor<typename ActualT::EleType>& src1Imag = (RegTensor<typename ActualT::EleType>&)srcReg1.reg[1];
    RegTensor<typename ActualT::EleType>& dstReal = (RegTensor<typename ActualT::EleType>&)dstReg.reg[0];
    RegTensor<typename ActualT::EleType>& dstImag = (RegTensor<typename ActualT::EleType>&)dstReg.reg[1];
    if constexpr (SupportType<ActualT, complex32>()) {
        // (a + bi) * (c + di) = (ac - bd) + (bc + ad)i.
        RegTensor<typename ActualT::EleType> e;
        RegTensor<typename ActualT::EleType> f;
        RegTensor<typename ActualT::EleType> g;
        RegTensor<typename ActualT::EleType> h;
        Mul(e, src0Real, src1Real, mask);
        Mul(f, src0Imag, src1Imag, mask);
        Mul(g, src0Imag, src1Real, mask);
        Mul(h, src0Real, src1Imag, mask);
        Sub(dstReal, e, f, mask);
        Add(dstImag, g, h, mask);
    } else {
        // FMA path for (a + bi) * (c + di):
        // first Mul initializes real = (-b)d and imag = bc,
        // then MulAddDst accumulates ac and ad.
        RegTensor<typename ActualT::EleType> src0ImagMinus;
        Muls(src0ImagMinus, src0Imag, -1, mask);
        Mul(dstReal, src0ImagMinus, src1Imag, mask);
        MulAddDst(dstReal, src0Real, src1Real, mask);
        Mul(dstImag, src0Imag, src1Real, mask);
        MulAddDst(dstImag, src0Real, src1Imag, mask);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void MulComplex64Impl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 8, "data type should be B64");
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "U should be RegTraitNumTwo");
    ComplexMulKernel<T, mode, Reg::RegTensor<ActualT, Reg::RegTraitNumTwo>>(dstReg, srcReg0, srcReg1, mask);
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void MulComplex32TwoImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 4, "data type should be B32");
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "U should be RegTraitNumTwo");
    ComplexMulKernel<T, mode, Reg::RegTensor<ActualT, Reg::RegTraitNumTwo>>(dstReg, srcReg0, srcReg1, mask);
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void MulComplex32OnetraitImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 4, "data type should be B32");
    static_assert(CheckRegTrait<U, RegTraitNumOne>(), "U should be RegTraitNumOne");

    MaskReg maskTrait2;
    MaskPack(maskTrait2, mask);
    RegTensor<ActualT, RegTraitNumTwo> mulTraitTwoSrcReg0;
    RegTensor<ActualT, RegTraitNumTwo> mulTraitTwoSrcReg1;
    RegTensor<ActualT, RegTraitNumTwo> mulTraitTwoDstReg;

    B32TraitOneToTraitTwo(mulTraitTwoSrcReg0, srcReg0);
    B32TraitOneToTraitTwo(mulTraitTwoSrcReg1, srcReg1);
    MulComplex32TwoImpl(mulTraitTwoDstReg, mulTraitTwoSrcReg0, mulTraitTwoSrcReg1, maskTrait2);
    B32TraitTwoToTraitOne(dstReg, mulTraitTwoDstReg);
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void MulImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(
        SupportType<
            ActualT, uint16_t, int16_t, uint32_t, int32_t, half, float, bfloat16_t, uint64_t, int64_t, complex32,
            complex64>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING>(), "current Mul api only supported Mode ZEROING on current device!");
    if constexpr (SupportType<ActualT, complex32>()) {
        if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            MulComplex32TwoImpl(dstReg, srcReg0, srcReg1, mask);
        } else {
            MulComplex32OnetraitImpl(dstReg, srcReg0, srcReg1, mask);
        }
    } else if constexpr (SupportBytes<ActualT, 8>()) {
        if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            U dstTemp;
            MulB64Impl(dstTemp, srcReg0, srcReg1, mask);
            dstReg = dstTemp;
        } else {
            CalTraitOneByTransToTraitTwo<T, mode, U, MulB64Impl<T, mode, Reg::RegTensor<ActualT, Reg::RegTraitNumTwo>>>(
                dstReg, srcReg0, srcReg1, mask);
        }
    } else {
        constexpr auto modeValue = GetMaskMergeMode<mode>();
        vmul(dstReg, srcReg0, srcReg1, mask, modeValue);
    }
}

template <typename T>
__simd_callee__ inline void AbsUsingS32(T& dstReg, T& srcReg0, MaskReg& mask)
{
    RegTensor<int32_t, RegTraitNumOne> vTmp1, vTmp2, vTmp3;
    MaskReg carry0, carry1, carry2;
    vbr(vTmp1, 0);
    vcmp_lt(carry0, (RegTensor<int32_t>&)srcReg0.reg[1], vTmp1, mask);
    Sub(carry1, vTmp2, vTmp1, (RegTensor<int32_t>&)srcReg0.reg[0], carry0);
    SubC(carry2, vTmp3, vTmp1, (RegTensor<int32_t>&)srcReg0.reg[1], carry1, carry0);
    vsel((RegTensor<int32_t>&)dstReg.reg[0], vTmp2, (RegTensor<int32_t>&)srcReg0.reg[0], carry0);
    vsel((RegTensor<int32_t>&)dstReg.reg[1], vTmp3, (RegTensor<int32_t>&)srcReg0.reg[1], carry0);
}

template <typename T>
__simd_callee__ inline void VbrUsingU32(T& dstReg, uint32_t highScalar, uint32_t lowScalar)
{
    vbr((RegTensor<uint32_t>&)dstReg.reg[0], highScalar);
    vbr((RegTensor<uint32_t>&)dstReg.reg[1], lowScalar);
}

template <typename T, typename U>
__simd_callee__ inline void VcvtS642F32(T& vDst, U& vSrc)
{
    // 当前芯片 vcvt 不支持 B64 与其他类型互转, 软实现 s64->f32:
    // 按 v = hi*2^32 + lo_u32 分解(补码全范围成立, 含负数与 INT64_MIN),
    // f32(v) = f32(hi)*2^32 + f32(lo), 相对误差与硬件 RN 同量级(~2^-24), 满足倒数估计场景的精度要求
    constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    constexpr auto mergingValue = GetMaskMergeMode<MaskMergeMode::MERGING>();
    constexpr auto roundRintValue = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_RINT>()>();
    constexpr uint32_t expBias32 = 0x10000000U; // f32 指数域 +32, 等价于数值乘 2^32
    MaskReg fullMask = pset_b32(PAT_ALL);
    RegTensor<int32_t> zeroS;
    RegTensor<float> loF, hiF, two32F;
    MaskReg loNeg, hiZero;
    vbr(zeroS, 0);
    vbr(two32F, 4294967296.0f);
    // lo 按无符号处理: 先按有符号转换, 符号位为 1 的 lane 补加 2^32
    vcvt(loF, (RegTensor<int32_t>&)vSrc.reg[0], fullMask, roundRintValue, modeValue);
    vcmp_lt(loNeg, (RegTensor<int32_t>&)vSrc.reg[0], zeroS, fullMask);
    vadd(loF, loF, two32F, loNeg, mergingValue);
    // hi 转换后经指数偏置放大 2^32 再与 lo 相加; hi == 0 时直接取 lo 的结果
    vcvt(hiF, (RegTensor<int32_t>&)vSrc.reg[1], fullMask, roundRintValue, modeValue);
    vadds((RegTensor<uint32_t>&)hiF, (RegTensor<uint32_t>&)hiF, expBias32, fullMask);
    vadd(hiF, hiF, loF, fullMask, modeValue);
    vcmps_eq(hiZero, (RegTensor<uint32_t>&)vSrc.reg[1], static_cast<uint32_t>(0), fullMask);
    vsel((RegTensor<uint32_t>&)vDst, (RegTensor<uint32_t>&)hiF, (RegTensor<uint32_t>&)loF, hiZero);
}

template <typename T, typename U>
__simd_callee__ inline void VcvtF322S64(T& vDst, U& vSrc)
{
    // 当前芯片 vcvt 不支持 B64 与其他类型互转, 软实现 f32->s64(向零截断):
    // y 按三段拆分 y = h1*2^48 + h0*2^32 + lo, 每段经 f32 指数域缩放(精确)后截断转 s32,
    // 逐段相减无双重舍入误差; lo >= 2^31 时依赖 NO_SAT 回绕得到正确低位
    // 仅保证非负有限输入正确(倒数估计场景), 负值/inf/NaN lane 输出无意义
    constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    constexpr auto satModeValue =
        std::integral_constant<::RoundingSaturation, static_cast<::RoundingSaturation>(SatMode::NO_SAT)>();
    constexpr auto roundRintValue = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_RINT>()>();
    constexpr auto roundTruncValue = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_TRUNC>()>();
    constexpr uint32_t expBias32 = 0x10000000U;         // f32 指数域 +32, 等价于数值乘 2^32
    constexpr uint32_t expBias48 = 0x18000000U;         // f32 指数域 +48, 等价于数值乘 2^48
    constexpr uint32_t expNegBias32 = 0U - 0x10000000U; // 指数域 -32, 等价于数值乘 2^-32
    constexpr uint32_t expNegBias48 = 0U - 0x18000000U; // 指数域 -48, 等价于数值乘 2^-48
    MaskReg fullMask = pset_b32(PAT_ALL);
    RegTensor<float> tF, remF;
    RegTensor<int32_t> h1, h0;
    // h1 = trunc(y * 2^-48)
    vadds((RegTensor<uint32_t>&)tF, (RegTensor<uint32_t>&)vSrc, expNegBias48, fullMask);
    vcvt(h1, tF, fullMask, roundTruncValue, satModeValue, modeValue);
    // rem = y - h1 * 2^48, 两段均可被 f32 精确表示, 减法无舍入
    vcvt(tF, h1, fullMask, roundRintValue, modeValue);
    vadds((RegTensor<uint32_t>&)tF, (RegTensor<uint32_t>&)tF, expBias48, fullMask);
    vsub(remF, (RegTensor<float>&)vSrc, tF, fullMask, modeValue);
    // h0 = trunc(rem * 2^-32)
    vadds((RegTensor<uint32_t>&)tF, (RegTensor<uint32_t>&)remF, expNegBias32, fullMask);
    vcvt(h0, tF, fullMask, roundTruncValue, satModeValue, modeValue);
    // lo = trunc(rem - h0 * 2^32)
    vcvt(tF, h0, fullMask, roundRintValue, modeValue);
    vadds((RegTensor<uint32_t>&)tF, (RegTensor<uint32_t>&)tF, expBias32, fullMask);
    vsub(tF, remF, tF, fullMask, modeValue);
    vcvt((RegTensor<int32_t>&)vDst.reg[0], tF, fullMask, roundTruncValue, satModeValue, modeValue);
    // 高 32bit = h1 * 2^16 + h0
    vshls(h1, h1, static_cast<int16_t>(16), fullMask, modeValue);
    vadd((RegTensor<int32_t>&)vDst.reg[1], h1, h0, fullMask, modeValue);
}

template <typename T>
__simd_callee__ inline void VnotInPlace(T& vReg, MaskReg& mask)
{
    vnot((RegTensor<uint32_t>&)vReg.reg[0], (RegTensor<uint32_t>&)vReg.reg[0], mask, MODE_ZEROING);
    vnot((RegTensor<uint32_t>&)vReg.reg[1], (RegTensor<uint32_t>&)vReg.reg[1], mask, MODE_ZEROING);
}

template <typename T, typename U, typename S>
__simd_callee__ inline void VmulUsingU32(T& vDst, U& vSrc0, S& vSrc1, MaskReg& mask)
{
    constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    Mull(
        (RegTensor<uint32_t>&)vDst.reg[0], (RegTensor<uint32_t>&)vDst.reg[1], (RegTensor<uint32_t>&)vSrc0.reg[0],
        (RegTensor<uint32_t>&)vSrc1.reg[0], mask);
    vmula(
        (RegTensor<uint32_t>&)vDst.reg[1], (RegTensor<uint32_t>&)vSrc0.reg[0], (RegTensor<uint32_t>&)vSrc1.reg[1], mask,
        modeValue);
    vmula(
        (RegTensor<uint32_t>&)vDst.reg[1], (RegTensor<uint32_t>&)vSrc0.reg[1], (RegTensor<uint32_t>&)vSrc1.reg[0], mask,
        modeValue);
}

template <typename T, typename U, typename S>
__simd_callee__ inline void B128Calc(T& vSrc0, U& vSrc1, S& vSrc2, MaskReg& mask)
{
    RegTensor<uint64_t, RegTraitNumTwo> vTmp128Mul0, vTmp128Mul1, vTmp128Mul2, vTmp128Mul3;
    RegTensor<uint32_t, RegTraitNumOne> vTmp128Dst0, vTmp128Dst1;
    MaskReg pTmp128Carry0, pTmp128Carry1;
    Mull(
        (RegTensor<uint32_t>&)vTmp128Mul0.reg[0], (RegTensor<uint32_t>&)vTmp128Mul0.reg[1],
        (RegTensor<uint32_t>&)vSrc0.reg[0], (RegTensor<uint32_t>&)vSrc1.reg[0], mask);
    Mull(
        (RegTensor<uint32_t>&)vTmp128Mul1.reg[0], (RegTensor<uint32_t>&)vTmp128Mul1.reg[1],
        (RegTensor<uint32_t>&)vSrc0.reg[0], (RegTensor<uint32_t>&)vSrc1.reg[1], mask);
    Mull(
        (RegTensor<uint32_t>&)vTmp128Mul2.reg[0], (RegTensor<uint32_t>&)vTmp128Mul2.reg[1],
        (RegTensor<uint32_t>&)vSrc0.reg[1], (RegTensor<uint32_t>&)vSrc1.reg[0], mask);
    Mull(
        (RegTensor<uint32_t>&)vTmp128Mul3.reg[0], (RegTensor<uint32_t>&)vTmp128Mul3.reg[1],
        (RegTensor<uint32_t>&)vSrc0.reg[1], (RegTensor<uint32_t>&)vSrc1.reg[1], mask);
    Add(pTmp128Carry0, vTmp128Dst0, (RegTensor<uint32_t>&)vTmp128Mul0.reg[1], (RegTensor<uint32_t>&)vTmp128Mul1.reg[0],
        mask);
    Add(pTmp128Carry1, vTmp128Dst1, vTmp128Dst0, (RegTensor<uint32_t>&)vTmp128Mul2.reg[0], mask);
    AddC(
        pTmp128Carry0, vTmp128Dst0, (RegTensor<uint32_t>&)vTmp128Mul3.reg[0], (RegTensor<uint32_t>&)vTmp128Mul1.reg[1],
        pTmp128Carry0, mask);
    AddC(
        pTmp128Carry1, (RegTensor<uint32_t>&)vSrc1.reg[0], vTmp128Dst0, (RegTensor<uint32_t>&)vTmp128Mul2.reg[1],
        pTmp128Carry1, mask);
    AddC(pTmp128Carry0, vTmp128Dst0, vSrc2, (RegTensor<uint32_t>&)vTmp128Mul3.reg[1], pTmp128Carry0, mask);
    AddC(pTmp128Carry0, (RegTensor<uint32_t>&)vSrc1.reg[1], vSrc2, vTmp128Dst0, pTmp128Carry1, mask);
}

template <typename T, typename U, typename S>
__simd_callee__ inline void VselUsingU32(T& vDst, U& vSrc0, S& vSrc1, MaskReg& mask)
{
    vsel(
        (RegTensor<uint32_t>&)vDst.reg[0], (RegTensor<uint32_t>&)vSrc0.reg[0], (RegTensor<uint32_t>&)vSrc1.reg[0],
        mask);
    vsel(
        (RegTensor<uint32_t>&)vDst.reg[1], (RegTensor<uint32_t>&)vSrc0.reg[1], (RegTensor<uint32_t>&)vSrc1.reg[1],
        mask);
}

template <typename T, typename U>
__simd_callee__ inline void VcmpEqUsingU32(MaskReg& Dst, T& vSrc0, U& vSrc1, MaskReg& Src)
{
    MaskReg cmpEqLow, cmpEqHigh;
    vcmp_eq(cmpEqLow, (RegTensor<uint32_t>&)vSrc0.reg[0], (RegTensor<uint32_t>&)vSrc1.reg[0], Src);
    vcmp_eq(cmpEqHigh, (RegTensor<uint32_t>&)vSrc0.reg[1], (RegTensor<uint32_t>&)vSrc1.reg[1], Src);
    pand(Dst, cmpEqLow, cmpEqHigh, Src);
}

template <typename T, typename U>
__simd_callee__ inline void VcmpGeUsingU32(MaskReg& Dst, T& vSrc0, U& vSrc1, MaskReg& Src)
{
    MaskReg highEq, lowCmp, highCmp;
    vcmp_eq(highEq, (RegTensor<uint32_t>&)vSrc0.reg[1], (RegTensor<uint32_t>&)vSrc1.reg[1], Src);
    vcmp_ge(lowCmp, (RegTensor<uint32_t>&)vSrc0.reg[0], (RegTensor<uint32_t>&)vSrc1.reg[0], Src);
    vcmp_ge(highCmp, (RegTensor<uint32_t>&)vSrc0.reg[1], (RegTensor<uint32_t>&)vSrc1.reg[1], Src);
    psel(Dst, lowCmp, highCmp, highEq);
}

template <typename T, typename U, typename S>
__simd_callee__ inline void VaddUsingU32(T& vDst, U& vSrc0, S& vSrc1, MaskReg& mask)
{
    MaskReg pTmpAdd0, pTmpAdd1;
    Add(pTmpAdd0, (RegTensor<uint32_t>&)vDst.reg[0], (RegTensor<uint32_t>&)vSrc0.reg[0],
        (RegTensor<uint32_t>&)vSrc1.reg[0], mask);
    AddC(
        pTmpAdd1, (RegTensor<uint32_t>&)vDst.reg[1], (RegTensor<uint32_t>&)vSrc0.reg[1],
        (RegTensor<uint32_t>&)vSrc1.reg[1], pTmpAdd0, mask);
}

template <typename T, typename U, typename S>
__simd_callee__ inline void VsubUsingU32(T& vDst, U& vSrc0, S& vSrc1, MaskReg& mask)
{
    MaskReg carry0, carry1;
    Sub(carry0, (RegTensor<uint32_t>&)vDst.reg[0], (RegTensor<uint32_t>&)vSrc0.reg[0],
        (RegTensor<uint32_t>&)vSrc1.reg[0], mask);
    SubC(
        carry1, (RegTensor<uint32_t>&)vDst.reg[1], (RegTensor<uint32_t>&)vSrc0.reg[1],
        (RegTensor<uint32_t>&)vSrc1.reg[1], carry0, mask);
}

template <typename T, typename U, typename S>
__simd_callee__ inline void DivSignCal(MaskReg& signResP, T& vSrc0, U& vSrc1, S& vSrc2, MaskReg& mask)
{
    MaskReg src0Ge0P, src1Ge0P;
    vcmp_ge(src0Ge0P, (RegTensor<int32_t>&)vSrc0.reg[1], (RegTensor<int32_t>&)vSrc2, mask);
    vcmp_ge(src1Ge0P, (RegTensor<int32_t>&)vSrc1.reg[1], (RegTensor<int32_t>&)vSrc2, mask);
    pxor(signResP, src0Ge0P, src1Ge0P, mask);
    pnot(signResP, signResP, mask);
}

template <typename T, typename U, typename S>
__simd_callee__ inline void F32PreProcess(T& vTmp4, U& vTmp3, S& vTmp2, MaskReg& mask)
{
    constexpr uint32_t divF32Bias = 0x1FFFFFFEU;
    vbr(vTmp3, static_cast<float>(1));
    vdiv(vTmp3, vTmp3, vTmp2, mask, MODE_ZEROING);
    vadds(vTmp4, (RegTensor<uint32_t>&)vTmp3, divF32Bias, mask);
}

template <typename T>
__simd_callee__ inline void DivS64Impl(T& dstReg, T& srcReg0, T& srcReg1, MaskReg& mask)
{
    MaskReg startMask = mask;
    RegTensor<int64_t, RegTraitNumTwo> vAbsSrc0, vAbsSrc1, vTmp5;
    RegTensor<float, RegTraitNumOne> vTmp2, vTmp3;
    RegTensor<uint32_t, RegTraitNumOne> vTmp4;
    RegTensor<uint64_t, RegTraitNumTwo> vConstDup0, qZero, vConstDup1, vTmp6, vTmp7, vTmp8, vTmp9;
    MaskReg zeroMask, nonZeroMask, oriMask, nonOneMask, oneMask, signResP, ge0P;
    AbsUsingS32(vAbsSrc0, srcReg0, mask);
    AbsUsingS32(vAbsSrc1, srcReg1, mask);
    VbrUsingU32(vConstDup0, static_cast<uint32_t>(0), static_cast<uint32_t>(0));
    constexpr uint32_t u32MaxNum = 0xFFFFFFFFU;
    VbrUsingU32(qZero, u32MaxNum, u32MaxNum);
    VbrUsingU32(vConstDup1, static_cast<uint32_t>(1), static_cast<uint32_t>(0));
    VcmpEqUsingU32(zeroMask, srcReg1, vConstDup0, mask);
    pnot(nonZeroMask, zeroMask, mask);
    mask = nonZeroMask;
    VcmpEqUsingU32(oneMask, vAbsSrc1, vConstDup1, mask);
    pnot(nonOneMask, oneMask, mask);
    oriMask = mask;
    pand(mask, nonOneMask, nonZeroMask, mask);
    VcvtS642F32(vTmp2, vAbsSrc1);
    F32PreProcess(vTmp4, vTmp3, vTmp2, mask);
    VcvtF322S64(vTmp5, vTmp4);
    VmulUsingU32(vTmp6, vAbsSrc1, vTmp5, mask);
    VnotInPlace(vTmp6, mask);
    AddB64Impl(vTmp6, vTmp6, vConstDup1, mask);
    vbr(vTmp4, 0);
    B128Calc(vTmp5, vTmp6, vTmp4, mask);
    VaddUsingU32(vTmp7, vTmp5, vTmp6, mask);
    VmulUsingU32(vTmp6, vAbsSrc1, vTmp7, mask);
    VnotInPlace(vTmp6, mask);
    AddB64Impl(vTmp6, vTmp6, vConstDup1, mask);
    B128Calc(vTmp7, vTmp6, vTmp4, mask);
    VaddUsingU32(vTmp6, vTmp7, vTmp6, mask);
    B128Calc(vAbsSrc0, vTmp6, vTmp4, mask);
    VmulUsingU32(vTmp7, vTmp6, vAbsSrc1, mask);
    VsubUsingU32(vTmp7, vAbsSrc0, vTmp7, mask);
    VcmpGeUsingU32(ge0P, vTmp7, vAbsSrc1, mask);
    VsubUsingU32(vTmp8, vTmp7, vAbsSrc1, ge0P);
    AddB64Impl(vTmp9, vTmp6, vConstDup1, ge0P);
    VselUsingU32(vTmp7, vTmp8, vTmp7, ge0P);
    VselUsingU32(vTmp6, vTmp9, vTmp6, ge0P);
    VcmpGeUsingU32(ge0P, vTmp7, vAbsSrc1, mask);
    AddB64Impl(vTmp9, vTmp6, vConstDup1, ge0P);
    VselUsingU32(vTmp6, vTmp9, vTmp6, ge0P);
    VselUsingU32(vTmp6, vAbsSrc0, vTmp6, oneMask);
    mask = oriMask;
    DivSignCal(signResP, srcReg0, srcReg1, vTmp4, mask);
    MaskReg pg1 = pset_b32(PAT_ALL);
    VsubUsingU32(vTmp8, vConstDup0, vTmp6, pg1);
    VselUsingU32(vTmp6, vTmp6, vTmp8, signResP);
    VselUsingU32(dstReg, qZero, vTmp6, zeroMask);
    mask = startMask;
}

template <typename T>
__simd_callee__ inline void DivU64Impl(T& dstReg, T& srcReg0, T& srcReg1, MaskReg& mask)
{
    MaskReg beginMask = mask;
    MaskReg zeroMask, nonZeroMask, nonOneMask, oneMask, sDivMask, uSrc1Mask, srcCmpMask, cmpDivMask, ge0P;
    RegTensor<uint64_t, RegTraitNumTwo> vConstDup0, qZero, vConstDup1, vAbsSrc0, vResQ, vTmp6, vTmp7, vTmp8, vTmp9;
    RegTensor<int64_t, RegTraitNumTwo> vAbsSrc1, vTmp5;
    RegTensor<float, RegTraitNumOne> vTmp2, vTmp3;
    RegTensor<uint32_t, RegTraitNumOne> vTmp4;
    VbrUsingU32(vConstDup0, static_cast<uint32_t>(0), static_cast<uint32_t>(0));
    constexpr uint32_t u32MaxNum = 0xFFFFFFFFU;
    VbrUsingU32(qZero, u32MaxNum, u32MaxNum);
    VbrUsingU32(vConstDup1, static_cast<uint32_t>(1), static_cast<uint32_t>(0));
    VcmpEqUsingU32(zeroMask, srcReg1, vConstDup0, mask);
    pnot(nonZeroMask, zeroMask, mask);
    mask = nonZeroMask;
    vAbsSrc0 = srcReg0;
    vmov((RegTensor<int32_t>&)vAbsSrc1.reg[0], (RegTensor<int32_t>&)srcReg1.reg[0]);
    vmov((RegTensor<int32_t>&)vAbsSrc1.reg[1], (RegTensor<int32_t>&)srcReg1.reg[1]);
    constexpr uint64_t vdivU64Const = 0x7FFFFFFFFFFFFFFFULL;
    CompareScalar<uint64_t, CMPMODE::GT, T>(uSrc1Mask, srcReg1, vdivU64Const, mask);
    Compare<uint64_t, CMPMODE::GE, T>(srcCmpMask, srcReg0, srcReg1, mask);
    pand(cmpDivMask, srcCmpMask, uSrc1Mask, mask);
    Select<uint64_t, T>(vResQ, vConstDup1, vConstDup0, cmpDivMask);
    cmpDivMask = pset_b32(PAT_ALLF);
    srcCmpMask = pset_b32(PAT_ALLF);
    Compare<uint64_t, CMPMODE::LT, T>(srcCmpMask, srcReg0, srcReg1, mask);
    pand(cmpDivMask, srcCmpMask, uSrc1Mask, mask);
    Select<uint64_t, T>(vResQ, vConstDup0, vConstDup1, cmpDivMask);
    CompareScalar<uint64_t, CMPMODE::LE, T>(sDivMask, srcReg1, vdivU64Const, mask);
    mask = sDivMask;
    VcmpEqUsingU32(oneMask, vAbsSrc1, vConstDup1, mask);
    pnot(nonOneMask, oneMask, mask);
    pand(mask, nonOneMask, nonZeroMask, mask);
    VcvtS642F32(vTmp2, vAbsSrc1);
    F32PreProcess(vTmp4, vTmp3, vTmp2, mask);
    VcvtF322S64(vTmp5, vTmp4);
    VmulUsingU32(vTmp6, vAbsSrc1, vTmp5, mask);
    VnotInPlace(vTmp6, mask);
    AddB64Impl(vTmp6, vTmp6, vConstDup1, mask);
    vbr(vTmp4, 0);
    B128Calc(vTmp5, vTmp6, vTmp4, mask);
    VaddUsingU32(vTmp7, vTmp5, vTmp6, mask);
    VmulUsingU32(vTmp6, vAbsSrc1, vTmp7, mask);
    VnotInPlace(vTmp6, mask);
    AddB64Impl(vTmp6, vTmp6, vConstDup1, mask);
    B128Calc(vTmp7, vTmp6, vTmp4, mask);
    VaddUsingU32(vTmp6, vTmp7, vTmp6, mask);
    B128Calc(vAbsSrc0, vTmp6, vTmp4, mask);
    VmulUsingU32(vTmp7, vTmp6, vAbsSrc1, mask);
    VsubUsingU32(vTmp7, vAbsSrc0, vTmp7, mask);
    VcmpGeUsingU32(ge0P, vTmp7, vAbsSrc1, mask);
    VsubUsingU32(vTmp8, vTmp7, vAbsSrc1, mask);
    AddB64Impl(vTmp9, vTmp6, vConstDup1, ge0P);
    VselUsingU32(vTmp7, vTmp8, vTmp7, ge0P);
    VselUsingU32(vTmp6, vTmp9, vTmp6, ge0P);
    VcmpGeUsingU32(ge0P, vTmp7, vAbsSrc1, mask);
    AddB64Impl(vTmp9, vTmp6, vConstDup1, ge0P);
    VselUsingU32(vTmp6, vTmp9, vTmp6, ge0P);
    VselUsingU32(vTmp6, vResQ, vTmp6, uSrc1Mask);
    VselUsingU32(vTmp6, vAbsSrc0, vTmp6, oneMask);
    VselUsingU32(dstReg, qZero, vTmp6, zeroMask);
    mask = beginMask;
}

// 当前芯片 vdiv 指令仅支持 half/float, 整数除法使用软实现
// 注意: 该芯片 vcvt 不支持 B64 与其他类型互转, 全程只使用 B32/B16 与 f32 的转换
// 前置声明: 定义在文件后部(Mull 软实现处)
template <typename U>
__simd_callee__ inline void MullU32SoftImpl(U& dstReg0, U& dstReg1, U& srcReg0, U& srcReg1, MaskReg& mask);

// 掩码粒度换算 b16->b32 / b32->b16 (按 Part 取偶/奇索引 lane 的掩码位),
// 逻辑与 common impl 的 AdjustMask_b16_b32/AdjustMask_b32_b16 一致;
// 因部分芯片目录的 common impl 缺少 AdjustMask 族, 在 binary impl 内本地定义以保持自包含
template <RegLayout layoutMode>
__simd_callee__ inline void DivAdjustMaskB16ToB32(MaskReg& dstMask, MaskReg& srcMask)
{
    static_assert(
        SupportEnum<layoutMode, RegLayout::ZERO, RegLayout::ONE>(),
        "DivAdjustMaskB16ToB32 only supports RegLayout ZERO and ONE");
    MaskReg zeroMask = CreateMask<uint8_t, MaskPattern::ALLF>();
    MaskReg tmpMask;
    ppack(dstMask, srcMask, std::integral_constant<::HiloPart, static_cast<::HiloPart>(HighLowPart::LOWEST)>());
    if constexpr (layoutMode == RegLayout::ZERO) {
        pdintlv_b8(dstMask, tmpMask, dstMask, dstMask);
    } else {
        pdintlv_b8(tmpMask, dstMask, dstMask, dstMask);
    }
    pintlv_b8(dstMask, tmpMask, dstMask, zeroMask);
    pintlv_b8(dstMask, tmpMask, dstMask, zeroMask);
}

template <RegLayout layoutMode>
__simd_callee__ inline void DivAdjustMaskB32ToB16(MaskReg& dstMask, MaskReg& srcMask)
{
    static_assert(
        SupportEnum<layoutMode, RegLayout::ZERO, RegLayout::ONE>(),
        "DivAdjustMaskB32ToB16 only supports RegLayout ZERO and ONE");
    MaskReg zeroMask = CreateMask<uint8_t, MaskPattern::ALLF>();
    MaskReg tmpMask;
    ppack(dstMask, srcMask, std::integral_constant<::HiloPart, static_cast<::HiloPart>(HighLowPart::LOWEST)>());
    ppack(dstMask, dstMask, std::integral_constant<::HiloPart, static_cast<::HiloPart>(HighLowPart::LOWEST)>());
    if constexpr (layoutMode == RegLayout::ZERO) {
        pintlv_b8(dstMask, tmpMask, dstMask, zeroMask);
        pintlv_b8(dstMask, tmpMask, dstMask, zeroMask);
    } else {
        pintlv_b8(dstMask, tmpMask, zeroMask, dstMask);
        pintlv_b8(dstMask, tmpMask, dstMask, zeroMask);
    }
}

// 32bit 精确除法, 仅适用于 |操作数| < 2^24 (u16/s16 扩位后的场景), 有符号/无符号通用:
// 两种输入都按 s32 视图转 f32 (u16 零扩展值恒为正, s16 符号扩展值在 s32 范围内), 转换精确;
// IEEE 除法舍入误差 < 1/|b|, 不会跨越整数边界, 截断即精确商
// 除 0 时写入全 1: u16 解释为 0xFFFF, s16 解释为 -1, 位模式一致
template <typename U>
__simd_callee__ inline void Div32ExactViaF32Impl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(SupportType<ActualT, uint32_t, int32_t>(), "Div32ExactViaF32Impl only supports uint32_t/int32_t!");
    constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    constexpr auto satModeValue =
        std::integral_constant<::RoundingSaturation, static_cast<::RoundingSaturation>(SatMode::NO_SAT)>();
    constexpr auto roundRintValue = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_RINT>()>();
    constexpr auto roundTruncValue = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_TRUNC>()>();
    RegTensor<float> src0F, src1F, qF;
    RegTensor<uint32_t> qZero;
    MaskReg zeroMask;
    vcvt(src0F, (RegTensor<int32_t>&)srcReg0, mask, roundRintValue, modeValue);
    vcvt(src1F, (RegTensor<int32_t>&)srcReg1, mask, roundRintValue, modeValue);
    vdiv(qF, src0F, src1F, mask, modeValue);
    vcvt((RegTensor<int32_t>&)dstReg, qF, mask, roundTruncValue, satModeValue, modeValue);
    vbr(qZero, static_cast<uint32_t>(0xFFFFFFFFU));
    vcmps_eq(zeroMask, (RegTensor<uint32_t>&)srcReg1, static_cast<uint32_t>(0), mask);
    vsel((RegTensor<uint32_t>&)dstReg, qZero, (RegTensor<uint32_t>&)dstReg, zeroMask);
}

// u32 除法: f32 倒数估计 + 一次 Newton 迭代 + 余数修正, 全程无 128bit 乘且不使用 B64 vcvt
// f32 路径适用于 b in [3, 2^30): y = floor(2^32/b) in (4, 1.44*2^30], f32 转 s32 不溢出
// 迭代 y' = y + signed_high32(y * (2^32 - b*y)) 使相对误差平方降到 ~2^-48,
// 再偏置 y'' = y' - 1, 保证 q = high32(a * y'') 落在 [真商-3, 真商], 只需向上修正且余数不回卷
// b >= 2^30 (商 <= 3) 走比较减法, b == 2 (商 = a >> 1), b == 1, b == 0 单独选择结果
template <typename U>
__simd_callee__ inline void DivU32SoftImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    constexpr uint32_t expBias32 = 0x10000000U;  // f32 指数域 +32, 等价于数值乘 2^32
    constexpr uint32_t bigDivisor = 0x40000000U; // 2^30
    // 条件更新必须用 MERGING: ZEROING 会把未选中 lane 的 dst 清零
    constexpr auto mergingValue = GetMaskMergeMode<MaskMergeMode::MERGING>();
    constexpr auto satModeValue =
        std::integral_constant<::RoundingSaturation, static_cast<::RoundingSaturation>(SatMode::NO_SAT)>();
    constexpr auto roundRintValue = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_RINT>()>();
    constexpr auto roundTruncValue = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_TRUNC>()>();
    RegTensor<uint32_t>& a = (RegTensor<uint32_t>&)srcReg0;
    RegTensor<uint32_t>& b = (RegTensor<uint32_t>&)srcReg1;
    RegTensor<uint32_t> zeroVec, oneVec, qZero;
    RegTensor<uint32_t> qBig, rBig, qSmall, q, r, y, t, e;
    RegTensor<float> bF, oneF, yF;
    MaskReg zeroMask, oneMask, smallMask, bigMask, cmpMask;
    vbr(zeroVec, static_cast<uint32_t>(0));
    vbr(oneVec, static_cast<uint32_t>(1));
    vbr(qZero, static_cast<uint32_t>(0xFFFFFFFFU));
    vcmps_eq(zeroMask, b, static_cast<uint32_t>(0), mask);
    vcmps_eq(oneMask, b, static_cast<uint32_t>(1), mask);
    vcmps_le(smallMask, b, static_cast<uint32_t>(2), mask);
    vcmps_ge(bigMask, b, bigDivisor, mask);
    // 小除数路径: b == 2 时商为 a >> 1, b == 1 时商为 a
    vshrs(qSmall, a, static_cast<int16_t>(1), mask, modeValue);
    vsel(qSmall, a, qSmall, oneMask);
    // 大除数路径: 商 <= 3, 三轮比较减法直接得到
    vmov(rBig, a);
    vbr(qBig, static_cast<uint32_t>(0));
    vcmp_ge(cmpMask, rBig, b, bigMask);
    vsub(rBig, rBig, b, cmpMask, mergingValue);
    vadd(qBig, qBig, oneVec, cmpMask, mergingValue);
    vcmp_ge(cmpMask, rBig, b, bigMask);
    vsub(rBig, rBig, b, cmpMask, mergingValue);
    vadd(qBig, qBig, oneVec, cmpMask, mergingValue);
    vcmp_ge(cmpMask, rBig, b, bigMask);
    vsub(rBig, rBig, b, cmpMask, mergingValue);
    vadd(qBig, qBig, oneVec, cmpMask, mergingValue);
    // y = floor(2^32/b): f32 倒数 + 指数偏置(乘 2^32) + 截断转 s32 (b >= 3 时 y < 2^31 不溢出)
    vcvt(bF, (RegTensor<int32_t>&)b, mask, roundRintValue, modeValue);
    vbr(oneF, static_cast<float>(1));
    vdiv(yF, oneF, bF, mask, modeValue);
    vadds((RegTensor<uint32_t>&)yF, (RegTensor<uint32_t>&)yF, expBias32, mask);
    vcvt((RegTensor<int32_t>&)y, yF, mask, roundTruncValue, satModeValue, modeValue);
    // 一次 Newton 迭代: y' = y + signed_high32(y * e), 其中 e = 2^32 - b*y in (-537, b)
    vmul(t, b, y, mask, modeValue);
    vsub(e, zeroVec, t, mask, modeValue);
    MullU32SoftImpl(t, q, y, e, mask);
    // y 恒为正: e 为负时无符号乘的高 32bit 多计了一个 y, 减回即得有符号高位
    vcmp_lt(cmpMask, (RegTensor<int32_t>&)e, (RegTensor<int32_t>&)zeroVec, mask);
    vsub(q, q, y, cmpMask, mergingValue);
    vadd(y, y, q, mask, modeValue);
    // 向下偏置 1, 保证商只偏小不偏大, 余数修正只需无符号比较
    vsub(y, y, oneVec, mask, modeValue);
    // q = high32(a * y''), 余数 r = a - q*b in [rho, rho + 3b), b < 2^30 时不会回卷
    MullU32SoftImpl(t, q, a, y, mask);
    vmul(t, q, b, mask, modeValue);
    vsub(r, a, t, mask, modeValue);
    // 三轮向上修正: r >= b 时 q++, r -= b
    vcmp_ge(cmpMask, r, b, mask);
    vsub(r, r, b, cmpMask, mergingValue);
    vadd(q, q, oneVec, cmpMask, mergingValue);
    vcmp_ge(cmpMask, r, b, mask);
    vsub(r, r, b, cmpMask, mergingValue);
    vadd(q, q, oneVec, cmpMask, mergingValue);
    vcmp_ge(cmpMask, r, b, mask);
    vsub(r, r, b, cmpMask, mergingValue);
    vadd(q, q, oneVec, cmpMask, mergingValue);
    // 合并特殊情况后按掩码写回: 未选中 lane 取 zeroVec 清零, 与硬件 vdiv ZEROING 行为一致
    // (带掩码 vmov 仅支持 MERGING, ZEROING 语义用 vsel 实现)
    vsel(q, qBig, q, bigMask);
    vsel(q, qSmall, q, smallMask);
    vsel(q, qZero, q, zeroMask);
    vsel((RegTensor<uint32_t>&)dstReg, q, zeroVec, mask);
}

// s32 除法: 取绝对值后走 u32 路径, 最后按符号取负; 向零取整与 C 语义一致
template <typename U>
__simd_callee__ inline void DivS32SoftImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    RegTensor<uint32_t> zeroVec, absSrc0, absSrc1, q, negQ, qZero;
    MaskReg neg0Mask, neg1Mask, qNegMask, zeroMask;
    vbr(zeroVec, static_cast<uint32_t>(0));
    vcmp_lt(neg0Mask, (RegTensor<int32_t>&)srcReg0, (RegTensor<int32_t>&)zeroVec, mask);
    vsub(absSrc0, zeroVec, (RegTensor<uint32_t>&)srcReg0, mask, modeValue);
    vsel(absSrc0, absSrc0, (RegTensor<uint32_t>&)srcReg0, neg0Mask);
    vcmp_lt(neg1Mask, (RegTensor<int32_t>&)srcReg1, (RegTensor<int32_t>&)zeroVec, mask);
    vsub(absSrc1, zeroVec, (RegTensor<uint32_t>&)srcReg1, mask, modeValue);
    vsel(absSrc1, absSrc1, (RegTensor<uint32_t>&)srcReg1, neg1Mask);
    pxor(qNegMask, neg0Mask, neg1Mask, mask);
    DivU32SoftImpl(q, absSrc0, absSrc1, mask);
    // 符号不同则商取负; INT32_MIN/-1 溢出按回绕得到 INT32_MIN
    vsub(negQ, zeroVec, q, mask, modeValue);
    vsel(q, negQ, q, qNegMask);
    // 除 0 结果(全 1)取负后会变成 1, 需要重新选回全 1
    vbr(qZero, static_cast<uint32_t>(0xFFFFFFFFU));
    vcmps_eq(zeroMask, (RegTensor<uint32_t>&)srcReg1, static_cast<uint32_t>(0), mask);
    vsel(q, qZero, q, zeroMask);
    vsel((RegTensor<int32_t>&)dstReg, (RegTensor<int32_t>&)q, (RegTensor<int32_t>&)zeroVec, mask);
}

// u16/s16 除法: 扩位到 32bit(分 Part 两轮)后操作数均 < 2^24, f32 除法 + 截断即精确商
// 掩码通过 DivAdjustMask* 在 b16/b32 粒度间换算, 语义与 Cast 框架的 AdjustMask 保持一致
template <typename U>
__simd_callee__ inline void DivU16SoftImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(SupportType<ActualT, uint16_t>(), "DivU16SoftImpl only supports uint16_t!");
    constexpr auto zeroingValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    constexpr auto mergingValue = GetMaskMergeMode<MaskMergeMode::MERGING>();
    constexpr auto satModeValue =
        std::integral_constant<::RoundingSaturation, static_cast<::RoundingSaturation>(SatMode::NO_SAT)>();
    constexpr auto partZeroValue = std::integral_constant<::Part, static_cast<::Part>(RegLayout::ZERO)>();
    constexpr auto partOneValue = std::integral_constant<::Part, static_cast<::Part>(RegLayout::ONE)>();
    MaskReg maskPart0, maskPart1, mask16Part0, mask16Part1;
    DivAdjustMaskB16ToB32<RegLayout::ZERO>(maskPart0, mask);
    DivAdjustMaskB16ToB32<RegLayout::ONE>(maskPart1, mask);
    DivAdjustMaskB32ToB16<RegLayout::ZERO>(mask16Part0, maskPart0);
    DivAdjustMaskB32ToB16<RegLayout::ONE>(mask16Part1, maskPart1);
    RegTensor<uint32_t> src0Part0, src1Part0, src0Part1, src1Part1, qPart0, qPart1;
    RegTensor<uint16_t> tmpPart0, tmpPart1, zero16;
    vcvt(src0Part0, srcReg0, mask, partZeroValue, zeroingValue);
    vcvt(src1Part0, srcReg1, mask, partZeroValue, zeroingValue);
    vcvt(src0Part1, srcReg0, mask, partOneValue, zeroingValue);
    vcvt(src1Part1, srcReg1, mask, partOneValue, zeroingValue);
    Div32ExactViaF32Impl(qPart0, src0Part0, src1Part0, maskPart0);
    Div32ExactViaF32Impl(qPart1, src0Part1, src1Part1, maskPart1);
    // NO_SAT 截断写回: 除 0 结果(全 1)截断后仍为全 1
    vcvt(tmpPart0, (RegTensor<int32_t>&)qPart0, maskPart0, satModeValue, partZeroValue, zeroingValue);
    vcvt(tmpPart1, (RegTensor<int32_t>&)qPart1, maskPart1, satModeValue, partOneValue, zeroingValue);
    // part0 用 vsel 写回(未选中 lane 清零, 等效 ZEROING), part1 用 MERGING vmov 保留 part0 结果
    vbr(zero16, static_cast<uint16_t>(0));
    vsel((RegTensor<uint16_t>&)dstReg, (RegTensor<uint16_t>&)tmpPart0, zero16, mask16Part0);
    vmov((RegTensor<uint16_t>&)dstReg, (RegTensor<uint16_t>&)tmpPart1, mask16Part1, mergingValue);
}

template <typename U>
__simd_callee__ inline void DivS16SoftImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(SupportType<ActualT, int16_t>(), "DivS16SoftImpl only supports int16_t!");
    constexpr auto zeroingValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    constexpr auto mergingValue = GetMaskMergeMode<MaskMergeMode::MERGING>();
    constexpr auto satModeValue =
        std::integral_constant<::RoundingSaturation, static_cast<::RoundingSaturation>(SatMode::NO_SAT)>();
    constexpr auto partZeroValue = std::integral_constant<::Part, static_cast<::Part>(RegLayout::ZERO)>();
    constexpr auto partOneValue = std::integral_constant<::Part, static_cast<::Part>(RegLayout::ONE)>();
    MaskReg maskPart0, maskPart1, mask16Part0, mask16Part1;
    DivAdjustMaskB16ToB32<RegLayout::ZERO>(maskPart0, mask);
    DivAdjustMaskB16ToB32<RegLayout::ONE>(maskPart1, mask);
    DivAdjustMaskB32ToB16<RegLayout::ZERO>(mask16Part0, maskPart0);
    DivAdjustMaskB32ToB16<RegLayout::ONE>(mask16Part1, maskPart1);
    RegTensor<int32_t> src0Part0, src1Part0, src0Part1, src1Part1, qPart0, qPart1;
    RegTensor<int16_t> tmpPart0, tmpPart1;
    RegTensor<uint16_t> zero16;
    vcvt(src0Part0, srcReg0, mask, partZeroValue, zeroingValue);
    vcvt(src1Part0, srcReg1, mask, partZeroValue, zeroingValue);
    vcvt(src0Part1, srcReg0, mask, partOneValue, zeroingValue);
    vcvt(src1Part1, srcReg1, mask, partOneValue, zeroingValue);
    Div32ExactViaF32Impl(qPart0, src0Part0, src1Part0, maskPart0);
    Div32ExactViaF32Impl(qPart1, src0Part1, src1Part1, maskPart1);
    // NO_SAT 截断写回: INT16_MIN/-1 = 32768 回绕为 INT16_MIN, 除 0 结果(全 1)截断后为 -1
    vcvt(tmpPart0, qPart0, maskPart0, satModeValue, partZeroValue, zeroingValue);
    vcvt(tmpPart1, qPart1, maskPart1, satModeValue, partOneValue, zeroingValue);
    vbr(zero16, static_cast<uint16_t>(0));
    vsel((RegTensor<uint16_t>&)dstReg, (RegTensor<uint16_t>&)tmpPart0, zero16, mask16Part0);
    vmov((RegTensor<uint16_t>&)dstReg, (RegTensor<uint16_t>&)tmpPart1, mask16Part1, mergingValue);
}

template <typename U>
__simd_callee__ inline void DivIntSoftImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(
        SupportType<ActualT, uint16_t, int16_t, uint32_t, int32_t>(), "DivIntSoftImpl only supports u16/s16/u32/s32!");
    static_assert(CheckRegTrait<U, RegTraitNumOne>(), "DivIntSoftImpl only supports RegTraitNumOne!");
    if constexpr (SupportType<ActualT, uint32_t>()) {
        DivU32SoftImpl(dstReg, srcReg0, srcReg1, mask);
    } else if constexpr (SupportType<ActualT, int32_t>()) {
        DivS32SoftImpl(dstReg, srcReg0, srcReg1, mask);
    } else if constexpr (SupportType<ActualT, uint16_t>()) {
        DivU16SoftImpl(dstReg, srcReg0, srcReg1, mask);
    } else {
        DivS16SoftImpl(dstReg, srcReg0, srcReg1, mask);
    }
}

template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void ComplexDivKernel(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "U should be RegTraitNumTwo");
    static_assert(
        SupportType<ActualT, complex32, complex64>(), "current data type is not supported on current device!");

    RegTensor<typename ActualT::EleType>& src0Real = (RegTensor<typename ActualT::EleType>&)srcReg0.reg[0];
    RegTensor<typename ActualT::EleType>& src0Imag = (RegTensor<typename ActualT::EleType>&)srcReg0.reg[1];
    RegTensor<typename ActualT::EleType>& src1Real = (RegTensor<typename ActualT::EleType>&)srcReg1.reg[0];
    RegTensor<typename ActualT::EleType>& src1Imag = (RegTensor<typename ActualT::EleType>&)srcReg1.reg[1];
    RegTensor<typename ActualT::EleType>& dstReal = (RegTensor<typename ActualT::EleType>&)dstReg.reg[0];
    RegTensor<typename ActualT::EleType>& dstImag = (RegTensor<typename ActualT::EleType>&)dstReg.reg[1];
    RegTensor<typename ActualT::EleType> absSrc1Real;
    RegTensor<typename ActualT::EleType> absSrc1Imag;
    RegTensor<typename ActualT::EleType> dstRealTmp;
    RegTensor<typename ActualT::EleType> dstImagTmp;
    // abs_c
    Abs(absSrc1Real, src1Real, mask);
    // abs_d
    Abs(absSrc1Imag, src1Imag, mask);
    RegTensor<typename ActualT::EleType> one;
    RegTensor<typename ActualT::EleType> zero;
    Reg::Duplicate<typename ActualT::EleType, typename ActualT::EleType, RegTensor<typename ActualT::EleType>>(
        one, 1.0f);
    Reg::Duplicate<typename ActualT::EleType, typename ActualT::EleType, RegTensor<typename ActualT::EleType>>(
        zero, 0.0f);
    MaskReg maskFull = Reg::CreateMask<typename ActualT::EleType, Reg::MaskPattern::ALL>();

    // condition : if abs_c >= abs_d
    MaskReg compareAbsCDMask;
    Compare<typename ActualT::EleType, CMPMODE::GE, RegTensor<typename ActualT::EleType>>(
        compareAbsCDMask, absSrc1Real, absSrc1Imag, mask);
    // condition : if abs_c < abs_d
    MaskReg compareAbsCDMaskNot;
    MaskNot(compareAbsCDMaskNot, compareAbsCDMask, maskFull);
    // condition: abs_c == 0 && abs_d == 0
    MaskReg equalZero;
    Compare<typename ActualT::EleType, CMPMODE::EQ, RegTensor<typename ActualT::EleType>>(
        equalZero, absSrc1Real, zero, compareAbsCDMask);
    Compare<typename ActualT::EleType, CMPMODE::EQ, RegTensor<typename ActualT::EleType>>(
        equalZero, absSrc1Imag, zero, equalZero);
    RegTensor<typename ActualT::EleType> dstRealTmp0;
    RegTensor<typename ActualT::EleType> dstImagTmp0;
    Div<typename ActualT::EleType, mode, RegTensor<typename ActualT::EleType>>(
        dstRealTmp0, src0Real, absSrc1Real, equalZero);
    Select(dstRealTmp, dstRealTmp0, zero, equalZero);
    Div<typename ActualT::EleType, mode, RegTensor<typename ActualT::EleType>>(
        dstImagTmp0, src0Imag, absSrc1Imag, equalZero);
    Select(dstImagTmp, dstImagTmp0, zero, equalZero);

    // abs_c >= abs_d && !(abs_c == 0 && abs_d == 0)
    MaskNot(compareAbsCDMask, equalZero, compareAbsCDMask);
    RegTensor<typename ActualT::EleType> rat;
    // formula: rat = d / c
    Div<typename ActualT::EleType, mode, RegTensor<typename ActualT::EleType>>(
        rat, src1Imag, src1Real, compareAbsCDMask);
    RegTensor<typename ActualT::EleType> dMulRat;
    // formula: d*rat
    Mul(dMulRat, src1Imag, rat, compareAbsCDMask);
    // formula: c + d*rat
    Add(dMulRat, src1Real, dMulRat, compareAbsCDMask);
    // formula: scl = 1/(c + d*rat)
    Div<typename ActualT::EleType, mode, RegTensor<typename ActualT::EleType>>(dMulRat, one, dMulRat, compareAbsCDMask);
    RegTensor<typename ActualT::EleType> bMulRat;
    // formula: b * rat
    Mul(bMulRat, src0Imag, rat, compareAbsCDMask);
    // formula: a + b*rat
    Add(bMulRat, src0Real, bMulRat, compareAbsCDMask);
    // formula: (a + b*rat)*scl
    Mul<typename ActualT::EleType, MaskMergeMode::ZEROING, RegTensor<typename ActualT::EleType>>(
        dstRealTmp0, bMulRat, dMulRat, compareAbsCDMask);
    Select(dstRealTmp, dstRealTmp0, dstRealTmp, compareAbsCDMask);
    RegTensor<typename ActualT::EleType> aMulRat;
    // formula: a * rat
    Mul(aMulRat, src0Real, rat, compareAbsCDMask);
    // formula: b - a * rat
    Sub(aMulRat, src0Imag, aMulRat, compareAbsCDMask);
    // formula: (b - a * rat)*scl
    Mul<typename ActualT::EleType, MaskMergeMode::ZEROING, RegTensor<typename ActualT::EleType>>(
        dstImagTmp0, aMulRat, dMulRat, compareAbsCDMask);
    Select(dstImagTmp, dstImagTmp0, dstImagTmp, compareAbsCDMask);

    // formula: rat = c/d
    Div<typename ActualT::EleType, mode, RegTensor<typename ActualT::EleType>>(
        rat, src1Real, src1Imag, compareAbsCDMaskNot);
    // formula: c*rat
    Mul(dMulRat, src1Real, rat, compareAbsCDMaskNot);
    // formula: d+c*rat
    Add(dMulRat, src1Imag, dMulRat, compareAbsCDMaskNot);
    // formula: scl = 1/(d+c*rat)
    Div<typename ActualT::EleType, mode, RegTensor<typename ActualT::EleType>>(
        dMulRat, one, dMulRat, compareAbsCDMaskNot);
    // formula: a*rat
    Mul(aMulRat, src0Real, rat, compareAbsCDMaskNot);
    // formula: a*rat + b
    Add(aMulRat, aMulRat, src0Imag, compareAbsCDMaskNot);
    // formula: (a*rat + b)*scl
    Mul<typename ActualT::EleType, MaskMergeMode::ZEROING, RegTensor<typename ActualT::EleType>>(
        dstRealTmp0, aMulRat, dMulRat, compareAbsCDMaskNot);
    Select(dstRealTmp, dstRealTmp0, dstRealTmp, compareAbsCDMaskNot);
    // formula: b*rat
    Mul(bMulRat, src0Imag, rat, compareAbsCDMaskNot);
    // formula: b*rat - a
    Sub(bMulRat, bMulRat, src0Real, compareAbsCDMaskNot);
    // formula: (b*rat - a) * scl
    Mul<typename ActualT::EleType, MaskMergeMode::ZEROING, RegTensor<typename ActualT::EleType>>(
        dstImagTmp0, bMulRat, dMulRat, compareAbsCDMaskNot);

    Select(dstImagTmp, dstImagTmp0, dstImagTmp, compareAbsCDMaskNot);
    dstReal = dstRealTmp;
    dstImag = dstImagTmp;
}

template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void DivComplex64Impl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 8, "data type should be B64");
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "U should be RegTraitNumTwo");
    ComplexDivKernel<T, mode, Reg::RegTensor<ActualT, Reg::RegTraitNumTwo>>(dstReg, srcReg0, srcReg1, mask);
}

template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void DivB64Impl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 8, "DivB64Impl data type should be B64");
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "DivB64Impl U should be RegTraitNumTwo");
    if constexpr (SupportType<ActualT, complex64>()) {
        DivComplex64Impl<T, mode, U>(dstReg, srcReg0, srcReg1, mask);
    } else {
        if constexpr (Std::is_same_v<ActualT, uint64_t>) {
            U dstTemp;
            DivU64Impl(dstTemp, srcReg0, srcReg1, mask);
            dstReg = dstTemp;
        } else if constexpr (Std::is_same_v<ActualT, int64_t>) {
            U dstTemp;
            DivS64Impl(dstTemp, srcReg0, srcReg1, mask);
            dstReg = dstTemp;
        }
    }
}

template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void MergeTwoFloatELementRegs(U& dst, U& src0, U& src1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(CheckRegTrait<U, RegTraitNumOne>(), "U should be RegTraitNumOne");
    static_assert(SupportType<ActualT, float, half>(), "current data type is not supported on current device!");
    if constexpr (SupportType<ActualT, float>()) {
        Or((RegTensor<uint32_t>&)dst, (RegTensor<uint32_t>&)src0, (RegTensor<uint32_t>&)src1, mask);
    } else if constexpr (SupportType<ActualT, half>()) {
        Or((RegTensor<uint16_t>&)dst, (RegTensor<uint16_t>&)src0, (RegTensor<uint16_t>&)src1, mask);
    }
}

template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void DivComplex32TwoImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 4, "data type should be B32");
    static_assert(CheckRegTrait<U, RegTraitNumTwo>(), "U should be RegTraitNumTwo");
    ComplexDivKernel<T, mode, Reg::RegTensor<ActualT, Reg::RegTraitNumTwo>>(dstReg, srcReg0, srcReg1, mask);
}

template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void DivComplex32OnetraitImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(sizeof(ActualT) == 4, "data type should be B32");
    static_assert(CheckRegTrait<U, RegTraitNumOne>(), "U should be RegTraitNumOne");
    MaskReg maskTrait2;
    MaskPack(maskTrait2, mask);
    RegTensor<ActualT, RegTraitNumTwo> divTraitTwoSrcReg0;
    RegTensor<ActualT, RegTraitNumTwo> divTraitTwoSrcReg1;
    RegTensor<ActualT, RegTraitNumTwo> divTraitTwoDstReg;

    B32TraitOneToTraitTwo(divTraitTwoSrcReg0, srcReg0);
    B32TraitOneToTraitTwo(divTraitTwoSrcReg1, srcReg1);
    DivComplex32TwoImpl(divTraitTwoDstReg, divTraitTwoSrcReg0, divTraitTwoSrcReg1, maskTrait2);
    B32TraitTwoToTraitOne(dstReg, divTraitTwoDstReg);
}

template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void DivPrecisionImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    // Improves Reg with high precision mode by using error-complemention approach with following formula.
    //
    // sign = vand(a, -0.0)
    // sign_one = vor(1.0, sign)

    // inf_nan_tmp = vor(x1, neg_zero)
    // inf_nan_temp_uint32 = 0xFF800000
    // inf_nan_cmp_mask = vcmp(inf_nan_temp_uint32, neg_inf_uint32, "ge", "bit")

    // x1 = div(a, b)
    // temp_b = -1.0*b

    // a = fma(a, temp_b, x1)  // a = a + temp_b*x1, high precision fma instr is required.
    // temp_mask = vcmp(a, a, "eq", "bit")
    // temp_a = vsel(temp_mask, a, one_temp)
    // e = div(temp_a, b)

    // x = x1 + e
    // x = vsel(inf_nan_cmp_mask, x1, x)
    constexpr DivSpecificMode sprMode = Internal::GetDivSpecificMode(mode);
    constexpr auto modeValue = GetMaskMergeMode<sprMode.mrgMode>();
    constexpr uint32_t infNanBound = 0xff800000u;
    constexpr uint32_t signBitNum = 0x80000000u;
    RegTensor<T> regNegZero;
    RegTensor<T> tmpDst;
    RegTensor<T> r, z, y;
    RegTensor<uint32_t> infNan;

    MaskReg cmpMaskReg;
    MaskReg infNanCmp;
    MaskReg zeroCmp;
    MaskReg preg;
    preg = pset_b8(PAT_ALL);
    vdup((vector_u32&)regNegZero, signBitNum, preg, modeValue);
    vdiv(z, srcReg0, srcReg1, mask, modeValue);
    vor(infNan, (vector_u32&)z, (vector_u32&)regNegZero, mask, modeValue);
    tmpDst = z;
    vcmps_eq(zeroCmp, z, 0.0f, mask);
    vcmps_ge(infNanCmp, infNan, infNanBound, mask);
    por(infNanCmp, infNanCmp, zeroCmp, mask);

    vmuls(y, srcReg1, -1.0f, mask, modeValue);
    r = srcReg0;
    vmula(r, z, y, mask, modeValue);
    RegTensor<T> rPre, rNext, zPre, zNext;

    vadds((vector_s32&)zPre, (vector_s32&)z, -1, mask, modeValue);
    vadds((vector_s32&)zNext, (vector_s32&)z, 1, mask, modeValue);

    rPre = srcReg0;
    rNext = srcReg0;

    vmula(rPre, zPre, y, mask, modeValue);
    vmula(rNext, zNext, y, mask, modeValue);

    vabs(r, r, mask, modeValue);
    vabs(rPre, rPre, mask, modeValue);
    vabs(rNext, rNext, mask, modeValue);
    vcmp_lt(cmpMaskReg, r, rPre, mask);
    vsel(r, r, rPre, cmpMaskReg);
    vsel(z, z, zPre, cmpMaskReg);

    vcmp_lt(cmpMaskReg, rNext, r, mask);
    vsel(z, zNext, z, cmpMaskReg);
    vsel(dstReg, tmpDst, z, infNanCmp);
}

template <auto mode = MaskMergeMode::ZEROING>
__simd_callee__ inline void DivF32StickyDir(
    MaskReg& stickyPositive, MaskReg& stickyZero, RegTensor<float>& q, RegTensor<float>& src0Norm,
    RegTensor<float>& src1Norm, MaskReg& mask)
{
    constexpr DivSpecificMode sprMode = Internal::GetDivSpecificMode(mode);
    constexpr auto modeValue = GetMaskMergeMode<sprMode.mrgMode>();
    constexpr uint32_t signExtractor = 0x80000000u;

    RegTensor<float> negSrc1;
    RegTensor<float> residual;
    RegTensor<uint32_t> signMask;
    RegTensor<uint32_t> src0Sign;
    RegTensor<uint32_t> zeroBits;
    MaskReg residualGtZero;
    MaskReg residualLtZero;
    MaskReg src0Positive;
    MaskReg src0Negative;
    MaskReg positiveByPositiveSrc;
    MaskReg positiveByNegativeSrc;

    vmuls(negSrc1, src1Norm, -1.0f, mask, modeValue);
    residual = src0Norm;
    vmula(residual, q, negSrc1, mask, modeValue);

    Compares<float, CMPMODE::GT>(residualGtZero, residual, 0.0f, mask);
    Compares<float, CMPMODE::LT>(residualLtZero, residual, 0.0f, mask);
    Compares<float, CMPMODE::EQ>(stickyZero, residual, 0.0f, mask);

    Duplicate(signMask, signExtractor, mask);
    And(src0Sign, (RegTensor<uint32_t>&)src0Norm, signMask, mask);
    Duplicate(zeroBits, static_cast<uint32_t>(0), mask);
    Compare<uint32_t, CMPMODE::EQ>(src0Positive, src0Sign, zeroBits, mask);
    MaskNot(src0Negative, src0Positive, mask);

    MaskAnd(positiveByPositiveSrc, residualGtZero, src0Positive, mask);
    MaskAnd(positiveByNegativeSrc, residualLtZero, src0Negative, mask);
    MaskOr(stickyPositive, positiveByPositiveSrc, positiveByNegativeSrc, mask);
}

template <bool useSticky>
__simd_callee__ inline void DivF32ScaleRound(
    RegTensor<float>& dst, RegTensor<int32_t>& scale, MaskReg& stickyPositive, MaskReg& stickyZero, MaskReg& mask)
{
    constexpr uint32_t signExtractor = 0x80000000u;
    constexpr uint32_t exponentExtractor = 0x7F800000u;
    constexpr uint32_t mantissaMask = 0x007FFFFFu;
    constexpr uint32_t implicitBit = 0x00800000u;

    RegTensor<uint32_t> qSign;
    RegTensor<uint32_t> qExponent;
    RegTensor<uint32_t> qMantissa;
    RegTensor<uint32_t> m24;
    RegTensor<uint32_t> resultBits;
    RegTensor<uint32_t> tmpU0;
    RegTensor<uint32_t> tmpU1;
    RegTensor<uint32_t> tmpU2;
    RegTensor<uint32_t> oneU;
    RegTensor<uint32_t> zeroU;
    RegTensor<int32_t> e;
    RegTensor<int32_t> drop;
    RegTensor<int32_t> dropMinusOne;
    RegTensor<int32_t> tmpI0;
    RegTensor<int32_t> tmpI1;

    MaskReg overflowMask;
    MaskReg notOverflowMask;
    MaskReg normalMask;
    MaskReg subnormalMask;
    MaskReg zeroMask;
    MaskReg subnormalRoundMask;
    MaskReg remGtHalfMask;
    MaskReg remEqHalfMask;
    MaskReg keptOddMask;
    MaskReg tieUpMask;
    MaskReg roundUpMask;

    Duplicate(tmpU0, signExtractor, mask);
    And(qSign, (RegTensor<uint32_t>&)dst, tmpU0, mask);

    Duplicate(tmpU0, exponentExtractor, mask);
    And(qExponent, (RegTensor<uint32_t>&)dst, tmpU0, mask);
    ShiftRights(qExponent, qExponent, (int16_t)23, mask);
    Duplicate(tmpI0, 127, mask);
    Sub(e, (RegTensor<int32_t>&)qExponent, tmpI0, mask);
    Add(e, e, scale, mask);

    Duplicate(tmpU0, mantissaMask, mask);
    And(qMantissa, (RegTensor<uint32_t>&)dst, tmpU0, mask);
    Duplicate(tmpU0, implicitBit, mask);
    Or(m24, qMantissa, tmpU0, mask);

    Duplicate(tmpI0, 127, mask);
    Compare<int32_t, CMPMODE::GT>(overflowMask, e, tmpI0, mask);
    Duplicate(tmpU0, F32_INF, overflowMask);
    Or(resultBits, qSign, tmpU0, overflowMask);
    Select((RegTensor<uint32_t>&)dst, resultBits, (RegTensor<uint32_t>&)dst, overflowMask);

    MaskNot(notOverflowMask, overflowMask, mask);
    Duplicate(tmpI0, -126, notOverflowMask);
    Compare<int32_t, CMPMODE::GE>(normalMask, e, tmpI0, notOverflowMask);
    Adds(tmpI1, e, 127, normalMask);
    ShiftLefts(tmpU0, (RegTensor<uint32_t>&)tmpI1, (int16_t)23, normalMask);
    Or(resultBits, qSign, tmpU0, normalMask);
    Or(resultBits, resultBits, qMantissa, normalMask);
    Select((RegTensor<uint32_t>&)dst, resultBits, (RegTensor<uint32_t>&)dst, normalMask);

    MaskNot(subnormalMask, normalMask, notOverflowMask);
    Duplicate(tmpI0, -126, subnormalMask);
    Sub(drop, tmpI0, e, subnormalMask);
    Duplicate(tmpI0, 24, subnormalMask);
    Compare<int32_t, CMPMODE::GT>(zeroMask, drop, tmpI0, subnormalMask);
    Select((RegTensor<uint32_t>&)dst, qSign, (RegTensor<uint32_t>&)dst, zeroMask);

    MaskNot(subnormalRoundMask, zeroMask, subnormalMask);
    Duplicate(oneU, static_cast<uint32_t>(1), subnormalRoundMask);
    Duplicate(zeroU, static_cast<uint32_t>(0), subnormalRoundMask);
    ShiftRight(tmpU0, m24, drop, subnormalRoundMask); // kept
    ShiftLeft(tmpU1, oneU, drop, subnormalRoundMask);
    Sub(tmpU1, tmpU1, oneU, subnormalRoundMask); // rem mask
    And(tmpU2, m24, tmpU1, subnormalRoundMask);  // rem
    Duplicate(tmpI0, 1, subnormalRoundMask);
    Sub(dropMinusOne, drop, tmpI0, subnormalRoundMask);
    ShiftLeft(tmpU1, oneU, dropMinusOne, subnormalRoundMask); // half

    Compare<uint32_t, CMPMODE::GT>(remGtHalfMask, tmpU2, tmpU1, subnormalRoundMask);
    Compare<uint32_t, CMPMODE::EQ>(remEqHalfMask, tmpU2, tmpU1, subnormalRoundMask);
    And(tmpU2, tmpU0, oneU, subnormalRoundMask);
    Compare<uint32_t, CMPMODE::NE>(keptOddMask, tmpU2, zeroU, subnormalRoundMask);

    if constexpr (useSticky) {
        MaskReg stickyZeroOddMask;
        MaskAnd(stickyZeroOddMask, stickyZero, keptOddMask, subnormalRoundMask);
        MaskOr(tieUpMask, stickyPositive, stickyZeroOddMask, subnormalRoundMask);
        MaskAnd(tieUpMask, tieUpMask, remEqHalfMask, subnormalRoundMask);
    } else {
        MaskAnd(tieUpMask, remEqHalfMask, keptOddMask, subnormalRoundMask);
    }
    MaskOr(roundUpMask, remGtHalfMask, tieUpMask, subnormalRoundMask);
    Add(tmpU2, tmpU0, oneU, roundUpMask);
    Select(tmpU0, tmpU2, tmpU0, roundUpMask);

    Or(resultBits, qSign, tmpU0, subnormalRoundMask);
    Select((RegTensor<uint32_t>&)dst, resultBits, (RegTensor<uint32_t>&)dst, subnormalRoundMask);
}

template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U, bool is0ULP>
__simd_callee__ inline void DivIEEE754FloatImpl(
    RegTensor<float>& dst, RegTensor<float>& src0, RegTensor<float>& src1, MaskReg& mask)
{
    constexpr uint32_t exponentExtractor = 0x807FFFFF;
    constexpr uint32_t exponentNormalizer = 0x3F800000;

    NotNumUnion subnormalThreshold;
    subnormalThreshold.i = 0x00800000; // min normal

    NotNumUnion nan;
    nan.i = F32_NAN;

    NotNumUnion normalizeScaleEnlarge;
    normalizeScaleEnlarge.i = 0x4B000000; // 2^23

    Reg::RegTensor<float> minNormal;
    Reg::RegTensor<uint32_t> tmp0;
    Reg::RegTensor<int32_t> tmp1;

    Reg::RegTensor<float> src0Abs;
    Reg::RegTensor<float> src0Subnormal;
    Reg::RegTensor<float> src0Norm;
    Reg::RegTensor<float> src0All;

    Reg::RegTensor<float> src1Abs;
    Reg::RegTensor<float> src1Subnormal;
    Reg::RegTensor<float> src1Norm;
    Reg::RegTensor<float> src1All;

    MaskReg maskSrc0Normal;
    MaskReg maskSrc0Subnormal;
    MaskReg maskSrc1Normal;
    MaskReg maskSrc1Subnormal;
    MaskReg maskTmp;
    MaskReg maskNan;      // divisor or dividend 0
    MaskReg maskInf;      // divisor or dividend inf
    MaskReg maskSrc0Zero; // dividend 0
    MaskReg maskSrc1Zero; // divisor 0
    MaskReg maskValid;
    MaskReg stickyPositive;
    MaskReg stickyZero;

    RegTensor<uint32_t> src0Exponent;
    RegTensor<uint32_t> src1Exponent;

    RegTensor<int32_t> scale;

    // subnormal threshold
    Duplicate(minNormal, subnormalThreshold.f, mask);

    // ===========================================================
    // acquiring valid numbers (no inf, no 0)
    // ===========================================================
    Abs(src0Abs, src0, mask);
    Abs(src1Abs, src1, mask);

    // get positions of inf values
    Duplicate(tmp0, F32_INF, mask);
    Compare<uint32_t, CMPMODE::EQ>(maskInf, (RegTensor<uint32_t>&)src0Abs, tmp0, mask);
    Compare<uint32_t, CMPMODE::EQ>(maskTmp, (RegTensor<uint32_t>&)src1Abs, tmp0, mask);
    MaskOr(maskValid, maskInf, maskTmp, mask);
    // get positions of 0 divisor or dividend
    Duplicate(tmp0, 0, mask);
    Compare<uint32_t, CMPMODE::EQ>(maskSrc0Zero, (RegTensor<uint32_t>&)src0Abs, tmp0, mask);
    // merge for positions of invalid numbers
    MaskOr(maskValid, maskValid, maskSrc0Zero, mask);
    Compare<uint32_t, CMPMODE::EQ>(maskSrc1Zero, (RegTensor<uint32_t>&)src1Abs, tmp0, mask);
    // negating for positions of valid numbers
    MaskOr(maskValid, maskValid, maskSrc1Zero, mask);
    MaskNot(maskValid, maskValid, mask);

    // normalize subnormal elements of src0
    // get positions of subnormal numbers in dividend
    Compare<float, CMPMODE::LT>(maskSrc0Subnormal, src0Abs, minNormal, mask);
    // negating for normal positions
    MaskNot(maskSrc0Normal, maskSrc0Subnormal, mask);
    // normalizatoin
    Muls(src0Subnormal, src0, normalizeScaleEnlarge.f, maskSrc0Subnormal);

    // normalize subnormal elements of src1
    Compare<float, CMPMODE::LT>(maskSrc1Subnormal, src1Abs, minNormal, mask);
    MaskNot(maskSrc1Normal, maskSrc1Subnormal, mask);
    Muls(src1Subnormal, src1, normalizeScaleEnlarge.f, maskSrc1Subnormal);

    // merge the normalized subnormal elements with normal elements
    Select(src0All, src0, src0Subnormal, maskSrc0Normal);
    Select(src1All, src1, src1Subnormal, maskSrc1Normal);

    // standardized the exponent bits of src0 and src1
    // zero out the exponent bits 00000000
    Duplicate(tmp0, exponentExtractor, mask);
    And((RegTensor<uint32_t>&)src0Norm, (RegTensor<uint32_t>&)src0All, tmp0, maskValid);
    And((RegTensor<uint32_t>&)src1Norm, (RegTensor<uint32_t>&)src1All, tmp0, maskValid);
    // set the exponent bits to 01111111
    Duplicate(tmp0, exponentNormalizer, mask);
    Add((RegTensor<uint32_t>&)src0Norm, (RegTensor<uint32_t>&)src0Norm, tmp0, maskValid);
    Add((RegTensor<uint32_t>&)src1Norm, (RegTensor<uint32_t>&)src1Norm, tmp0, maskValid);
    Select(src0Norm, src0Norm, src0All, maskValid);
    Select(src1Norm, src1Norm, src1All, maskValid);

    if constexpr (is0ULP) {
        DivPrecisionImpl<float, mode, U>(dst, src0Norm, src1Norm, mask);
        DivF32StickyDir<mode>(stickyPositive, stickyZero, dst, src0Norm, src1Norm, maskValid);
    } else {
        constexpr DivSpecificMode sprMode = Internal::GetDivSpecificMode(MaskMergeMode::ZEROING);
        constexpr auto modeValue = GetMaskMergeMode<sprMode.mrgMode>();
        vdiv(dst, src0Norm, src1Norm, mask, modeValue);
    }

    // ===========================================================
    // exponent operation
    // ===========================================================
    // extract the exponent section 0 11..11 00..00
    Duplicate(tmp0, F32_INF, mask);
    And(src0Exponent, (RegTensor<uint32_t>&)src0All, tmp0, mask);
    And(src1Exponent, (RegTensor<uint32_t>&)src1All, tmp0, mask);

    // exponent subtraction (effectively fp number division)
    ShiftRights(src0Exponent, src0Exponent, (int16_t)23, mask);
    ShiftRights(src1Exponent, src1Exponent, (int16_t)23, mask);
    Sub(scale, (RegTensor<int32_t>&)src0Exponent, (RegTensor<int32_t>&)src1Exponent, mask);
    Adds(tmp1, scale, -23, maskSrc0Subnormal);
    Select(scale, tmp1, scale, maskSrc0Subnormal);
    Adds(tmp1, scale, 23, maskSrc1Subnormal);
    Select(scale, tmp1, scale, maskSrc1Subnormal);
    DivF32ScaleRound<is0ULP>(dst, scale, stickyPositive, stickyZero, maskValid);

    // get the position of nan
    Duplicate(tmp0, nan.i, mask);
    Compare<float, CMPMODE::NE>(maskNan, src0Abs, src0Abs, mask);
    Compare<float, CMPMODE::NE>(maskTmp, src1Abs, src1Abs, mask);
    MaskOr(maskNan, maskNan, maskTmp, mask);
    // set output with nan input to nan
    Select(dst, (RegTensor<float>&)tmp0, dst, maskNan);
}

template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void DivIEEE754HalfImpl(
    RegTensor<half>& dst, RegTensor<half>& src0, RegTensor<half>& src1, MaskReg& mask)
{
    constexpr uint16_t exponentExtractor = 0x83FF;
    constexpr uint16_t signExtractor = 0x8000;
    constexpr uint16_t exponentNormalizer = 0x3C00;
    constexpr uint16_t F16_INF = 0x7C00;

    HalfUnion subnormalThreshold;
    subnormalThreshold.i = 0x0400; // minimum positive normal

    HalfUnion nan;
    nan.i = 0x7E00;
    HalfUnion min_denormal;
    min_denormal.i = 0x1;

    HalfUnion normalizeScaleEnlarge;
    normalizeScaleEnlarge.i = 0x6400; // 2^10
    HalfUnion normalizeScaleReduce;
    normalizeScaleReduce.i = 0x1400; // 2^-10

    Reg::RegTensor<half> maxSubnormal;
    Reg::RegTensor<uint16_t> tmp0;
    Reg::RegTensor<int16_t> tmp1;
    Reg::RegTensor<uint16_t> tmp2;

    Reg::RegTensor<half> src0Abs;
    Reg::RegTensor<half> src0Subnormal;
    Reg::RegTensor<half> src0Norm;
    Reg::RegTensor<half> src0All;
    Reg::RegTensor<half> src0AbsNorm;

    Reg::RegTensor<half> src1Abs;
    Reg::RegTensor<half> src1Subnormal;
    Reg::RegTensor<half> src1Norm;
    Reg::RegTensor<half> src1All;
    Reg::RegTensor<half> src1AbsNorm;

    MaskReg mask0;
    MaskReg maskSrc0Normal;
    MaskReg maskSrc0Subnormal;
    MaskReg maskSrc1Normal;
    MaskReg maskSrc1Subnormal;
    MaskReg maskTmp;
    MaskReg maskNan;      // divisor or dividend 0
    MaskReg maskInf;      // divisor or dividend inf
    MaskReg maskSrc0Zero; // dividend 0
    MaskReg maskSrc1Zero; // divisor 0
    MaskReg maskValid;
    MaskReg maskNorm;

    RegTensor<uint16_t> src0Exponent;
    RegTensor<uint16_t> src1Exponent;

    RegTensor<half> z1;
    RegTensor<half> z2;
    RegTensor<int16_t> scale;
    RegTensor<uint16_t> dstExponent;
    RegTensor<uint16_t> dstSign;

    // subnormal threshold
    Duplicate(maxSubnormal, subnormalThreshold.f, mask);

    // ===========================================================
    // acquiring valid numbers (no inf, no 0)
    // ===========================================================
    Abs(src0Abs, src0, mask);
    Abs(src1Abs, src1, mask);

    // get positions of inf values
    Duplicate(tmp0, F16_INF, mask);
    Compare<uint16_t, CMPMODE::EQ>(maskInf, (RegTensor<uint16_t>&)src0Abs, tmp0, mask);
    Compare<uint16_t, CMPMODE::EQ>(maskTmp, (RegTensor<uint16_t>&)src1Abs, tmp0, mask);
    MaskOr(maskValid, maskInf, maskTmp, mask);
    // get positions of 0 divisor or dividend
    Duplicate(tmp0, 0, mask);
    Compare<uint16_t, CMPMODE::EQ>(maskSrc0Zero, (RegTensor<uint16_t>&)src0Abs, tmp0, mask);
    // merge for positions of invalid numbers
    MaskOr(maskValid, maskValid, maskSrc0Zero, mask);
    Compare<uint16_t, CMPMODE::EQ>(maskSrc1Zero, (RegTensor<uint16_t>&)src1Abs, tmp0, mask);
    // negating for positions of valid numbers
    MaskOr(maskValid, maskValid, maskSrc1Zero, mask);
    MaskNot(maskValid, maskValid, mask);

    // normalize subnormal elements of src0
    // get positions of subnormal numbers in dividend
    Compare<half, CMPMODE::LT>(maskSrc0Subnormal, src0Abs, maxSubnormal, mask);
    // negating for normal positions
    MaskNot(maskSrc0Normal, maskSrc0Subnormal, mask);
    // normalizatoin
    Muls(src0Subnormal, src0, normalizeScaleEnlarge.f, maskSrc0Subnormal);

    // normalize subnormal elements of src1
    Compare<half, CMPMODE::LT>(maskSrc1Subnormal, src1Abs, maxSubnormal, mask);
    MaskNot(maskSrc1Normal, maskSrc1Subnormal, mask);
    Muls(src1Subnormal, src1, normalizeScaleEnlarge.f, maskSrc1Subnormal);

    // merge the normalized subnormal elements with normal elements
    Select(src0All, src0, src0Subnormal, maskSrc0Normal);
    Select(src1All, src1, src1Subnormal, maskSrc1Normal);

    // standardized the exponent bits of src0 and src1
    // zero out the exponent bits 00000000
    Duplicate(tmp0, exponentExtractor, mask);
    And((RegTensor<uint16_t>&)src0Norm, (RegTensor<uint16_t>&)src0All, tmp0, maskValid);
    And((RegTensor<uint16_t>&)src1Norm, (RegTensor<uint16_t>&)src1All, tmp0, maskValid);
    // set the exponent bits to 01111111
    Duplicate(tmp0, exponentNormalizer, mask);
    Add((RegTensor<uint16_t>&)src0Norm, (RegTensor<uint16_t>&)src0Norm, tmp0, maskValid);
    Add((RegTensor<uint16_t>&)src1Norm, (RegTensor<uint16_t>&)src1Norm, tmp0, maskValid);
    Select(src0Norm, src0Norm, src0All, maskValid);
    Select(src1Norm, src1Norm, src1All, maskValid);
    Abs(src0AbsNorm, src0Norm, maskValid);
    Abs(src1AbsNorm, src1Norm, maskValid);
    Compare<half, CMPMODE::LE>(maskNorm, src0AbsNorm, src1AbsNorm, maskValid);

    constexpr DivSpecificMode sprMode = Internal::GetDivSpecificMode(MaskMergeMode::ZEROING);
    constexpr auto modeValue = GetMaskMergeMode<sprMode.mrgMode>();
    vdiv(dst, src0Norm, src1Norm, mask, modeValue);

    // subnormal dividend, normal divisor
    MaskAnd(mask0, maskSrc0Subnormal, maskSrc1Normal, mask);
    // normalization compensation
    Muls(z1, dst, normalizeScaleReduce.f, mask0);
    Select(dst, z1, dst, mask0);

    // normal dividend, subnormal divisor
    MaskAnd(mask0, maskSrc0Normal, maskSrc1Subnormal, mask);
    // normalization compensation
    Muls(z1, dst, normalizeScaleEnlarge.f, mask0);
    // merge the compensated result
    Select(dst, z1, dst, mask0);

    // preserve sign for error handling section below
    Duplicate(tmp0, signExtractor, mask);
    And((RegTensor<uint16_t>&)dstSign, (RegTensor<uint16_t>&)dst, tmp0, mask);

    // ===========================================================
    // exponent operation
    // ===========================================================
    // extract the exponent section 0 11..11 00..00
    Duplicate(tmp0, F16_INF, mask);
    And(src0Exponent, (RegTensor<uint16_t>&)src0All, tmp0, mask);
    And(src1Exponent, (RegTensor<uint16_t>&)src1All, tmp0, mask);
    And(dstExponent, (RegTensor<uint16_t>&)dst, tmp0, mask);

    // exponent subtraction (effectively fp number division)
    ShiftRights(src0Exponent, src0Exponent, (int16_t)10, mask);
    ShiftRights(src1Exponent, src1Exponent, (int16_t)10, mask);
    ShiftRights(dstExponent, dstExponent, (int16_t)10, mask);
    Sub(scale, (RegTensor<int16_t>&)src0Exponent, (RegTensor<int16_t>&)src1Exponent, mask);
    Adds(scale, scale, 15, mask);
    // ===========================================================
    // exception handling
    // ===========================================================
    // overflow (exponent over 31) underflow (exponent under -9) detection // FP16:1S + 5E + 9M
    Duplicate(tmp1, -9, mask);
    Compare<int16_t, CMPMODE::EQ>(mask0, scale, (RegTensor<int16_t>&)tmp1, mask);
    MaskAnd(mask0, mask0, maskValid, mask);
    Duplicate(tmp0, min_denormal.i, mask0);
    Add((RegTensor<uint16_t>&)z1, (RegTensor<uint16_t>&)dstSign, tmp0, mask0);
    Duplicate(tmp2, static_cast<uint16_t>(0), mask0);
    Add((RegTensor<uint16_t>&)z2, (RegTensor<uint16_t>&)dstSign, tmp2, mask0);
    Select(z1, z2, z1, maskNorm);
    Select(dst, z1, dst, mask0);
    MaskNot(mask0, mask0, mask);
    MaskAnd(maskValid, mask0, maskValid, mask);
    // True if underflow/overflow
    Compare<int16_t, CMPMODE::LT>(mask0, scale, (RegTensor<int16_t>&)tmp1, mask);
    // set overflown/underflown result to infinity
    MaskAnd(mask0, mask0, maskValid, mask);
    Duplicate(tmp0, 0, mask); // set to 0
    Add((RegTensor<uint16_t>&)z1, (RegTensor<uint16_t>&)dstSign, tmp0, mask0);
    Select(dst, z1, dst, mask0);
    MaskNot(mask0, mask0, mask);
    MaskAnd(maskValid, mask0, maskValid, mask);

    Duplicate(tmp0, 31, mask);
    Compare<int16_t, CMPMODE::EQ>(mask0, scale, (RegTensor<int16_t>&)tmp0, mask);
    MaskAnd(mask0, mask0, maskValid, mask);
    Reg::Duplicate(tmp1, 1, mask0);
    Reg::Sub(tmp1, scale, tmp1, mask0);
    Select(scale, tmp1, scale, mask0);
    Muls(z1, dst, 2, mask0);
    Select(dst, z1, dst, mask0);

    Compare<int16_t, CMPMODE::GT>(mask0, scale, (RegTensor<int16_t>&)tmp0, mask);
    MaskAnd(mask0, mask0, maskValid, mask);
    Duplicate(tmp0, F16_INF, mask); // set to infinity
    Add((RegTensor<uint16_t>&)z1, (RegTensor<uint16_t>&)dstSign, tmp0, mask0);
    Select(dst, z1, dst, mask0);
    MaskNot(mask0, mask0, mask);
    MaskAnd(maskValid, mask0, maskValid, mask);

    Duplicate(tmp0, 0, maskValid);
    Compare<int16_t, CMPMODE::GT>(mask0, scale, (RegTensor<int16_t>&)tmp0, maskValid);
    ShiftLefts(tmp1, scale, (int16_t)10, mask0);
    Mul(z1, dst, (RegTensor<half>&)tmp1, mask0);
    Select(dst, z1, dst, mask0);

    MaskNot(mask0, mask0, maskValid);
    Duplicate(tmp0, 512, mask0); // set 0x0200
    Abs(scale, scale, mask0);
    ShiftRight(scale, (RegTensor<int16_t>&)tmp0, scale, mask0);
    Mul(z1, dst, (RegTensor<half>&)scale, mask0);
    Select(dst, z1, dst, mask0);

    // get the position of nan
    Duplicate(tmp0, nan.i, mask);
    Compare<half, CMPMODE::NE>(maskNan, src0Abs, src0Abs, mask);
    Compare<half, CMPMODE::NE>(maskTmp, src1Abs, src1Abs, mask);
    MaskOr(maskNan, maskNan, maskTmp, mask);
    // set output with nan input to nan
    Select(dst, (RegTensor<half>&)tmp0, dst, maskNan);
}

template <typename T = DefaultType, auto mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void DivImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(
        IsSameType<decltype(mode), MaskMergeMode>::value || IsSameType<decltype(mode), const DivSpecificMode*>::value,
        "mode type must be either MaskMergeMode or const DivSpecificMode* ");
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    constexpr DivSpecificMode sprMode = Internal::GetDivSpecificMode(mode);
    static_assert(
        SupportType<
            ActualT, uint16_t, int16_t, uint32_t, int32_t, half, float, int64_t, uint64_t, complex32, complex64>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportEnum<sprMode.mrgMode, MaskMergeMode::ZEROING>(),
        "current Div api only supported Mode ZEROING on current device!");
    constexpr auto modeValue = GetMaskMergeMode<sprMode.mrgMode>();
    if constexpr (sprMode.precisionMode) {
        static_assert(
            SupportType<ActualT, float, complex64>(),
            "only float and complex64 data type is supported in precsion mode.");
    }
    if constexpr (sprMode.algo == DivAlgo::PRECISION_1ULP_FTZ_FALSE) {
        static_assert(SupportType<T, half, float>(), "Reg Div for PRECISION_1ULP_FTZ_FALSE only supports half/float.");
    } else if constexpr (sprMode.algo == DivAlgo::PRECISION_0ULP_FTZ_FALSE) {
        static_assert(SupportType<T, float>(), "Reg Div for PRECISION_0ULP_FTZ_FALSE only supports float.");
    } else if constexpr (sprMode.algo == DivAlgo::PRECISION_0ULP_FTZ_TRUE) {
        static_assert(
            SupportType<T, float, complex64>(), "Reg Div for PRECISION_0ULP_FTZ_TRUE only supports float/complex64.");
    }
    if constexpr (SupportType<ActualT, complex32>()) {
        if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            DivComplex32TwoImpl(dstReg, srcReg0, srcReg1, mask);
        } else {
            DivComplex32OnetraitImpl(dstReg, srcReg0, srcReg1, mask);
        }
    } else if constexpr (SupportBytes<ActualT, 8>()) {
        if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            U dstTemp;
            DivB64Impl<T, mode, Reg::RegTensor<ActualT, Reg::RegTraitNumTwo>>(dstTemp, srcReg0, srcReg1, mask);
            dstReg = dstTemp;
        } else {
            CalTraitOneByTransToTraitTwo<T, mode, U, DivB64Impl<T, mode, Reg::RegTensor<ActualT, Reg::RegTraitNumTwo>>>(
                dstReg, srcReg0, srcReg1, mask);
        }
    } else if constexpr (SupportType<ActualT, half>()) {
        if constexpr (sprMode.algo == DivAlgo::PRECISION_1ULP_FTZ_FALSE) {
            DivIEEE754HalfImpl<T, mode, U>(dstReg, srcReg0, srcReg1, mask);
        } else {
            vdiv(dstReg, srcReg0, srcReg1, mask, modeValue);
        }
    } else if constexpr (SupportType<ActualT, float>()) {
        if constexpr (sprMode.precisionMode) {
            if constexpr (sprMode.algo == DivAlgo::PRECISION_0ULP_FTZ_FALSE) {
                DivIEEE754FloatImpl<T, mode, U, true>(dstReg, srcReg0, srcReg1, mask);
            } else {
                DivPrecisionImpl<T, mode, U>(dstReg, srcReg0, srcReg1, mask);
            }
        } else {
            if constexpr (sprMode.algo == DivAlgo::PRECISION_0ULP_FTZ_FALSE) {
                DivIEEE754FloatImpl<T, mode, U, true>(dstReg, srcReg0, srcReg1, mask);
            } else if constexpr (sprMode.algo == DivAlgo::PRECISION_1ULP_FTZ_FALSE) {
                DivIEEE754FloatImpl<T, mode, U, false>(dstReg, srcReg0, srcReg1, mask);
            } else if constexpr (sprMode.algo == DivAlgo::PRECISION_0ULP_FTZ_TRUE) {
                DivPrecisionImpl<T, mode, U>(dstReg, srcReg0, srcReg1, mask);
            } else {
                vdiv(dstReg, srcReg0, srcReg1, mask, modeValue);
            }
        }
    } else {
        // u16/s16/u32/s32: vdiv 指令不支持整数类型, 使用软实现
        DivIntSoftImpl(dstReg, srcReg0, srcReg1, mask);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void MaxOperator(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (CheckRegTrait<U, RegTraitNumOne>()) {
        if constexpr (sizeof(ActualT) < 8) {
            vmax(dstReg, srcReg0, srcReg1, mask, modeValue);
        } else if constexpr (sizeof(ActualT) == 8) {
            MaskReg maskTrait2;
            MaskPack(maskTrait2, mask);
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg0;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg1;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoDstReg;
            B64TraitOneToTraitTwo(traitTwoSrcReg0, srcReg0);
            B64TraitOneToTraitTwo(traitTwoSrcReg1, srcReg1);
            MaskReg selMask;
            Compare<ActualT, CMPMODE::GT>(selMask, traitTwoSrcReg0, traitTwoSrcReg1, maskTrait2);
            Select(traitTwoDstReg, traitTwoSrcReg0, traitTwoSrcReg1, selMask);
            B64TraitTwoToTraitOne(dstReg, traitTwoDstReg);
        }
    } else if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
        MaskReg selMask;
        Compare<ActualT, CMPMODE::GT>(selMask, srcReg0, srcReg1, mask);
        Select(dstReg, srcReg0, srcReg1, selMask);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void MaxImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(
        SupportType<
            ActualT, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, half, float, bfloat16_t, uint64_t,
            int64_t>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING, MaskMergeMode::MERGING>(),
        "current Max api only supported Mode ZEROING/MERGING on current device!");
    if constexpr (mode == MaskMergeMode::ZEROING) {
        MaxOperator<T, mode, U>(dstReg, srcReg0, srcReg1, mask);
    } else if constexpr (mode == MaskMergeMode::MERGING) {
        U dstCopyReg;
        MaxOperator<T, MaskMergeMode::ZEROING, U>(dstCopyReg, srcReg0, srcReg1, mask);
        CopyMerging(dstReg, dstCopyReg, mask);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void MinOperator(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (CheckRegTrait<U, RegTraitNumOne>()) {
        if constexpr (sizeof(ActualT) < 8) {
            vmin(dstReg, srcReg0, srcReg1, mask, modeValue);
        } else if constexpr (sizeof(ActualT) == 8) {
            MaskReg maskTrait2;
            MaskPack(maskTrait2, mask);
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg0;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg1;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoDstReg;
            B64TraitOneToTraitTwo(traitTwoSrcReg0, srcReg0);
            B64TraitOneToTraitTwo(traitTwoSrcReg1, srcReg1);
            MaskReg selMask;
            Compare<ActualT, CMPMODE::LT>(selMask, traitTwoSrcReg0, traitTwoSrcReg1, maskTrait2);
            Select(traitTwoDstReg, traitTwoSrcReg0, traitTwoSrcReg1, selMask);
            B64TraitTwoToTraitOne(dstReg, traitTwoDstReg);
        }
    } else if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
        MaskReg selMask;
        Compare<ActualT, CMPMODE::LT>(selMask, srcReg0, srcReg1, mask);
        Select(dstReg, srcReg0, srcReg1, selMask);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void MinImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(
        SupportType<
            ActualT, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, half, float, bfloat16_t, uint64_t,
            int64_t>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING, MaskMergeMode::MERGING>(),
        "current Min api only supported Mode ZEROING/MERGING on current device!");
    if constexpr (mode == MaskMergeMode::ZEROING) {
        MinOperator<T, mode, U>(dstReg, srcReg0, srcReg1, mask);
    } else if constexpr (mode == MaskMergeMode::MERGING) {
        U dstCopyReg;
        MinOperator<T, MaskMergeMode::ZEROING, U>(dstCopyReg, srcReg0, srcReg1, mask);
        CopyMerging(dstReg, dstCopyReg, mask);
    }
}

template <typename T = int32_t, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U, typename S>
__simd_callee__ inline void ShiftL(U& dstReg, U& srcReg0, S& srcReg1, MaskReg& mask)
{
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    constexpr int32_t Bias = 32;
    RegTensor<int32_t> vRegTemp0, vRegTemp1, vRegTemp2;
    RegTensor<uint32_t> vRegTemp3;
    RegTensor<T> vRegTemp4;
    Duplicate(vRegTemp0, Bias, mask);
    Sub(vRegTemp1, vRegTemp0, srcReg1, mask);
    Adds(vRegTemp2, srcReg1, Bias, mask);
    vshl(vRegTemp3, (RegTensor<uint32_t>&)srcReg0.reg[0], srcReg1, mask, modeValue);
    vshl(vRegTemp4, (RegTensor<T>&)srcReg0.reg[1], vRegTemp2, mask, modeValue);
    Or((RegTensor<uint32_t>&)dstReg.reg[0], vRegTemp3, (RegTensor<uint32_t>&)vRegTemp4, mask);
    vshr(vRegTemp3, (RegTensor<uint32_t>&)srcReg0.reg[0], vRegTemp1, mask, modeValue);
    vshl(vRegTemp4, (RegTensor<T>&)srcReg0.reg[1], srcReg1, mask, modeValue);
    Or((RegTensor<T>&)dstReg.reg[1], (RegTensor<T>&)vRegTemp3, vRegTemp4, mask);
}

template <MaskMergeMode mode = MaskMergeMode::ZEROING, typename T, typename U>
__simd_callee__ inline void ShiftLeftB64Impl(T& dstReg, T& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename T::ActualT;
    using ActualU = typename U::ActualT;
    static_assert(sizeof(ActualT) == 8, "T data type should be B64");
    static_assert(sizeof(ActualU) == 4, "U data type should be int32_t");
    static_assert(CheckRegTrait<T, RegTraitNumTwo>(), "T should be RegTraitNumTwo");
    if (Std::is_same_v<ActualT, uint64_t>) {
        ShiftL<uint32_t, mode>(dstReg, srcReg0, srcReg1, mask);
    } else if (Std::is_same_v<ActualT, int64_t>) {
        ShiftL<int32_t, mode>(dstReg, srcReg0, srcReg1, mask);
    }
}

template <
    typename T = DefaultType, typename U = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename S,
    typename V>
__simd_callee__ inline void ShiftLeftImpl(S& dstReg, S& srcReg0, V& srcReg1, MaskReg& mask)
{
    using ActualT = typename S::ActualT;
    using ActualU = typename V::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(Std::is_same_v<U, DefaultType> || Std::is_same_v<U, ActualU>, "T type is not correct!");
    static_assert(
        SupportType<ActualT, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportType<ActualU, int8_t, int16_t, int32_t, int64_t>(),
        "current src1 data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING>(),
        "current ShiftLeft api only supported Mode ZEROING on current device!");
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (sizeof(ActualT) != 8) {
        vshl(dstReg, srcReg0, srcReg1, mask, modeValue);
    } else {
        if constexpr (CheckRegTrait<S, RegTraitNumOne>()) {
            MaskReg maskTrait2;
            MaskPack(maskTrait2, mask);
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg0, traitTwoSrcReg1;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoDstReg;
            B64TraitOneToTraitTwo(traitTwoSrcReg0, srcReg0);
            B64TraitOneToTraitTwo(traitTwoSrcReg1, srcReg1);
            ShiftLeftB64Impl<mode>(traitTwoDstReg, traitTwoSrcReg0, (RegTensor<int32_t>&)traitTwoSrcReg1, maskTrait2);
            B64TraitTwoToTraitOne(dstReg, traitTwoDstReg);
        } else if constexpr (CheckRegTrait<S, RegTraitNumTwo>()) {
            S dstTemp;
            ShiftLeftB64Impl<mode>(dstTemp, srcReg0, (RegTensor<int32_t>&)srcReg1, mask);
            dstReg = dstTemp;
        }
    }
}

template <typename T = int32_t, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U, typename S>
__simd_callee__ inline void ShiftR(U& dstReg, U& srcReg0, S& srcReg1, MaskReg& mask)
{
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    constexpr int32_t Bias = 32;
    RegTensor<int32_t> vRegTemp0, vRegTemp1, vRegTemp2;
    RegTensor<uint32_t> vRegTemp3;
    RegTensor<T> vRegTemp4;
    Duplicate(vRegTemp0, Bias, mask);
    Sub(vRegTemp1, vRegTemp0, srcReg1, mask);
    Adds(vRegTemp2, srcReg1, Bias, mask);
    vshr(vRegTemp3, (RegTensor<uint32_t>&)srcReg0.reg[0], srcReg1, mask, modeValue);
    vshl(vRegTemp4, (RegTensor<T>&)srcReg0.reg[1], vRegTemp1, mask, modeValue);
    Or((RegTensor<uint32_t>&)dstReg.reg[0], vRegTemp3, (RegTensor<uint32_t>&)vRegTemp4, mask);
    vshr(vRegTemp3, (RegTensor<uint32_t>&)srcReg0.reg[0], vRegTemp2, mask, modeValue);
    vshr(vRegTemp4, (RegTensor<T>&)srcReg0.reg[1], srcReg1, mask, modeValue);
    Or((RegTensor<T>&)dstReg.reg[1], (RegTensor<T>&)vRegTemp3, vRegTemp4, mask);
}

template <MaskMergeMode mode = MaskMergeMode::ZEROING, typename T, typename U>
__simd_callee__ inline void ShiftRightB64Impl(T& dstReg, T& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename T::ActualT;
    using ActualU = typename U::ActualT;
    static_assert(sizeof(ActualT) == 8, "T data type should be B64");
    static_assert(sizeof(ActualU) == 4, "U data type should be int32_t");
    static_assert(CheckRegTrait<T, RegTraitNumTwo>(), "T should be RegTraitNumTwo");
    if (Std::is_same_v<ActualT, uint64_t>) {
        ShiftR<uint32_t, mode>(dstReg, srcReg0, srcReg1, mask);
    } else if (Std::is_same_v<ActualT, int64_t>) {
        ShiftR<int32_t, mode>(dstReg, srcReg0, srcReg1, mask);
    }
}

template <
    typename T = DefaultType, typename U = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename S,
    typename V>
__simd_callee__ inline void ShiftRightImpl(S& dstReg, S& srcReg0, V& srcReg1, MaskReg& mask)
{
    using ActualT = typename S::ActualT;
    using ActualU = typename V::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(Std::is_same_v<U, DefaultType> || Std::is_same_v<U, ActualU>, "T type is not correct!");
    static_assert(
        SupportType<ActualT, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportType<ActualU, int8_t, int16_t, int32_t, int64_t>(),
        "current src1 data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING>(),
        "current ShiftRight api only supported Mode ZEROING on current device!");
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (sizeof(ActualT) != 8) {
        vshr(dstReg, srcReg0, srcReg1, mask, modeValue);
    } else {
        if constexpr (CheckRegTrait<S, RegTraitNumOne>()) {
            MaskReg maskTrait2;
            MaskPack(maskTrait2, mask);
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg0, traitTwoSrcReg1;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoDstReg;
            B64TraitOneToTraitTwo(traitTwoSrcReg0, srcReg0);
            B64TraitOneToTraitTwo(traitTwoSrcReg1, srcReg1);
            ShiftRightB64Impl<mode>(traitTwoDstReg, traitTwoSrcReg0, (RegTensor<int32_t>&)traitTwoSrcReg1, maskTrait2);
            B64TraitTwoToTraitOne(dstReg, traitTwoDstReg);
        } else if constexpr (CheckRegTrait<S, RegTraitNumTwo>()) {
            S dstTemp;
            ShiftRightB64Impl<mode>(dstTemp, srcReg0, (RegTensor<int32_t>&)srcReg1, mask);
            dstReg = dstTemp;
        }
    }
}

template <MaskMergeMode mode = MaskMergeMode::ZEROING, typename T>
__simd_callee__ inline void AndB64Impl(T& dstReg, T& srcReg0, T& srcReg1, MaskReg& mask)
{
    using ActualT = typename T::ActualT;
    static_assert(sizeof(ActualT) == 8, "AndB64Impl data type should be B64");
    static_assert(CheckRegTrait<T, RegTraitNumTwo>(), "AndB64Impl T should be RegTraitNumTwo");
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (Std::is_same_v<ActualT, uint64_t>) {
        vand(
            (RegTensor<uint32_t>&)dstReg.reg[0], (RegTensor<uint32_t>&)srcReg0.reg[0],
            (RegTensor<uint32_t>&)srcReg1.reg[0], mask, modeValue);
        vand(
            (RegTensor<uint32_t>&)dstReg.reg[1], (RegTensor<uint32_t>&)srcReg0.reg[1],
            (RegTensor<uint32_t>&)srcReg1.reg[1], mask, modeValue);
    } else if constexpr (Std::is_same_v<ActualT, int64_t>) {
        vand(
            (RegTensor<uint32_t>&)dstReg.reg[0], (RegTensor<uint32_t>&)srcReg0.reg[0],
            (RegTensor<uint32_t>&)srcReg1.reg[0], mask, modeValue);
        vand(
            (RegTensor<int32_t>&)dstReg.reg[1], (RegTensor<int32_t>&)srcReg0.reg[1],
            (RegTensor<int32_t>&)srcReg1.reg[1], mask, modeValue);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void AndImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(
        SupportType<ActualT, bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING>(), "current And api only supported Mode ZEROING on current device!");
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (IsSameType<ActualT, bool>::value) {
        vand((RegTensor<int8_t>&)dstReg, (RegTensor<int8_t>&)srcReg0, (RegTensor<int8_t>&)srcReg1, mask, modeValue);
    } else if constexpr (sizeof(ActualT) != 8) {
        vand(dstReg, srcReg0, srcReg1, mask, modeValue);
    } else {
        if constexpr (CheckRegTrait<U, RegTraitNumOne>()) {
            MaskReg maskTrait2;
            MaskPack(maskTrait2, mask);
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg0;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg1;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoDstReg;
            B64TraitOneToTraitTwo(traitTwoSrcReg0, srcReg0);
            B64TraitOneToTraitTwo(traitTwoSrcReg1, srcReg1);
            AndB64Impl<mode>(traitTwoDstReg, traitTwoSrcReg0, traitTwoSrcReg1, maskTrait2);
            B64TraitTwoToTraitOne(dstReg, traitTwoDstReg);
        } else if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            U dstTemp;
            AndB64Impl<mode>(dstTemp, srcReg0, srcReg1, mask);
            dstReg = dstTemp;
        }
    }
}

template <MaskMergeMode mode = MaskMergeMode::ZEROING, typename T>
__simd_callee__ inline void OrB64Impl(T& dstReg, T& srcReg0, T& srcReg1, MaskReg& mask)
{
    using ActualT = typename T::ActualT;
    static_assert(sizeof(ActualT) == 8, "OrB64Impl data type should be B64");
    static_assert(CheckRegTrait<T, RegTraitNumTwo>(), "OrB64Impl T should be RegTraitNumTwo");
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (Std::is_same_v<ActualT, uint64_t>) {
        vor((RegTensor<uint32_t>&)dstReg.reg[0], (RegTensor<uint32_t>&)srcReg0.reg[0],
            (RegTensor<uint32_t>&)srcReg1.reg[0], mask, modeValue);
        vor((RegTensor<uint32_t>&)dstReg.reg[1], (RegTensor<uint32_t>&)srcReg0.reg[1],
            (RegTensor<uint32_t>&)srcReg1.reg[1], mask, modeValue);
    } else if constexpr (Std::is_same_v<ActualT, int64_t>) {
        vor((RegTensor<uint32_t>&)dstReg.reg[0], (RegTensor<uint32_t>&)srcReg0.reg[0],
            (RegTensor<uint32_t>&)srcReg1.reg[0], mask, modeValue);
        vor((RegTensor<int32_t>&)dstReg.reg[1], (RegTensor<int32_t>&)srcReg0.reg[1],
            (RegTensor<int32_t>&)srcReg1.reg[1], mask, modeValue);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void OrImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(
        SupportType<ActualT, bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING>(), "current Or api only supported Mode ZEROING on current device!");
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (IsSameType<ActualT, bool>::value) {
        vor((RegTensor<int8_t>&)dstReg, (RegTensor<int8_t>&)srcReg0, (RegTensor<int8_t>&)srcReg1, mask, modeValue);
    } else if constexpr (sizeof(ActualT) != 8) {
        vor(dstReg, srcReg0, srcReg1, mask, modeValue);
    } else {
        if constexpr (CheckRegTrait<U, RegTraitNumOne>()) {
            MaskReg maskTrait2;
            MaskPack(maskTrait2, mask);
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg0;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg1;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoDstReg;
            B64TraitOneToTraitTwo(traitTwoSrcReg0, srcReg0);
            B64TraitOneToTraitTwo(traitTwoSrcReg1, srcReg1);
            OrB64Impl<mode>(traitTwoDstReg, traitTwoSrcReg0, traitTwoSrcReg1, maskTrait2);
            B64TraitTwoToTraitOne(dstReg, traitTwoDstReg);
        } else if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            U dstTemp;
            OrB64Impl<mode>(dstTemp, srcReg0, srcReg1, mask);
            dstReg = dstTemp;
        }
    }
}

template <MaskMergeMode mode = MaskMergeMode::ZEROING, typename T>
__simd_callee__ inline void XorB64Impl(T& dstReg, T& srcReg0, T& srcReg1, MaskReg& mask)
{
    using ActualT = typename T::ActualT;
    static_assert(sizeof(ActualT) == 8, "XorB64Impl data type should be B64");
    static_assert(CheckRegTrait<T, RegTraitNumTwo>(), "XorB64Impl T should be RegTraitNumTwo");
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (Std::is_same_v<ActualT, uint64_t>) {
        vxor(
            (RegTensor<uint32_t>&)dstReg.reg[0], (RegTensor<uint32_t>&)srcReg0.reg[0],
            (RegTensor<uint32_t>&)srcReg1.reg[0], mask, modeValue);
        vxor(
            (RegTensor<uint32_t>&)dstReg.reg[1], (RegTensor<uint32_t>&)srcReg0.reg[1],
            (RegTensor<uint32_t>&)srcReg1.reg[1], mask, modeValue);
    } else if constexpr (Std::is_same_v<ActualT, int64_t>) {
        vxor(
            (RegTensor<uint32_t>&)dstReg.reg[0], (RegTensor<uint32_t>&)srcReg0.reg[0],
            (RegTensor<uint32_t>&)srcReg1.reg[0], mask, modeValue);
        vxor(
            (RegTensor<int32_t>&)dstReg.reg[1], (RegTensor<int32_t>&)srcReg0.reg[1],
            (RegTensor<int32_t>&)srcReg1.reg[1], mask, modeValue);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void XorImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(
        SupportType<ActualT, bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, uint64_t, int64_t>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING>(), "current Xor api only supported Mode ZEROING on current device!");
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (IsSameType<ActualT, bool>::value) {
        vxor((RegTensor<int8_t>&)dstReg, (RegTensor<int8_t>&)srcReg0, (RegTensor<int8_t>&)srcReg1, mask, modeValue);
    } else if constexpr (sizeof(ActualT) != 8) {
        vxor(dstReg, srcReg0, srcReg1, mask, modeValue);
    } else {
        if constexpr (CheckRegTrait<U, RegTraitNumOne>()) {
            MaskReg maskTrait2;
            MaskPack(maskTrait2, mask);
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg0;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg1;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoDstReg;
            B64TraitOneToTraitTwo(traitTwoSrcReg0, srcReg0);
            B64TraitOneToTraitTwo(traitTwoSrcReg1, srcReg1);
            XorB64Impl<mode>(traitTwoDstReg, traitTwoSrcReg0, traitTwoSrcReg1, maskTrait2);
            B64TraitTwoToTraitOne(dstReg, traitTwoDstReg);
        } else if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            U dstTemp;
            XorB64Impl<mode>(dstTemp, srcReg0, srcReg1, mask);
            dstReg = dstTemp;
        }
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void PreluImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(SupportType<ActualT, half, float>(), "current data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING>(),
        "current Prelu api only supported Mode ZEROING on current device!");
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    vprelu(dstReg, srcReg0, srcReg1, mask, modeValue);
}

// 当前芯片不支持 vmull 指令, 使用 16bit 拆分软实现 32bit x 32bit -> 64bit 无符号乘
// 低 32bit 用 1 次 vmul 直接得到, 高 32bit 用 4 次 16bit 部分积 vmul 累加得到
template <typename U>
__simd_callee__ inline void MullU32SoftImpl(U& dstReg0, U& dstReg1, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    constexpr int16_t halfBit = 16;
    U aH, aL, bH, bL;
    U p0, p1, p2, p3;
    U t0, t2;
    U mid;

    // 取高低 16bit: x & 0xFFFF == (x << 16) >> 16
    vshls(aL, srcReg0, halfBit, mask, modeValue);
    vshrs(aL, aL, halfBit, mask, modeValue);
    vshrs(aH, srcReg0, halfBit, mask, modeValue);
    vshls(bL, srcReg1, halfBit, mask, modeValue);
    vshrs(bL, bL, halfBit, mask, modeValue);
    vshrs(bH, srcReg1, halfBit, mask, modeValue);

    // 低 32bit 即乘积截断结果, 直接一次 vmul 得到
    // 注意: 必须在写 dstReg1 之前完成所有对 srcReg0/srcReg1 的读取, 以支持 dst 与 src 同寄存器的调用
    vmul(dstReg0, srcReg0, srcReg1, mask, modeValue);

    // 4 次 16bit x 16bit -> 32bit 乘
    vmul(p0, aL, bL, mask, modeValue);
    vmul(p1, aL, bH, mask, modeValue);
    vmul(p2, aH, bL, mask, modeValue);
    vmul(p3, aH, bH, mask, modeValue);

    // mid = p1 + (p2 & 0xFFFF) + (p0 >> 16), 结果不会超过 0xFFFFFFFF
    vshls(t2, p2, halfBit, mask, modeValue);
    vshrs(t2, t2, halfBit, mask, modeValue);
    vshrs(t0, p0, halfBit, mask, modeValue);
    vadd(mid, p1, t2, mask, modeValue);
    vadd(mid, mid, t0, mask, modeValue);

    // 高 32bit = p3 + (mid >> 16) + (p2 >> 16)
    vshrs(t0, mid, halfBit, mask, modeValue);
    vadd(dstReg1, p3, t0, mask, modeValue);
    vshrs(t2, p2, halfBit, mask, modeValue);
    vadd(dstReg1, dstReg1, t2, mask, modeValue);
}

template <typename T = DefaultType, typename U>
__simd_callee__ inline void MullImpl(U& dstReg0, U& dstReg1, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(SupportType<ActualT, uint32_t, int32_t>(), "current data type is not supported on current device!");
    if constexpr (IsSameType<ActualT, uint32_t>::value) {
        MullU32SoftImpl(
            (RegTensor<uint32_t>&)dstReg0, (RegTensor<uint32_t>&)dstReg1, (RegTensor<uint32_t>&)srcReg0,
            (RegTensor<uint32_t>&)srcReg1, mask);
    } else {
        // 有符号乘: 先按无符号算出 64bit 积, 再修正高 32bit
        // 修正量 = (src0 < 0 ? src1 : 0) + (src1 < 0 ? src0 : 0)
        // 修正量必须在软实现写 dstReg0/dstReg1 之前算完, 以支持 dst 与 src 为同一寄存器的调用
        constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
        constexpr int16_t signBit = 31;
        RegTensor<uint32_t> sign0, sign1, corr;
        vshrs(sign0, (RegTensor<uint32_t>&)srcReg0, signBit, mask, modeValue);
        vshrs(sign1, (RegTensor<uint32_t>&)srcReg1, signBit, mask, modeValue);
        vmul(corr, sign0, (RegTensor<uint32_t>&)srcReg1, mask, modeValue);
        vmul(sign0, sign1, (RegTensor<uint32_t>&)srcReg0, mask, modeValue);
        vadd(corr, corr, sign0, mask, modeValue);
        MullU32SoftImpl(
            (RegTensor<uint32_t>&)dstReg0, (RegTensor<uint32_t>&)dstReg1, (RegTensor<uint32_t>&)srcReg0,
            (RegTensor<uint32_t>&)srcReg1, mask);
        vsub((RegTensor<uint32_t>&)dstReg1, (RegTensor<uint32_t>&)dstReg1, corr, mask, modeValue);
    }
}

template <typename T = DefaultType, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void MulAddDstImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(
        SupportType<ActualT, uint16_t, int16_t, uint32_t, int32_t, half, float, bfloat16_t, int64_t, uint64_t>(),
        "current data type is not supported on current device!");
    static_assert(
        SupportEnum<mode, MaskMergeMode::ZEROING>(),
        "current MulAddDst api only supported Mode ZEROING on current device!");
    if constexpr (sizeof(ActualT) == 8) {
        RegTensor<ActualT, RegTraitNumTwo> traitTwoDstTmpReg;
        if constexpr (CheckRegTrait<U, RegTraitNumOne>()) {
            MaskReg maskTrait2;
            MaskPack(maskTrait2, mask);
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg0;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg1;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoDstReg;
            B64TraitOneToTraitTwo(traitTwoSrcReg0, srcReg0);
            B64TraitOneToTraitTwo(traitTwoSrcReg1, srcReg1);
            B64TraitOneToTraitTwo(traitTwoDstReg, dstReg);
            Mul(traitTwoDstTmpReg, traitTwoSrcReg0, traitTwoSrcReg1, maskTrait2);
            Add(traitTwoDstReg, traitTwoDstTmpReg, traitTwoDstReg, maskTrait2);

            B64TraitTwoToTraitOne(dstReg, traitTwoDstReg);
        } else if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            Mul(traitTwoDstTmpReg, srcReg0, srcReg1, mask);
            Add(dstReg, traitTwoDstTmpReg, dstReg, mask);
        }
    } else {
        constexpr auto modeValue = GetMaskMergeMode<mode>();
        vmula(dstReg, srcReg0, srcReg1, mask, modeValue);
    }
}

// 当前芯片不支持 vaddc/vaddcs 进位加指令, 使用 vadd/vnot/vcmp_gt 软实现进位输出
template <typename U>
__simd_callee__ inline void AddCarryOutSoftU32(MaskReg& carry, U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    U notSrcReg1;
    // 无符号 32bit 加法溢出判断: src0 + src1 >= 2^32 等价于 src0 > ~src1
    vnot(notSrcReg1, srcReg1, mask, modeValue);
    vcmp_gt(carry, srcReg0, notSrcReg1, mask);
    vadd(dstReg, srcReg0, srcReg1, mask, modeValue);
}

// 当前芯片不支持 vsubc/vsubcs 借位减指令, 使用 vsub/vcmp_lt 软实现借位输出
template <typename U>
__simd_callee__ inline void SubCarryOutSoftU32(MaskReg& carry, U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    // 无符号 32bit 减法借位判断: src0 < src1 即产生借位
    vcmp_lt(carry, srcReg0, srcReg1, mask);
    vsub(dstReg, srcReg0, srcReg1, mask, modeValue);
}

template <typename T = DefaultType, typename U>
__simd_callee__ inline void AddCarryOutsImpl(
    MaskReg& carry, U& dstReg, U& srcReg0, U& srcReg1, MaskReg& carrySrc, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(SupportType<ActualT, uint32_t, int32_t>(), "current data type is not supported on current device!");
    constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    RegTensor<uint32_t> sumReg, carryVec, oneVec, zeroVec;
    MaskReg carryMid, carryAdd;
    AddCarryOutSoftU32(carryMid, sumReg, (RegTensor<uint32_t>&)srcReg0, (RegTensor<uint32_t>&)srcReg1, mask);
    // 进位输入转成 0/1 向量后再累加一次, 两次进位取或即为最终进位输出
    vbr(oneVec, static_cast<uint32_t>(1));
    vbr(zeroVec, static_cast<uint32_t>(0));
    vsel(carryVec, oneVec, zeroVec, carrySrc);
    vadd((RegTensor<uint32_t>&)dstReg, sumReg, carryVec, mask, modeValue);
    vcmp_lt(carryAdd, (RegTensor<uint32_t>&)dstReg, sumReg, mask);
    por(carry, carryMid, carryAdd, mask);
}

template <typename T = DefaultType, typename U>
__simd_callee__ inline void SubCarryOutsImpl(
    MaskReg& carry, U& dstReg, U& srcReg0, U& srcReg1, MaskReg& carrySrc, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(SupportType<ActualT, uint32_t, int32_t>(), "current data type is not supported on current device!");
    constexpr auto modeValue = GetMaskMergeMode<MaskMergeMode::ZEROING>();
    RegTensor<uint32_t> diffReg, borrowVec, oneVec, zeroVec;
    MaskReg borrowMid, borrowAdd;
    SubCarryOutSoftU32(borrowMid, diffReg, (RegTensor<uint32_t>&)srcReg0, (RegTensor<uint32_t>&)srcReg1, mask);
    // 借位输入转成 0/1 向量后再减一次, 两次借位取或即为最终借位输出
    vbr(oneVec, static_cast<uint32_t>(1));
    vbr(zeroVec, static_cast<uint32_t>(0));
    vsel(borrowVec, oneVec, zeroVec, carrySrc);
    vsub((RegTensor<uint32_t>&)dstReg, diffReg, borrowVec, mask, modeValue);
    vcmp_lt(borrowAdd, diffReg, borrowVec, mask);
    por(carry, borrowMid, borrowAdd, mask);
}

template <typename T = DefaultType, typename U>
__simd_callee__ inline void AddCarryOutImpl(MaskReg& carry, U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(SupportType<ActualT, uint32_t, int32_t>(), "current data type is not supported on current device!");
    AddCarryOutSoftU32(
        carry, (RegTensor<uint32_t>&)dstReg, (RegTensor<uint32_t>&)srcReg0, (RegTensor<uint32_t>&)srcReg1, mask);
}

template <typename T = DefaultType, typename U>
__simd_callee__ inline void SubCarryOutImpl(MaskReg& carry, U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(Std::is_same_v<T, DefaultType> || Std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(SupportType<ActualT, uint32_t, int32_t>(), "current data type is not supported on current device!");
    SubCarryOutSoftU32(
        carry, (RegTensor<uint32_t>&)dstReg, (RegTensor<uint32_t>&)srcReg0, (RegTensor<uint32_t>&)srcReg1, mask);
}
} // namespace Reg
} // namespace AscendC
#endif // ASCENDC_MODULE_REG_COMPUTE_VEC_BINARY_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_VEC_BINARY_IMPL__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_VEC_BINARY_IMPL__
#endif
