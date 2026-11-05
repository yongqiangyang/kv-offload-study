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
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m300/kernel_operator_vec_createvecindex_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tpipe.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CREATEVECINDEX_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_VEC_CREATEVECINDEX_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_CREATEVECINDEX_IMPL_H
#include "../../../include/basic_api/kernel_tensor.h"
#include "../../../include/basic_api/kernel_common.h"
#include "../../../include/basic_api/kernel_tpipe.h"
#include "../../../include/basic_api/kernel_operator_block_sync_intf.h"

#if ASCENDC_CPU_DEBUG
#include "../kernel_check.h"
#endif

#pragma begin_pipe(V)
namespace AscendC {
constexpr float Fp32IdxIncrement = 64;
const half Fp16IdxIncrement = 128;
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
        SupportType<T, int16_t, int32_t, half, float>(),
        "CreateVecIndex level-2 api only support int16_t/int32_t/half/float/ on current device");
}

// VCI level-0 normal
template <typename T>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<T>& dst, const T firstValue, uint64_t mask, uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    CheckCreateVecIndexApi0SupportedType<T>();
}

template <typename T = int16_t>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<int16_t>& dst, const int16_t firstValue, uint64_t mask, uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    __ubuf__ T* dstLocalAddr = (__ubuf__ T*)dst.GetPhyAddr();
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    constexpr uint32_t blockCount = static_cast<uint32_t>(ONE_BLK_SIZE / sizeof(T));
    uint32_t sreg = static_cast<uint32_t>(mask);
    uint32_t strideConfig1 = ((static_cast<uint32_t>(dstBlkStride)) << 16);

    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsstb(vreg0, dstLocalAddr + i * dstRepStride * blockCount, strideConfig1, preg);
            vadds(vreg0, vreg0, sregLower, preg, MODE_ZEROING);
        }
    }
}

template <typename T = half>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<half>& dst, const half firstValue, uint64_t mask, uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    __ubuf__ T* dstLocalAddr = (__ubuf__ T*)dst.GetPhyAddr();
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    constexpr uint32_t blockCount = static_cast<uint32_t>(ONE_BLK_SIZE / sizeof(T));
    uint32_t sreg = static_cast<uint32_t>(mask);
    uint32_t strideConfig1 = ((static_cast<uint32_t>(dstBlkStride)) << 16);

    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_bool preg = plt_b16(sreg, POST_UPDATE);
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsstb(vreg0, dstLocalAddr + i * dstRepStride * blockCount, strideConfig1, preg);
            vadds(vreg0, vreg0, Fp16IdxIncrement, preg, MODE_ZEROING);
        }
    }
}

template <typename T = int32_t>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<int32_t>& dst, const int32_t firstValue, uint64_t mask, uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    __ubuf__ T* dstLocalAddr = (__ubuf__ T*)dst.GetPhyAddr();
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    constexpr uint32_t blockCount = static_cast<uint32_t>(ONE_BLK_SIZE / sizeof(T));
    uint32_t sreg = static_cast<uint32_t>(mask);
    uint32_t strideConfig1 = ((static_cast<uint32_t>(dstBlkStride)) << 16);

    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsstb(vreg0, dstLocalAddr + i * dstRepStride * blockCount, strideConfig1, preg);
            vadds(vreg0, vreg0, sregLower, preg, MODE_ZEROING);
        }
    }
}

template <typename T = float>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<float>& dst, const float firstValue, uint64_t mask, uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    __ubuf__ T* dstLocalAddr = (__ubuf__ T*)dst.GetPhyAddr();
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    constexpr uint32_t blockCount = static_cast<uint32_t>(ONE_BLK_SIZE / sizeof(T));
    uint32_t sreg = static_cast<uint32_t>(mask);
    uint32_t strideConfig1 = ((static_cast<uint32_t>(dstBlkStride)) << 16);

    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_bool preg = plt_b32(sreg, POST_UPDATE);
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsstb(vreg0, dstLocalAddr + i * dstRepStride * blockCount, strideConfig1, preg);
            vadds(vreg0, vreg0, Fp32IdxIncrement, preg, MODE_ZEROING);
        }
    }
}

// VCI level-0 bitwise
template <typename T>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<T>& dst, const T firstValue, uint64_t mask[], uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    CheckCreateVecIndexApi0SupportedType<T>();
}

template <typename T = int16_t>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<int16_t>& dst, const int16_t firstValue, uint64_t mask[], uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    SetVectorMask<T>(mask[1], mask[0]);

    __ubuf__ int16_t* dstLocalAddr = (__ubuf__ int16_t*)dst.GetPhyAddr();
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    constexpr uint32_t blockCount = static_cast<uint32_t>(ONE_BLK_SIZE / sizeof(T));
    uint32_t strideConfig1 = ((static_cast<uint32_t>(dstBlkStride)) << 16);

    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_bool preg = movp_b16();
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsstb(vreg0, dstLocalAddr + i * dstRepStride * blockCount, strideConfig1, preg);
            vadds(vreg0, vreg0, sregLower, preg, MODE_ZEROING);
        }
    }
}

template <typename T = int32_t>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<int32_t>& dst, const int32_t firstValue, uint64_t mask[], uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    SetVectorMask<T>(mask[1], mask[0]);

    __ubuf__ int32_t* dstLocalAddr = (__ubuf__ int32_t*)dst.GetPhyAddr();
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    constexpr uint32_t blockCount = static_cast<uint32_t>(ONE_BLK_SIZE / sizeof(T));
    uint32_t strideConfig1 = ((static_cast<uint32_t>(dstBlkStride)) << 16);

    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_bool preg = movp_b32();
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsstb(vreg0, dstLocalAddr + i * dstRepStride * blockCount, strideConfig1, preg);
            vadds(vreg0, vreg0, sregLower, preg, MODE_ZEROING);
        }
    }
}

