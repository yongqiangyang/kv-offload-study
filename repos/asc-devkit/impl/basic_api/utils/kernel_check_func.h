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
 * \file kernel_check_func.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_check_func.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_FUNC_H__
#endif
#ifndef ASCENDC_MODULE_CHECK_FUNC_H
#define ASCENDC_MODULE_CHECK_FUNC_H

#if ASCENDC_CPU_DEBUG
#include "../kernel_check_util.h"
#include "../../../include/basic_api/kernel_common.h"
#include "../../../include/basic_api/kernel_struct_brcb.h"
#include "../../../include/basic_api/kernel_struct_gather.h"
#include "../../../include/basic_api/kernel_struct_transpose.h"
#include "../../../include/basic_api/kernel_struct_proposal.h"

namespace AscendC {
template <typename T>
bool CheckFunDup(
    const LocalTensor<T>& dst, const uint64_t mask, const uint8_t repeatTime, const uint16_t dstBlockStride,
    const uint8_t dstRepeatStride, const char* intriName)
{
    check::VecDupApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        repeatTime,
        static_cast<uint16_t>(dstBlockStride),
        static_cast<uint16_t>(dstRepeatStride),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition())};
    return CheckFunDupImpl(chkParams, mask, intriName);
}

template <typename T>
bool CheckFunDup(
    const LocalTensor<T>& dst, const uint64_t mask[], const uint8_t repeatTime, const uint16_t dstBlockStride,
    const uint8_t dstRepeatStride, const char* intriName)
{
    check::VecDupApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        repeatTime,
        static_cast<uint16_t>(dstBlockStride),
        static_cast<uint16_t>(dstRepeatStride),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition())};
    return CheckFunDupImplForMaskArray(chkParams, mask, intriName);
}

template <typename T>
bool CheckFunDup(const LocalTensor<T>& dst, const int32_t& count, const char* intriName)
{
    check::VecDupApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())), static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)), static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint32_t>(count)};
    return CheckFunDupImpl(chkParams, intriName);
}

template <typename T>
bool CheckFunBcB(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const uint8_t repeatTime,
    const BrcbRepeatParams& repeatParams, const char* intriName)
{
    check::VecBroadCastApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        repeatTime,
        static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.dstRepStride),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition())};
    return CheckFunBcBImpl(chkParams, sizeof(PrimT<T>), intriName);
}

template <typename T, typename U>
bool CheckFuncGatherb(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& offset, const uint8_t repeatTime,
    const GatherRepeatParams& repeatParams, const char* intriName)
{
    check::VecGatherApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(offset.GetPhyAddr())),
        repeatTime,
        static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.dstRepStride),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<U>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(offset.GetSize() * sizeof(PrimT<U>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(offset.GetPosition())};
    return CheckFuncGatherbImpl(chkParams, sizeof(PrimT<T>), intriName);
}

template <typename T, typename U>
check::VecGatherApiParams BuildVecGatherApiParams(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& offset, const uint32_t srcBaseOffset,
    const uint8_t repeatTime, const uint16_t dstRepStride)
{
    return check::VecGatherApiParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(offset.GetPhyAddr())),
        srcBaseOffset,
        repeatTime,
        static_cast<uint16_t>(1),
        dstRepStride,
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<U>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(offset.GetSize() * sizeof(PrimT<U>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(offset.GetPosition())};
}

template <typename T, typename U>
bool CheckFuncGather(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& offset, const uint32_t srcBaseOffset,
    const uint64_t mask, const uint8_t repeatTime, const uint16_t dstRepStride, const char* intriName)
{
    auto chkParams = BuildVecGatherApiParams(dst, src0, offset, srcBaseOffset, repeatTime, dstRepStride);
    return CheckFuncGatherImpl(chkParams, mask, intriName);
}

