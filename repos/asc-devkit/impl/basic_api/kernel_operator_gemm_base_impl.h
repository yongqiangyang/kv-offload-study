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
 * \file kernel_operator_gemm_base_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_gemm_base_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_gemm_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GEMM_BASE_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_GEMM_BASE_IMPL_H
#define ASCENDC_MODULE_OPERATOR_GEMM_BASE_IMPL_H
#if ASCENDC_CPU_DEBUG
#include <unordered_set>
#endif
#include "../../include/basic_api/kernel_tensor.h"
#include "kernel_operator_mm_base_impl.h"
#include "../../include/basic_api/kernel_struct_conv2d.h"
#include "../../include/basic_api/kernel_struct_mm.h"
#include "../../include/basic_api/kernel_tpipe.h"
#include "../../include/basic_api/kernel_operator_block_sync_intf.h"

namespace AscendC {
#if ASCENDC_CPU_DEBUG

const std::unordered_set<std::string> MATMUL_SUPPORT_TYPE{"s8s8s32", "f16f16f32", "f16f16f16"};

template <typename T>
__aicore__ inline std::string GetTypeStr(const LocalTensor<T>& input)
{
    if (std::is_same<PrimT<T>, uint8_t>::value) {
        return "u8";
    } else if (std::is_same<PrimT<T>, int8_t>::value) {
        return "s8";
    } else if (std::is_same<PrimT<T>, half>::value) {
        return "f16";
    } else if (std::is_same<PrimT<T>, float>::value) {
        return "f32";
    } else if (std::is_same<PrimT<T>, int32_t>::value) {
        return "s32";
    } else {
        return "None";
    }
}

__aicore__ inline bool CheckRange(std::pair<uint32_t, uint32_t>& range, const uint32_t num)
{
    if (num < range.first || num > range.second) {
        return false;
    } else {
        return true;
    }
}

template <typename T, typename U, typename S>
__aicore__ inline bool CheckOverflow(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<S>& src1, const uint32_t m,
    const uint32_t k, const uint32_t n, GemmTiling& tiling)
{
    // check l0c
    uint32_t roundM = DivCeil(m, tiling.blockSize) * tiling.blockSize;
    uint32_t roundN = DivCeil(n, tiling.blockSize) * tiling.blockSize;
    uint32_t roundK = DivCeil(k, tiling.c0Size) * tiling.c0Size;

    uint32_t needElementLoc = roundM * roundN * sizeof(uint32_t);
    if (needElementLoc > TOTAL_L0C_SIZE) {
        return false;
    }

    // check l0a:
    uint32_t needElementL0a = roundM * roundK * sizeof(PrimT<U>);

    // check l0b:
    uint32_t needElementL0b = roundN * roundK * sizeof(PrimT<S>);
    if ((needElementL0b + needElementL0a) > TOTAL_L1_SIZE) {
        return false;
    }

    return true;
}

template <typename T, typename U, typename S>
__aicore__ inline bool CheckParams(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<S>& src1, const uint32_t m,
    const uint32_t k, const uint32_t n, GemmTiling& tiling)
{
    // check c0Size
    if (tiling.c0Size != 16 && tiling.c0Size != 32) {
        return false;
    }
    // check scope
    const Hardware src0Scope = GetPhyType((TPosition)src0.GetPosition());
    const Hardware src1Scope = GetPhyType((TPosition)src1.GetPosition());
    const Hardware dstScope = GetPhyType((TPosition)dst.GetPosition());
    if (src0Scope != Hardware::L1 || src1Scope != Hardware::L1) {
        return false;
    }
    if (dstScope != Hardware::UB && dstScope != Hardware::L0C) {
        return false;
    }

    // check dtype
    std::string dtypeStr = GetTypeStr(src0) + GetTypeStr(src1) + GetTypeStr(dst);
    if (MATMUL_SUPPORT_TYPE.find(dtypeStr) == MATMUL_SUPPORT_TYPE.end()) {
        return false;
    }

    // check m/k/n range
    std::pair<uint32_t, uint32_t> mRange(1, 4096);
    std::pair<uint32_t, uint32_t> nRange = mRange;
    std::pair<uint32_t, uint32_t> kRange;
    if (std::is_same<U, half>::value) {
        kRange = std::make_pair(1, 32768);
    } else {
        kRange = std::make_pair(1, 16384);
    }

    if (!CheckRange(mRange, m) || !CheckRange(nRange, n) || !CheckRange(kRange, k)) {
        return false;
    }

    // check overflow
    if (!CheckOverflow(dst, src0, src1, m, k, n, tiling)) {
        return false;
    }

    return true;
}
#endif

__aicore__ inline void CalculateGemmTiling(GemmTiling& tiling)
{
    tiling.mIterNum = 1;
    tiling.nIterNum = 1;
    tiling.kIterNum = DivCeil(tiling.kBlockNum, tiling.kTileBlock);

    tiling.mTileBlock = DivCeil(tiling.mBlockNum, tiling.mIterNum);
    tiling.nTileBlock = DivCeil(tiling.nBlockNum, tiling.nIterNum);

    tiling.kTailBlock = tiling.kBlockNum - (tiling.kIterNum - 1) * tiling.kTileBlock;
    tiling.mTailBlock = tiling.mBlockNum - (tiling.mIterNum - 1) * tiling.mTileBlock; // mTailBlock <= mBlockNum
    tiling.nTailBlock = tiling.nBlockNum - (tiling.nIterNum - 1) * tiling.nTileBlock;

    tiling.kHasTail = tiling.kTailBlock != tiling.kTileBlock;
    tiling.kHasTailEle = tiling.roundK != tiling.kNum;
    tiling.kTailEle = tiling.kNum % (tiling.kTileBlock * tiling.c0Size);

    if (tiling.mNum != tiling.mTileBlock * tiling.blockSize) {
        tiling.mHasTail = true;
    } else {
        tiling.mHasTail = false;
    }
    tiling.nHasTail = tiling.nTileBlock != tiling.nTailBlock;
}

template <typename T>
__aicore__ inline void LoadL0B(
    uint32_t kBlocks, uint32_t nBlocks, GemmTiling tiling, uint32_t i, uint32_t j, const LocalTensor<T>& src1,
    const LocalTensor<T>& l0b)
{
    if (tiling.nIterNum == 1) {
        uint32_t wSize = tiling.blockSize * tiling.c0Size;
        uint32_t wIdx = (i * tiling.kTileBlock * tiling.nBlockNum + j * tiling.nTileBlock) * wSize;
        LoadData2DParams params;
        params.startIndex = 0;
        params.repeatTimes = kBlocks * nBlocks;
        params.srcStride = 1;
        LoadDataImpl(l0b, src1[wIdx], params);
    } else {
        // load data row by row
        for (size_t index = 0; index < kBlocks; ++index) {
            uint32_t wSize = j * tiling.nTileBlock * tiling.blockSize * tiling.c0Size;
            uint32_t wIdx =
                (i * tiling.kTileBlock + index) * tiling.nBlockNum * tiling.blockSize * tiling.c0Size + wSize;
            uint32_t l0bIdx = index * nBlocks * tiling.blockSize * tiling.c0Size;
            LoadData2DParams params;
            params.startIndex = 0;
            params.repeatTimes = nBlocks;
            params.srcStride = 1;
            LoadDataImpl(l0b[l0bIdx], src1[wIdx], params);
        }
    }
}

template <typename T>
__aicore__ inline void LoadL0A(
    uint32_t kBlocks, uint32_t mBlocks, GemmTiling tiling, uint32_t i, uint32_t t, const LocalTensor<T>& src0,
    const LocalTensor<T>& l0a)
{
    if (kBlocks == 1) {
        uint32_t l1aSize = i * tiling.kTileBlock * tiling.mBlockNum * tiling.blockSize * tiling.c0Size;
        uint32_t l1aOffset = t * tiling.mTileBlock * tiling.blockSize * tiling.c0Size + l1aSize;
        LoadData2DParams params;
        params.startIndex = 0;
        params.repeatTimes = mBlocks;
        params.srcStride = 1;
        LoadDataImpl(l0a, src0[l1aOffset], params);
    } else {
        // load data row by row
        for (size_t index = 0; index < mBlocks; index++) {
            uint32_t l0aOffset = index * kBlocks * tiling.blockSize * tiling.c0Size;
            uint32_t l1aOffset = (t * tiling.mTileBlock + index) * tiling.blockSize * tiling.c0Size +
                                 i * tiling.kTileBlock * tiling.mBlockNum * tiling.blockSize * tiling.c0Size;
            LoadData2DParams params;
            params.startIndex = 0;
            params.repeatTimes = kBlocks;
            params.srcStride = tiling.mBlockNum;
            LoadDataImpl(l0a[l0aOffset], src0[l1aOffset], params);
        }
    }
}

template <typename T, typename U, typename S>
__aicore__ inline void MmadFunc(
    const LocalTensor<U>& l0a, const LocalTensor<S>& l0b, const LocalTensor<T>& l0c, int32_t initValue,
    GemmTiling tiling, size_t i)
{
    MmadParams mmadParams;
    mmadParams.m = tiling.mTileBlock * tiling.blockSize;
    mmadParams.n = tiling.nTileBlock * tiling.blockSize;
    mmadParams.isBias = 1;

    if (tiling.kIterNum == 1) {
        mmadParams.k = tiling.kNum;
        mmadParams.isBias = initValue;
    } else if (initValue == 1 && tiling.kHasTailEle) {
        if (i == tiling.kIterNum - 1) {
            mmadParams.k = tiling.kTailEle;
        } else {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
        }
    } else if (initValue != 1 && tiling.kHasTailEle) {
        if (i == 0) {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
            mmadParams.isBias = 0;
        } else if (i == tiling.kIterNum - 1) {
            mmadParams.k = tiling.kTailEle;
        } else {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
        }
    } else if (initValue == 1 && !tiling.kHasTailEle) {
        if (i == tiling.kIterNum - 1) {
            mmadParams.k = tiling.kTailBlock * tiling.c0Size;
        } else {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
        }
    } else {
        if (i == 0) {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
            mmadParams.isBias = 0;
        } else if (i == tiling.kIterNum - 1) {
            mmadParams.k = tiling.kTailBlock * tiling.c0Size;
        } else {
            mmadParams.k = tiling.kTileBlock * tiling.c0Size;
        }
    }
    MmadImpl(l0c, l0a, l0b, mmadParams);
}

template <typename T, typename U>
__aicore__ inline void GetPingPongBuffer(
    LocalTensor<T>& l0aPing, LocalTensor<T>& l0aPong, LocalTensor<U>& l0bPing, LocalTensor<U>& l0bPong)
{
    // L0Abuffer
    TBuffAddr tbufaPing;
    tbufaPing.logicPos = static_cast<uint8_t>(TPosition::A2);
    l0aPing.SetAddr(tbufaPing);
    l0aPing.InitBuffer(0, TOTAL_L0A_SIZE / 2 / sizeof(PrimT<T>));

    TBuffAddr tbufaPong;
    tbufaPong.logicPos = static_cast<uint8_t>(TPosition::A2);
    l0aPong.SetAddr(tbufaPong);
    l0aPong.InitBuffer(TOTAL_L0A_SIZE / 2, TOTAL_L0A_SIZE / 2 / sizeof(PrimT<T>));

    // L0Bbuffer
    TBuffAddr tbufbPing;
    tbufbPing.logicPos = static_cast<uint8_t>(TPosition::B2);
    l0bPing.SetAddr(tbufbPing);
    l0bPing.InitBuffer(0, TOTAL_L0B_SIZE / 2 / sizeof(PrimT<U>));

    TBuffAddr tbufbPong;
    tbufbPong.logicPos = static_cast<uint8_t>(TPosition::B2);
    l0bPong.SetAddr(tbufbPong);
    l0bPong.InitBuffer(TOTAL_L0B_SIZE / 2, TOTAL_L0B_SIZE / 2 / sizeof(PrimT<U>));
    return;
}

template <typename T, typename U>
__aicore__ inline void GetSingleThreadBuffer(LocalTensor<T>& l0a, LocalTensor<U>& l0b)
{
    // L0Abuffer
    TBuffAddr tbufa;
    tbufa.logicPos = static_cast<uint8_t>(TPosition::A2);
    l0a.SetAddr(tbufa);
    l0a.InitBuffer(0, TOTAL_L0A_SIZE / sizeof(PrimT<T>));

    // L0Bbuffer
    TBuffAddr tbufb;
    tbufb.logicPos = static_cast<uint8_t>(TPosition::B2);
    l0b.SetAddr(tbufb);
    l0b.InitBuffer(0, TOTAL_L0B_SIZE / sizeof(PrimT<U>));
    return;
}

template <typename T, typename U, typename S>
__aicore__ inline void GemmExecNmNopingpong(
    const LocalTensor<T>& l0c, const LocalTensor<U>& src0, const LocalTensor<S>& src1, GemmTiling tiling,
    const int32_t initValue)
{
    LocalTensor<U> l0a;
    LocalTensor<S> l0b;
    GetSingleThreadBuffer(l0a, l0b);
    event_t eventIdMToMte1 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::M_MTE1));
    SetFlag<HardEvent::M_MTE1>(eventIdMToMte1);
    for (size_t indexK = 0; indexK < tiling.kIterNum; indexK++) {
        uint32_t kBlocks = tiling.kTileBlock;
        if (indexK == tiling.kIterNum - 1) {
            kBlocks = tiling.kTailBlock;
        }
        WaitFlag<HardEvent::M_MTE1>(eventIdMToMte1);
        for (size_t indexN = 0; indexN < tiling.nIterNum; indexN++) {
            // load data from l1 to l0b
            LoadL0B(kBlocks, tiling.nTileBlock, tiling, indexK, indexN, src1, l0b);
            for (size_t indexM = 0; indexM < tiling.mIterNum; indexM++) {
                // load data from l1 to l0a
                LoadL0A(kBlocks, tiling.mTileBlock, tiling, indexK, indexM, src0, l0a);
                event_t eventIdMte1ToM = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE1_M));
                SetFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                WaitFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                PipeBarrier<PIPE_M>();
                MmadFunc(l0a, l0b, l0c, initValue, tiling, indexK);
            }
        }
        SetFlag<HardEvent::M_MTE1>(eventIdMToMte1);
    }
    WaitFlag<HardEvent::M_MTE1>(eventIdMToMte1);
}

