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
 * \file kernel_operator_vec_vconv_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_c100/kernel_operator_vec_vconv_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_VCONV_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_VCONV_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_VCONV_IMPL_H
#include "../kernel_utils.h"
#include "../../../include/basic_api/kernel_struct_binary.h"
#include "../../../include/basic_api/kernel_struct_unary.h"
#include "../../../include/basic_api/kernel_struct_vdeq.h"

namespace AscendC {
__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ int32_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    vconv_deq(
        dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
        repeatParams.srcRepStride);
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ int8_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if (roundMode == RoundMode::CAST_NONE) {
        vconv_s82f16(
            dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
            repeatParams.srcRepStride);
    } else {
        ASCENDC_ASSERT(false, {
            KERNEL_LOG(
                KERNEL_ERROR, "Current RoundMode of Cast from int8_t to half is not supported on current device, only "
                              "RoundMode::CAST_NONE support!");
        });
    }
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ uint8_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if (roundMode == RoundMode::CAST_NONE) {
        vconv_u82f16(
            dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
            repeatParams.srcRepStride);
    } else {
        ASCENDC_ASSERT(false, {
            KERNEL_LOG(
                KERNEL_ERROR, "Current RoundMode of Cast from uint8_t to half is not supported on current device, only "
                              "RoundMode::CAST_NONE support!");
        });
    }
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ int32_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if (roundMode == RoundMode::CAST_NONE) {
        vconv_s322f32(
            dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
            repeatParams.srcRepStride);
    } else {
        ASCENDC_ASSERT(false, {
            KERNEL_LOG(
                KERNEL_ERROR,
                "Current RoundMode of Cast from int32_t to float is not supported on current device, only "
                "RoundMode::CAST_NONE support!");
        });
    }
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ half* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if (roundMode == RoundMode::CAST_NONE) {
        vconv_f162f32(
            dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
            repeatParams.srcRepStride);
    } else {
        ASCENDC_ASSERT(false, {
            KERNEL_LOG(
                KERNEL_ERROR, "Current RoundMode of Cast from half to float is not supported on current device, only "
                              "RoundMode::CAST_NONE "
                              "support!");
        });
    }
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ int32_t* dst, __ubuf__ half* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    switch (roundMode) {
        case RoundMode::CAST_RINT:
            vconv_f162s32r(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_FLOOR:
            vconv_f162s32f(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_CEIL:
            vconv_f162s32c(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_ROUND:
            vconv_f162s32a(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_TRUNC:
            vconv_f162s32z(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_ODD:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "RoundMode::CAST_ODD of Cast from half to int32_t is not supported on current device");
            });
            break;
        case RoundMode::CAST_NONE:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "RoundMode::CAST_NONE of Cast from half to int32_t is not supported on current device");
            });
            break;
        default:
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Cast: An invalid RoundMode!"); });
            break;
    }
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ int8_t* dst, __ubuf__ half* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    switch (roundMode) {
        case RoundMode::CAST_FLOOR:
            vconv_f162s8f(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_CEIL:
            vconv_f162s8c(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_ROUND:
            vconv_f162s8a(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_TRUNC:
            vconv_f162s8z(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_NONE:
            vconv_f162s8(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_ODD:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR, "RoundMode::CAST_ODD of Cast from half to int8_t is not supported on current device");
            });
            break;
        case RoundMode::CAST_RINT:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "RoundMode::CAST_RINT of Cast from half to int8_t is not supported on current device");
            });
            break;
        default:
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Cast: An invalid RoundMode!"); });
            break;
    }
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ uint8_t* dst, __ubuf__ half* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    switch (roundMode) {
        case RoundMode::CAST_FLOOR:
            vconv_f162u8f(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_CEIL:
            vconv_f162u8c(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_ROUND:
            vconv_f162u8a(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_TRUNC:
            vconv_f162u8z(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_NONE:
            vconv_f162u8(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_RINT:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "RoundMode::CAST_RINT of Cast from half to uint8_t is not supported on current device");
            });
            break;
        case RoundMode::CAST_ODD:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "RoundMode::CAST_ODD of Cast from half to uint8_t is not supported on current device");
            });
            break;
        default:
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Cast: An invalid RoundMode!"); });
            break;
    }
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ float* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    switch (roundMode) {
        case RoundMode::CAST_ODD:
            vconv_f322f16o(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_NONE:
            vconv_f322f16(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_RINT:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR, "RoundMode::CAST_RINT of Cast from float to half is not supported on current device");
            });
            break;
        case RoundMode::CAST_FLOOR:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "RoundMode::CAST_FLOOR of Cast from float to half is not supported on current device");
            });
            break;
        case RoundMode::CAST_CEIL:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR, "RoundMode::CAST_CEIL of Cast from float to half is not supported on current device");
            });
            break;
        case RoundMode::CAST_ROUND:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "RoundMode::CAST_ROUND of Cast from float to half is not supported on current device");
            });
            break;
        case RoundMode::CAST_TRUNC:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "RoundMode::CAST_TRUNC of Cast from float to half is not supported on current device");
            });
            break;
        default:
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Cast: An invalid RoundMode!"); });
            break;
    }
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ int32_t* dst, __ubuf__ float* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    switch (roundMode) {
        case RoundMode::CAST_RINT:
            vconv_f322s32r(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_FLOOR:
            vconv_f322s32f(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_CEIL:
            vconv_f322s32c(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_ROUND:
            vconv_f322s32a(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_TRUNC:
            vconv_f322s32z(
                dst, src, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride, repeatParams.dstRepStride,
                repeatParams.srcRepStride);
            break;
        case RoundMode::CAST_ODD:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "RoundMode::CAST_ODD of Cast from float to int32_t is not supported on current device");
            });
            break;
        case RoundMode::CAST_NONE:
            ASCENDC_ASSERT(false, {
                KERNEL_LOG(
                    KERNEL_ERROR,
                    "RoundMode::CAST_NONE of Cast from float to int32_t is not supported on current device");
            });
            break;
        default:
            ASCENDC_ASSERT(false, { KERNEL_LOG(KERNEL_ERROR, "Cast: An invalid RoundMode!"); });
            break;
    }
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ half* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type half to int16_t");
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ uint8_t* dst, __ubuf__ int16_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type int16_t to uint8_t");
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ int8_t* dst, __ubuf__ int16_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type int16_t to int8_t");
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ int16_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type int16_t to half");
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ float* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type float to float");
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ int64_t* dst, __ubuf__ float* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type float to int64_t");
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ float* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type float to int16_t");
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ int16_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type int16_t to float");
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ int16_t* dst, __ubuf__ int32_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type int32_t to int16_t");
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ int64_t* dst, __ubuf__ int32_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type int32_t to int64_t");
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ float* dst, __ubuf__ int64_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type int64_t to float");
}

