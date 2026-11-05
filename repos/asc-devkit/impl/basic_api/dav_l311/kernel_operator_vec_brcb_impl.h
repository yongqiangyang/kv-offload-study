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
 * \file kernel_operator_vec_brcb_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_l311/kernel_operator_vec_brcb_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BRCB_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_BRCB_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_BRCB_IMPL_H
#include "kernel_operator_common_impl.h"
namespace AscendC {
/* **************************************************************************************************
 * Brcb                                             *
 * ************************************************************************************************* */
template <typename T>
typename std::enable_if_t<
    !std::is_same<T, uint16_t>::value && !std::is_same<T, half>::value && !std::is_same<T, uint32_t>::value &&
    !std::is_same<T, float>::
        value> __aicore__ inline BrcbImpl(__ubuf__ T* dst, __ubuf__ T* src0, const uint8_t repeatTime, const BrcbRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
}

template <typename T>
typename std::enable_if_t<std::is_same<T, uint16_t>::value || std::is_same<T, half>::value> __aicore__ inline BrcbImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, const uint8_t repeatTime, const BrcbRepeatParams& repeatParams)
{
    int32_t dstBlkStride = repeatParams.dstBlkStride > 0 ? repeatParams.dstBlkStride : 1;
    __VEC_SCOPE__
    {
        int32_t dstRepOffset = 0;
        RegTensor<T> vDst;
        int32_t sreg0 = 0;
        int32_t sreg1 = 0;
        uint32_t blockElm = ONE_BLOCK_SIZE / sizeof(T);
        uint32_t maskNum = ONE_BLOCK_SIZE / sizeof(T);
        MaskReg preg = CreatePredicate<T>(maskNum);
        for (uint16_t i = 0; i < (uint16_t)repeatTime; ++i) {
            sreg0 = ((int32_t)repeatParams.dstRepStride * i * blockElm);
            sreg1 = (int32_t)BRCB_BROADCAST_NUMBER * i;
            for (uint16_t j = 0; j < (uint16_t)BRCB_BROADCAST_NUMBER; ++j) {
                dstRepOffset = (sreg0 + dstBlkStride * j * blockElm);
                DataCopy<T, Dist::DIST_BRC_B16>(vDst, src0, sreg1 + j);
                DataCopy<T, DistVST::DIST_NORM_B16>(dst, vDst, dstRepOffset, preg);
            }
        }
    }
}

template <typename T>
typename std::enable_if_t<std::is_same<T, uint32_t>::value || std::is_same<T, float>::value> __aicore__ inline BrcbImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, const uint8_t repeatTime, const BrcbRepeatParams& repeatParams)
{
    int32_t dstBlkStride = repeatParams.dstBlkStride > 0 ? repeatParams.dstBlkStride : 1;
    __VEC_SCOPE__
    {
        int32_t dstRepOffset = 0;
        RegTensor<T> vDst;
        int32_t sreg0 = 0;
        int32_t sreg1 = 0;
        uint32_t blockElm = ONE_BLOCK_SIZE / sizeof(T);
        uint32_t maskNum = ONE_BLOCK_SIZE / sizeof(T);
        MaskReg preg = CreatePredicate<T>(maskNum);
        for (uint16_t i = 0; i < (uint16_t)repeatTime; ++i) {
            sreg0 = ((int32_t)repeatParams.dstRepStride * i * blockElm);
            sreg1 = (int32_t)BRCB_BROADCAST_NUMBER * i;
            for (uint16_t j = 0; j < (uint16_t)BRCB_BROADCAST_NUMBER; ++j) {
                dstRepOffset = (sreg0 + dstBlkStride * j * blockElm);
                DataCopy<T, Dist::DIST_BRC_B32>(vDst, src0, sreg1 + j);
                DataCopy<T, DistVST::DIST_NORM_B32>(dst, vDst, dstRepOffset, preg);
            }
        }
    }
}

} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_BRCB_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BRCB_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BRCB_IMPL_H__
#endif
