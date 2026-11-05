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
 * \file kernel_operator_cube_group_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_cube_group_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CUBE_GROUP_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_CUBE_GROUP_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_CUBE_GROUP_INTERFACE_H

#include "../../../../impl/basic_api/kernel_macros.h"
#include "../../../../impl/basic_api/utils/kernel_utils_macros.h"
#include "../../../../impl/basic_api/utils/kernel_utils_cube_group.h"
#if __NPU_ARCH__ == 2201
#include "../../../../impl/basic_api/dav_c220/core_mng/roc/kernel_operator_cube_group_info.h"
#elif __NPU_ARCH__ == 3510
#include "../../../../impl/basic_api/dav_3510/core_mng/roc/kernel_operator_cube_group_info.h"
#elif __NPU_ARCH__ == 9201
#include "../../../../impl/basic_api/dav_9201/core_mng/roc/kernel_operator_cube_group_info.h"
#elif __NPU_ARCH__ == 9202
#include "../../../../impl/basic_api/dav_9202/core_mng/roc/kernel_operator_cube_group_info.h"
#endif

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
class KfcWorkspace {
public:
    __aicore__ inline KfcWorkspace(GM_ADDR workspace);
    __aicore__ inline void UpdateKfcWorkspace(uint32_t offset);
    __aicore__ inline GM_ADDR GetKfcWorkspace();
    __aicore__ inline ~KfcWorkspace();

private:
    friend __aicore__ inline uint8_t GetEventId(KfcWorkspace& desc);
    __aicore__ inline KfcWorkspace() = delete;
    GM_ADDR msgStart;
    uint8_t evtID;
};

template <typename CubeMsgType>
class CubeResGroupHandle {
public:
    __aicore__ inline CubeResGroupHandle() = default;

    __aicore__ inline CubeResGroupHandle(
        GM_ADDR workspace, uint8_t blockStart, uint8_t blockSize, uint8_t msgQueueSize, uint8_t evtIDIn);

    // aiv call: update aivNumForCurAic + msgHead + msgCurrent by aic, aiv index
    // queueIdIn : index of msgque in in this cubegrouphandle aicSubgroupID: index of aic in this cubegrouphandle
    // aivSubGroupID: index of aiv among aivs for that aic
    __aicore__ inline void AssignQueue(uint8_t queueIdIn);

    // aiv call: check msgState of msgCurrent is FREE, can allocate only when state is FREE
    // to guarantee the order, not allowed to skip, must wait until msgState is updated to FREE
    template <PipeMode pipeMode = PipeMode::SCALAR_MODE>
    __aicore__ inline __gm__ CubeMsgType* AllocMessage();

    // aiv call: use dcci to send msg. return offset, the msg position is msgHead + offset
    template <PipeMode pipeMode = PipeMode::SCALAR_MODE>
    __aicore__ inline uint16_t PostMessage(__gm__ CubeMsgType* msg, CubeMsgType& msgInput);

    // aic call: set msgState to FREE without waiting
    __aicore__ inline uint16_t FreeMessage(__gm__ CubeMsgType* msg);

    // aic call: wait until aiv update msgState to waitState, then aic set state to FREE. Used in FAKE msg.
    __aicore__ inline uint16_t FreeMessage(__gm__ CubeMsgType* msg, CubeMsgState waitState);

    // aiv call: set msgState to FAKE. Then aic can tell status by updating msgState
    __aicore__ inline uint16_t PostFakeMsg(__gm__ CubeMsgType* msg);

    // aiv call: send msg to tell aic to update aivWorkState cause one aiv ends service
    __aicore__ inline void SetQuit(__gm__ CubeMsgType* msg);

    // aic move msgPos + msgCurrent due to skipCnt. do not allow skip to next row.
    // Ex: 1 row has aiv 0~4, current in aiv1. allow skipCnt=3, next pos is aiv0 of next row. Not allow skipCnt=4
    __aicore__ inline void SetSkipMsg(uint8_t skipCnt);