__aicore__ inline void CastIntrinsicsImpl(
    __ubuf__ int32_t* dst, __ubuf__ int64_t* src, const RoundMode& roundMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Cast from type int64_t to int32_t");
}

// Cast::Level 2
template <typename T, typename U>
__aicore__ inline void CastImpl(__ubuf__ T* dst, __ubuf__ U* src, const RoundMode& roundMode, const uint32_t count)
{
    struct UnaryRepeatParams repeatParams;
    if (sizeof(T) < sizeof(U)) {
        repeatParams.dstRepStride /= 2;
    } else if (sizeof(T) > sizeof(U)) {
        repeatParams.srcRepStride /= 2;
    }

    IntriInfo dstIntriInfo = AscendCUtils::CalIntriInfo(sizeof(T), count, repeatParams.dstRepStride);
    IntriInfo srcIntriInfo = AscendCUtils::CalIntriInfo(sizeof(U), count, repeatParams.srcRepStride);

    uint32_t dstOffset = 0;
    uint32_t srcOffset = 0;
    const auto dstOffsetCount = MAX_REPEAT_TIMES * repeatParams.dstRepStride * dstIntriInfo.c0Count;
    const auto srcOffsetCount = MAX_REPEAT_TIMES * repeatParams.srcRepStride * srcIntriInfo.c0Count;

    uint32_t c0Count = dstIntriInfo.c0Count < srcIntriInfo.c0Count ? dstIntriInfo.c0Count : srcIntriInfo.c0Count;
    const int32_t fullMask = c0Count * DEFAULT_BLK_NUM;
    uint32_t repeatRounding = dstIntriInfo.repeatRounding < srcIntriInfo.repeatRounding ? dstIntriInfo.repeatRounding :
                                                                                          srcIntriInfo.repeatRounding;
    for (int32_t i = 0; i < repeatRounding; i++) {
        CastImpl(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ U*)(src + srcOffset), roundMode, fullMask, MAX_REPEAT_TIMES,
            repeatParams);
        dstOffset += dstOffsetCount;
        srcOffset += srcOffsetCount;
    }
    dstOffset = (dstIntriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.dstRepStride * dstIntriInfo.c0Count;
    srcOffset = (srcIntriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.srcRepStride * srcIntriInfo.c0Count;

    if (dstIntriInfo.repeatRemaining != 0) {
        CastImpl(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ U*)(src + srcOffset), roundMode, fullMask,
            dstIntriInfo.repeatRemaining, repeatParams);
    }

    if (dstIntriInfo.tail != 0) {
        dstOffset = dstIntriInfo.repeat * repeatParams.dstRepStride * dstIntriInfo.c0Count;
        srcOffset = dstIntriInfo.repeat * repeatParams.srcRepStride * srcIntriInfo.c0Count;
        CastImpl(
            (__ubuf__ T*)(dst + dstOffset), (__ubuf__ U*)(src + srcOffset), roundMode, dstIntriInfo.tail, 1,
            repeatParams);
    }
}

