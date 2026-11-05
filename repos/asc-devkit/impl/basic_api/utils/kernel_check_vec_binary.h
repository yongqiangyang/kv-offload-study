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
 * \file kernel_check_vec_binary.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/utils/kernel_check_vec_binary.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_BINARY_H__
#endif
#ifndef ASCENDC_MODULE_CHECK_VEC_BINARY_H
#define ASCENDC_MODULE_CHECK_VEC_BINARY_H

#if ASCENDC_CPU_DEBUG
#include "../kernel_check_util.h"
#include "../../../include/basic_api/kernel_common.h"
#include "../../../include/basic_api/kernel_struct_binary.h"

namespace AscendC {
template <typename T>
check::VecBinaryApiParams BuildVecBinaryApiParams(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    using PrimType = PrimT<T>;

    return check::VecBinaryApiParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        repeatTime,
        static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.src0BlkStride),
        static_cast<uint16_t>(repeatParams.src1BlkStride),
        static_cast<uint16_t>(repeatParams.dstRepStride),
        static_cast<uint16_t>(repeatParams.src0RepStride),
        static_cast<uint16_t>(repeatParams.src1RepStride),
        static_cast<uint32_t>(sizeof(PrimType)),
        static_cast<uint32_t>(sizeof(PrimType)),
        static_cast<uint32_t>(sizeof(PrimType)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimType)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimType)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimType)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition())};
}

template <typename T>
bool CheckFuncVecBinary(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1, const uint64_t mask[],
    const uint8_t repeatTime, const BinaryRepeatParams& repeatParams, const char* intriName)
{
    auto chkParams = BuildVecBinaryApiParams(dst, src0, src1, repeatTime, repeatParams);
    return CheckFuncVecBinaryImplForMaskArray(chkParams, mask, intriName);
}

template <typename T>
bool CheckFuncVecBinary(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1, const uint64_t mask,
    const uint8_t repeatTime, const BinaryRepeatParams& repeatParams, const char* intriName)
{
    auto chkParams = BuildVecBinaryApiParams(dst, src0, src1, repeatTime, repeatParams);
    return CheckFuncVecBinaryImpl(chkParams, mask, intriName);
}

template <typename T>
bool CheckFuncVecBinary(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1, const int32_t& count,
    const char* intriName)
{
    using PrimType = PrimT<T>;

    check::VecBinaryApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        static_cast<uint32_t>(sizeof(PrimType)),
        static_cast<uint32_t>(sizeof(PrimType)),
        static_cast<uint32_t>(sizeof(PrimType)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimType)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimType)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimType)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition()),
        static_cast<uint32_t>(count)};
    return CheckFuncVecBinaryImpl(chkParams, intriName);
}

template <typename T, typename U>
check::VecSelectApiParams BuildVecSelectApiParams(
    const LocalTensor<T>& dst, const LocalTensor<U>& selMask, const LocalTensor<T>& src0, const LocalTensor<T>& src1,
    uint8_t repeatTime, const BinaryRepeatParams& repeatParams)
{
    using PrimDstSrcType = PrimT<T>;
    using PrimSelMaskType = PrimT<U>;

    return check::VecSelectApiParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(selMask.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        repeatTime,
        static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.src0BlkStride),
        static_cast<uint16_t>(repeatParams.src1BlkStride),
        static_cast<uint16_t>(repeatParams.dstRepStride),
        static_cast<uint16_t>(repeatParams.src0RepStride),
        static_cast<uint16_t>(repeatParams.src1RepStride),
        static_cast<uint32_t>(sizeof(PrimDstSrcType)),
        static_cast<uint32_t>(sizeof(PrimSelMaskType)),
        static_cast<uint32_t>(sizeof(PrimDstSrcType)),
        static_cast<uint32_t>(sizeof(PrimDstSrcType)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimDstSrcType)),
        static_cast<uint64_t>(selMask.GetSize() * sizeof(PrimSelMaskType)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimDstSrcType)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimDstSrcType)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(selMask.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition())};
}

