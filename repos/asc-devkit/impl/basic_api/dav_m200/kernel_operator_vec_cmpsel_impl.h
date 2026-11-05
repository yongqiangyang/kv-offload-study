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
 * \file kernel_operator_vec_cmpsel_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m200/kernel_operator_vec_cmpsel_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CMPSEL_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_CMPSEL_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_CMPSEL_IMPL_H
#include "../../../include/basic_api/kernel_common.h"
#include "../kernel_utils.h"
#include "kernel_operator_vec_duplicate_impl.h"
#include "../../../include/basic_api/kernel_struct_binary.h"
#include "../../../include/basic_api/kernel_struct_unary.h"

namespace AscendC {
/* ***************************************************************************************
 * ************************************** Compare ****************************************
 * ************************************************************************************** */
template <typename T>
__aicore__ inline void VcmpvIntrinsicsImpl(
    __ubuf__ uint8_t* dst, __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    switch (cmpMode) {
        case CMPMODE::LT: {
            vcmpv_lt(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
                repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
                repeatParams.src1RepStride);
            break;
        }
        case CMPMODE::GT: {
            vcmpv_gt(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
                repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
                repeatParams.src1RepStride);
            break;
        }
        case CMPMODE::EQ: {
            vcmpv_eq(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
                repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
                repeatParams.src1RepStride);
            break;
        }
        case CMPMODE::LE: {
            vcmpv_le(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
                repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
                repeatParams.src1RepStride);
            break;
        }
        case CMPMODE::GE: {
            vcmpv_ge(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
                repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
                repeatParams.src1RepStride);
            break;
        }
        case CMPMODE::NE: {
            vcmpv_ne(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
                repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
                repeatParams.src1RepStride);
            break;
        }
        default:
            ASCENDC_ASSERT(
                (false), { KERNEL_LOG(KERNEL_ERROR, "illegal input cmp mode %d", static_cast<int32_t>(cmpMode)); });
            break;
    }
}

template <typename T>
__aicore__ inline void VcmpIntrinsicsImpl(
    __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const BinaryRepeatParams& repeatParams)
{
    switch (cmpMode) {
        case CMPMODE::LT: {
            vcmp_lt(
                src0, src1, 1, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
                repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
            break;
        }
        case CMPMODE::GT: {
            vcmp_gt(
                src0, src1, 1, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
                repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
            break;
        }
        case CMPMODE::EQ: {
            vcmp_eq(
                src0, src1, 1, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
                repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
            break;
        }
        case CMPMODE::LE: {
            vcmp_le(
                src0, src1, 1, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
                repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
            break;
        }
        case CMPMODE::GE: {
            vcmp_ge(
                src0, src1, 1, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
                repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
            break;
        }
        case CMPMODE::NE: {
            vcmp_ne(
                src0, src1, 1, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
                repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
            break;
        }
        default:
            ASCENDC_ASSERT(
                (false), { KERNEL_LOG(KERNEL_ERROR, "illegal input cmp mode %d", static_cast<int32_t>(cmpMode)); });
            break;
    }
}

template <typename T, typename U>
__aicore__ inline void CompareCompute(
    __ubuf__ U* dst, __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const uint32_t count)
{
    struct BinaryRepeatParams repeatParams;
    uint32_t sumRepeat = count * sizeof(T) / ONE_REPEAT_BYTE_SIZE;
    constexpr uint32_t repeatNormal = 252;
    uint32_t repeatRound = sumRepeat / repeatNormal;
    uint32_t repeatTail = sumRepeat % repeatNormal;
    uint32_t srcOffset = repeatNormal * ONE_REPEAT_BYTE_SIZE / sizeof(T);
    uint32_t dstOffset = srcOffset / ONE_BYTE_BIT_SIZE;

    for (uint32_t i = 0; i < repeatRound; ++i) {
        VcmpvImpl(
            reinterpret_cast<__ubuf__ uint8_t*>(dst) + i * dstOffset, src0 + i * srcOffset, src1 + i * srcOffset,
            cmpMode, MASK_PLACEHOLDER, repeatNormal, repeatParams);
    }
    VcmpvImpl(
        reinterpret_cast<__ubuf__ uint8_t*>(dst) + repeatRound * dstOffset, src0 + repeatRound * srcOffset,
        src1 + repeatRound * srcOffset, cmpMode, MASK_PLACEHOLDER, repeatTail, repeatParams);
}

template <typename T, typename U>
__aicore__ inline void CheckVcmpvDtype()
{
    ASCENDC_ASSERT((SupportType<T, half, float>() && SupportType<U, int8_t, uint8_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR, "Failed to check dtype in Compare, current api support dtype combination is src: "
                          "half / float, dst: int8_t / uint8_t.");
    });
}

template <typename T>
__aicore__ inline void CheckVcmpDtype()
{
    ASCENDC_ASSERT((SupportType<T, half, float>()), {
        KERNEL_LOG(
            KERNEL_ERROR, "Failed to check dtype in Compare, "
                          "current api support dtype combination is src: half / float.");
    });
}

template <typename T, typename U>
__aicore__ inline void CheckVcmpvsDtype()
{
    ASCENDC_ASSERT((SupportType<T, half, float>() && SupportType<U, uint8_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR, "Failed to check dtype in Compares / CompareScalar, current api support dtype "
                          "combination is src0: half / float, dst: uint8_t.");
    });
}

// Compare::Level 0 - mask bit mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void VcmpvImpl(
    __ubuf__ U* dst, __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const uint64_t mask[], uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    (void)(mask);
    CheckVcmpvDtype<T, U>();
    VcmpvIntrinsicsImpl(reinterpret_cast<__ubuf__ uint8_t*>(dst), src0, src1, cmpMode, repeatTime, repeatParams);
}

// Compare::Level 0 - mask count mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void VcmpvImpl(
    __ubuf__ U* dst, __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const uint64_t mask, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    (void)(mask);
    CheckVcmpvDtype<T, U>();
    VcmpvIntrinsicsImpl(reinterpret_cast<__ubuf__ uint8_t*>(dst), src0, src1, cmpMode, repeatTime, repeatParams);
}

// Compare::Level 2
template <typename T, typename U>
__aicore__ inline void VcmpvImpl(
    __ubuf__ U* dst, __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const uint32_t count)
{
    CheckVcmpvDtype<T, U>();
    CompareCompute(dst, src0, src1, cmpMode, count);
}

// Compare written to CMPMASK
template <typename T, bool isSetMask = true>
__aicore__ inline void VcmpImpl(
    __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const uint64_t mask[], const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask[1], mask[0]);
    }
    CheckVcmpDtype<T>();
    VcmpIntrinsicsImpl(src0, src1, cmpMode, repeatParams);
}

template <typename T, bool isSetMask = true>
__aicore__ inline void VcmpImpl(
    __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const uint64_t mask, const BinaryRepeatParams& repeatParams)
{
    if constexpr (isSetMask) {
        AscendCUtils::SetMask<T>(mask);
    }
    CheckVcmpDtype<T>();
    VcmpIntrinsicsImpl(src0, src1, cmpMode, repeatParams);
}

/* ***************************************************************************************
 * *********************************** CompareScalar *************************************
 * ************************************************************************************** */
template <typename T>
__aicore__ inline void VcmpvsIntrinsicsImpl(
    __ubuf__ uint8_t* dst, __ubuf__ T* src0, T src1, CMPMODE cmpMode, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    switch (cmpMode) {
        case CMPMODE::LT: {
            vcmpvs_lt(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride,
                repeatParams.dstRepStride, repeatParams.srcRepStride);
            break;
        }
        case CMPMODE::GT: {
            vcmpvs_gt(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride,
                repeatParams.dstRepStride, repeatParams.srcRepStride);
            break;
        }
        case CMPMODE::EQ: {
            vcmpvs_eq(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride,
                repeatParams.dstRepStride, repeatParams.srcRepStride);
            break;
        }
        case CMPMODE::LE: {
            vcmpvs_le(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride,
                repeatParams.dstRepStride, repeatParams.srcRepStride);
            break;
        }
        case CMPMODE::GE: {
            vcmpvs_ge(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride,
                repeatParams.dstRepStride, repeatParams.srcRepStride);
            break;
        }
        case CMPMODE::NE: {
            vcmpvs_ne(
                dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.srcBlkStride,
                repeatParams.dstRepStride, repeatParams.srcRepStride);
            break;
        }
        default:
            ASCENDC_ASSERT(
                (false), { KERNEL_LOG(KERNEL_ERROR, "illegal input cmp mode %d", static_cast<int32_t>(cmpMode)); });
            break;
    }
}

// CompareScalar::Level 0 - mask bit mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void VcmpvsImpl(
    __ubuf__ U* dst, __ubuf__ T* src0, T src1, CMPMODE cmpMode, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    (void)(mask);
    CheckVcmpvsDtype<T, U>();
    VcmpvsIntrinsicsImpl(reinterpret_cast<__ubuf__ uint8_t*>(dst), src0, src1, cmpMode, repeatTime, repeatParams);
}

