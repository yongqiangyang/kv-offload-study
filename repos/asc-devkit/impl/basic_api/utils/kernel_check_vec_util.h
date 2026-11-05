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
 * \file kernel_check_vec_util.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_check_vec_util.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_UTIL_H__
#endif
#ifndef ASCENDC_CHECK_VEC_UTIL_H
#define ASCENDC_CHECK_VEC_UTIL_H
#if ASCENDC_CPU_DEBUG
#include <string>
#include "../kernel_utils.h"
#include "../../../include/basic_api/kernel_struct_transpose.h"
#include "kernel_check_vec_padding_util.h"
namespace AscendC {
namespace check {
struct VecScatterApiParams {
    VecScatterApiParams() {}
    VecScatterApiParams(
        uint64_t dstAddrIn, uint64_t srcAddrIn, uint64_t dstOffsetAddrIn, uint32_t dstDtypeBytesIn,
        uint32_t srcDtypeBytesIn, uint32_t dstOffsetDtypeBytesIn, uint32_t dstBaseAddrIn, uint32_t countIn,
        uint64_t dstSizeIn, uint64_t srcSizeIn, uint64_t dstOffsetSizeIn, uint8_t dstPosIn, uint8_t srcPosIn,
        uint8_t dstOffsetPosIn)
    {
        dstAddr = dstAddrIn;
        srcAddr = srcAddrIn;
        dstOffsetAddr = dstOffsetAddrIn;
        dstDtypeBytes = dstDtypeBytesIn;
        srcDtypeBytes = srcDtypeBytesIn;
        dstOffsetDtypeBytes = dstOffsetDtypeBytesIn;
        dstBaseAddr = dstBaseAddrIn;
        count = countIn;
        dstSize = dstSizeIn;
        srcSize = srcSizeIn;
        dstOffsetSize = dstOffsetSizeIn;
        dstLogicPos = dstPosIn;
        srcLogicPos = srcPosIn;
        dstOffsetLogicPos = dstOffsetPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        srcPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(srcPosIn)));
        dstOffsetPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstOffsetPosIn)));
    }
    VecScatterApiParams(
        uint64_t dstAddrIn, uint64_t srcAddrIn, uint64_t dstOffsetAddrIn, uint32_t dstDtypeBytesIn,
        uint32_t srcDtypeBytesIn, uint32_t dstOffsetDtypeBytesIn, uint32_t dstBaseAddrIn, uint8_t repeatTimesIn,
        uint16_t srcRepStrideIn, uint64_t dstSizeIn, uint64_t srcSizeIn, uint64_t dstOffsetSizeIn, uint8_t dstPosIn,
        uint8_t srcPosIn, uint8_t dstOffsetPosIn)
    {
        dstAddr = dstAddrIn;
        srcAddr = srcAddrIn;
        dstOffsetAddr = dstOffsetAddrIn;
        dstDtypeBytes = dstDtypeBytesIn;
        srcDtypeBytes = srcDtypeBytesIn;
        dstOffsetDtypeBytes = dstOffsetDtypeBytesIn;
        dstBaseAddr = dstBaseAddrIn;
        repeatTimes = repeatTimesIn;
        srcRepStride = srcRepStrideIn;
        dstSize = dstSizeIn;
        srcSize = srcSizeIn;
        dstOffsetSize = dstOffsetSizeIn;
        dstLogicPos = dstPosIn;
        srcLogicPos = srcPosIn;
        dstOffsetLogicPos = dstOffsetPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        srcPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(srcPosIn)));
        dstOffsetPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstOffsetPosIn)));
    }
    uint64_t dstAddr = 0;
    uint64_t srcAddr = 0;
    uint64_t dstOffsetAddr = 0;
    uint8_t repeatTimes = 0;
    uint32_t dstDtypeBytes = 0;
    uint32_t srcDtypeBytes = 0;
    uint32_t dstOffsetDtypeBytes = 0;
    uint32_t dstBaseAddr = 0;
    uint64_t dstSize = 0;
    uint64_t srcSize = 0;
    uint64_t dstOffsetSize = 0;
    uint16_t srcRepStride = 0;
    uint8_t dstLogicPos = 0;
    uint8_t srcLogicPos = 0;
    uint8_t dstOffsetLogicPos = 0;
    uint8_t dstPos = 0;
    uint8_t srcPos = 0;
    uint8_t dstOffsetPos = 0;
    uint32_t count = 0;
};
struct VecCmpRgtApiParams {
    VecCmpRgtApiParams() {}
    VecCmpRgtApiParams(
        uint64_t src0AddrIn, uint64_t src1AddrIn, uint16_t src0BlockStrideIn, uint16_t src1BlockStrideIn,
        uint16_t src0RepeatStrideIn, uint16_t src1RepeatStrideIn, uint32_t src0DtypeBytesIn, uint32_t src1DtypeBytesIn,
        uint64_t src0SizeIn, uint64_t src1SizeIn, uint8_t src0PosIn, uint8_t src1PosIn)
    {
        src0Addr = src0AddrIn;
        src1Addr = src1AddrIn;
        src0BlockStride = src0BlockStrideIn;
        src1BlockStride = src1BlockStrideIn;
        src0RepeatStride = src0RepeatStrideIn;
        src1RepeatStride = src1RepeatStrideIn;
        src0DtypeBytes = src0DtypeBytesIn;
        src1DtypeBytes = src1DtypeBytesIn;
        src0Size = src0SizeIn;
        src1Size = src1SizeIn;
        src0LogicPos = src0PosIn;
        src1LogicPos = src1PosIn;
        src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
        src1Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src1PosIn)));
    }
    uint64_t src0Addr = 0;
    uint64_t src1Addr = 0;
    uint8_t repeatTimes = 1;
    uint16_t src0BlockStride = 0;
    uint16_t src1BlockStride = 0;
    uint16_t src0RepeatStride = 0;
    uint16_t src1RepeatStride = 0;
    uint32_t src0DtypeBytes = 0;
    uint32_t src1DtypeBytes = 0;
    uint64_t src0Size = 0;
    uint64_t src1Size = 0;
    uint8_t src0LogicPos = 0;
    uint8_t src1LogicPos = 0;
    uint8_t src0Pos = 0;
    uint8_t src1Pos = 0;
};

