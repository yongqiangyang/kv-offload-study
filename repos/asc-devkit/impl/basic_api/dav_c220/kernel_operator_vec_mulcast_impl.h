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
 * \file kernel_operator_vec_mulcast_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_c220/kernel_operator_vec_mulcast_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_MULCAST_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_MULCAST_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_MULCAST_IMPL_H
#include "../../../include/basic_api/kernel_tensor.h"
#include "../../../include/basic_api/kernel_struct_binary.h"
#if ASCENDC_CPU_DEBUG
#include "../kernel_check.h"
#endif

#pragma begin_pipe(V)
namespace AscendC {

template <typename T, typename U>
__aicore__ inline void MulCastIntrinsicsImpl(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<U>& src1, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if constexpr (IsSameType<PrimT<T>, int8_t>::value) {
        vmulconv_f162s8(
            (__ubuf__ int8_t*)dst.GetPhyAddr(), (__ubuf__ half*)src0.GetPhyAddr(), (__ubuf__ half*)src1.GetPhyAddr(),
            repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
            repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
    } else {
        vmulconv_f162u8(
            (__ubuf__ uint8_t*)dst.GetPhyAddr(), (__ubuf__ half*)src0.GetPhyAddr(), (__ubuf__ half*)src1.GetPhyAddr(),
            repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
            repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
    }
}

template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void MulCastCalc(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<U>& src1, uint64_t mask,
    const uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<PrimT<U>, isSetMask>(mask);
        MulCastIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
    }
}

template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void MulCastCalc(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<U>& src1, uint64_t mask[],
    const uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        AscendCUtils::SetMask<PrimT<T>, isSetMask>(mask[1], mask[0]);
        MulCastIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
    }
}

template <typename T, typename U>
__aicore__ inline void MulCastCalc(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<U>& src1, uint32_t count)
{
    ASCENDC_DEBUG_ASSERT(
        (SupportType<PrimT<U>, half>() && SupportType<PrimT<T>, int8_t, uint8_t>()),
        KERNEL_LOG_INTERNAL(
            KERNEL_ERROR,
            "Failed "
            "to check dtype in MulCast, current api support dtype combination is src: half, dst: int8_t / uint8_t.\n"));
    if ASCEND_IS_AIV {
        BinaryRepeatParams repeatParams;
        repeatParams.dstRepStride = HALF_DEFAULT_REPEAT_STRIDE;
        set_mask_count();
        set_vector_mask(0, count);
        MulCastIntrinsicsImpl(dst, src0, src1, 1, repeatParams);
        set_mask_norm();
        ResetMask();
    }
}
} // namespace AscendC
#pragma end_pipe
#endif // ASCENDC_MODULE_OPERATOR_VEC_MULCAST_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_MULCAST_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_MULCAST_IMPL_H__
#endif
