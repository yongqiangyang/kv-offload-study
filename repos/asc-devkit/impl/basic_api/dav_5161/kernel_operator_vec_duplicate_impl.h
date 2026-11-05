/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_5161/kernel_operator_vec_duplicate_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_IMPL_H
#include <type_traits>
#include "../reg_compute/kernel_reg_compute_intf_impl.h"

namespace AscendC {
template <typename T>
constexpr __aicore__ inline void CheckDuplicateSupportedType()
{
    static_assert(
        SupportType<T, bool, int8_t, uint8_t, half, int16_t, uint16_t, int32_t, uint32_t, float>(),
        "Duplicate instr only support bool/int8_t/uint8_t/half/int16_t/uint16_t/int32_t/uint32_t/float"
        "type on current device");
}

template <typename T, typename RegType>
__simd_vf__ inline void DuplicateFromScalarImpl(__ubuf__ T* dstLocal, const T scalarValue, const int32_t calCount)
{
    RegType dstReg;
    uint32_t sreg = static_cast<uint32_t>(calCount);
    Reg::MaskReg mask;
    constexpr uint32_t repeatStride = static_cast<uint32_t>(GetVecLen() / sizeof(T) * RegType::trait.REG_NUM);
    uint16_t repeatTime = static_cast<uint16_t>(CeilDivision(calCount, repeatStride));
    Reg::MaskReg maskFull = Reg::CreateMask<T, Reg::MaskPattern::ALL, RegType::trait>();
    Reg::Duplicate(dstReg, scalarValue, maskFull);
    for (uint16_t i = 0; i < repeatTime; ++i) {
        mask = Reg::UpdateMask<T, RegType::trait>(sreg);
        Reg::StoreAlign(dstLocal + i * repeatStride, dstReg, mask);
    }
}

template <typename T>
__aicore__ inline void DuplicateImpl(__ubuf__ T* dstLocal, const T& scalarValue, const int32_t& calCount)
{
    CheckDuplicateSupportedType<T>();
    DuplicateFromScalarImpl<T, Reg::RegTensor<T, Reg::RegTraitNumOne>>(dstLocal, scalarValue, calCount);
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_DUPLICATE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_DUPLICATE_IMPL_H__
#endif