// CompareScalar::Level 0 - mask count mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void VcmpvsImpl(
    __ubuf__ U* dst, __ubuf__ T* src0, T src1, CMPMODE cmpMode, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    (void)(mask);
    CheckVcmpvsDtype<T, U>();
    VcmpvsIntrinsicsImpl(reinterpret_cast<__ubuf__ uint8_t*>(dst), src0, src1, cmpMode, repeatTime, repeatParams);
}

template <typename T, typename U>
__aicore__ inline void CompareScalarCompute(
    __ubuf__ U* dst, __ubuf__ T* src0, T src1, CMPMODE cmpMode, const uint32_t count)
{
    struct UnaryRepeatParams repeatParams;
    uint32_t sumRepeat = count * sizeof(T) / ONE_REPEAT_BYTE_SIZE;
    constexpr uint32_t repeatNormal = 252;
    uint32_t repeatRound = sumRepeat / repeatNormal;
    uint32_t repeatTail = sumRepeat % repeatNormal;
    uint32_t srcOffset = repeatNormal * ONE_REPEAT_BYTE_SIZE / sizeof(T);
    uint32_t dstOffset = srcOffset / ONE_BYTE_BIT_SIZE;
    for (uint32_t i = 0; i < repeatRound; ++i) {
        VcmpvsImpl(
            reinterpret_cast<__ubuf__ uint8_t*>(dst) + i * dstOffset, src0 + i * srcOffset, src1, cmpMode,
            MASK_PLACEHOLDER, repeatNormal, repeatParams);
    }
    VcmpvsImpl(
        reinterpret_cast<__ubuf__ uint8_t*>(dst) + repeatRound * dstOffset, src0 + repeatRound * srcOffset, src1,
        cmpMode, MASK_PLACEHOLDER, repeatTail, repeatParams);
}

