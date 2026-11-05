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
 * \file kernel_check_vec_select_util.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_check_vec_select_util.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_SELECT_UTIL_H__
#endif
#ifndef ASCENDC_CHECK_VEC_SELECT_UTIL_H
#define ASCENDC_CHECK_VEC_SELECT_UTIL_H
#if ASCENDC_CPU_DEBUG
#include <string>
#include "../kernel_utils.h"
namespace AscendC {
namespace check {

struct VecSelectApiParams {
    VecSelectApiParams() {}
    VecSelectApiParams(
        uint64_t dstAddrIn, uint64_t selMaskAddrIn, uint64_t src0AddrIn, uint64_t src1AddrIn, uint8_t repeatIn,
        uint16_t dstBlockStrideIn, uint16_t src0BlockStrideIn, uint16_t src1BlockStrideIn, uint16_t dstRepeatStrideIn,
        uint16_t src0RepeatStrideIn, uint16_t src1RepeatStrideIn, uint32_t dstDtypeBytesIn,
        uint32_t selMaskDtypeBytesIn, uint32_t src0DtypeBytesIn, uint32_t src1DtypeBytesIn, uint64_t dstSizeIn,
        uint64_t selMaskSizeIn, uint64_t src0SizeIn, uint64_t src1SizeIn, uint8_t dstPosIn, uint8_t selMaskIn,
        uint8_t src0PosIn, uint8_t src1PosIn)
    {
        dstAddr = dstAddrIn;
        selMaskAddr = selMaskAddrIn;
        src0Addr = src0AddrIn;
        src1Addr = src1AddrIn;
        repeatTimes = repeatIn;
        dstBlockStride = dstBlockStrideIn;
        src0BlockStride = src0BlockStrideIn;
        src1BlockStride = src1BlockStrideIn;
        dstRepeatStride = dstRepeatStrideIn;
        src0RepeatStride = src0RepeatStrideIn;
        src1RepeatStride = src1RepeatStrideIn;
        dstDtypeBytes = dstDtypeBytesIn;
        selMaskDtypeBytes = selMaskDtypeBytesIn;
        src0DtypeBytes = src0DtypeBytesIn;
        src1DtypeBytes = src1DtypeBytesIn;
        dstSize = dstSizeIn;
        selMaskSize = selMaskSizeIn;
        src0Size = src0SizeIn;
        src1Size = src1SizeIn;
        dstLogicPos = dstPosIn;
        selMaskLogicPos = selMaskIn;
        src0LogicPos = src0PosIn;
        src1LogicPos = src1PosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        selMaskPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(selMaskIn)));
        src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
        src1Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src1PosIn)));
    }

    VecSelectApiParams(
        uint64_t dstAddrIn, uint64_t selMaskAddrIn, uint64_t src0AddrIn, uint64_t src1AddrIn, uint32_t dstDtypeBytesIn,
        uint32_t selMaskDtypeBytesIn, uint32_t src0DtypeBytesIn, uint32_t src1DtypeBytesIn, uint64_t dstSizeIn,
        uint64_t selMaskSizeIn, uint64_t src0SizeIn, uint64_t src1SizeIn, uint8_t dstPosIn, uint8_t selMaskIn,
        uint8_t src0PosIn, uint8_t src1PosIn, uint32_t count)
    {
        dstAddr = dstAddrIn;
        selMaskAddr = selMaskAddrIn;
        src0Addr = src0AddrIn;
        src1Addr = src1AddrIn;
        dstDtypeBytes = dstDtypeBytesIn;
        selMaskDtypeBytes = selMaskDtypeBytesIn;
        src0DtypeBytes = src0DtypeBytesIn;
        src1DtypeBytes = src1DtypeBytesIn;
        dstSize = dstSizeIn;
        selMaskSize = selMaskSizeIn;
        src0Size = src0SizeIn;
        src1Size = src1SizeIn;
        dstLogicPos = dstPosIn;
        selMaskLogicPos = selMaskIn;
        src0LogicPos = src0PosIn;
        src1LogicPos = src1PosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        selMaskPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(selMaskIn)));
        src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
        src1Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src1PosIn)));
        calCount = count;
    }

    uint64_t dstAddr = 0;
    uint64_t selMaskAddr = 0;
    uint64_t src0Addr = 0;
    uint64_t src1Addr = 0;
    uint8_t repeatTimes = 0;
    uint16_t dstBlockStride = 0;
    uint16_t src0BlockStride = 0;
    uint16_t src1BlockStride = 0;
    uint16_t dstRepeatStride = 0;
    uint16_t src0RepeatStride = 0;
    uint16_t src1RepeatStride = 0;
    uint32_t dstDtypeBytes = 0;
    uint32_t selMaskDtypeBytes = 0;
    uint32_t src0DtypeBytes = 0;
    uint32_t src1DtypeBytes = 0;
    uint64_t dstSize = 0;
    uint64_t selMaskSize = 0;
    uint64_t src0Size = 0;
    uint64_t src1Size = 0;
    uint8_t dstLogicPos = 0;
    uint8_t selMaskLogicPos = 0;
    uint8_t src0LogicPos = 0;
    uint8_t src1LogicPos = 0;
    uint8_t dstPos = 0;
    uint8_t selMaskPos = 0;
    uint8_t src0Pos = 0;
    uint8_t src1Pos = 0;
    uint32_t calCount = 0;
};

