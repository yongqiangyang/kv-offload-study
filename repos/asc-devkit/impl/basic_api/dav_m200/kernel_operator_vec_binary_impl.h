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
 * \file kernel_operator_vec_binary_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m200/kernel_operator_vec_binary_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_BINARY_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_BINARY_IMPL_H
#include "../kernel_utils.h"
#include "../../../include/basic_api/kernel_struct_binary.h"

namespace AscendC {
/* **************************************************************************************************
 * Add                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void AddIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float, int16_t, int32_t>(),
        "Failed to check dtype in Add, current api support "
        "dtype combination is src and dst both: half / float / int16_t / int32_t.");
    vadd(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// Add::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void AddImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    AddIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AddImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    AddIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Add::Level 2
template <typename T>
__aicore__ inline void AddImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    const BinaryRepeatParams binaryParam;
    AddIntrinsicsImpl(dst, src0, src1, 1, binaryParam);
    SetMaskNorm();
    ResetMask();
}

/* **************************************************************************************************
 * Sub                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void SubIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float, int16_t, int32_t>(),
        "Failed to check dtype in Sub, current api support "
        "dtype combination is src and dst both: half / float / int16_t / int32_t.");
    vsub(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// Sub::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void SubImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    SubIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void SubImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    SubIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Sub::Level 2
template <typename T>
__aicore__ inline void SubImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    const BinaryRepeatParams binaryParam;
    SubIntrinsicsImpl(dst, src0, src1, 1, binaryParam);
    SetMaskNorm();
    ResetMask();
}
/* **************************************************************************************************
 * Mul                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void MulIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float, int16_t, int32_t>(),
        "Failed to check dtype in Mul, current api support "
        "dtype combination is src and dst both: half / float / int16_t / int32_t.");
    vmul(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}
// Mul::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MulImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    MulIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MulImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    MulIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Mul::Level 2
template <typename T>
__aicore__ inline void MulImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    const BinaryRepeatParams binaryParam;
    MulIntrinsicsImpl(dst, src0, src1, 1, binaryParam);
    SetMaskNorm();
    ResetMask();
}
/* **************************************************************************************************
 * Div                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void DivIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float>(), "Failed to check dtype in Div, current api support dtype combination "
                                       "is src and dst both: half / float.");
    vdiv(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// Div::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void DivImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    DivIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void DivImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    DivIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Div::Level 2
template <typename T>
__aicore__ inline void DivImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    const BinaryRepeatParams binaryParam;
    DivIntrinsicsImpl(dst, src0, src1, 1, binaryParam);
    SetMaskNorm();
    ResetMask();
}

/* **************************************************************************************************
 * Max                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void MaxIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float, int16_t, int32_t>(),
        "Failed to check dtype in Max, current api support "
        "dtype combination is src and dst both: half / float / int16_t / int32_t.");
    vmax(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// Max::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MaxImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    MaxIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MaxImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    MaxIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Max::Level 2
template <typename T>
__aicore__ inline void MaxImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    const BinaryRepeatParams binaryParam;
    MaxIntrinsicsImpl(dst, src0, src1, 1, binaryParam);
    SetMaskNorm();
    ResetMask();
}

/* **************************************************************************************************
 * Min                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void MinIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float, int16_t, int32_t>(),
        "Failed to check dtype in Min, current api support "
        "dtype combination is src and dst both: half / float / int16_t / int32_t.");
    vmin(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// Min::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void MinImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    MinIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void MinImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    MinIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Min::Level 2
template <typename T>
__aicore__ inline void MinImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    const BinaryRepeatParams binaryParam;
    MinIntrinsicsImpl(dst, src0, src1, 1, binaryParam);
    SetMaskNorm();
    ResetMask();
}

/* **************************************************************************************************
 * And                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void AndIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, int16_t, uint16_t>(), "Failed to check dtype in And, current api support dtype "
                                             "combination is src and dst both: int16_t / uint16_t.");
    vand(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// And::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void AndImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    AndIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AndImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    AndIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// And::Level 2
template <typename T>
__aicore__ inline void AndImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    const BinaryRepeatParams binaryParam;
    AndIntrinsicsImpl(dst, src0, src1, 1, binaryParam);
    SetMaskNorm();
    ResetMask();
}

/* **************************************************************************************************
 * Or                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void OrIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, int16_t, uint16_t>(), "Failed to check dtype in Or, current api support dtype "
                                             "combination is src and dst both: int16_t / uint16_t.");
    vor(dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// Or::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void OrImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    OrIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void OrImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    OrIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// Or::Level 2
template <typename T>
__aicore__ inline void OrImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    const BinaryRepeatParams binaryParam;
    OrIntrinsicsImpl(dst, src0, src1, 1, binaryParam);
    SetMaskNorm();
    ResetMask();
}

/* **************************************************************************************************
 * AddRelu                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void AddReluIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, int16_t, half, float>(), "Failed to check dtype in AddRelu, current api support dtype "
                                                "combination is src and dst both: int16_t / half / float.");
    vaddrelu(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride, 0, 0);
}

template <typename T>
__aicore__ inline void AddReluImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    vaddrelu(
        (__ubuf__ T*)dst, (__ubuf__ T*)src0, (__ubuf__ T*)src1, 1, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE,
        DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE, 0, 0);
    SetMaskNorm();
    set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
}

// AddRelu::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void AddReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    AddReluIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void AddReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    AddReluIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

/* **************************************************************************************************
 * AddDeqRelu                                             *
 * ************************************************************************************************* */
__aicore__ inline void AddDeqReluIntrinsicsImpl(
    __ubuf__ half* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    vadddeqrelu(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride, 0, 0);
}

__aicore__ inline void AddDeqReluImpl(
    __ubuf__ half* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<half, MaskMode::COUNTER>(0, count);
    vadddeqrelu(
        dst, src0, src1, 1, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, 4, DEFAULT_REPEAT_STRIDE,
        DEFAULT_REPEAT_STRIDE, 0, 0);
    SetMaskNorm();
    set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
}

// AddDeqRelu::Level 0
template <bool isSetMask = true>
__aicore__ inline void AddDeqReluImpl(
    __ubuf__ half* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<int32_t, isSetMask>(mask[1], mask[0]);
    AddDeqReluIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <bool isSetMask = true>
__aicore__ inline void AddDeqReluImpl(
    __ubuf__ half* dst, __ubuf__ int32_t* src0, __ubuf__ int32_t* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<int32_t, isSetMask>(mask);
    AddDeqReluIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

/* **************************************************************************************************
 * FusedMulAdd                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void FusedMulAddIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float>(), "Failed to check dtype in FusedMulAdd, current api support dtype "
                                       "combination is src and dst both: half / float.");
    vmadd(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// FusedMulAdd::Level 2
template <typename T>
__aicore__ inline void FusedMulAddImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    vmadd(
        dst, src0, src1, 1, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE,
        DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE);
    SetMaskNorm();
    set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
}

// FusedMulAdd::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void FusedMulAddImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    FusedMulAddIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void FusedMulAddImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    FusedMulAddIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

/* **************************************************************************************************
 * MulAddDst                                             *
 * ************************************************************************************************* */
template <typename T, typename U>
__aicore__ inline void MulAddDstIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<Tuple<T, U>, Tuple<half, half>, Tuple<float, float>, Tuple<float, half>>(),
        "Failed to "
        "check dtype in MulAddDst, current api support dtype combination is src: half, dst: half / float; src: float, "
        "dst: float.");
    vmla(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// MulAddDst::Level 2
template <typename T, typename U>
__aicore__ inline void MulAddDstImpl(__ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    if constexpr (sizeof(T) == sizeof(U)) {
        vmla(
            dst, src0, src1, 1, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE,
            DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE);
    } else {
        vmla(
            dst, src0, src1, 1, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE,
            HALF_DEFAULT_REPEAT_STRIDE, HALF_DEFAULT_REPEAT_STRIDE);
    }
    SetMaskNorm();
    set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
}

// MulAddDst::Level 0
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void MulAddDstImpl(
    __ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    MulAddDstIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void MulAddDstImpl(
    __ubuf__ T* dst, __ubuf__ U* src0, __ubuf__ U* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    MulAddDstIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

/* **************************************************************************************************
 * FusedMulAddRelu                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void FusedMulAddReluIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, half, float>(), "Failed to check dtype in FusedMulAddRelu, current api support dtype "
                                       "combination is src and dst both: half / float.");
    vmaddrelu(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
}

// FusedMulAddRelu::Level 2
template <typename T>
__aicore__ inline void FusedMulAddReluImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    vmaddrelu(
        dst, src0, src1, 1, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE,
        DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE);
    SetMaskNorm();
    set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
}

// FusedMulAddRelu::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void FusedMulAddReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    FusedMulAddReluIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void FusedMulAddReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    FusedMulAddReluIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

/* **************************************************************************************************
 * SubRelu                                             *
 * ************************************************************************************************* */
template <typename T>
__aicore__ inline void SubReluIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    static_assert(
        SupportType<T, int16_t, half, float>(), "Failed to check dtype in SubRelu, current api support dtype "
                                                "combination is src and dst both: int16_t / half / float.");
    vsubrelu(
        dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride, 0, 0);
}

// SubRelu::Level 0
template <typename T, bool isSetMask = true>
__aicore__ inline void SubReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask[], const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask[1], mask[0]);
    SubReluIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void SubReluImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const uint64_t mask, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    AscendCUtils::SetMask<T, isSetMask>(mask);
    SubReluIntrinsicsImpl(dst, src0, src1, repeatTime, repeatParams);
}

// SubRelu::Level 2
template <typename T>
__aicore__ inline void SubReluImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, const int32_t& count)
{
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, count);
    vsubrelu(
        dst, src0, src1, 1, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE,
        DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE, 0, 0);
    SetMaskNorm();
    set_vector_mask(static_cast<uint64_t>(-1), static_cast<uint64_t>(-1));
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_BINARY_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_IMPL_H__
#endif