struct VecBilinearInterpolationApiParams {
    VecBilinearInterpolationApiParams() {}
    VecBilinearInterpolationApiParams(
        uint64_t dstAddrIn, uint64_t src0AddrIn, uint64_t offsetAddrIn, uint64_t src1AddrIn, uint8_t hRepeatIn,
        bool repeatModeIn, uint16_t dstBlockStrideIn, uint16_t vROffsetIn, uint16_t vRepeatIn, uint32_t dstDtypeBytesIn,
        uint32_t src0DtypeBytesIn, uint32_t offsetDtypeBytesIn, uint32_t src1DtypeBytesIn, uint64_t dstSizeIn,
        uint64_t src0SizeIn, uint64_t offsetSizeIn, uint64_t src1SizeIn, uint8_t dstPosIn, uint8_t src0PosIn,
        uint8_t offsetPosIn, uint8_t src1PosIn)
    {
        dstAddr = dstAddrIn;
        src0Addr = src0AddrIn;
        offsetAddr = offsetAddrIn;
        src1Addr = src1AddrIn;
        hRepeat = hRepeatIn;
        repeatMode = repeatModeIn;
        dstBlockStride = dstBlockStrideIn;
        vROffset = vROffsetIn;
        vRepeat = vRepeatIn;
        dstDtypeBytes = dstDtypeBytesIn;
        src0DtypeBytes = src0DtypeBytesIn;
        offsetDtypeBytes = offsetDtypeBytesIn;
        src1DtypeBytes = src1DtypeBytesIn;
        dstSize = dstSizeIn;
        src0Size = src0SizeIn;
        offsetSize = offsetSizeIn;
        src1Size = src1SizeIn;
        dstLogicPos = dstPosIn;
        src0LogicPos = src0PosIn;
        offsetLogicPos = offsetPosIn;
        src1LogicPos = src1PosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        ;
        src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
        ;
        offsetPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(offsetPosIn)));
        ;
        src1Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src1PosIn)));
        ;
    }
    uint64_t dstAddr = 0;
    uint64_t src0Addr = 0;
    uint64_t offsetAddr = 0;
    uint64_t src1Addr = 0;
    uint8_t hRepeat = 0;
    bool repeatMode = false;
    uint16_t dstBlockStride = 0;
    uint16_t vROffset = 0;
    uint16_t vRepeat = 0;
    uint32_t dstDtypeBytes = 0;
    uint32_t src0DtypeBytes = 0;
    uint32_t offsetDtypeBytes = 0;
    uint32_t src1DtypeBytes = 0;
    uint64_t dstSize = 0;
    uint64_t src0Size = 0;
    uint64_t offsetSize = 0;
    uint64_t src1Size = 0;
    uint8_t dstLogicPos = 0;
    uint8_t src0LogicPos = 0;
    uint8_t offsetLogicPos = 0;
    uint8_t src1LogicPos = 0;
    uint8_t dstPos = 0;
    uint8_t src0Pos = 0;
    uint8_t offsetPos = 0;
    uint8_t src1Pos = 0;
};