// CompareScalar::Level 2
template <typename T, typename U>
__aicore__ inline void VcmpvsImpl(__ubuf__ U* dst, __ubuf__ T* src0, T src1, CMPMODE cmpMode, const uint32_t count)
{
    CheckVcmpvsDtype<T, U>();
    CompareScalarCompute(reinterpret_cast<__ubuf__ uint8_t*>(dst), src0, src1, cmpMode, count);
}

template <pipe_t pipe>
__aicore__ inline void PipeBarrier();
/* ***************************************************************************************
 * *************************************** Select ****************************************
 * ************************************************************************************** */
// ============ select mode: 0/2 ============
template <typename T>
__aicore__ inline void VselIntrinsicsImplPre(__ubuf__ T* sel, SELMODE selMode)
{
    if (selMode == SELMODE::VSEL_CMPMASK_SPR) {
        set_cmpmask(sel);
        PipeBarrier<PIPE_V>();
    } else if (selMode == SELMODE::VSEL_TENSOR_TENSOR_MODE) {
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        __ubuf__ int64_t* tempBuf = AscendCUtils::GetTemporaryBufferAddr<int64_t>(TMP_UB_OFFSET, 2);
        *(reinterpret_cast<__ubuf__ int64_t*>(tempBuf)) =
            reinterpret_cast<int64_t>(reinterpret_cast<__ubuf__ int64_t*>(sel));
        set_cmpmask(tempBuf);
        AscendCUtils::FreeTemporaryBuffer<int64_t>(tempBuf);
#else
        uint32_t selAddr = static_cast<uint32_t>(reinterpret_cast<int64_t>(reinterpret_cast<__ubuf__ int64_t*>(sel)));
        __ubuf__ uint32_t* tempBuf = AscendCUtils::GetTemporaryBufferAddr<uint32_t>(TMP_UB_OFFSET, ONE_BLK_SIZE);

        AscendCUtils::SetMask<uint32_t>(ONE_BLK_SIZE);
        DuplicateIntrinsicsImpl(tempBuf, selAddr, 1, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE);
        PipeBarrier<PIPE_V>();

        set_cmpmask(tempBuf);
        PipeBarrier<PIPE_V>();

        AscendCUtils::FreeTemporaryBuffer<uint32_t>(tempBuf);
#endif
    }
}

