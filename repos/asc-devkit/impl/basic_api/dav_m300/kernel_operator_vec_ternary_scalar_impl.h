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
 * \file kernel_operator_vec_ternary_scalar_impl.h
 * \brief AscendC v300 support vaxpy level 0/2 api.
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m300/kernel_operator_vec_ternary_scalar_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tpipe.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H
#include "kernel_operator_common_impl.h"
#include "../kernel_utils.h"
#include "../../../include/basic_api/kernel_struct_unary.h"
#include "../../../include/basic_api/kernel_common.h"
#include "../../../include/basic_api/kernel_tpipe.h"

namespace AscendC {
#define NORMAL_AXPY_IMPL(dst, src, scalarValue, repeatTime, repeatParams, preg, dataBits)                  \
    vector_f##dataBits vreg0;                                                                              \
    vector_f##dataBits vreg1;                                                                              \
    vector_f##dataBits vreg2;                                                                              \
    vector_f##dataBits tempVreg;                                                                           \
    uint32_t srcSm = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << BLOCK_STRIDE_POS_IN_SM);       \
    uint32_t dstSm = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << BLOCK_STRIDE_POS_IN_SM);       \
    for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {                                     \
        vsldb(vreg0, src + i * repeatParams.srcRepStride * B##dataBits##_DATA_NUM_PER_BLOCK, srcSm, preg); \
        vmuls(tempVreg, vreg0, scalarValue, preg, MODE_ZEROING);                                           \
        vsldb(vreg1, dst + i * repeatParams.dstRepStride * B##dataBits##_DATA_NUM_PER_BLOCK, dstSm, preg); \
        vadd(vreg2, tempVreg, vreg1, preg, MODE_ZEROING);                                                  \
        vsstb(vreg2, dst + i * repeatParams.dstRepStride * B##dataBits##_DATA_NUM_PER_BLOCK, dstSm, preg); \
    }

#define COUNTER_AXPY_IMPL(dst, src, scalarValue, count, dataBits)                       \
    vector_f##dataBits vreg0;                                                           \
    vector_f##dataBits vreg1;                                                           \
    vector_f##dataBits vreg2;                                                           \
    vector_f##dataBits tempVreg;                                                        \
    vector_bool preg;                                                                   \
    uint32_t sreg = static_cast<uint32_t>(count);                                       \
    uint16_t repeatTime = CeilDivision(count, B##dataBits##_DATA_NUM_PER_REPEAT);       \
    for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {                  \
        preg = plt_b##dataBits(sreg, POST_UPDATE);                                      \
        vlds(vreg0, src, i* B##dataBits##_DATA_NUM_PER_REPEAT, NORM);                   \
        vmuls(tempVreg, vreg0, scalarValue, preg, MODE_ZEROING);                        \
        vlds(vreg1, dst, i* B##dataBits##_DATA_NUM_PER_REPEAT, NORM);                   \
        vadd(vreg2, tempVreg, vreg1, preg, MODE_ZEROING);                               \
        vsts(vreg2, dst, i* B##dataBits##_DATA_NUM_PER_REPEAT, NORM_B##dataBits, preg); \
    }

#define MIX_AXPY_IMPL(dst, src, scalarValue, repeatTime, repeatParams, preg)                         \
    vector_f16 srcVreg;                                                                              \
    vector_f16 tmpVreg;                                                                              \
    vector_f16 zeroVreg;                                                                             \
    vector_f32 cvtVreg;                                                                              \
    vector_f32 dstVreg;                                                                              \
    vector_f32 addVreg;                                                                              \
    vector_f32 mulVreg;                                                                              \
    vector_bool fullPreg;                                                                            \
    uint32_t fullSreg = FULL_MASK_LEN;                                                               \
    fullPreg = plt_b16(fullSreg, POST_UPDATE);                                                       \
    vdup(zeroVreg, 0, fullPreg, MODE_ZEROING);                                                       \
    uint32_t srcSm = ((static_cast<uint32_t>(repeatParams.srcBlkStride)) << BLOCK_STRIDE_POS_IN_SM); \
    uint32_t dstSm = ((static_cast<uint32_t>(repeatParams.dstBlkStride)) << BLOCK_STRIDE_POS_IN_SM); \
    for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {                               \
        vsldb(srcVreg, src + i * repeatParams.srcRepStride * B16_DATA_NUM_PER_BLOCK, srcSm, preg);   \
        vintlv(srcVreg, tmpVreg, srcVreg, zeroVreg);                                                 \
        vcvt(cvtVreg, srcVreg, preg, PART_EVEN);                                                     \
        vmuls(mulVreg, cvtVreg, scalarValue, preg, MODE_ZEROING);                                    \
        vsldb(dstVreg, dst + i * repeatParams.dstRepStride * B32_DATA_NUM_PER_BLOCK, dstSm, preg);   \
        vadd(addVreg, mulVreg, dstVreg, preg, MODE_ZEROING);                                         \
        vsstb(addVreg, dst + i * repeatParams.dstRepStride * B32_DATA_NUM_PER_BLOCK, dstSm, preg);   \
    }

__aicore__ inline void AxpyIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_bool preg;
        uint32_t sreg = static_cast<uint32_t>(mask);
        preg = plt_b16(sreg, POST_UPDATE);
        NORMAL_AXPY_IMPL(dst, src, scalarValue, repeatTime, repeatParams, preg, 16);
    }
}

__aicore__ inline void AxpyIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_bool preg;
        uint32_t sreg = static_cast<uint32_t>(mask);
        preg = plt_b32(sreg, POST_UPDATE);
        NORMAL_AXPY_IMPL(dst, src, scalarValue, repeatTime, repeatParams, preg, 32);
    }
}

__aicore__ inline void AxpyIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    SetVectorMask<half>(mask[1], mask[0]);

    __VEC_SCOPE__
    {
        vector_bool preg;
        preg = movp_b16();
        NORMAL_AXPY_IMPL(dst, src, scalarValue, repeatTime, repeatParams, preg, 16);
    }
}

__aicore__ inline void AxpyIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    SetVectorMask<float>(mask[1], mask[0]);

    __VEC_SCOPE__
    {
        vector_bool preg;
        preg = movp_b32();
        NORMAL_AXPY_IMPL(dst, src, scalarValue, repeatTime, repeatParams, preg, 32);
    }
}

__aicore__ inline void AxpyIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ half* src, half scalarValue, const int32_t& count)
{
    __VEC_SCOPE__ { COUNTER_AXPY_IMPL(dst, src, scalarValue, count, 16); }
}