    // aiv call: sync = true: wait until aic finish task and set state to FREE  sync = false: check if aic finish task
    // return true: aic has finished the task      false: aic has not finished yet
    template <bool sync = true>
    __aicore__ inline bool Wait(uint16_t offset);

private:
    template <typename T>
    friend __aicore__ inline bool __IsRun(T handle);

    template <typename T>
    friend __aicore__ inline uint32_t __GetMsgAreaLen(T handle, uint32_t sizeOfCubeMsgStruct);

    // aic call: set index aivID in aivWorkState to 0
    template <typename T>
    friend __aicore__ inline void __SetAivQuit(T* handle, uint8_t aivID);

    // aic call: must wait until msgState in msgCurrent is not FREE
    template <typename T>
    friend __aicore__ inline __gm__ T* __RcvMessage(CubeResGroupHandle<T> handle);

    // aic call: update msgCurrent to next available pos(skipCnt + not quit)
    template <typename T>
    friend __aicore__ inline void __ReleaseMessage(T* handle);

    // return the msgHead of this aic
    __aicore__ inline GM_ADDR __GetAicMsgHead(GM_ADDR workspace, uint8_t aicStart, uint16_t msgSizePerAic);

    // aiv update msgPos + msgCurrent to next place
    __aicore__ inline void __AivUpdateMsgCurrent();

    // aic update msgPos + msgCurrent + aivWork to next place
    __aicore__ inline void __AicUpdateMsgCurrent();

    // true: aiv run   false: aiv quit
    __aicore__ inline bool __AivIsRun(uint8_t aivID);

    // msg data has been prepared. Refresh Scalar of whole CubeMsgType
    __aicore__ inline void __WriteGmCubeMsgByScalar(__gm__ CubeMsgType* msg);

    // set msgState to newState by Scalar
    __aicore__ inline void __WriteGmStateByScalar(__gm__ CubeMsgType* msg, CubeMsgState newState);

    __aicore__ inline void __WriteGmCubeMsgByDatacopy(__gm__ CubeMsgType* msgPtr, CubeMsgType& cubeMsgInput);

    // copy msgInput to GM
    __aicore__ inline void __CopyCubeMsg(__gm__ CubeMsgType* msg, CubeMsgType& msgInput);

    uint8_t aicSize = 0;         // aic num in current CubeGroup
    uint8_t aivSize = 0;         // aiv num in current CubeGroup
    uint8_t aivPerAic = 0;       // each aic has Ceil(aivSize, aicSize) aiv in average
    uint8_t aivNumForCurAic = 0; // in fact, aiv num that current aic has.
    uint8_t queueId = 0;         // queue id in CubeResGroupHandle

    __gm__ CubeMsgType* msgHead;    // start pos(fixed) in this CubeGroup. For aic, 1st block of 1st aiv for this aic
                                    // For aiv, 1st block of that aiv column
    __gm__ CubeMsgType* msgCurrent; // current pointer pos of CubeGroup

    __ubuf__ CubeMsgType* ubMsg; // msg from ub to gm

    uint16_t msgPos = 0;  // number of msgBody that msgCurrent moved from msgHead
                          // aic: in unit of msgBody   aiv: in unit of aivPerAic * msgBody range[0, 3]
    uint16_t msgSize = 0; // total CubeMsg count in whole table. In unit of number of CubeMsg

    // argument for aic only
    uint64_t aivWorkState = 0; // for cur aic, bitmap of aiv sharedTmpBuffer state(len is aivNumForAic)
    uint8_t aivWork = 0;       // msg from which aiv will be read
    uint8_t eventID;
};

template <int groupID, class MatmulApiType, template <class, class> class CallBack, typename CubeMsgType>
__aicore__ inline CubeResGroupHandle<CubeMsgType> CreateCubeResGroup(
    KfcWorkspace& desc, uint8_t blockStart, uint8_t blockSize, uint8_t msgQueueSize, GM_ADDR tiling);
} // namespace AscendC

#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CUBE_GROUP_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_CUBE_GROUP_INTF_H__
#endif