template <typename T, typename U>
__aicore__ inline void VselIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ U* sel, __ubuf__ T* src0, __ubuf__ T* src1, SELMODE selMode, int32_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    (void)sel;
    if (selMode == SELMODE::VSEL_CMPMASK_SPR) {
        vsel(
            dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
            repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
            repeatParams.src1RepStride);
    } else if (selMode == SELMODE::VSEL_TENSOR_TENSOR_MODE) {
        vsel(
            dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
            repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
            repeatParams.src1RepStride, static_cast<uint8_t>(selMode), repeatParams.repeatStrideMode,
            repeatParams.strideSizeMode);
    }
}

// ============ select mode: 1 ============
template <typename T>
__aicore__ inline void VselIntrinsicsImplPre(T src1)
{
    __ubuf__ T* tempBuf = AscendCUtils::GetTemporaryBufferAddr<T>(TMP_UB_OFFSET, ONE_BLK_SIZE);

    AscendCUtils::SetMask<T>(ONE_BLK_SIZE);
    DuplicateIntrinsicsImpl(tempBuf, src1, 1, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE);
    PipeBarrier<PIPE_V>();

    set_cmpmask(tempBuf);
    PipeBarrier<PIPE_V>();

    AscendCUtils::FreeTemporaryBuffer<T>(tempBuf);
}

template <typename T, typename U>
__aicore__ inline void VselIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ U* sel, __ubuf__ T* src0, T src1, SELMODE selMode, int32_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    (void)src1;
    vsel(
        dst, src0, sel, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride,
        static_cast<uint8_t>(selMode), repeatParams.repeatStrideMode, repeatParams.strideSizeMode);
}

template <typename T, SELMODE selMode>
__aicore__ inline void SelectCal(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, int32_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    if constexpr (selMode == SELMODE::VSEL_CMPMASK_SPR) {
        vsel(
            dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
            repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
            repeatParams.src1RepStride);
    } else if constexpr (selMode == SELMODE::VSEL_TENSOR_TENSOR_MODE) {
        vsel(
            dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
            repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
            repeatParams.src1RepStride, static_cast<uint8_t>(selMode), repeatParams.repeatStrideMode,
            repeatParams.strideSizeMode);
    }
}

template <typename T, typename U>
__aicore__ inline void SelectCal(
    __ubuf__ T* dst, __ubuf__ U* sel, __ubuf__ T* src0, int32_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    vsel(
        dst, src0, sel, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride,
        static_cast<uint8_t>(SELMODE::VSEL_TENSOR_SCALAR_MODE), repeatParams.repeatStrideMode,
        repeatParams.strideSizeMode);
}

/* ***************************************************************************************
 * *************************************** Select ****************************************
 * ************************************************************************************** */