__aicore__ inline void AxpyIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, float scalarValue, const int32_t& count)
{
    __VEC_SCOPE__ { COUNTER_AXPY_IMPL(dst, src, scalarValue, count, 32); }
}

__aicore__ inline void AxpyFmixImpl(
    __ubuf__ float* dst, __ubuf__ half* src, half scalarValue, uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __VEC_SCOPE__
    {
        vector_bool preg;
        uint32_t sreg = static_cast<uint32_t>(mask);
        preg = plt_b32(sreg, POST_UPDATE);
        MIX_AXPY_IMPL(dst, src, scalarValue, repeatTime, repeatParams, preg);
    }
}

__aicore__ inline void AxpyFmixImpl(
    __ubuf__ float* dst, __ubuf__ half* src, half scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    __ubuf__ uint64_t* pldsBuffer = AscendCUtils::GetTemporaryBufferAddr<uint64_t>(TMP_UB_OFFSET, PLD_BUFFER_SIZE);
    (*(__ubuf__ uint64_t*)((__ubuf__ uint64_t*)pldsBuffer)) = (static_cast<uint64_t>(mask[0]));
    (*(__ubuf__ uint64_t*)((__ubuf__ uint64_t*)pldsBuffer + 1)) = (static_cast<uint64_t>(mask[1]));
    event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
    SetFlag<HardEvent::S_V>(eventIdSToV);
    WaitFlag<HardEvent::S_V>(eventIdSToV);
    __VEC_SCOPE__
    {
        vector_bool preg;
        plds(preg, ((__ubuf__ uint32_t*)pldsBuffer), 0, US);
        punpack(preg, preg, LOWER);
        MIX_AXPY_IMPL(dst, src, scalarValue, repeatTime, repeatParams, preg);
    }
    AscendCUtils::FreeTemporaryBuffer<uint64_t>(pldsBuffer);
}

