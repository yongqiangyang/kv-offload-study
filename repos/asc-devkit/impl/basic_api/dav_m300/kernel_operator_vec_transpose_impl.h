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
 * \file kernel_operator_vec_transpose_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m300/kernel_operator_vec_transpose_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TRANSPOSE_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_TRANSPOSE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_TRANSPOSE_IMPL_H
#include "../../../include/basic_api/kernel_struct_transpose.h"

namespace AscendC {
template <typename T>
constexpr int8_t TWO_NUM = 2;
__aicore__ inline void TransDataTo5HDIntrinsicsImpl(
    __ubuf__ float* dstList[16], __ubuf__ float* srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b32(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

__aicore__ inline void TransDataTo5HDIntrinsicsImpl(
    __ubuf__ int32_t* dstList[16], __ubuf__ int32_t* srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b32(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

__aicore__ inline void TransDataTo5HDIntrinsicsImpl(
    __ubuf__ uint32_t* dstList[16], __ubuf__ uint32_t* srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b32(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

__aicore__ inline void TransDataTo5HDIntrinsicsImpl(
    __ubuf__ int16_t* dstList[16], __ubuf__ int16_t* srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b16(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

__aicore__ inline void TransDataTo5HDIntrinsicsImpl(
    __ubuf__ uint16_t* dstList[16], __ubuf__ uint16_t* srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b16(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

__aicore__ inline void TransDataTo5HDIntrinsicsImpl(
    __ubuf__ half* dstList[16], __ubuf__ half* srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b16(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

template <typename T>
__aicore__ inline void TransDataTo5HDB8IntrinsicsImpl(
    __ubuf__ T* dstList[16], __ubuf__ T* srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    if ((transDataTo5HDParams.dstHighHalf == false) && (transDataTo5HDParams.srcHighHalf == false)) {
        scatter_vnchwconv_b8(
            VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
            transDataTo5HDParams.srcRepStride, false, false);
    } else if ((transDataTo5HDParams.dstHighHalf == false) && (transDataTo5HDParams.srcHighHalf == true)) {
        scatter_vnchwconv_b8(
            VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
            transDataTo5HDParams.srcRepStride, false, true);
    } else if ((transDataTo5HDParams.dstHighHalf == true) && (transDataTo5HDParams.srcHighHalf == true)) {
        scatter_vnchwconv_b8(
            VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
            transDataTo5HDParams.srcRepStride, true, true);
    } else {
        scatter_vnchwconv_b8(
            VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
            transDataTo5HDParams.srcRepStride, true, false);
    }
}

__aicore__ inline void TransDataTo5HDIntrinsicsImpl(
    __ubuf__ int8_t* dstList[16], __ubuf__ int8_t* srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    TransDataTo5HDB8IntrinsicsImpl(dstList, srcList, transDataTo5HDParams);
}

__aicore__ inline void TransDataTo5HDIntrinsicsImpl(
    __ubuf__ uint8_t* dstList[16], __ubuf__ uint8_t* srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    TransDataTo5HDB8IntrinsicsImpl(dstList, srcList, transDataTo5HDParams);
}

template <typename T>
__aicore__ inline void TransDataTo5HDIntrinsicsImpl(
    uint64_t dstList[16], uint64_t srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    ASSERT(false && "TransDataTo5HD with current dtype is not supported on current device!");
}

template <>
__aicore__ inline void TransDataTo5HDIntrinsicsImpl<float>(
    uint64_t dstList[16], uint64_t srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b32(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

template <>
__aicore__ inline void TransDataTo5HDIntrinsicsImpl<int32_t>(
    uint64_t dstList[16], uint64_t srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b32(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

template <>
__aicore__ inline void TransDataTo5HDIntrinsicsImpl<uint32_t>(
    uint64_t dstList[16], uint64_t srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b32(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

template <>
__aicore__ inline void TransDataTo5HDIntrinsicsImpl<int16_t>(
    uint64_t dstList[16], uint64_t srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b16(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

template <>
__aicore__ inline void TransDataTo5HDIntrinsicsImpl<uint16_t>(
    uint64_t dstList[16], uint64_t srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b16(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

template <>
__aicore__ inline void TransDataTo5HDIntrinsicsImpl<half>(
    uint64_t dstList[16], uint64_t srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    scatter_vnchwconv_b16(
        VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
        transDataTo5HDParams.srcRepStride);
}

template <typename T>
__aicore__ inline void TransDataTo5HDB8IntrinsicsImpl(
    uint64_t dstList[16], uint64_t srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    if ((transDataTo5HDParams.dstHighHalf == false) && (transDataTo5HDParams.srcHighHalf == false)) {
        scatter_vnchwconv_b8(
            VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
            transDataTo5HDParams.srcRepStride, false, false);
    } else if ((transDataTo5HDParams.dstHighHalf == false) && (transDataTo5HDParams.srcHighHalf == true)) {
        scatter_vnchwconv_b8(
            VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
            transDataTo5HDParams.srcRepStride, false, true);
    } else if ((transDataTo5HDParams.dstHighHalf == true) && (transDataTo5HDParams.srcHighHalf == true)) {
        scatter_vnchwconv_b8(
            VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
            transDataTo5HDParams.srcRepStride, true, true);
    } else {
        scatter_vnchwconv_b8(
            VA0, VA2, transDataTo5HDParams.repeatTimes, transDataTo5HDParams.dstRepStride,
            transDataTo5HDParams.srcRepStride, true, false);
    }
}

template <>
__aicore__ inline void TransDataTo5HDIntrinsicsImpl<int8_t>(
    uint64_t dstList[16], uint64_t srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    TransDataTo5HDB8IntrinsicsImpl<int8_t>(dstList, srcList, transDataTo5HDParams);
}

template <>
__aicore__ inline void TransDataTo5HDIntrinsicsImpl<uint8_t>(
    uint64_t dstList[16], uint64_t srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    TransDataTo5HDB8IntrinsicsImpl<uint8_t>(dstList, srcList, transDataTo5HDParams);
}

template <typename T>
__aicore__ inline void SetVaReg(__ubuf__ T* dstList[16], __ubuf__ T* srcList[16])
{
    uint64_t vaRegArray1[VA_REG_ARRAY_LEN];
    uint64_t vaRegArray2[VA_REG_ARRAY_LEN];
    uint64_t vaRegArray3[VA_REG_ARRAY_LEN];
    uint64_t vaRegArray4[VA_REG_ARRAY_LEN];

    for (int32_t i = 0; i < VA_REG_ARRAY_LEN; i++) {
        vaRegArray1[i] = (uint64_t)dstList[i];
        vaRegArray2[i] = (uint64_t)dstList[VA_REG_ARRAY_LEN + i];
        vaRegArray3[i] = (uint64_t)srcList[i];
        vaRegArray4[i] = (uint64_t)srcList[VA_REG_ARRAY_LEN + i];
    }

    set_va_reg_sb(VA0, vaRegArray1);
    set_va_reg_sb(VA1, vaRegArray2);
    set_va_reg_sb(VA2, vaRegArray3);
    set_va_reg_sb(VA3, vaRegArray4);
}

__aicore__ inline void SetVaReg(uint64_t dst[NCHW_CONV_ADDR_LIST_SIZE], uint64_t src[NCHW_CONV_ADDR_LIST_SIZE])
{
    set_va_reg_sb(VA0, dst);
    set_va_reg_sb(VA1, dst + VA_REG_ARRAY_LEN);
    set_va_reg_sb(VA2, src);
    set_va_reg_sb(VA3, src + VA_REG_ARRAY_LEN);
}

__aicore__ inline void VldVaReg(__ubuf__ uint64_t* dst, __ubuf__ uint64_t* src)
{
    vld_va_reg(VA0, dst, L128);
    vld_va_reg(VA1, dst, H128);
    vld_va_reg(VA2, src, L128);
    vld_va_reg(VA3, src, H128);
}

template <typename T>
__aicore__ inline void TransDataTo5HDImpl(
    __ubuf__ T* dstList[16], __ubuf__ T* srcList[16], const TransDataTo5HDParams& transDataTo5HDParams)
{
    ASCENDC_ASSERT((SupportType<T, int8_t, uint8_t, int16_t, uint16_t, half, float, int32_t, uint32_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR, "Failed to check dtype in TransDataTo5HD, current api support dtype combination is "
                          "src and dst both: int8_t, uint8_t, int16_t, uint16_t, half, float, int32_t, uint32_t");
    });
    SetVaReg(dstList, srcList);
    TransDataTo5HDIntrinsicsImpl(dstList, srcList, transDataTo5HDParams);
}

template <typename T>
__aicore__ inline void TransDataTo5HDImpl(
    uint64_t dstList[NCHW_CONV_ADDR_LIST_SIZE], uint64_t srcList[NCHW_CONV_ADDR_LIST_SIZE],
    const TransDataTo5HDParams& transDataTo5HDParams)
{
    ASCENDC_ASSERT((SupportType<T, int8_t, uint8_t, int16_t, uint16_t, half, float, int32_t, uint32_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR, "Failed to check dtype in TransDataTo5HD, current api support dtype combination is "
                          "src and dst both: int8_t, uint8_t, int16_t, uint16_t, half, float, int32_t, uint32_t");
    });
    SetVaReg(dstList, srcList);
    TransDataTo5HDIntrinsicsImpl<T>(dstList, srcList, transDataTo5HDParams);
}

template <typename T>
__aicore__ inline void TransDataTo5HDVldVaRegImpl(
    __ubuf__ uint64_t* dst, __ubuf__ uint64_t* src, const TransDataTo5HDParams& transDataTo5HDParams)
{
    ASCENDC_ASSERT((SupportType<T, int8_t, uint8_t, int16_t, uint16_t, half, float, int32_t, uint32_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR, "Failed to check dtype in TransDataTo5HD, current api support dtype combination is "
                          "src and dst both: int8_t, uint8_t, int16_t, uint16_t, half, float, int32_t, uint32_t");
    });
    VldVaReg(dst, src);
    uint64_t dstList[NCHW_CONV_ADDR_LIST_SIZE] = {0};
    uint64_t srcList[NCHW_CONV_ADDR_LIST_SIZE] = {0};
    TransDataTo5HDIntrinsicsImpl<T>(dstList, srcList, transDataTo5HDParams);
}

// Transpose::Level 0
template <typename T>
__aicore__ inline void TransposeIntrinsicsImpl(__ubuf__ T* dst, __ubuf__ T* src)
{
    vtranspose((__ubuf__ uint16_t*)dst, (__ubuf__ uint16_t*)src);
}

// Transpose::Level 0
template <typename T>
__aicore__ inline void TransposeImpl(__ubuf__ T* dst, __ubuf__ T* src)
{
    TransposeIntrinsicsImpl((__ubuf__ uint16_t*)dst, (__ubuf__ uint16_t*)src);
}

template <typename T>
__aicore__ inline void Transpose4DImpl(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const TransposeParamsExt& transposeParams)
{
    ASCENDC_ASSERT(
        false, { KERNEL_LOG(KERNEL_ERROR, "unsupported transpose between NCHW and NHWC on current device!"); });
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_TRANSPOSE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TRANSPOSE_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TRANSPOSE_IMPL_H__
#endif