template <typename T, typename U, typename S>
__aicore__ inline void GemmExecNmPingPong(
    const LocalTensor<T>& l0c, const LocalTensor<U>& src0, const LocalTensor<S>& src1, GemmTiling tiling,
    const int32_t initValue)
{
    uint32_t ping = 1;
    LocalTensor<U> l0aPing;
    LocalTensor<U> l0aPong;
    LocalTensor<S> l0bPing;
    LocalTensor<S> l0bPong;
    GetPingPongBuffer(l0aPing, l0aPong, l0bPing, l0bPong);

    event_t eventId0 = static_cast<event_t>(GetTPipePtr()->AllocEventID<HardEvent::M_MTE1>());
    event_t eventId1 = static_cast<event_t>(GetTPipePtr()->AllocEventID<HardEvent::M_MTE1>());
    SetFlag<HardEvent::M_MTE1>(eventId0);
    SetFlag<HardEvent::M_MTE1>(eventId1);

    for (size_t i = 0; i < tiling.kIterNum; i++) {
        uint32_t kBlocks = tiling.kTileBlock;
        if (i == tiling.kIterNum - 1) {
            kBlocks = tiling.kTailBlock;
        }
        if (ping == 1) {
            WaitFlag<HardEvent::M_MTE1>(eventId0);
            for (size_t indexN = 0; indexN < tiling.nIterNum; indexN++) {
                // load data from l1 to l0b
                LoadL0B(kBlocks, tiling.nTileBlock, tiling, i, indexN, src1, l0bPing);
                for (size_t indexM = 0; indexM < tiling.mIterNum; indexM++) {
                    // load data from l1 to l0a
                    LoadL0A(kBlocks, tiling.mTileBlock, tiling, i, indexM, src0, l0aPing);
                    event_t eventIdMte1ToM = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE1_M));
                    SetFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                    WaitFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                    PipeBarrier<PIPE_M>();
                    MmadFunc(l0aPing, l0bPing, l0c, initValue, tiling, i);
                }
            }
            SetFlag<HardEvent::M_MTE1>(eventId0);
        } else {
            WaitFlag<HardEvent::M_MTE1>(eventId1);
            for (size_t indexN = 0; indexN < tiling.nIterNum; indexN++) {
                // load data from l1 to l0b
                LoadL0B(kBlocks, tiling.nTileBlock, tiling, i, indexN, src1, l0bPong);
                for (size_t indexM = 0; indexM < tiling.mIterNum; indexM++) {
                    // load data from l1 to l0a
                    LoadL0A(kBlocks, tiling.mTileBlock, tiling, i, indexM, src0, l0aPong);
                    event_t eventIdMte1ToM = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE1_M));
                    SetFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                    WaitFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                    PipeBarrier<PIPE_M>();
                    MmadFunc(l0aPong, l0bPong, l0c, initValue, tiling, i);
                }
            }
            SetFlag<HardEvent::M_MTE1>(eventId1);
        }
        ping = 1 - ping;
    }