template <typename T, typename U>
bool CheckFuncGather(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& offset, const uint32_t srcBaseOffset,
    const uint64_t mask[], const uint8_t repeatTime, const uint16_t dstRepStride, const char* intriName)
{
    auto chkParams = BuildVecGatherApiParams(dst, src0, offset, srcBaseOffset, repeatTime, dstRepStride);
    return CheckFuncGatherImpl(chkParams, mask, intriName);
}

template <typename T, typename U>
bool CheckFuncGather(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& offset, const uint32_t srcBaseOffset,
    const uint32_t count, const char* intriName)
{
    check::VecGatherApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(offset.GetPhyAddr())),
        srcBaseOffset,
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<U>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(offset.GetSize() * sizeof(PrimT<U>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(offset.GetPosition()),
        count};
    return CheckFuncGatherImpl(chkParams, intriName);
}

template <typename T>
bool CheckFuncCreateVecIndex(
    const LocalTensor<T>& dst, const uint64_t mask, const uint8_t repeatTime, const uint16_t dstBlkStride,
    const uint16_t dstRepStride, const char* intriName)
{
    check::VecCreateVecIndexApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        repeatTime,
        dstBlkStride,
        dstRepStride,
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint32_t>(0)};
    return CheckFuncCreateVecIndexImpl(chkParams, mask, intriName);
}

template <typename T>
bool CheckFuncCreateVecIndex(
    const LocalTensor<T>& dst, const uint64_t mask[], const uint8_t repeatTime, const uint16_t dstBlkStride,
    const uint16_t dstRepStride, const char* intriName)
{
    check::VecCreateVecIndexApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        repeatTime,
        dstBlkStride,
        dstRepStride,
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint32_t>(0)};
    return CheckFuncCreateVecIndexImpl(chkParams, mask, intriName);
}

template <typename T>
bool CheckFuncCreateVecIndex(const LocalTensor<T>& dst, const uint32_t count, const char* intriName)
{
    check::VecCreateVecIndexApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint8_t>(1),
        static_cast<uint16_t>(1),
        static_cast<uint16_t>(1),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        count};
    return CheckFuncCreateVecIndexImpl(chkParams, intriName);
}

template <typename T>
bool CheckFuncInitConstValue(
    const LocalTensor<T>& dst, const uint16_t repeatTime, const uint16_t blockNum, const uint16_t dstGap,
    const char* intriName)
{
    check::CubeInitConstValueApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        repeatTime,
        blockNum,
        dstGap,
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition())};
    return CheckFuncInitConstValueImpl(chkParams, intriName);
}

template <typename T, typename U>
check::VecBilinearInterpolationApiParams BuildVecBilinearInterpolationApiParams(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& src0Offset, const LocalTensor<T>& src1,
    const uint8_t hRepeat, const bool repeatMode, const uint16_t dstBlkStride, const uint16_t vROffset,
    const uint8_t vRepeat)
{
    return check::VecBilinearInterpolationApiParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0Offset.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        hRepeat,
        repeatMode,
        dstBlkStride,
        vROffset,
        vRepeat,
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<U>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src0Offset.GetSize() * sizeof(PrimT<U>)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src0Offset.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition())};
}

template <typename T, typename U>
bool CheckFuncBilinearInterpolation(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& src0Offset, const LocalTensor<T>& src1,
    const uint64_t mask[], const uint8_t hRepeat, const bool repeatMode, const uint16_t dstBlkStride,
    const uint16_t vROffset, const uint8_t vRepeat, const char* intriName)
{
    auto chkParams = BuildVecBilinearInterpolationApiParams(
        dst, src0, src0Offset, src1, hRepeat, repeatMode, dstBlkStride, vROffset, vRepeat);
    return CheckFuncBilinearInterpolationImpl(chkParams, mask, intriName);
}