struct VecTransposeApiParams {
    VecTransposeApiParams() {}
    VecTransposeApiParams(
        uint64_t dstAddrIn, uint64_t srcAddrIn, uint8_t repeatIn, uint16_t dstRepeatStrideIn,
        uint16_t srcRepeatStrideIn, uint32_t dstDtypeBytesIn, uint32_t srcDtypeBytesIn, uint64_t dstSizeIn,
        uint64_t srcSizeIn, uint8_t dstPosIn, uint8_t srcPosIn, int8_t indexIn = -1)
    {
        dstAddr = dstAddrIn;
        srcAddr = srcAddrIn;
        repeatTimes = repeatIn;
        dstRepeatStride = dstRepeatStrideIn;
        srcRepeatStride = srcRepeatStrideIn;
        dstDtypeBytes = dstDtypeBytesIn;
        srcDtypeBytes = srcDtypeBytesIn;
        dstSize = dstSizeIn;
        srcSize = srcSizeIn;
        dstLogicPos = dstPosIn;
        srcLogicPos = srcPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        srcPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(srcPosIn)));
        index = indexIn;
    }
    VecTransposeApiParams(
        uint64_t dstAddrIn, uint64_t srcAddrIn, uint8_t repeatIn, uint16_t dstRepeatStrideIn,
        uint16_t srcRepeatStrideIn, uint32_t dstDtypeBytesIn, uint32_t srcDtypeBytesIn, uint64_t dstSizeIn,
        uint64_t srcSizeIn, uint64_t tmpBufferSizeIn, uint8_t dstPosIn, uint8_t srcPosIn, uint16_t nSizeIn,
        uint16_t cSizeIn, uint16_t hSizeIn, uint16_t wSizeIn, TransposeType transposeTypeIn)
    {
        dstAddr = dstAddrIn;
        srcAddr = srcAddrIn;
        repeatTimes = repeatIn;
        dstRepeatStride = dstRepeatStrideIn;
        srcRepeatStride = srcRepeatStrideIn;
        dstDtypeBytes = dstDtypeBytesIn;
        srcDtypeBytes = srcDtypeBytesIn;
        dstSize = dstSizeIn;
        srcSize = srcSizeIn;
        tmpBufferSize = tmpBufferSizeIn;
        dstLogicPos = dstPosIn;
        srcLogicPos = srcPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        srcPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(srcPosIn)));
        nSize = nSizeIn;
        cSize = cSizeIn;
        hSize = hSizeIn;
        wSize = wSizeIn;
        transposeType = transposeTypeIn;
    }
    uint64_t dstAddr = 0;
    uint64_t srcAddr = 0;
    uint8_t repeatTimes = 0;
    uint16_t dstBlockStride = 1;
    uint16_t dstRepeatStride = 0;
    uint16_t srcBlockStride = 1;
    uint16_t srcRepeatStride = 0;
    uint32_t dstDtypeBytes = 0;
    uint64_t dstSize = 0;
    uint32_t srcDtypeBytes = 0;
    uint64_t srcSize = 0;
    uint64_t tmpBufferSize = 0;
    uint16_t nSize = 0;
    uint16_t cSize = 0;
    uint16_t hSize = 0;
    uint16_t wSize = 0;
    uint8_t dstLogicPos = 0;
    uint8_t srcLogicPos = 0;
    uint8_t dstPos = 0;
    uint8_t srcPos = 0;
    int8_t index = 0; // transdata [16]
    TransposeType transposeType = TransposeType::TRANSPOSE_TYPE_NONE;
};