#if __NPU_ARCH__ == 2201
    WaitFlag<HardEvent::M_MTE1>(eventId0);
    GetTPipePtr()->ReleaseEventID<HardEvent::M_MTE1>(eventId0);
    WaitFlag<HardEvent::M_MTE1>(eventId1);
    GetTPipePtr()->ReleaseEventID<HardEvent::M_MTE1>(eventId1);
#else
    WaitFlag<HardEvent::M_MTE1>(eventId0);
    WaitFlag<HardEvent::M_MTE1>(eventId1);
#endif
}

template <typename T, typename U, typename S>
__aicore__ inline void GemmExecNm(
    const LocalTensor<T>& l0c, const LocalTensor<U>& src0, const LocalTensor<S>& src1, GemmTiling tiling,
    const int32_t initValue)
{
    uint32_t needL0Asize = tiling.roundM * tiling.dtypeSize * tiling.c0Size * tiling.kTileBlock * 2;
    uint32_t needL0Bsize = tiling.roundN * tiling.dtypeSize * tiling.c0Size * tiling.kTileBlock * 2;
    if (needL0Asize > TOTAL_L0A_SIZE || needL0Bsize > TOTAL_L0B_SIZE) {
        GemmExecNmNopingpong(l0c, src0, src1, tiling, initValue);
        return;
    }
    GemmExecNmPingPong(l0c, src0, src1, tiling, initValue);
}