__aicore__ inline void AxpyFmixImpl(__ubuf__ float* dst, __ubuf__ half* src, half scalarValue, const int32_t& count)
{
    __VEC_SCOPE__
    {
        vector_f16 srcVreg;
        vector_f16 tmpVreg;
        vector_f16 zeroVreg;
        vector_f32 cvtVreg;
        vector_f32 dstVreg;
        vector_f32 addVreg;
        vector_f32 mulVreg;
        vector_bool preg;
        vector_bool fullPreg;
        uint32_t fullSreg = FULL_MASK_LEN;
        fullPreg = plt_b16(fullSreg, POST_UPDATE);
        vdup(zeroVreg, 0, fullPreg, MODE_ZEROING);
        uint32_t sreg = static_cast<uint32_t>(count);
        uint16_t repeatTime = CeilDivision(count, B32_DATA_NUM_PER_REPEAT);
        for (uint16_t i = 0; i < static_cast<uint16_t>(repeatTime); ++i) {
            preg = plt_b32(sreg, POST_UPDATE);
            vlds(srcVreg, src, i * B32_DATA_NUM_PER_REPEAT, NORM);
            vintlv(srcVreg, tmpVreg, srcVreg, zeroVreg);
            vcvt(cvtVreg, srcVreg, preg, PART_EVEN);
            vmuls(mulVreg, cvtVreg, scalarValue, preg, MODE_ZEROING);
            vlds(dstVreg, dst, i * B32_DATA_NUM_PER_REPEAT, NORM);
            vadd(addVreg, mulVreg, dstVreg, preg, MODE_ZEROING);
            vsts(addVreg, dst, i * B32_DATA_NUM_PER_REPEAT, NORM_B32, preg);
        }
    }
}

// Axpy::Level 0
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void AxpyImpl(
    __ubuf__ T* dst, __ubuf__ U* src, const U& scalarValue, uint64_t mask[], const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if constexpr (sizeof(T) == sizeof(U)) {
        return AxpyIntrinsicsImpl(dst, src, scalarValue, mask, repeatTime, repeatParams);
    } else if constexpr (sizeof(T) > sizeof(U)) {
        return AxpyFmixImpl(dst, src, scalarValue, mask, repeatTime, repeatParams);
    }
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
}

template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void AxpyImpl(
    __ubuf__ T* dst, __ubuf__ U* src, const U& scalarValue, uint64_t mask, const uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if constexpr (sizeof(T) == sizeof(U)) {
        return AxpyIntrinsicsImpl(dst, src, scalarValue, mask, repeatTime, repeatParams);
    } else if constexpr (sizeof(T) > sizeof(U)) {
        return AxpyFmixImpl(dst, src, scalarValue, mask, repeatTime, repeatParams);
    }
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
}

// Add::Level 2
template <typename T, typename U>
__aicore__ inline void AxpyImpl(__ubuf__ T* dst, __ubuf__ U* src, const U& scalarValue, const int32_t& count)
{
    if constexpr (sizeof(T) == sizeof(U)) {
        return AxpyIntrinsicsImpl(dst, src, scalarValue, count);
    } else if constexpr (sizeof(T) > sizeof(U)) {
        return AxpyFmixImpl(dst, src, scalarValue, count);
    }
    ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "current data type is not supported!"); });
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_TERNARY_SCALAR_IMPL_H__
#endif