// ============ select mode: 0/2 ============
template <typename T, typename U>
__aicore__ inline void VselImpl(
    __ubuf__ T* dst, __ubuf__ U* sel, __ubuf__ T* src0, __ubuf__ T* src1, SELMODE selMode, uint32_t count)
{
    ASCENDC_ASSERT((SupportType<T, half, float>() && SupportType<U, uint8_t, uint16_t, uint32_t, uint64_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to check dtype in Select, current api support dtype combination is dst, "
            "src0Local and src1Local are both: half / float, selMask:  uint8_t / uint16_t / uint32_t / uint64_t");
    });
    BinaryRepeatParams repeatParams;
    SetMaskCount();

    if (selMode == SELMODE::VSEL_CMPMASK_SPR) {
        set_cmpmask(sel);
        PipeBarrier<PIPE_V>();

        AscendCUtils::SetMask<U>(0, count);
        vsel(
            dst, src0, src1, 1, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
            repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride);
        PipeBarrier<PIPE_V>();
    } else if (selMode == SELMODE::VSEL_TENSOR_TENSOR_MODE) {
#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
        __ubuf__ int64_t* tempBuf = AscendCUtils::GetTemporaryBufferAddr<int64_t>(TMP_UB_OFFSET, 2);
        *(reinterpret_cast<__ubuf__ int64_t*>(tempBuf)) =
            reinterpret_cast<int64_t>(reinterpret_cast<__ubuf__ int64_t*>(sel));
        set_cmpmask(tempBuf);
        AscendCUtils::FreeTemporaryBuffer<int64_t>(tempBuf);
#else
        uint32_t selAddr = static_cast<uint32_t>(reinterpret_cast<int64_t>(reinterpret_cast<__ubuf__ int64_t*>(sel)));
        __ubuf__ uint32_t* tempBuf = AscendCUtils::GetTemporaryBufferAddr<uint32_t>(TMP_UB_OFFSET, ONE_BLK_SIZE);

        AscendCUtils::SetMask<U>(0, ONE_BLK_SIZE);
        DuplicateIntrinsicsImpl(tempBuf, selAddr, 1, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE);
        PipeBarrier<PIPE_V>();

        set_cmpmask(tempBuf);
        PipeBarrier<PIPE_V>();

        AscendCUtils::FreeTemporaryBuffer<uint32_t>(tempBuf);
#endif
        AscendCUtils::SetMask<U>(0, count);
        vsel(
            dst, src0, src1, 1, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
            repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride,
            static_cast<uint8_t>(selMode), repeatParams.repeatStrideMode, repeatParams.strideSizeMode);
        PipeBarrier<PIPE_V>();
    }

    ResetMask();
    SetMaskNorm();
}

template <typename T, typename U>
__aicore__ inline void VselImpl(
    __ubuf__ T* dst, __ubuf__ U* sel, __ubuf__ T* src0, T src1, SELMODE selMode, uint32_t count)
{
    ASCENDC_ASSERT((SupportType<T, half, float>() && SupportType<U, uint8_t, uint16_t, uint32_t, uint64_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to check dtype in Select, current api support dtype combination is dst, "
            "src0Local and src1Local are both: half / float, selMask:  uint8_t / uint16_t / uint32_t / uint64_t");
    });
    SetMaskCount();
    __ubuf__ T* tempBuf = AscendCUtils::GetTemporaryBufferAddr<T>(TMP_UB_OFFSET, ONE_BLK_SIZE);

    AscendCUtils::SetMask<U>(0, ONE_BLK_SIZE);
    DuplicateIntrinsicsImpl(tempBuf, src1, 1, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE);
    PipeBarrier<PIPE_V>();

    set_cmpmask(tempBuf);
    PipeBarrier<PIPE_V>();

    AscendCUtils::FreeTemporaryBuffer<T>(tempBuf);

    AscendCUtils::SetMask<U>(0, count);
    BinaryRepeatParams repeatParams;
    vsel(
        dst, src0, sel, 1, repeatParams.dstBlkStride, repeatParams.src0BlkStride, repeatParams.src1BlkStride,
        repeatParams.dstRepStride, repeatParams.src0RepStride, repeatParams.src1RepStride,
        static_cast<uint8_t>(selMode), repeatParams.repeatStrideMode, repeatParams.strideSizeMode);
    PipeBarrier<PIPE_V>();

    ResetMask();
    SetMaskNorm();
}