template <typename T, typename U, typename S>
__aicore__ inline void GemmExecMnNopingpong(
    const LocalTensor<T>& l0c, const LocalTensor<U>& src0, const LocalTensor<S>& src1, GemmTiling tiling,
    const int32_t initValue)
{
    LocalTensor<S> l0b;
    LocalTensor<U> l0a;
    GetSingleThreadBuffer(l0a, l0b);
    event_t eventIdMToMte1 = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::M_MTE1));
    SetFlag<HardEvent::M_MTE1>(eventIdMToMte1);
    for (size_t indexK = 0; indexK < tiling.kIterNum; indexK++) {
        uint32_t kBlocks = tiling.kTileBlock;
        if (indexK == tiling.kIterNum - 1) {
            kBlocks = tiling.kTailBlock;
        }
        WaitFlag<HardEvent::M_MTE1>(eventIdMToMte1);
        for (size_t indexM = 0; indexM < tiling.mIterNum; indexM++) {
            // load data from l1 to l0a
            LoadL0A(kBlocks, tiling.mTileBlock, tiling, indexK, indexM, src0, l0a);
            for (size_t indexN = 0; indexN < tiling.nIterNum; indexN++) {
                // load data from l1 to l0b
                LoadL0B(kBlocks, tiling.nTileBlock, tiling, indexK, indexN, src1, l0b);
                event_t eventIdMte1ToM = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE1_M));
                SetFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                WaitFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                PipeBarrier<PIPE_M>();
                MmadFunc(l0a, l0b, l0c, initValue, tiling, indexK);
            }
        }
        SetFlag<HardEvent::M_MTE1>(eventIdMToMte1);
    }
    WaitFlag<HardEvent::M_MTE1>(eventIdMToMte1);
}

