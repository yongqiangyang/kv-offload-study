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
    "impl/basic_api/dav_c100/kernel_operator_vec_cmpsel_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CMPSEL_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_CMPSEL_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_CMPSEL_IMPL_H
#include "../../../include/basic_api/kernel_common.h"
#include "../kernel_utils.h"
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

// Compare::Level 0 - mask bit mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void VcmpvImpl(
    __ubuf__ U* dst, __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const uint64_t mask[], uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    (void)(mask);
    VcmpvIntrinsicsImpl(reinterpret_cast<__ubuf__ uint8_t*>(dst), src0, src1, cmpMode, repeatTime, repeatParams);
}

// Compare::Level 0 - mask count mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void VcmpvImpl(
    __ubuf__ U* dst, __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const uint64_t mask, uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    (void)(mask);
    VcmpvIntrinsicsImpl(reinterpret_cast<__ubuf__ uint8_t*>(dst), src0, src1, cmpMode, repeatTime, repeatParams);
}

// Compare::Level 2
template <typename T, typename U>
__aicore__ inline void VcmpvImpl(
    __ubuf__ U* dst, __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const uint32_t count)
{
    CompareCompute(dst, src0, src1, cmpMode, count);
}

// Compare written to CMPMASK
template <typename T, bool isSetMask = true>
__aicore__ inline void VcmpImpl(
    __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const uint64_t mask[], const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Compare written to CMPMASK");
}

template <typename T, bool isSetMask = true>
__aicore__ inline void VcmpImpl(
    __ubuf__ T* src0, __ubuf__ T* src1, CMPMODE cmpMode, const uint64_t mask, const BinaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Compare written to CMPMASK");
}

/* ***************************************************************************************
 * *********************************** CompareScalar *************************************
 * ************************************************************************************** */
// CompareScalar::Level 0 - mask bit mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void VcmpvsImpl(
    __ubuf__ U* dst, __ubuf__ T* src0, T src1, CMPMODE cmpMode, const uint64_t mask[], uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "CompareScalar");
}

// CompareScalar::Level 0 - mask count mode
template <typename T, typename U, bool isSetMask = true>
__aicore__ inline void VcmpvsImpl(
    __ubuf__ U* dst, __ubuf__ T* src0, T src1, CMPMODE cmpMode, const uint64_t mask, uint8_t repeatTime,
    const UnaryRepeatParams& repeatParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "CompareScalar");
}

// CompareScalar::Level 2
template <typename T, typename U>
__aicore__ inline void VcmpvsImpl(__ubuf__ U* dst, __ubuf__ T* src0, T src1, CMPMODE cmpMode, const uint32_t count)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "CompareScalar");
}

/* ***************************************************************************************
 * *************************************** Select ****************************************
 * ************************************************************************************** */
// ============ select mode: 0/2 ============
template <typename T, typename U>
__aicore__ inline void VselIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ U* sel, __ubuf__ T* src0, __ubuf__ T* src1, SELMODE selMode, int32_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if (selMode == SELMODE::VSEL_CMPMASK_SPR) {
        set_cmpmask(sel);
        PipeBarrierImpl<PIPE_V>();
        vsel(
            dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
            repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
            repeatParams.src1RepStride);
    } else {
        ASCENDC_ASSERT(
            false, { KERNEL_LOG(KERNEL_ERROR, "current device only support select mode 0 (VSEL_CMPMASK_SPR) !"); });
    }
}

template <typename T, SELMODE selMode = SELMODE::VSEL_CMPMASK_SPR>
__aicore__ inline void SelectCal(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, int32_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    if constexpr (selMode == SELMODE::VSEL_CMPMASK_SPR) {
        vsel(
            dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
            repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
            repeatParams.src1RepStride);
    } else {
        ASCENDC_ASSERT(
            false, { KERNEL_LOG(KERNEL_ERROR, "current device only support select mode 0 (VSEL_CMPMASK_SPR) !"); });
    }
}

template <typename T, typename U>
__aicore__ inline void SelectCal(
    __ubuf__ T* dst, __ubuf__ U* sel, __ubuf__ T* src0, int32_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(
        false, { KERNEL_LOG(KERNEL_ERROR, "current device only support select mode 0 (VSEL_CMPMASK_SPR) !"); });
}

/* ***************************************************************************************
 * *************************************** Select ****************************************
 * ************************************************************************************** */