template <typename T, typename U>
bool CheckFuncSelectVec(
    const LocalTensor<T>& dst, const LocalTensor<U>& selMask, const LocalTensor<T>& src0, const LocalTensor<T>& src1,
    uint64_t mask[], uint8_t repeatTime, const BinaryRepeatParams& repeatParams, const char* intriName)
{
    auto chkParams = BuildVecSelectApiParams(dst, selMask, src0, src1, repeatTime, repeatParams);
    return CheckFuncVecSelectImplForMaskArray(chkParams, mask, intriName);
}

template <typename T, typename U>
bool CheckFuncSelectVec(
    const LocalTensor<T>& dst, const LocalTensor<U>& selMask, const LocalTensor<T>& src0, const LocalTensor<T>& src1,
    uint64_t mask, uint8_t repeatTime, const BinaryRepeatParams& repeatParams, const char* intriName)
{
    auto chkParams = BuildVecSelectApiParams(dst, selMask, src0, src1, repeatTime, repeatParams);
    return CheckFuncVecSelectImpl(chkParams, mask, intriName);
}

template <typename T, typename U>
bool CheckFuncSelectVec(
    const LocalTensor<T>& dst, const LocalTensor<U>& selMask, const LocalTensor<T>& src0, const LocalTensor<T>& src1,
    const int32_t& count, const char* intriName)
{
    using PrimDstSrcType = PrimT<T>;
    using PrimSelMaskType = PrimT<U>;

    check::VecSelectApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(selMask.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        static_cast<uint32_t>(sizeof(PrimDstSrcType)),
        static_cast<uint32_t>(sizeof(PrimSelMaskType)),
        static_cast<uint32_t>(sizeof(PrimDstSrcType)),
        static_cast<uint32_t>(sizeof(PrimDstSrcType)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimDstSrcType)),
        static_cast<uint64_t>(selMask.GetSize() * sizeof(PrimSelMaskType)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimDstSrcType)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimDstSrcType)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(selMask.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition()),
        static_cast<uint32_t>(count)};
    return CheckFuncVecSelectImpl(chkParams, intriName);
}

template <typename T, typename U>
check::VecBinaryApiParams BuildVecBinaryApiParamsDiffType(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<U>& src1, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    using PrimDstType = PrimT<T>;
    using PrimSrcType = PrimT<U>;

    return check::VecBinaryApiParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        repeatTime,
        static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.src0BlkStride),
        static_cast<uint16_t>(repeatParams.src1BlkStride),
        static_cast<uint16_t>(repeatParams.dstRepStride),
        static_cast<uint16_t>(repeatParams.src0RepStride),
        static_cast<uint16_t>(repeatParams.src1RepStride),
        static_cast<uint32_t>(sizeof(PrimDstType)),
        static_cast<uint32_t>(sizeof(PrimSrcType)),
        static_cast<uint32_t>(sizeof(PrimSrcType)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimDstType)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimSrcType)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimSrcType)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition())};
}

template <typename T, typename U>
bool CheckFuncVecBinaryDiffType(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<U>& src1, const uint64_t mask[],
    const uint8_t repeatTime, const BinaryRepeatParams& repeatParams, const char* intriName)
{
    auto chkParams = BuildVecBinaryApiParamsDiffType(dst, src0, src1, repeatTime, repeatParams);
    return CheckFuncVecBinaryImplForMaskArray(chkParams, mask, intriName);
}

template <typename T, typename U>
bool CheckFuncVecBinaryDiffType(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<U>& src1, const uint64_t mask,
    const uint8_t repeatTime, const BinaryRepeatParams& repeatParams, const char* intriName)
{
    auto chkParams = BuildVecBinaryApiParamsDiffType(dst, src0, src1, repeatTime, repeatParams);
    return CheckFuncVecBinaryImpl(chkParams, mask, intriName);
}

template <typename T, typename U>
bool CheckFuncVecBinaryDiffType(
    const LocalTensor<T>& dst, const LocalTensor<U>& src0, const LocalTensor<U>& src1, const int32_t& count,
    const char* intriName)
{
    using PrimDstType = PrimT<T>;
    using PrimSrcType = PrimT<U>;

    check::VecBinaryApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        static_cast<uint32_t>(sizeof(PrimDstType)),
        static_cast<uint32_t>(sizeof(PrimSrcType)),
        static_cast<uint32_t>(sizeof(PrimSrcType)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimDstType)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimSrcType)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimSrcType)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition()),
        static_cast<uint32_t>(count)};
    return CheckFuncVecBinaryImpl(chkParams, intriName);
}