template <typename T, typename U>
bool CheckFuncBilinearInterpolation(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& src0Offset, const LocalTensor<T>& src1,
    const uint64_t mask, const uint8_t hRepeat, const bool repeatMode, const uint16_t dstBlkStride,
    const uint16_t vROffset, const uint8_t vRepeat, const char* intriName)
{
    auto chkParams = BuildVecBilinearInterpolationApiParams(
        dst, src0, src0Offset, src1, hRepeat, repeatMode, dstBlkStride, vROffset, vRepeat);
    return CheckFuncBilinearInterpolationImpl(chkParams, mask, intriName);
}

template <typename T>
bool CheckFunTranspose(const LocalTensor<T>& dst, const LocalTensor<T>& src0, const char* intriName)
{
    const uint16_t defaultStride = 16;
    check::VecTransposeApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        1,
        defaultStride,
        defaultStride,
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition())};
    return CheckFunTransposeImpl(chkParams, intriName);
}

template <typename T, typename U>
bool CheckFunTranspose(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& sharedTmpBuffer,
    const TransposeParamsExt& transposeParams, const char* intriName)
{
    const uint16_t defaultStride = 16;
    check::VecTransposeApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        1,
        defaultStride,
        defaultStride,
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(sharedTmpBuffer.GetSize() * sizeof(PrimT<U>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        transposeParams.nSize,
        transposeParams.cSize,
        transposeParams.hSize,
        transposeParams.wSize,
        transposeParams.transposeType};
    return CheckFunTransposeImpl(chkParams, intriName);
}

template <typename T, typename U>
bool CheckFunTransDataTo5HD(
    const LocalTensor<U>& dst, const LocalTensor<U> src0, const TransDataTo5HDParams& nchwconvParams,
    const char* intriName)
{
    check::VecTransposeApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        nchwconvParams.repeatTimes,
        nchwconvParams.dstRepStride,
        nchwconvParams.srcRepStride,
        static_cast<uint32_t>(sizeof(PrimT<U>)),
        static_cast<uint32_t>(sizeof(PrimT<U>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<U>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<U>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition())};
    return CheckFunTransposeImpl(chkParams, intriName);
}

template <typename T>
bool CheckFunTransDataTo5HD(
    const LocalTensor<T> (&dst)[16], const LocalTensor<T> (&src0)[16], const TransDataTo5HDParams& nchwconvParams,
    const char* intriName)
{
    const int8_t dataNum = 16;
    for (int8_t i = 0; i < dataNum; i++) {
        check::VecTransposeApiParams chkParams{
            static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst[i].GetPhyAddr())),
            static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0[i].GetPhyAddr())),
            nchwconvParams.repeatTimes,
            nchwconvParams.dstRepStride,
            nchwconvParams.srcRepStride,
            static_cast<uint32_t>(sizeof(PrimT<T>)),
            static_cast<uint32_t>(sizeof(PrimT<T>)),
            static_cast<uint64_t>(dst[i].GetSize() * sizeof(PrimT<T>)),
            static_cast<uint64_t>(src0[i].GetSize() * sizeof(PrimT<T>)),
            static_cast<uint8_t>(dst[i].GetPosition()),
            static_cast<uint8_t>(src0[i].GetPosition()),
            i};
        if (!CheckFunTransposeImpl(chkParams, intriName)) {
            return false;
        }
    }
    return true;
}

template <typename T>
bool CheckFunProposal(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime, const char* intriName)
{
    check::VecProposalApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src.GetPhyAddr())),
        static_cast<uint8_t>(repeatTime),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src.GetPosition())};
    return CheckFunProposalImpl(chkParams, intriName);
}

template <typename T, typename U>
bool CheckFunProposal(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<U>& src1, const int32_t repeatTime,
    const char* intriName)
{
    check::VecProposalApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        static_cast<uint8_t>(repeatTime),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<U>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimT<U>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition())};
    return CheckFunProposalImpl(chkParams, intriName);
}

template <typename T>
bool CheckFunProposal(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1, const int32_t repeatTime,
    const char* intriName)
{
    check::VecProposalApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        static_cast<uint8_t>(repeatTime),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition())};
    return CheckFunProposalImpl(chkParams, intriName);
}

