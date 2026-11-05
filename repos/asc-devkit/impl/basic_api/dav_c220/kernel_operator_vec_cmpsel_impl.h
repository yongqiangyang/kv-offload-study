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
    "impl/basic_api/dav_c220/kernel_operator_vec_cmpsel_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CMPSEL_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_VEC_CMPSEL_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_CMPSEL_IMPL_H

#include "../kernel_utils.h"
#include "../../../include/basic_api/kernel_struct_binary.h"
#include "kernel_operator_vec_cmp_impl.h"
#include "kernel_operator_vec_duplicate_impl.h"
#include "../../../include/basic_api/kernel_operator_block_sync_intf.h"

namespace AscendC {
/* ***************************************************************************************
 * *************************************** Select ****************************************
 * ************************************************************************************** */
// ============ select mode: 0/2 ============
template <typename T>
__aicore__ inline void VselIntrinsicsImplPre(__ubuf__ T* sel, SELMODE selMode)
{
    if ASCEND_IS_AIC {
        return;
    }

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
    if ASCEND_IS_AIC {
        return;
    }

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
            repeatParams.src1RepStride, static_cast<uint8_t>(selMode));
    }
}

// ============ select mode: 1 ============
template <typename T>
__aicore__ inline void VselIntrinsicsImplPre(T src1)
{
    if ASCEND_IS_AIV {
        __ubuf__ T* tempBuf = AscendCUtils::GetTemporaryBufferAddr<T>(TMP_UB_OFFSET, ONE_BLK_SIZE);

        AscendCUtils::SetMask<T>(ONE_BLK_SIZE);
        DuplicateIntrinsicsImpl(tempBuf, src1, 1, DEFAULT_BLK_STRIDE, DEFAULT_REPEAT_STRIDE);
        PipeBarrier<PIPE_V>();

        set_cmpmask(tempBuf);
        PipeBarrier<PIPE_V>();

        AscendCUtils::FreeTemporaryBuffer<T>(tempBuf);
    }
}

template <typename T, typename U>
__aicore__ inline void VselIntrinsicsImpl(
    __ubuf__ T* dst, __ubuf__ U* sel, __ubuf__ T* src0, T src1, SELMODE selMode, int32_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        (void)src1;
        vsel(
            dst, src0, sel, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
            repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
            repeatParams.src1RepStride, static_cast<uint8_t>(selMode));
    }
}

template <typename T, SELMODE selMode = SELMODE::VSEL_CMPMASK_SPR>
__aicore__ inline void SelectCal(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, int32_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIC {
        return;
    }

    if constexpr (selMode == SELMODE::VSEL_CMPMASK_SPR) {
        vsel(
            dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
            repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
            repeatParams.src1RepStride);
    } else if constexpr (selMode == SELMODE::VSEL_TENSOR_TENSOR_MODE) {
        vsel(
            dst, src0, src1, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
            repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
            repeatParams.src1RepStride, static_cast<uint8_t>(selMode));
    }
}

template <typename T, typename U>
__aicore__ inline void SelectCal(
    __ubuf__ T* dst, __ubuf__ U* sel, __ubuf__ T* src0, int32_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    if ASCEND_IS_AIV {
        vsel(
            dst, src0, sel, repeatTime, repeatParams.dstBlkStride, repeatParams.src0BlkStride,
            repeatParams.src1BlkStride, repeatParams.dstRepStride, repeatParams.src0RepStride,
            repeatParams.src1RepStride, static_cast<uint8_t>(SELMODE::VSEL_TENSOR_SCALAR_MODE));
    }
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
    if ASCEND_IS_AIC {
        return;
    }

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
            static_cast<uint8_t>(selMode));
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
    if ASCEND_IS_AIC {
        return;
    }

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
        static_cast<uint8_t>(selMode));

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
    if ASCEND_IS_AIV {
        VselIntrinsicsImplPre(selMask, selMode);

        AscendCUtils::SetMask<T>(mask[1], mask[0]);
        VselIntrinsicsImpl(dstLocal, selMask, src0Local, src1Local, selMode, repeatTime, repeatParams);
    }
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
    if ASCEND_IS_AIV {
        VselIntrinsicsImplPre(selMask, selMode);

        AscendCUtils::SetMask<T>(mask);
        VselIntrinsicsImpl(dstLocal, selMask, src0Local, src1Local, selMode, repeatTime, repeatParams);
    }
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
    if ASCEND_IS_AIV {
        VselIntrinsicsImplPre(src1Local);

        AscendCUtils::SetMask<T>(mask[1], mask[0]);
        VselIntrinsicsImpl(dstLocal, selMask, src0Local, src1Local, selMode, repeatTime, repeatParams);
    }
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
    if ASCEND_IS_AIV {
        VselIntrinsicsImplPre(src1Local);

        AscendCUtils::SetMask<T>(mask);
        VselIntrinsicsImpl(dstLocal, selMask, src0Local, src1Local, selMode, repeatTime, repeatParams);
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_CMPSEL_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CMPSEL_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_CMPSEL_IMPL_H__
#endif
