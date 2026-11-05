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
 * \file kernel_check_vec_padding_util.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_check_vec_padding_util.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_PADDING_UTIL_H__
#endif
#ifndef ASCENDC_CHECK_VEC_PADDING_UTIL_H
#define ASCENDC_CHECK_VEC_PADDING_UTIL_H
#if ASCENDC_CPU_DEBUG
#include <string>
#include "../kernel_utils.h"
namespace AscendC {
namespace check {
struct VectorPaddingApiParams {
    VectorPaddingApiParams() {}
    VectorPaddingApiParams(
        uint64_t dstAddrIn, uint64_t srcAddrIn, uint8_t repeatIn, uint16_t dstBlockStrideIn, uint16_t srcBlockStrideIn,
        uint16_t dstRepeatStrideIn, uint16_t srcRepeatStrideIn, uint32_t dstDtypeBytesIn, uint32_t srcDtypeBytesIn,
        uint64_t dstSizeIn, uint64_t srcSizeIn, uint8_t dstPosIn, uint8_t srcPosIn, uint8_t padModeIn, bool padSideIn)
    {
        dstAddr = dstAddrIn;
        srcAddr = srcAddrIn;
        repeatTimes = repeatIn;
        dstBlockStride = dstBlockStrideIn;
        srcBlockStride = srcBlockStrideIn;
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
        padMode = padModeIn;
        padSide = padSideIn;
    }
    VectorPaddingApiParams(
        uint64_t dstAddrIn, uint64_t srcAddrIn, uint32_t dstDtypeBytesIn, uint32_t srcDtypeBytesIn, uint64_t dstSizeIn,
        uint64_t srcSizeIn, uint8_t dstPosIn, uint8_t srcPosIn, uint32_t count, uint8_t padModeIn, bool padSideIn)
    {
        dstAddr = dstAddrIn;
        srcAddr = srcAddrIn;
        dstDtypeBytes = dstDtypeBytesIn;
        srcDtypeBytes = srcDtypeBytesIn;
        dstSize = dstSizeIn;
        srcSize = srcSizeIn;
        dstLogicPos = dstPosIn;
        srcLogicPos = srcPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        srcPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(srcPosIn)));
        calCount = count;
        padMode = padModeIn;
        padSide = padSideIn;
    }
    uint64_t dstAddr = 0;
    uint64_t srcAddr = 0;
    uint8_t repeatTimes = 0;
    uint16_t dstBlockStride = 0;
    uint16_t srcBlockStride = 0;
    uint16_t dstRepeatStride = 0;
    uint16_t srcRepeatStride = 0;
    uint32_t dstDtypeBytes = 0;
    uint32_t srcDtypeBytes = 0;
    uint64_t dstSize = 0;
    uint64_t srcSize = 0;
    uint8_t dstLogicPos = 0;
    uint8_t srcLogicPos = 0;
    uint8_t dstPos = 0;
    uint8_t srcPos = 0;
    uint32_t calCount = 0;
    uint8_t padMode = 0;
    bool padSide = false;
};

struct VecBroadCastToMMApiParams {
    VecBroadCastToMMApiParams() {}
    VecBroadCastToMMApiParams(
        uint64_t dstAddrIn, uint64_t srcAddrIn, uint32_t dstDtypeBytesIn, uint32_t srcDtypeBytesIn, uint64_t dstSizeIn,
        uint64_t srcSizeIn, uint8_t dstPosIn, uint8_t srcPosIn, uint32_t blockCountIn, uint8_t blockLenIn,
        uint8_t srcGapIn, uint8_t dstGapIn)
    {
        dstAddr = dstAddrIn;
        srcAddr = srcAddrIn;
        dstDtypeBytes = dstDtypeBytesIn;
        srcDtypeBytes = srcDtypeBytesIn;
        dstSize = dstSizeIn;
        srcSize = srcSizeIn;
        dstLogicPos = dstPosIn;
        srcLogicPos = srcPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        srcPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(srcPosIn)));
        blockCount = blockCountIn;
        blockLen = blockLenIn;
        srcGap = srcGapIn;
        dstGap = dstGapIn;
    }
    uint64_t dstAddr = 0;
    uint64_t srcAddr = 0;
    uint32_t dstDtypeBytes = 0;
    uint32_t srcDtypeBytes = 0;
    uint64_t dstSize = 0;
    uint64_t srcSize = 0;
    uint8_t dstLogicPos = 0;
    uint8_t srcLogicPos = 0;
    uint8_t dstPos = 0;
    uint8_t srcPos = 0;
    uint32_t calCount = 0;
    uint32_t blockCount = 0;
    uint8_t blockLen = 0;
    uint8_t srcGap = 0;
    uint8_t dstGap = 0;
};

struct VecBroadCastApiParams {
    VecBroadCastApiParams() {}
    VecBroadCastApiParams(
        uint64_t dstAddrIn, uint64_t srcAddrIn, uint8_t repeatIn, uint16_t dstBlockStrideIn, uint16_t dstRepeatStrideIn,
        uint32_t dstDtypeBytesIn, uint32_t srcDtypeBytesIn, uint64_t dstSizeIn, uint64_t srcSizeIn, uint8_t dstPosIn,
        uint8_t srcPosIn)
    {
        dstAddr = dstAddrIn;
        srcAddr = srcAddrIn;
        repeatTimes = repeatIn;
        dstBlockStride = dstBlockStrideIn;
        dstRepeatStride = dstRepeatStrideIn;
        dstDtypeBytes = dstDtypeBytesIn;
        srcDtypeBytes = srcDtypeBytesIn;
        dstSize = dstSizeIn;
        srcSize = srcSizeIn;
        dstLogicPos = dstPosIn;
        srcLogicPos = srcPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        srcPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(srcPosIn)));
    }
    uint64_t dstAddr = 0;
    uint64_t srcAddr = 0;
    uint8_t repeatTimes = 0;
    uint16_t dstBlockStride = 0;
    uint16_t dstRepeatStride = 0;
    uint32_t dstDtypeBytes = 0;
    uint32_t srcDtypeBytes = 0;
    uint64_t dstSize = 0;
    uint64_t srcSize = 0;
    uint8_t dstLogicPos = 0;
    uint8_t srcLogicPos = 0;
    uint8_t dstPos = 0;
    uint8_t srcPos = 0;
};

bool CheckVectorPaddingForMaskArray(VectorPaddingApiParams& chkParams, const uint64_t mask[], const char* intriName);
bool CheckVectorPadding(VectorPaddingApiParams& chkParams, const uint64_t mask, const char* intriName);
bool CheckVectorPadding(VectorPaddingApiParams& chkParams, const char* intriName);
bool CheckFuncBroadCastToMMImpl(VecBroadCastToMMApiParams& chkParams, const char* intriName);
bool CheckFunBcBImpl(VecBroadCastApiParams& chkParams, uint32_t dtypeSize, const char* intriName);
} // namespace check
} // namespace AscendC
#endif
#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_PADDING_UTIL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_PADDING_UTIL_H__
#endif