template <typename T>
bool CheckFunProposal(
    const LocalTensor<T>& dst, const MrgSortSrcList<T>& src, const MrgSort4Info& params, const uint8_t elementSize,
    const char* intriName)
{
    uint64_t dstAddr = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr()));
    uint64_t src1Addr = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src.src1.GetPhyAddr()));
    uint64_t src2Addr = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src.src2.GetPhyAddr()));
    uint64_t src3Addr = (params.validBit & 0x4) ?
                            static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src.src3.GetPhyAddr())) :
                            static_cast<uint64_t>(-1);
    uint64_t src4Addr = (params.validBit & 0x8) ?
                            static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src.src4.GetPhyAddr())) :
                            static_cast<uint64_t>(-1);
    bool isContinuous = ((src1Addr + params.elementLengths[0] * elementSize) == src2Addr) &&
                        ((src2Addr + params.elementLengths[1] * elementSize) == src3Addr) &&
                        ((src3Addr + params.elementLengths[2] * elementSize) == src4Addr);
    check::VecProposalApiParams chkParams{
        dstAddr,
        src1Addr,
        static_cast<uint8_t>(params.repeatTimes),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src.src1.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src.src1.GetPosition()),
        params.validBit,
        params.elementLengths,
        1,
        params.ifExhaustedSuspension,
        isContinuous};
    bool res = CheckFunProposalImpl(chkParams, intriName); // src1 res
    chkParams.src0Addr = src2Addr;
    chkParams.src0Size = src.src2.GetSize() * sizeof(PrimT<T>);
    chkParams.src0LogicPos = static_cast<uint8_t>(src.src2.GetPosition());
    chkParams.src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src.src2.GetPosition())));
    chkParams.srcIndex = 2;                                  // 2 means src2
    res = res && CheckFunProposalImpl(chkParams, intriName); // src2 res
    if (params.validBit == 7 || params.validBit == 15) {     // 7, 15 means need calculate src3 res
        chkParams.src0Addr = src3Addr;
        chkParams.src0Size = src.src3.GetSize() * sizeof(PrimT<T>);
        chkParams.src0LogicPos = static_cast<uint8_t>(src.src3.GetPosition());
        chkParams.src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src.src3.GetPosition())));
        chkParams.srcIndex = 3;                                  // 3 means src3
        res = res && CheckFunProposalImpl(chkParams, intriName); // src3 res
    }
    if (params.validBit == 15) { // 15 means need calculate src4 res
        chkParams.src0Addr = src4Addr;
        chkParams.src0Size = src.src4.GetSize() * sizeof(PrimT<T>);
        chkParams.src0LogicPos = static_cast<uint8_t>(src.src4.GetPosition());
        chkParams.src0Pos = static_cast<uint8_t>(GetPhyType(static_cast<TPosition>(src.src4.GetPosition())));
        chkParams.srcIndex = 4;                                  // 4 means src4
        res = res && CheckFunProposalImpl(chkParams, intriName); // src4 res
    }
    return res;
}

template <typename T, typename U, bool isFullSort>
bool CheckFuncSort(
    const LocalTensor<T>& dst, const LocalTensor<T>& concat, const LocalTensor<U>& index, const LocalTensor<T>& tmp,
    const int32_t repeatTime, const char* intriName)
{
    check::SortApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(concat.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(index.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(tmp.GetPhyAddr())),
        static_cast<uint8_t>(repeatTime),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<U>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(concat.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(index.GetSize() * sizeof(PrimT<U>)),
        static_cast<uint64_t>(tmp.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(concat.GetPosition()),
        static_cast<uint8_t>(index.GetPosition()),
        static_cast<uint8_t>(tmp.GetPosition()),
        isFullSort};
    return CheckSortImpl(chkParams, intriName);
}
} // namespace AscendC
#endif

#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_FUNC_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_FUNC_H__
#endif