template <typename T, typename U, typename S>
__aicore__ inline void GemmExecMnPingPong(
    const LocalTensor<T>& l0c, const LocalTensor<U>& src0, const LocalTensor<S>& src1, GemmTiling tiling,
    const int32_t initValue)
{
    uint32_t ping = 1;
    LocalTensor<U> l0aPing;
    LocalTensor<U> l0aPong;
    LocalTensor<S> l0bPing;
    LocalTensor<S> l0bPong;
    GetPingPongBuffer(l0aPing, l0aPong, l0bPing, l0bPong);

    event_t eventId0 = static_cast<event_t>(GetTPipePtr()->AllocEventID<HardEvent::M_MTE1>());
    event_t eventId1 = static_cast<event_t>(GetTPipePtr()->AllocEventID<HardEvent::M_MTE1>());
    SetFlag<HardEvent::M_MTE1>(eventId0);
    SetFlag<HardEvent::M_MTE1>(eventId1);

    for (size_t i = 0; i < tiling.kIterNum; i++) {
        uint32_t kBlocks = tiling.kTileBlock;
        if (i == tiling.kIterNum - 1) {
            kBlocks = tiling.kTailBlock;
        }
        if (ping == 1) {
            WaitFlag<HardEvent::M_MTE1>(eventId0);
            for (size_t indexM = 0; indexM < tiling.mIterNum; indexM++) {
                // load data from l1 to l0a
                LoadL0A(kBlocks, tiling.mTileBlock, tiling, i, indexM, src0, l0aPing);
                for (size_t indexN = 0; indexN < tiling.nIterNum; indexN++) {
                    // load data from l1 to l0b
                    LoadL0B(kBlocks, tiling.nTileBlock, tiling, i, indexN, src1, l0bPing);

                    event_t eventIdMte1ToM = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE1_M));
                    SetFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                    WaitFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                    PipeBarrier<PIPE_M>();
                    MmadFunc(l0aPing, l0bPing, l0c, initValue, tiling, i);
                }
            }
            SetFlag<HardEvent::M_MTE1>(eventId0);
        } else {
            WaitFlag<HardEvent::M_MTE1>(eventId1);
            for (size_t indexM = 0; indexM < tiling.mIterNum; indexM++) {
                // load data from l1 to l0a
                LoadL0A(kBlocks, tiling.mTileBlock, tiling, i, indexM, src0, l0aPong);
                for (size_t indexN = 0; indexN < tiling.nIterNum; indexN++) {
                    // load data from l1 to l0b
                    LoadL0B(kBlocks, tiling.nTileBlock, tiling, i, indexN, src1, l0bPong);
                    event_t eventIdMte1ToM = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::MTE1_M));
                    SetFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                    WaitFlag<HardEvent::MTE1_M>(eventIdMte1ToM);
                    PipeBarrier<PIPE_M>();
                    MmadFunc(l0aPong, l0bPong, l0c, initValue, tiling, i);
                }
            }
            SetFlag<HardEvent::M_MTE1>(eventId1);
        }
        ping = 1 - ping;
    }

    WaitFlag<HardEvent::M_MTE1>(eventId0);
    GetTPipePtr()->ReleaseEventID<HardEvent::M_MTE1>(eventId0);
    WaitFlag<HardEvent::M_MTE1>(eventId1);
    GetTPipePtr()->ReleaseEventID<HardEvent::M_MTE1>(eventId1);
}

template <typename T, typename U, typename S>
__aicore__ inline void GemmExecMn(
    const LocalTensor<T>& l0c, const LocalTensor<U>& src0, const LocalTensor<S>& src1, GemmTiling tiling,
    const int32_t initValue)
{
    uint32_t needL0Bsize = tiling.roundN * tiling.dtypeSize * tiling.c0Size * tiling.kTileBlock * 2;
    uint32_t needL0Asize = tiling.roundM * tiling.dtypeSize * tiling.c0Size * tiling.kTileBlock * 2;
    if (needL0Asize > TOTAL_L0A_SIZE || needL0Bsize > TOTAL_L0B_SIZE) {
        GemmExecMnNopingpong(l0c, src0, src1, tiling, initValue);
        return;
    }
    GemmExecMnPingPong(l0c, src0, src1, tiling, initValue);
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_GEMM_BASE_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GEMM_BASE_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_GEMM_BASE_IMPL_H__
#endif