template <typename T = half>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<half>& dst, const half firstValue, uint64_t mask[], uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    SetVectorMask<T>(mask[1], mask[0]);

    __ubuf__ half* dstLocalAddr = (__ubuf__ half*)dst.GetPhyAddr();
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    constexpr uint32_t blockCount = static_cast<uint32_t>(ONE_BLK_SIZE / sizeof(T));
    uint32_t strideConfig1 = ((static_cast<uint32_t>(dstBlkStride)) << 16);

    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_bool preg = movp_b16();
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsstb(vreg0, dstLocalAddr + i * dstRepStride * blockCount, strideConfig1, preg);
            vadds(vreg0, vreg0, Fp16IdxIncrement, preg, MODE_ZEROING);
        }
    }
}

template <typename T = float>
__aicore__ inline void CreateVecIndexCalc(
    LocalTensor<float>& dst, const float firstValue, uint64_t mask[], uint8_t repeatTime, uint16_t dstBlkStride,
    uint8_t dstRepStride)
{
    SetVectorMask<T>(mask[1], mask[0]);

    __ubuf__ float* dstLocalAddr = (__ubuf__ float*)dst.GetPhyAddr();
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    constexpr uint32_t blockCount = static_cast<uint32_t>(ONE_BLK_SIZE / sizeof(T));
    uint32_t strideConfig1 = ((static_cast<uint32_t>(dstBlkStride)) << 16);

    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_bool preg = movp_b32();
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vsstb(vreg0, dstLocalAddr + i * dstRepStride * blockCount, strideConfig1, preg);
            vadds(vreg0, vreg0, Fp32IdxIncrement, preg, MODE_ZEROING);
        }
    }
}

// VCI level-2
template <typename T>
__aicore__ inline void CreateVecIndexCalc(LocalTensor<T> dst, const T firstValue, uint32_t count)
{
    CheckCreateVecIndexApi2SupportedType<T>();
}

template <typename T = int16_t>
__aicore__ inline void CreateVecIndexCalc(LocalTensor<int16_t> dst, const int16_t firstValue, uint32_t count)
{
    __ubuf__ T* dstLocalAddr = (__ubuf__ T*)dst.GetPhyAddr();
    uint32_t sreg = static_cast<uint32_t>(count);
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    uint16_t repeatTime = CeilDivision(count, sregLower);

    __VEC_SCOPE__
    {
        vector_s16 vreg0;
        vector_bool preg;
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vector_bool preg = plt_b16(sreg, POST_UPDATE);
            vsts(vreg0, dstLocalAddr, i * sregLower, NORM_B16, preg);
            vadds(vreg0, vreg0, sregLower, preg, MODE_ZEROING);
        }
    }
}

template <typename T = half>
__aicore__ inline void CreateVecIndexCalc(LocalTensor<half> dst, const half firstValue, uint32_t count)
{
    __ubuf__ T* dstLocalAddr = (__ubuf__ T*)dst.GetPhyAddr();
    uint32_t sreg = static_cast<uint32_t>(count);
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    uint16_t repeatTime = CeilDivision(count, sregLower);

    __VEC_SCOPE__
    {
        vector_f16 vreg0;
        vector_bool preg;
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vector_bool preg = plt_b16(sreg, POST_UPDATE);
            vsts(vreg0, dstLocalAddr, i * sregLower, NORM_B16, preg);
            vadds(vreg0, vreg0, Fp16IdxIncrement, preg, MODE_ZEROING);
        }
    }
}

template <typename T = int32_t>
__aicore__ inline void CreateVecIndexCalc(LocalTensor<int32_t> dst, const int32_t firstValue, uint32_t count)
{
    __ubuf__ T* dstLocalAddr = (__ubuf__ T*)dst.GetPhyAddr();
    uint32_t sreg = static_cast<uint32_t>(count);
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    uint16_t repeatTime = CeilDivision(count, sregLower);

    __VEC_SCOPE__
    {
        vector_s32 vreg0;
        vector_bool preg;
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vector_bool preg = plt_b32(sreg, POST_UPDATE);
            vsts(vreg0, dstLocalAddr, i * sregLower, NORM_B32, preg);
            vadds(vreg0, vreg0, sregLower, preg, MODE_ZEROING);
        }
    }
}

template <typename T = float>
__aicore__ inline void CreateVecIndexCalc(LocalTensor<float> dst, const float firstValue, uint32_t count)
{
    __ubuf__ T* dstLocalAddr = (__ubuf__ T*)dst.GetPhyAddr();
    uint32_t sreg = static_cast<uint32_t>(count);
    constexpr uint32_t sregLower = static_cast<uint32_t>(VECTOR_REG_WIDTH / sizeof(T));
    uint16_t repeatTime = CeilDivision(count, sregLower);

    __VEC_SCOPE__
    {
        vector_f32 vreg0;
        vector_bool preg;
        vci(vreg0, firstValue, INC_ORDER);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            vector_bool preg = plt_b32(sreg, POST_UPDATE);
            vsts(vreg0, dstLocalAddr, i * sregLower, NORM_B32, preg);
            vadds(vreg0, vreg0, Fp32IdxIncrement, preg, MODE_ZEROING);
        }
    }
}
} // namespace AscendC
#pragma end_pipe
#endif // ASCENDC_MODULE_OPERATOR_VEC_CREATEVECINDEX_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CREATEVECINDEX_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CREATEVECINDEX_IMPL_H__
#endif