struct VecGatherApiParams {
    VecGatherApiParams() {}
    VecGatherApiParams(
        uint64_t dstAddrIn, uint64_t srcAddrIn, uint64_t offsetAddrIn, uint8_t repeatIn, uint16_t dstBlockStrideIn,
        uint16_t dstRepeatStrideIn, uint32_t dstDtypeBytesIn, uint32_t srcDtypeBytesIn, uint32_t offsetDtypeBytesIn,
        uint64_t dstSizeIn, uint64_t srcSizeIn, uint64_t offsetSizeIn, uint8_t dstPosIn, uint8_t srcPosIn,
        uint8_t offsetPosIn)
    {
        dstAddr = dstAddrIn;
        srcAddr = srcAddrIn;
        offsetAddr = offsetAddrIn;
        repeatTimes = repeatIn;
        dstBlockStride = dstBlockStrideIn;
        dstRepeatStride = dstRepeatStrideIn;
        dstDtypeBytes = dstDtypeBytesIn;
        srcDtypeBytes = srcDtypeBytesIn;
        offsetDtypeBytes = offsetDtypeBytesIn;
        dstSize = dstSizeIn;
        srcSize = srcSizeIn;
        offsetSize = offsetSizeIn;
        dstLogicPos = dstPosIn;
        srcLogicPos = srcPosIn;
        offsetLogicPos = offsetPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        srcPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(srcPosIn)));
        offsetPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(offsetPosIn)));
    }

    VecGatherApiParams(
        uint64_t dstAddrIn, uint64_t srcAddrIn, uint64_t offsetAddrIn, uint32_t srcBaseOffsetIn, uint8_t repeatIn,
        uint16_t dstBlockStrideIn, uint16_t dstRepeatStrideIn, uint32_t dstDtypeBytesIn, uint32_t srcDtypeBytesIn,
        uint32_t offsetDtypeBytesIn, uint64_t dstSizeIn, uint64_t srcSizeIn, uint64_t offsetSizeIn, uint8_t dstPosIn,
        uint8_t srcPosIn, uint8_t offsetPosIn)
    {
        dstAddr = dstAddrIn;
        srcAddr = srcAddrIn;
        offsetAddr = offsetAddrIn;
        srcBaseOffset = srcBaseOffsetIn;
        repeatTimes = repeatIn;
        dstBlockStride = dstBlockStrideIn;
        dstRepeatStride = dstRepeatStrideIn;
        dstDtypeBytes = dstDtypeBytesIn;
        srcDtypeBytes = srcDtypeBytesIn;
        offsetDtypeBytes = offsetDtypeBytesIn;
        dstSize = dstSizeIn;
        srcSize = srcSizeIn;
        offsetSize = offsetSizeIn;
        dstLogicPos = dstPosIn;
        srcLogicPos = srcPosIn;
        offsetLogicPos = offsetPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        srcPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(srcPosIn)));
        offsetPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(offsetPosIn)));
    }

    VecGatherApiParams(
        uint64_t dstAddrIn, uint64_t srcAddrIn, uint64_t offsetAddrIn, uint32_t srcBaseOffsetIn,
        uint32_t dstDtypeBytesIn, uint32_t srcDtypeBytesIn, uint32_t offsetDtypeBytesIn, uint64_t dstSizeIn,
        uint64_t srcSizeIn, uint64_t offsetSizeIn, uint8_t dstPosIn, uint8_t srcPosIn, uint8_t offsetPosIn,
        uint32_t calCountIn)
    {
        dstAddr = dstAddrIn;
        srcAddr = srcAddrIn;
        offsetAddr = offsetAddrIn;
        srcBaseOffset = srcBaseOffsetIn;
        dstDtypeBytes = dstDtypeBytesIn;
        srcDtypeBytes = srcDtypeBytesIn;
        offsetDtypeBytes = offsetDtypeBytesIn;
        dstSize = dstSizeIn;
        srcSize = srcSizeIn;
        offsetSize = offsetSizeIn;
        dstLogicPos = dstPosIn;
        srcLogicPos = srcPosIn;
        offsetLogicPos = offsetPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        srcPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(srcPosIn)));
        offsetPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(offsetPosIn)));
        calCount = calCountIn;
    }

    uint64_t dstAddr = 0;
    uint64_t srcAddr = 0;
    uint64_t offsetAddr = 0;
    uint32_t srcBaseOffset = 0;
    uint8_t repeatTimes = 0;
    uint16_t dstBlockStride = 0;
    uint16_t dstRepeatStride = 0;
    uint32_t dstDtypeBytes = 0;
    uint32_t srcDtypeBytes = 0;
    uint32_t offsetDtypeBytes = 0;
    uint64_t dstSize = 0;
    uint64_t srcSize = 0;
    uint64_t offsetSize = 0;
    uint8_t dstLogicPos = 0;
    uint8_t srcLogicPos = 0;
    uint8_t offsetLogicPos = 0;
    uint8_t dstPos = 0;
    uint8_t srcPos = 0;
    uint8_t offsetPos = 0;
    uint32_t calCount = 0;
};