template <typename T, typename U>
check::VecBinaryApiParams BuildVecBinaryApiParamsCmp(
    const LocalTensor<U>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1, const uint8_t repeatTime,
    const BinaryRepeatParams& repeatParams)
{
    return check::VecBinaryApiParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        repeatTime,
        static_cast<uint16_t>(repeatParams.dstBlkStride),
        static_cast<uint16_t>(repeatParams.src0BlkStride),
        static_cast<uint16_t>(repeatParams.src1BlkStride),
        static_cast<uint16_t>(repeatParams.dstRepStride),
        static_cast<uint16_t>(repeatParams.src0RepStride),
        static_cast<uint16_t>(repeatParams.src1RepStride),
        static_cast<uint32_t>(sizeof(PrimT<U>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<U>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition())};
}

template <typename T, typename U>
bool CheckFuncVecBinaryCmp(
    const LocalTensor<U>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1, const uint64_t mask[],
    const uint8_t repeatTime, const BinaryRepeatParams& repeatParams, const char* intriName)
{
    auto chkParams = BuildVecBinaryApiParamsCmp(dst, src0, src1, repeatTime, repeatParams);
    return CheckFuncVecBinaryCmpImplForMaskArray(chkParams, mask, intriName);
}

template <typename T, typename U>
bool CheckFuncVecBinaryCmp(
    const LocalTensor<U>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1, const uint64_t mask,
    const uint8_t repeatTime, const BinaryRepeatParams& repeatParams, const char* intriName)
{
    auto chkParams = BuildVecBinaryApiParamsCmp(dst, src0, src1, repeatTime, repeatParams);
    return CheckFuncVecBinaryCmpImpl(chkParams, mask, intriName);
}

template <typename T, typename U>
bool CheckFuncVecBinaryCmp(
    const LocalTensor<U>& dst, const LocalTensor<T>& src0, const LocalTensor<T>& src1, const int32_t& count,
    const char* intriName)
{
    check::VecBinaryApiParams chkParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(dst.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        static_cast<uint32_t>(sizeof(PrimT<U>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(dst.GetSize() * sizeof(PrimT<U>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(dst.GetPosition()),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition()),
        static_cast<uint32_t>(count)};
    return CheckFuncVecBinaryCmpImpl(chkParams, intriName);
}

template <typename T>
check::VecCmpRgtApiParams BuildVecCmpRgtApiParams(
    const LocalTensor<T>& src0, const LocalTensor<T>& src1, const BinaryRepeatParams& repeatParams)
{
    return check::VecCmpRgtApiParams{
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src0.GetPhyAddr())),
        static_cast<uint64_t>(reinterpret_cast<uintptr_t>(src1.GetPhyAddr())),
        static_cast<uint16_t>(repeatParams.src0BlkStride),
        static_cast<uint16_t>(repeatParams.src1BlkStride),
        static_cast<uint16_t>(repeatParams.src0RepStride),
        static_cast<uint16_t>(repeatParams.src1RepStride),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint32_t>(sizeof(PrimT<T>)),
        static_cast<uint64_t>(src0.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint64_t>(src1.GetSize() * sizeof(PrimT<T>)),
        static_cast<uint8_t>(src0.GetPosition()),
        static_cast<uint8_t>(src1.GetPosition())};
}

template <typename T>
bool CheckFuncVecBinaryCmpRgt(
    const LocalTensor<T>& src0, const LocalTensor<T>& src1, const uint64_t mask[],
    const BinaryRepeatParams& repeatParams, const char* intriName)
{
    auto chkParams = BuildVecCmpRgtApiParams(src0, src1, repeatParams);
    return CheckFuncVecCmpRgtImplForMaskArray(chkParams, mask, intriName);
}

template <typename T>
bool CheckFuncVecBinaryCmpRgt(
    const LocalTensor<T>& src0, const LocalTensor<T>& src1, const uint64_t mask, const BinaryRepeatParams& repeatParams,
    const char* intriName)
{
    auto chkParams = BuildVecCmpRgtApiParams(src0, src1, repeatParams);
    return CheckFuncVecCmpRgtImpl(chkParams, mask, intriName);
}

} // namespace AscendC
#endif

#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_BINARY_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_CHECK_VEC_BINARY_H__
#endif