struct VecProposalApiParams {
    VecProposalApiParams() {}
    VecProposalApiParams(
        uint64_t dstAddrIn, uint64_t src0AddrIn, uint8_t repeatIn, uint32_t dstDtypeBytesIn, uint32_t src0DtypeBytesIn,
        uint64_t dstSizeIn, uint64_t src0SizeIn, uint8_t dstPosIn, uint8_t src0PosIn)
    {
        dstAddr = dstAddrIn;
        src0Addr = src0AddrIn;
        repeatTimes = repeatIn;
        dstDtypeBytes = dstDtypeBytesIn;
        src0DtypeBytes = src0DtypeBytesIn;
        dstSize = dstSizeIn;
        src0Size = src0SizeIn;
        dstLogicPos = dstPosIn;
        src0LogicPos = src0PosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
    }
    VecProposalApiParams(
        uint64_t dstAddrIn, uint64_t src0AddrIn, uint64_t src1AddrIn, uint8_t repeatIn, uint32_t dstDtypeBytesIn,
        uint32_t src0DtypeBytesIn, uint32_t src1DtypeBytesIn, uint64_t dstSizeIn, uint64_t src0SizeIn,
        uint64_t src1SizeIn, uint8_t dstPosIn, uint8_t src0PosIn, uint8_t src1PosIn)
    {
        dstAddr = dstAddrIn;
        src0Addr = src0AddrIn;
        src1Addr = src1AddrIn;
        repeatTimes = repeatIn;
        dstDtypeBytes = dstDtypeBytesIn;
        src0DtypeBytes = src0DtypeBytesIn;
        src1DtypeBytes = src1DtypeBytesIn;
        dstSize = dstSizeIn;
        src0Size = src0SizeIn;
        src1Size = src1SizeIn;
        dstLogicPos = dstPosIn;
        src0LogicPos = src0PosIn;
        src1LogicPos = src1PosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
        src1Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src1PosIn)));
    }
    VecProposalApiParams(
        uint64_t dstAddrIn, uint64_t src0AddrIn, uint8_t repeatIn, uint32_t dstDtypeBytesIn, uint32_t src0DtypeBytesIn,
        uint64_t dstSizeIn, uint64_t src0SizeIn, uint8_t dstPosIn, uint8_t src0PosIn, uint16_t validBitIn,
        const uint16_t elementLenIn[4], uint8_t srcIndexIn, bool isExhaustedIn = false, bool isContinuousIn = false)
    {
        dstAddr = dstAddrIn;
        src0Addr = src0AddrIn;
        repeatTimes = repeatIn;
        dstDtypeBytes = dstDtypeBytesIn;
        src0DtypeBytes = src0DtypeBytesIn;
        dstSize = dstSizeIn;
        src0Size = src0SizeIn;
        dstLogicPos = dstPosIn;
        src0LogicPos = src0PosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
        validBit = validBitIn;
        for (size_t i = 0; i < eleSize; i++) {
            elementLengths[i] = elementLenIn[i];
        }
        srcIndex = srcIndexIn;
        isExhausted = isExhaustedIn;
        isContinuous = isContinuousIn;
    }
    const uint32_t eleSize = 4;
    uint64_t dstAddr = 0;
    uint64_t src0Addr = 0;
    uint64_t src1Addr = 0;
    uint8_t repeatTimes = 0;
    uint32_t dstDtypeBytes = 0;
    uint32_t src0DtypeBytes = 0;
    uint32_t src1DtypeBytes = 0;
    uint64_t dstSize = 0;
    uint64_t src0Size = 0;
    uint64_t src1Size = 0;
    uint8_t dstLogicPos = 0;
    uint8_t src0LogicPos = 0;
    uint8_t src1LogicPos = 0;
    uint8_t dstPos = 0;
    uint8_t src0Pos = 0;
    uint8_t src1Pos = 0;
    uint16_t validBit = 0;
    uint16_t elementLengths[4];
    uint8_t srcIndex = 0;
    bool isExhausted = false;
    bool isContinuous = false;
};
struct SortApiParams {
    SortApiParams() {}
    SortApiParams(
        uint64_t dstAddrIn, uint64_t concatAddrIn, uint64_t indexAddrIn, uint64_t tmpAddrIn, uint8_t repeatIn,
        uint32_t dstDtypeBytesIn, uint32_t concatDtypeBytesIn, uint32_t indexDtypeBytesIn, uint32_t tmpDtypeBytesIn,
        uint64_t dstSizeIn, uint64_t concatSizeIn, uint64_t indexSizeIn, uint64_t tmpSizeIn, uint8_t dstPosIn,
        uint8_t concatPosIn, uint8_t indexPosIn, uint8_t tmpPosIn, bool isFullSortIn)
    {
        dstAddr = dstAddrIn;
        concatAddr = concatAddrIn;
        indexAddr = indexAddrIn;
        tmpAddr = tmpAddrIn;
        repeatTimes = repeatIn;
        dstDtypeBytes = dstDtypeBytesIn;
        concatDtypeBytes = concatDtypeBytesIn;
        indexDtypeBytes = indexDtypeBytesIn;
        tmpDtypeBytes = tmpDtypeBytesIn;
        dstSize = dstSizeIn;
        concatSize = concatSizeIn;
        indexSize = indexSizeIn;
        tmpSize = tmpSizeIn;
        dstLogicPos = dstPosIn;
        concatLogicPos = concatPosIn;
        indexLogicPos = indexPosIn;
        tmpLogicPos = tmpPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        concatPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(concatPosIn)));
        indexPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(indexPosIn)));
        tmpPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(tmpPosIn)));
        isFullSort = isFullSortIn;
    }
    uint64_t dstAddr = 0;
    uint64_t concatAddr = 0;
    uint64_t indexAddr = 0;
    uint64_t tmpAddr = 0;
    uint8_t repeatTimes = 0;
    uint32_t dstDtypeBytes = 0;
    uint32_t concatDtypeBytes = 0;
    uint32_t indexDtypeBytes = 0;
    uint32_t tmpDtypeBytes = 0;
    uint64_t dstSize = 0;
    uint64_t concatSize = 0;
    uint64_t indexSize = 0;
    uint64_t tmpSize = 0;
    uint8_t dstLogicPos = 0;
    uint8_t concatLogicPos = 0;
    uint8_t indexLogicPos = 0;
    uint8_t tmpLogicPos = 0;
    uint8_t dstPos = 0;
    uint8_t concatPos = 0;
    uint8_t indexPos = 0;
    uint8_t tmpPos = 0;
    bool isFullSort = false;
};
bool CheckFuncVecCmpRgtImplForMaskArray(VecCmpRgtApiParams& chkParams, const uint64_t mask[], const char* intriName);
bool CheckFuncVecCmpRgtImpl(VecCmpRgtApiParams& chkParams, const uint64_t mask, const char* intriName);
bool CheckFuncBilinearInterpolationImpl(
    VecBilinearInterpolationApiParams& chkParams, const uint64_t mask, const char* intriName);
bool CheckFuncBilinearInterpolationImpl(
    VecBilinearInterpolationApiParams& chkParams, const uint64_t mask[], const char* intriName);
bool CheckFunTransposeImpl(VecTransposeApiParams& chkParams, const char* intriName);
bool CheckFunProposalImpl(VecProposalApiParams& chkParams, const char* intriName);
bool CheckFunScatterImpl(VecScatterApiParams& chkParams, const char* intriName);
bool CheckFunScatterImpl(VecScatterApiParams& chkParams, const uint64_t mask, const char* intriName);
bool CheckFunScatterImplForMaskArray(VecScatterApiParams& chkParams, const uint64_t mask[], const char* intriName);
bool CheckSortImpl(SortApiParams& chkParams, const char* intriName);
uint64_t GetHardWarebufferSize(uint8_t index);
} // namespace check
} // namespace AscendC
#endif
#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_UTIL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_UTIL_H__
#endif