const uint8_t BLKSTR = 1;
const uint8_t REPSTR = 8;
const uint16_t REPSIZE = 256;
struct VecGatherMaskApiParams {
    VecGatherMaskApiParams() {}
    VecGatherMaskApiParams(
        uint64_t dstAddrIn, uint64_t src0AddrIn, uint64_t src1AddrIn, bool reduceModeIn, uint8_t src0BlockStrideIn,
        uint16_t repeatTimesIn, uint16_t src0RepeatStrideIn, uint16_t src1RepeatStrideIn, uint64_t rsvdCntIn,
        uint32_t dstDtypeBytesIn, uint32_t src0DtypeBytesIn, uint32_t src1DtypeBytesIn, uint64_t dstSizeIn,
        uint64_t src0SizeIn, uint64_t src1SizeIn, uint8_t dstPosIn, uint8_t src0PosIn, uint8_t src1PosIn)
    {
        dstAddr = dstAddrIn;
        src0Addr = src0AddrIn;
        src1Addr = src1AddrIn;
        src1Pattern = 0;
        reduceMode = reduceModeIn;
        dstBlockStride = BLKSTR;
        src0BlockStride = src0BlockStrideIn;
        src1BlockStride = BLKSTR;
        repeatTimes = repeatTimesIn;
        dstRepeatStride = REPSTR;
        src0RepeatStride = src0RepeatStrideIn;
        src1RepeatStride = src1RepeatStrideIn;
        rsvdCnt = rsvdCntIn;
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

    VecGatherMaskApiParams(
        uint64_t dstAddrIn, uint64_t src0AddrIn, uint8_t src1PatternIn, bool reduceModeIn, uint8_t src0BlockStrideIn,
        uint16_t repeatTimesIn, uint16_t src0RepeatStrideIn, uint16_t src1RepeatStrideIn, uint64_t rsvdCntIn,
        uint32_t dstDtypeBytesIn, uint32_t src0DtypeBytesIn, uint64_t dstSizeIn, uint64_t src0SizeIn, uint8_t dstPosIn,
        uint8_t src0PosIn)
    {
        dstAddr = dstAddrIn;
        src0Addr = src0AddrIn;
        src1Addr = src0AddrIn + REPSIZE * src0DtypeBytesIn;
        src1Pattern = src1PatternIn;
        reduceMode = reduceModeIn;
        dstBlockStride = BLKSTR;
        src0BlockStride = src0BlockStrideIn;
        src1BlockStride = BLKSTR;
        repeatTimes = repeatTimesIn;
        dstRepeatStride = REPSTR;
        src0RepeatStride = src0RepeatStrideIn;
        src1RepeatStride = src1RepeatStrideIn;
        rsvdCnt = rsvdCntIn;
        dstDtypeBytes = dstDtypeBytesIn;
        src0DtypeBytes = src0DtypeBytesIn;
        src1DtypeBytes = src0DtypeBytesIn;
        dstSize = dstSizeIn;
        src0Size = src0SizeIn;
        src1Size = 0;
        dstLogicPos = dstPosIn;
        src0LogicPos = src0PosIn;
        src1LogicPos = src0PosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
        src1Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src0PosIn)));
    }

    uint64_t dstAddr = 0;
    uint64_t src0Addr = 0;
    uint64_t src1Addr = 0;
    bool reduceMode = 0;
    uint8_t dstBlockStride = 0;
    uint8_t src0BlockStride = 0;
    uint8_t src1BlockStride = 0;
    uint16_t repeatTimes = 0;
    uint16_t dstRepeatStride = 0;
    uint16_t src0RepeatStride = 0;
    uint16_t src1RepeatStride = 0;
    uint64_t rsvdCnt = 0;
    uint32_t dstDtypeBytes = 0;
    uint32_t src0DtypeBytes = 0;
    uint32_t src1DtypeBytes = 0;
    uint64_t dstSize = 0;
    uint64_t src0Size = 0;
    uint64_t src1Size = 0;
    uint8_t src1Pattern = 0;
    uint8_t dstLogicPos = 0;
    uint8_t src0LogicPos = 0;
    uint8_t src1LogicPos = 0;
    uint8_t dstPos = 0;
    uint8_t src0Pos = 0;
    uint8_t src1Pos = 0;
};

bool CheckFuncVecSelectImplForMaskArray(VecSelectApiParams& chkParams, const uint64_t mask[], const char* intriName);
bool CheckFuncVecSelectImpl(VecSelectApiParams& chkParams, const uint64_t mask, const char* intriName);
bool CheckFuncVecSelectImpl(VecSelectApiParams& chkParams, const char* intriName);

bool CheckFuncGatherbImpl(VecGatherApiParams& chkParams, uint32_t dtypeSize, const char* intriName);

bool CheckFuncGatherImpl(VecGatherApiParams& chkParams, const uint64_t mask, const char* intriName);
bool CheckFuncGatherImpl(VecGatherApiParams& chkParams, const uint64_t mask[], const char* intriName);
bool CheckFuncGatherImpl(VecGatherApiParams& chkParams, const char* intriName);

bool CheckFuncVecGatherMaskImpl(VecGatherMaskApiParams& chkParams, const uint32_t mask, const char* intriName);
} // namespace check
} // namespace AscendC
#endif
#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_SELECT_UTIL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_SELECT_UTIL_H__
#endif
