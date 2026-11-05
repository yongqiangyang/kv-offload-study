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
 * \file kernel_operator_sync_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m200/kernel_operator_sync_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SYNC_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_SYNC_IMPL_H
#define ASCENDC_MODULE_OPERATOR_SYNC_IMPL_H
namespace AscendC {
__aicore__ inline void ClcSyncCount(
    __gm__ int32_t* localSyncGM, __ubuf__ int32_t* ubWorkspaceAddr, const int32_t blockIdx, const int32_t totalBlocks,
    bool isFirst, int32_t& count)
{
    if (isFirst) {
        __ubuf__ int32_t* localUbAddr = ubWorkspaceAddr + (blockIdx * DEFAULT_BLK_NUM);
        *(reinterpret_cast<__ubuf__ int32_t*>(localUbAddr)) = 1;
        event_t eventIdSToMte3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_MTE3));
        SetFlag<HardEvent::S_MTE3>(eventIdSToMte3);
        WaitFlag<HardEvent::S_MTE3>(eventIdSToMte3);
        copy_ubuf_to_gm(
            static_cast<__gm__ void*>(localSyncGM), static_cast<__ubuf__ void*>(localUbAddr), 0, 1, 1, 0, 0);
        event_t eventIDMTE3ToMTE2 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE3_MTE2));
        SetFlag<HardEvent::MTE3_MTE2>(eventIDMTE3ToMTE2);
        WaitFlag<HardEvent::MTE3_MTE2>(eventIDMTE3ToMTE2);
        count = 1;
        for (int32_t i = 0; i < totalBlocks; i++) {
            if (i != blockIdx) {
                count += *(reinterpret_cast<__ubuf__ int32_t*>(ubWorkspaceAddr) + i * ONE_BLK_FLOAT_NUM);
            }
        }
    } else {
        for (int32_t i = 0; i < totalBlocks; i++) {
            count += *(reinterpret_cast<__ubuf__ int32_t*>(ubWorkspaceAddr) + i * ONE_BLK_FLOAT_NUM);
        }
    }
}

template <bool isAIVOnly = true>
__aicore__ inline void SoftSyncAllImpl(
    __gm__ int32_t* gmWorkspaceAddr, __ubuf__ int32_t* ubWorkspaceAddr, const int usedCores)
{
    __sync_all_stub(usedCores, isAIVOnly);
    PipeBarrier<PIPE_ALL>();

    int32_t totalBlocks = isAIVOnly ? get_block_num() : (GetTaskRationImpl() * get_block_num());
    totalBlocks = usedCores != 0 ? usedCores : totalBlocks;
    int32_t blockIdx = isAIVOnly ? get_block_idx() : GetBlockIdxImpl();

    __gm__ int32_t* localSyncGM = gmWorkspaceAddr + (blockIdx * DEFAULT_BLK_NUM);
    __ubuf__ int32_t* localUbAddr = ubWorkspaceAddr + (blockIdx * DEFAULT_BLK_NUM);

    copy_gm_to_ubuf(static_cast<__ubuf__ void*>(localUbAddr), static_cast<__gm__ void*>(localSyncGM), 0, 1, 1, 0, 0);
    event_t eventIdMte2ToS = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE2_S));
    SetFlag<HardEvent::MTE2_S>(eventIdMte2ToS);
    WaitFlag<HardEvent::MTE2_S>(eventIdMte2ToS);
    int32_t curValue = *(reinterpret_cast<__ubuf__ int32_t*>(localUbAddr)) + 1;
    bool isFirst = curValue == 1 ? true : false;
    *(reinterpret_cast<__ubuf__ int32_t*>(localUbAddr)) = curValue;
    event_t eventIdSToMte3 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_MTE3));
    SetFlag<HardEvent::S_MTE3>(eventIdSToMte3);
    WaitFlag<HardEvent::S_MTE3>(eventIdSToMte3);
    copy_ubuf_to_gm(static_cast<__gm__ void*>(localSyncGM), static_cast<__ubuf__ void*>(localUbAddr), 0, 1, 1, 0, 0);
    event_t eventIDMTE3ToMTE2 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE3_MTE2));
    SetFlag<HardEvent::MTE3_MTE2>(eventIDMTE3ToMTE2);
    WaitFlag<HardEvent::MTE3_MTE2>(eventIDMTE3ToMTE2);
    int32_t totalBlockCount = ONE_BLK_FLOAT_NUM * totalBlocks;
    uint16_t blockLen = totalBlockCount / AscendCUtils::GetC0Count(sizeof(int32_t));
    while (true) {
        copy_gm_to_ubuf(
            static_cast<__ubuf__ void*>(ubWorkspaceAddr), static_cast<__gm__ void*>(gmWorkspaceAddr), 0, 1, blockLen, 0,
            0);
        SetFlag<HardEvent::MTE2_S>(eventIdMte2ToS);
        WaitFlag<HardEvent::MTE2_S>(eventIdMte2ToS);
        int32_t count = 0;
        ClcSyncCount(localSyncGM, ubWorkspaceAddr, blockIdx, totalBlocks, isFirst, count);
        event_t eventIdSToMte2 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_MTE2));
        SetFlag<HardEvent::S_MTE2>(eventIdSToMte2);
        WaitFlag<HardEvent::S_MTE2>(eventIdSToMte2);
        if (count >= (totalBlocks * curValue)) {
            break;
        }
    }
    __sync_all_stub(usedCores, isAIVOnly);
}

template <pipe_t AIV_PIPE = PIPE_MTE3, pipe_t AIC_PIPE = PIPE_MTE3, bool FORCE = false>
__aicore__ inline void SetNextTaskStartImpl()
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "SetNextTaskStart");
}

template <int8_t earlyStartConfig = -1, bool FORCE = false>
__aicore__ inline void WaitPreTaskEndImpl()
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "WaitPreTaskEnd");
}

template <bool isAIVOnly = true>
__aicore__ inline void SyncAllImpl()
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Hardware SyncAll");
}

template <uint8_t modeId, pipe_t pipe>
__aicore__ inline void NotifyEventImpl(uint16_t flagId)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "NotifyEvent");
}

template <uint8_t modeId, pipe_t pipe>
__aicore__ inline void WaitEventImpl(uint16_t flagId)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "WaitEvent");
}

__aicore__ inline void SetSyncBaseAddr(uint64_t config) { ASCENDC_REPORT_NOT_SUPPORT(false, "SetSyncBaseAddr"); }
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_SYNC_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SYNC_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_SYNC_IMPL_H__
#endif