// Cast::Level 0 - mask bit mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void CastImpl(
    __ubuf__ T* dst, __ubuf__ U* src, const RoundMode& roundMode, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        if (sizeof(T) >= sizeof(U)) {
            AscendCUtils::SetMask<U>(mask[1], mask[0]);
        } else {
            AscendCUtils::SetMask<T>(mask[1], mask[0]);
        }
    }
    CastIntrinsicsImpl(dst, src, roundMode, repeatTime, repeatParams);
}

// Cast::Level 0 - mask count mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void CastImpl(
    __ubuf__ T* dst, __ubuf__ U* src, const RoundMode& roundMode, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        if (sizeof(T) >= sizeof(U)) {
            AscendCUtils::SetMask<U>(mask);
        } else {
            AscendCUtils::SetMask<T>(mask);
        }
    }
    CastIntrinsicsImpl(dst, src, roundMode, repeatTime, repeatParams);
}

template <typename T, typename U, bool isVecDeq, bool halfBlock>
__aicore__ inline void CastDeqImpl(__ubuf__ T* dst, __ubuf__ U* src, const uint32_t count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "CastDeq");
}

template <typename T, typename U, bool isSetMask, bool isVecDeq, bool halfBlock>
__aicore__ inline void CastDeqImpl(
    __ubuf__ T* dst, __ubuf__ U* src, const uint64_t mask[], uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "CastDeq");
}

template <typename T, typename U, bool isSetMask, bool isVecDeq, bool halfBlock>
__aicore__ inline void CastDeqImpl(
    __ubuf__ T* dst, __ubuf__ U* src, const int32_t mask, uint8_t repeatTime, const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "CastDeq");
}

// AddReluCast::Level 0 - mask count mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void AddReluCastImpl(
    __ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const uint64_t mask, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "AddReluCast");
}

// AddReluCast::Level 0 - mask bit mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void AddReluCastImpl(
    __ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const uint64_t mask[], uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "AddReluCast");
}

// AddReluCast::Level 2
template <typename T, typename U>
__aicore__ inline void AddReluCastImpl(__ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const uint32_t count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "AddReluCast");
}

// SubReluCast::Level 0 - mask count mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void SubReluCastImpl(
    __ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const uint64_t mask, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SubReluCast");
}

// SubReluCast::Level 0 - mask bit mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void SubReluCastImpl(
    __ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const uint64_t mask[], uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SubReluCast");
}

// SubReluCast::Level 2
template <typename T, typename U>
__aicore__ inline void SubReluCastImpl(__ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const uint32_t count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SubReluCast");
}

__aicore__ inline void SetDeqScaleImpl(float scale, int16_t offset, bool signMode)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetDeqScale");
}

template <typename T>
__aicore__ inline void SetDeqScaleImpl(const LocalTensor<T>& vdeq, const VdeqInfo& vdeqInfo)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetDeqScale");
}

template <typename T>
__aicore__ inline void SetDeqScaleImpl(T config)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetDeqScale");
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_VCONV_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_VCONV_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_VCONV_IMPL_H__
#endif
