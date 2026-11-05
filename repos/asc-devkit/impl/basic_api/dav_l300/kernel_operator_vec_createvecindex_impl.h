/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file kernel_operator_vec_createvecindex_impl.h
 * \brief AscendC l300 support vector create vector index api.
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_l300/kernel_operator_vec_createvecindex_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CREATEVECINDEX_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_VEC_CREATEVECINDEX_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_CREATEVECINDEX_IMPL_H
#include "../../../include/basic_api/kernel_tensor.h"
#include "kernel_operator_common_impl.h"
#if ASCENDC_CPU_DEBUG
#include "../kernel_check.h"
#endif

namespace AscendC {
template <typename T>
constexpr __aicore__ inline void CheckCreateVecIndexApi0SupportedType()
{
    static_assert(
        SupportType<T, int16_t, int32_t, half, float>(),
        "CreateVecIndex level-0 api only support int16_t/int32_t/half/float on current device");
}

template <typename T>
constexpr __aicore__ inline void CheckCreateVecIndexApi2SupportedType()
{
    static_assert(
        std::is_same<T, int8_t>::value || std::is_same<T, int16_t>::value || std::is_same<T, int32_t>::value ||
            std::is_same<T, half>::value || std::is_same<T, float>::value,
        "CreateVecIndex level-2 api only support int8_t/int16_t/int32_t/half/float");
}
namespace Internal {
template <bool isMaskBitMode, bool isNormalMode, typename T>
__simd_vf__ inline void VecCreateVecIndexLevel0VFImpl(
    __ubuf__ T* dst, const T firstValue, const BasicAPIMaskStruct maskArrayStruct, const uint64_t maskCount,
    const uint8_t repeatTime, uint16_t dstBlkStride, uint8_t dstRepStride, __ubuf__ uint64_t* maskBuf)
{
    constexpr uint16_t sreg = GetVecLen() / sizeof(T);
    uint32_t count = VecMicroGetCount<true, isNormalMode, isMaskBitMode>(maskArrayStruct.maskArray, maskCount, maskBuf);
    uint16_t newRepeatTimes = VecMicroGetRepeatTimes<T, isNormalMode>(count, repeatTime);
    Reg::MaskReg maskReg;
    if constexpr (isNormalMode) {
        maskReg = VecMicroGetMaskReg<T, true, isNormalMode, isMaskBitMode>(maskBuf, count);
    }
    constexpr uint8_t ElePerBlkT = GetDataBlockSizeInBytes() / sizeof(T);
    Reg::RegTensor<T> dstVreg;
    Reg::Arange(dstVreg, firstValue);
    for (uint16_t index = 0; index < newRepeatTimes; ++index) {
        if constexpr (!isNormalMode) {
            maskReg = VecMicroGetMaskReg<T, true, isNormalMode, isMaskBitMode>(maskBuf, count);
        }
        Reg::DataCopy<T, Reg::DataCopyMode::DATA_BLOCK_COPY>(
            dst + index * dstRepStride * ElePerBlkT, dstVreg, dstBlkStride, maskReg);
        Reg::Adds(dstVreg, dstVreg, static_cast<int32_t>(sreg), maskReg);
    }
}

template <bool isMaskBitMode, typename T>
__aicore__ inline void VecCreateVecIndexLevel0Template(
    __ubuf__ T* dst, const T firstValue, const uint64_t maskArray[], const uint64_t maskCount, const uint8_t repeatTime,
    uint16_t dstBlkStride, uint8_t dstRepStride)
{
    if constexpr (isMaskBitMode) {
        ASCENDC_ASSERT(maskCount == 0, "maskCount must be 0 when isMaskBitMode is true.");
    } else {
        ASCENDC_ASSERT(maskArray == nullptr, "maskArray must be nullptr when isMaskBitMode is false.");
    }
    uint16_t maskArraySize = (maskArray == nullptr) ? 0 : MASK_ARRAY_SIZE;
    BasicAPIMaskStruct maskArrayStruct;
    for (uint16_t i = 0; i < maskArraySize; i++) {
        maskArrayStruct.maskArray[i] = maskArray[i];
    }
    if (Internal::IsCounterMode()) {
        VecCreateVecIndexLevel0VFImpl<isMaskBitMode, false, T>(
            dst, firstValue, maskArrayStruct, maskCount, repeatTime, dstBlkStride, dstRepStride, nullptr);
    } else {
        if constexpr (isMaskBitMode) {
            SetVectorMask<T>(maskArray[1], maskArray[0]); // set mask to SPR.MASK, movp in VF
        }
        VecCreateVecIndexLevel0VFImpl<isMaskBitMode, true, T>(
            dst, firstValue, maskArrayStruct, maskCount, repeatTime, dstBlkStride, dstRepStride, nullptr);
    }
}
} // namespace Internal

// VCI level-0 normal
template <typename T>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<T>& dstLocal, const T firstValue, uint64_t mask, uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    CheckCreateVecIndexApi0SupportedType<T>();

    __ubuf__ T* dst = (__ubuf__ T*)dstLocal.GetPhyAddr();
    Internal::VecCreateVecIndexLevel0Template<false>(
        dst, firstValue, nullptr, mask, repeatTime, dstBlkStride, dstRepStride);
}

// VCI level-0 bitwise
template <typename T>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<T>& dstLocal, const T firstValue, uint64_t mask[], uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    CheckCreateVecIndexApi0SupportedType<T>();

    __ubuf__ T* dst = (__ubuf__ T*)dstLocal.GetPhyAddr();
    Internal::VecCreateVecIndexLevel0Template<true>(dst, firstValue, mask, 0, repeatTime, dstBlkStride, dstRepStride);
}

// VCI level-2
template <typename T>
__aicore__ inline void CreateVecIndexCalc(LocalTensor<T> dstLocal, const T firstValue, uint32_t calCount)
{
    CheckCreateVecIndexApi2SupportedType<T>();

    __ubuf__ T* dstLocalAddr = (__ubuf__ T*)dstLocal.GetPhyAddr();
    uint32_t sreg = (uint32_t)calCount;
    int32_t sregLower = (int32_t)(VECTOR_REG_WIDTH / sizeof(T));
    uint16_t repeatTimes = CeilDivision(calCount, sregLower);

    __VEC_SCOPE__
    {
        RegTensor<T> vreg0;
        MaskReg preg = CreatePredicate<T>(sreg);
        CreateVecIndex(vreg0, firstValue);
        DataCopy(dstLocalAddr, vreg0, 0, preg);
        for (uint16_t i = 1; i < (uint16_t)repeatTimes; ++i) {
            preg = CreatePredicate<T>(sreg);
            Adds(vreg0, vreg0, sregLower, preg);
            DataCopy(dstLocalAddr, vreg0, i * sregLower, preg);
        }
    }
}

} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_CREATEVECINDEX_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CREATEVECINDEX_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CREATEVECINDEX_IMPL_H__
#endif
