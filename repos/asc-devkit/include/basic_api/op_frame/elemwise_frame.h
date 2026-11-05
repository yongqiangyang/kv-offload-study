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
 * \file elemwise_frame.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "elemwise_frame.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ELEMWISE_FRAME_H__
#endif

#ifndef ASCENDC_KERNEL_FRAME_H
#define ASCENDC_KERNEL_FRAME_H
#include "kernel_operator.h"

[[deprecated(__FILE__ " is deprecated, please do not use!")]] typedef void ElemwiseFrameHeader;

namespace AscendC {
#define QUE_MAX_DEPTH 2
#ifndef STAGE_NUM
#define STAGE_NUM 3
#endif

class ElemwiseOpBase {
public:
    __aicore__ ElemwiseOpBase() {}

    __aicore__ inline void Init(int32_t loop, int32_t vecInLen, int32_t lcmLen, int32_t vecOutLen)
    {
        SetStagePieces(loop, 0);
        const uint8_t pingpongNum = 2;
        pipe.InitBuffer(inQueue, pingpongNum, vecInLen);
        pipe.InitBuffer(outQueue, pingpongNum, vecOutLen);
        pipe.InitBuffer(tbuf, lcmLen);
    }

    __aicore__ inline void SetStagePieces(const int32_t loopValue, const int32_t progressValue)
    {
        for (int32_t i = 0; i < STAGE_NUM; ++i) {
            stagePieceNum[i] = loopValue;
            stageProgress[i] = progressValue;
        }
    }

protected:
    int32_t stagePieceNum[STAGE_NUM];
    int32_t stageProgress[STAGE_NUM];
    void* tiling;
    TPipe pipe;
    TBuf<TPosition::LCM> tbuf;
    TQue<TPosition::VECIN, QUE_MAX_DEPTH> inQueue;
    TQue<TPosition::VECOUT, QUE_MAX_DEPTH> outQueue;

    enum DataLenType : uint8_t {
        DEFAULT = 0,
    };
};

template <class Op>
class ElemwiseFrame : public Op {
public:
    __aicore__ ElemwiseFrame() {}
    __aicore__ inline bool CopyIn(int32_t progress);
    __aicore__ inline bool Compute(int32_t progress);
    __aicore__ inline bool CopyOut(int32_t progress);
    __aicore__ inline bool RunStagePieces(int32_t curStage, int32_t progress);
    __aicore__ inline void Process();
};
template <class Op>
__aicore__ inline void ElemwiseFrame<Op>::Process()
{
    int32_t done = 0;
    while (done < STAGE_NUM) {
        done = 0;
        for (int32_t i = 0; i < STAGE_NUM; i++) {
            if (Op::stageProgress[i] >= Op::stagePieceNum[i]) {
                done++;
                continue;
            }
            if (RunStagePieces(i, Op::stageProgress[i])) {
                Op::stageProgress[i]++;
            }
        }
    }
};

template <class Op>
__aicore__ inline bool ElemwiseFrame<Op>::RunStagePieces(int32_t curStage, int32_t progress)
{
    switch (curStage) {
        case 0:
            return CopyIn(progress);
        case 1:
            return Compute(progress);
        case 2:
            return CopyOut(progress);
        default:
            ASSERT(0);
            break;
    }
    return true;
}

template <class Op>
__aicore__ inline bool ElemwiseFrame<Op>::CopyIn(int32_t progress)
{
    if (!Op::inQueue.VacantInQue()) {
        return false;
    }
    auto xBuf = Op::inQueue.template AllocTensor<typename Op::DType>();
    Op::MyCopyIn(progress, xBuf);
    Op::inQueue.EnQue(xBuf);
    return true;
}

template <class Op>
__aicore__ inline bool ElemwiseFrame<Op>::Compute(int32_t progress)
{
    if (!Op::outQueue.VacantInQue()) {
        return false;
    }
    if (Op::inQueue.GetTensorCountInQue() == 0) {
        return false;
    }

    auto xBuf = Op::inQueue.template DeQue<typename Op::DType>();
    auto yBuf = Op::outQueue.template AllocTensor<typename Op::DType>();
    Op::MyCompute(progress, xBuf, yBuf);

    Op::outQueue.template EnQue<typename Op::DType>(yBuf);
    Op::inQueue.template FreeTensor<typename Op::DType>(xBuf);
    return true;
}

template <class Op>
__aicore__ inline bool ElemwiseFrame<Op>::CopyOut(int32_t progress)
{
    if (Op::outQueue.GetTensorCountInQue() == 0) {
        return false;
    }

    auto yBuf = Op::outQueue.template DeQue<typename Op::DType>();
    Op::MyCopyOut(progress, yBuf);

    Op::outQueue.FreeTensor(yBuf);
    return true;
}
} // namespace AscendC

#endif // ASCENDC_KERNEL_FRAME_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ELEMWISE_FRAME_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ELEMWISE_FRAME_H__
#endif