// ============ select mode: 0/2 ============
// only for sel mode 0, cuz no selmask offset
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
    IntriInfo intriInfo = AscendCUtils::CalIntriInfo(sizeof(T), count);

    uint32_t dstOffset = 0;
    uint32_t src0Offset = 0;
    uint32_t src1Offset = 0;
    const auto dstOffsetCount = MAX_REPEAT_TIMES * repeatParams.dstRepStride * intriInfo.c0Count;
    const auto src0OffsetCount = MAX_REPEAT_TIMES * repeatParams.src0RepStride * intriInfo.c0Count;
    const auto src1OffsetCount = MAX_REPEAT_TIMES * repeatParams.src1RepStride * intriInfo.c0Count;

    const int32_t fullMask = intriInfo.c0Count * DEFAULT_BLK_NUM;
    AscendCUtils::SetMask<T>(fullMask);

    for (int32_t i = 0; i < intriInfo.repeatRounding; i++) {
        VselIntrinsicsImpl(
            reinterpret_cast<__ubuf__ T*>(dst + dstOffset), reinterpret_cast<__ubuf__ U*>(sel),
            reinterpret_cast<__ubuf__ T*>(src0 + src0Offset), reinterpret_cast<__ubuf__ T*>(src1 + src1Offset), selMode,
            MAX_REPEAT_TIMES, repeatParams);
        dstOffset += dstOffsetCount;
        src0Offset += src0OffsetCount;
        src1Offset += src1OffsetCount;
    }

    dstOffset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.dstRepStride * intriInfo.c0Count;
    src0Offset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.src0RepStride * intriInfo.c0Count;
    src1Offset = (intriInfo.repeatRounding * MAX_REPEAT_TIMES) * repeatParams.src1RepStride * intriInfo.c0Count;

    if (intriInfo.repeatRemaining != 0) {
        VselIntrinsicsImpl(
            reinterpret_cast<__ubuf__ T*>(dst + dstOffset), reinterpret_cast<__ubuf__ U*>(sel),
            reinterpret_cast<__ubuf__ T*>(src0 + src0Offset), reinterpret_cast<__ubuf__ T*>(src1 + src1Offset), selMode,
            intriInfo.repeatRemaining, repeatParams);
    }

    if (intriInfo.tail != 0) {
        AscendCUtils::SetMask<T>(intriInfo.tail);
        // cal sel mask offset, only for sel mode 0
        int32_t selMaskOriginOffset = intriInfo.repeat * DEFAULT_REPEAT_STRIDE * intriInfo.c0Count;
        int32_t selMaskOffset = selMaskOriginOffset / AscendCUtils::GetBitSize(sizeof(U));

        dstOffset = intriInfo.repeat * repeatParams.dstRepStride * intriInfo.c0Count;
        src0Offset = intriInfo.repeat * repeatParams.src0RepStride * intriInfo.c0Count;
        src1Offset = intriInfo.repeat * repeatParams.src1RepStride * intriInfo.c0Count;
        VselIntrinsicsImpl(
            reinterpret_cast<__ubuf__ T*>(dst + dstOffset), reinterpret_cast<__ubuf__ U*>(sel),
            reinterpret_cast<__ubuf__ T*>(src0 + src0Offset), reinterpret_cast<__ubuf__ T*>(src1 + src1Offset), selMode,
            1, repeatParams);
        ResetMask();
    }
}

template <typename T, typename U>
__aicore__ inline void VselImpl(
    __ubuf__ T* dst, __ubuf__ U* sel, __ubuf__ T* src0, T src1, SELMODE selMode, uint32_t count)
{
    ASCENDC_ASSERT(
        false, { KERNEL_LOG(KERNEL_ERROR, "current device only support select mode 0 (VSEL_CMPMASK_SPR) !"); });
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
    AscendCUtils::SetMask<T>(mask);
    VselIntrinsicsImpl(dstLocal, selMask, src0Local, src1Local, selMode, repeatTime, repeatParams);
}

// select mode: 1
template <typename T, typename U>
__aicore__ inline void VselImpl(
    __ubuf__ T* dstLocal, __ubuf__ U* selMask, __ubuf__ T* src0Local, T src1Local, SELMODE selMode,
    const uint64_t mask[], const uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(
        false, { KERNEL_LOG(KERNEL_ERROR, "current device only support select mode 0 (VSEL_CMPMASK_SPR) !"); });
}

// select mode: 1
template <typename T, typename U>
__aicore__ inline void VselImpl(
    __ubuf__ T* dstLocal, __ubuf__ U* selMask, __ubuf__ T* src0Local, T src1Local, SELMODE selMode, const uint64_t mask,
    const uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    ASCENDC_ASSERT(
        false, { KERNEL_LOG(KERNEL_ERROR, "current device only support select mode 0 (VSEL_CMPMASK_SPR) !"); });
}

template <typename T>
__aicore__ inline void GetCmpMaskImpl(__ubuf__ T* dst)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "GetCmpMask");
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