// select mode: 0/1/2
template <typename T, typename U>
__aicore__ inline void VselImpl(
    __ubuf__ T* dstLocal, __ubuf__ U* selMask, __ubuf__ T* src0Local, __ubuf__ T* src1Local, SELMODE selMode,
    const uint64_t mask[], const uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT((SupportType<T, half, float>() && SupportType<U, uint8_t, uint16_t, uint32_t, uint64_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to check dtype in Select, current api support dtype combination is dst, "
            "src0Local and src1Local are both: half / float, selMask:  uint8_t / uint16_t / uint32_t / uint64_t");
    });
    VselIntrinsicsImplPre(selMask, selMode);
    AscendCUtils::SetMask<T>(mask[1], mask[0]);
    VselIntrinsicsImpl(dstLocal, selMask, src0Local, src1Local, selMode, repeatTime, repeatParams);
}

// select mode: 0/1/2
template <typename T, typename U>
__aicore__ inline void VselImpl(
    __ubuf__ T* dstLocal, __ubuf__ U* selMask, __ubuf__ T* src0Local, __ubuf__ T* src1Local, SELMODE selMode,
    const uint64_t mask, const uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT((SupportType<T, half, float>() && SupportType<U, uint8_t, uint16_t, uint32_t, uint64_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to check dtype in Select, current api support dtype combination is dst, "
            "src0Local and src1Local are both: half / float, selMask:  uint8_t / uint16_t / uint32_t / uint64_t");
    });
    VselIntrinsicsImplPre(selMask, selMode);
    AscendCUtils::SetMask<T>(mask);
    VselIntrinsicsImpl(dstLocal, selMask, src0Local, src1Local, selMode, repeatTime, repeatParams);
}

// select mode: 1
template <typename T, typename U>
__aicore__ inline void VselImpl(
    __ubuf__ T* dstLocal, __ubuf__ U* selMask, __ubuf__ T* src0Local, T src1Local, SELMODE selMode,
    const uint64_t mask[], const uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT((SupportType<T, half, float>() && SupportType<U, uint8_t, uint16_t, uint32_t, uint64_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to check dtype in Select, current api support dtype combination is dst, "
            "src0Local and src1Local are both: half / float, selMask:  uint8_t / uint16_t / uint32_t / uint64_t");
    });
    VselIntrinsicsImplPre(src1Local);
    AscendCUtils::SetMask<T>(mask[1], mask[0]);
    VselIntrinsicsImpl(dstLocal, selMask, src0Local, src1Local, selMode, repeatTime, repeatParams);
}

// select mode: 1
template <typename T, typename U>
__aicore__ inline void VselImpl(
    __ubuf__ T* dstLocal, __ubuf__ U* selMask, __ubuf__ T* src0Local, T src1Local, SELMODE selMode, const uint64_t mask,
    const uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT((SupportType<T, half, float>() && SupportType<U, uint8_t, uint16_t, uint32_t, uint64_t>()), {
        KERNEL_LOG(
            KERNEL_ERROR,
            "Failed to check dtype in Select, current api support dtype combination is dst, "
            "src0Local and src1Local are both: half / float, selMask:  uint8_t / uint16_t / uint32_t / uint64_t");
    });
    VselIntrinsicsImplPre(src1Local);
    AscendCUtils::SetMask<T>(mask);
    VselIntrinsicsImpl(dstLocal, selMask, src0Local, src1Local, selMode, repeatTime, repeatParams);
}

template <typename T>
__aicore__ inline void GetCmpMaskImpl(__ubuf__ T* dst)
{
    get_cmpmask(dst);
}

template <typename T>
__aicore__ inline void SetCmpMaskImpl(__ubuf__ T* src)
{
    set_cmpmask(src);
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_CMPSEL_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CMPSEL_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CMPSEL_IMPL_H__
#endif
