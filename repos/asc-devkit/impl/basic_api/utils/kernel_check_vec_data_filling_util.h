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
 * \file kernel_check_vec_data_filling_util.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_check_vec_data_filling_util.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_DATA_FILLING_UTIL_H__
#endif
#ifndef ASCENDC_CHECK_VEC_DATA_FILLING_UTIL_H
#define ASCENDC_CHECK_VEC_DATA_FILLING_UTIL_H
#if ASCENDC_CPU_DEBUG
#include <string>
#include "../kernel_utils.h"
namespace AscendC {
namespace check {
struct VecDupApiParams {
    VecDupApiParams() {}
    VecDupApiParams(
        uint64_t dstAddrIn, uint8_t repeatIn, uint16_t dstBlockStrideIn, uint16_t dstRepeatStrideIn,
        uint32_t dstDtypeBytesIn, uint64_t dstSizeIn, uint8_t dstPosIn)
    {
        dstAddr = dstAddrIn;
        repeatTimes = repeatIn;
        dstBlockStride = dstBlockStrideIn;
        dstRepeatStride = dstRepeatStrideIn;
        dstDtypeBytes = dstDtypeBytesIn;
        dstSize = dstSizeIn;
        dstLogicPos = dstPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
    }
    VecDupApiParams(uint64_t dstAddrIn, uint32_t dstDtypeBytesIn, uint64_t dstSizeIn, uint8_t dstPosIn, uint32_t count)
    {
        dstAddr = dstAddrIn;
        dstDtypeBytes = dstDtypeBytesIn;
        dstSize = dstSizeIn;
        dstLogicPos = dstPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        calCount = count;
    }
    uint64_t dstAddr = 0;
    uint8_t repeatTimes = 0;
    uint16_t dstBlockStride = 0;
    uint16_t dstRepeatStride = 0;
    uint32_t dstDtypeBytes = 0;
    uint64_t dstSize = 0;
    uint8_t dstLogicPos = 0;
    uint8_t dstPos = 0;
    uint32_t calCount = 0;
};
struct VecCreateVecIndexApiParams {
    VecCreateVecIndexApiParams() {}
    VecCreateVecIndexApiParams(
        uint64_t dstAddrIn, uint8_t repeatIn, uint16_t dstBlkStrideIn, uint16_t dstRepStrideIn,
        uint32_t dstDtypeBytesIn, uint64_t dstSizeIn, uint8_t dstPosIn, uint32_t calCountIn)
    {
        dstAddr = dstAddrIn;
        repeatTimes = repeatIn;
        dstBlkStride = dstBlkStrideIn;
        dstRepStride = dstRepStrideIn;
        dstDtypeBytes = dstDtypeBytesIn;
        dstSize = dstSizeIn;
        dstLogicPos = dstPosIn;
        dstPos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(dstPosIn)));
        calCount = calCountIn;
    }
    uint64_t dstAddr = 0;
    uint8_t repeatTimes = 0;
    uint16_t dstBlkStride = 0;
    uint16_t dstRepStride = 0;
    uint32_t dstDtypeBytes = 0;
    uint64_t dstSize = 0;
    uint8_t dstLogicPos = 0;
    uint8_t dstPos = 0;
    uint32_t calCount = 0;
};
bool CheckFunDupImplForMaskArray(VecDupApiParams& chkParams, const uint64_t mask[], const char* intriName);
bool CheckFunDupImpl(VecDupApiParams& chkParams, const uint64_t mask, const char* intriName);
bool CheckFunDupImpl(VecDupApiParams& chkParams, const char* intriName);
bool CheckFuncCreateVecIndexImpl(VecCreateVecIndexApiParams& chkParams, const uint64_t mask, const char* intriName);
bool CheckFuncCreateVecIndexImpl(VecCreateVecIndexApiParams& chkParams, const uint64_t mask[], const char* intriName);
bool CheckFuncCreateVecIndexImpl(VecCreateVecIndexApiParams& chkParams, const char* intriName);
} // namespace check
} // namespace AscendC
#endif
#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_DATA_FILLING_UTIL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_DATA_FILLING_UTIL_H__
#endif
