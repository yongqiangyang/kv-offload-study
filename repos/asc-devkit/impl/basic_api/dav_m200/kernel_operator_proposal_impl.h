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
 * \file kernel_operator_proposal_impl.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_m200/kernel_operator_proposal_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_PROPOSAL_IMPL_H
#define ASCENDC_MODULE_OPERATOR_PROPOSAL_IMPL_H
#include "../kernel_operator_proposal_base_impl.h"
#include "../../../include/basic_api/kernel_struct_proposal.h"

namespace AscendC {
constexpr uint32_t singleSortElementCountV220 = 32;
constexpr uint32_t singleSortElementCountV200 = 16;
constexpr uint32_t regionProposalDataSize = 8;

template <typename T>
__aicore__ inline void Vmrgsort4Cal(__ubuf__ T* dstLocal, __ubuf__ T* addrArray[MRG_SORT_ELEMENT_LEN], uint64_t config)
{
    vmrgsort4(dstLocal, addrArray, config);
}

template <typename T>
__aicore__ inline void VbitsortCal(__ubuf__ T* dstLocal, __ubuf__ T* srcLocal, const ProposalIntriParams& intriParams)
{
    vbitsort(dstLocal, srcLocal, intriParams.repeat);
}

template <typename T>
[[deprecated("NOTICE: Sort32 is not deprecated. Currently, Sort32 is an unsupported API on current device. "
             "Please check your code!")]] __aicore__ inline void
VbitsortCal(
    __ubuf__ T* dstLocal, __ubuf__ T* src0Local, __ubuf__ uint32_t* src1Local, const ProposalIntriParams& intriParams)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "Sort32");
}

template <typename T>
__aicore__ inline void Vmrgsort4Cal(
    __ubuf__ T* dstLocal, __ubuf__ T* addrArray[MRG_SORT_ELEMENT_LEN], uint64_t src1, uint64_t config)
{
    ASCENDC_REPORT_NOT_SUPPORT(false, "MrgSort with Param MrgSort4Info");
}

template <typename T>
__aicore__ inline void VconcatCal(__ubuf__ T* dstLocal, __ubuf__ T* srcLocal, const ProposalIntriParams& intriParams)
{
    vconcat(dstLocal, srcLocal, intriParams.repeat, intriParams.modeNumber);
}

template <typename T>
__aicore__ inline void VextractCal(__ubuf__ T* dstLocal, __ubuf__ T* srcLocal, const ProposalIntriParams& intriParams)
{
    vextract(dstLocal, srcLocal, intriParams.repeat, intriParams.modeNumber);
}

template <typename T>
__aicore__ inline void MrgSortCal(
    const LocalTensor<T>& dst, const MrgSortSrcList<T>& sortList, const uint16_t elementCountList[4],
    uint32_t sortedNum[4], uint16_t validBit, const int32_t repeatTime)
{
    MrgSort4Info mrgSortInfo(elementCountList, false, validBit, static_cast<uint16_t>(repeatTime));
#if ASCENDC_CPU_DEBUG
    if (!CheckFunProposal(dst, sortList, mrgSortInfo, sizeof(T) * Internal::REGION_PROPOSAL_ELEMENT_NUM, "MrgSort4")) {
        ASCENDC_REPORT_CHECK_ERROR("MrgSort4", KernelFuncType::NONE_MODE);
    }
#endif
    uint64_t config = 0;
    config |= (mrgSortInfo.repeatTimes & 0xFF);
    config |= (uint64_t(mrgSortInfo.elementLengths[0] & 0xFFF) << 8);
    config |= (uint64_t(mrgSortInfo.elementLengths[1] & 0xFFF) << 20);
    config |= (uint64_t(mrgSortInfo.elementLengths[2] & 0xFFF) << 32);
    config |= (uint64_t(mrgSortInfo.elementLengths[3] & 0xFFF) << 44);
    config |= (uint64_t(mrgSortInfo.ifExhaustedSuspension & 0x1) << 59);
    config |= (uint64_t(mrgSortInfo.validBit & 0xF) << 60);

    __ubuf__ T* addrArray[MRG_SORT_ELEMENT_LEN] = {
        (__ubuf__ T*)sortList.src1.GetPhyAddr(), (__ubuf__ T*)sortList.src2.GetPhyAddr(),
        (__ubuf__ T*)sortList.src3.GetPhyAddr(), (__ubuf__ T*)sortList.src4.GetPhyAddr()};
    Vmrgsort4Cal((__ubuf__ T*)dst.GetPhyAddr(), addrArray, config);
}

template <typename T>
__aicore__ inline void FullSortInnerLoop(
    const LocalTensor<T>& dst, const LocalTensor<T>& tmp, const uint32_t baseOffset,
    const uint16_t singleMergeTmpElementCount, const int32_t mergeTmpRepeatTimes)
{
    if (mergeTmpRepeatTimes <= 0) {
        return;
    }
    MrgSortSrcList sortList = MrgSortSrcList(tmp[0], tmp[baseOffset], tmp[2 * baseOffset], tmp[3 * baseOffset]);
    const uint16_t elementCountList[MRG_SORT_ELEMENT_LEN] = {
        singleMergeTmpElementCount, singleMergeTmpElementCount, singleMergeTmpElementCount, singleMergeTmpElementCount};
    uint32_t sortedNum[MRG_SORT_ELEMENT_LEN];
    MrgSortCal<T>(dst, sortList, elementCountList, sortedNum, 0b1111, mergeTmpRepeatTimes);
}

template <typename T>
__aicore__ inline void FullSortInnerLoopTail(
    const LocalTensor<T>& dst, const LocalTensor<T>& tmp, const uint32_t baseOffset,
    const uint16_t singleMergeTmpElementCount, const uint32_t elementCountTail, const int32_t mergeTmpRepeatTimes,
    int32_t mergeTmpTailQueNum)
{
    if (mergeTmpTailQueNum <= 0) {
        return;
    }
    uint32_t offset0Tail = MRG_SORT_ELEMENT_LEN * baseOffset * mergeTmpRepeatTimes;
    uint32_t offset1Tail, offset2Tail, offset3Tail;
    uint16_t validBitTail;
    uint16_t elementCountListTail[MRG_SORT_ELEMENT_LEN] = {
        singleMergeTmpElementCount, singleMergeTmpElementCount, singleMergeTmpElementCount, singleMergeTmpElementCount};
    ComSortInnerLoopTail(
        offset0Tail, offset1Tail, offset2Tail, offset3Tail, validBitTail, elementCountListTail, baseOffset,
        elementCountTail, mergeTmpTailQueNum);
    if (mergeTmpTailQueNum > 1) {
        MrgSortSrcList sortListTail =
            MrgSortSrcList(tmp[offset0Tail], tmp[offset1Tail], tmp[offset2Tail], tmp[offset3Tail]);
        uint32_t sortedNumTail[MRG_SORT_ELEMENT_LEN];
        MrgSortCal<T>(dst[offset0Tail], sortListTail, elementCountListTail, sortedNumTail, validBitTail, 1);
    } else {
        if constexpr (IsSameType<T, half>::value) {
            DataCopy(dst[offset0Tail], tmp[offset0Tail], elementCountTail * 4);
        } else {
            DataCopy(dst[offset0Tail], tmp[offset0Tail], elementCountTail * 2);
        }
    }
}

__aicore__ inline uint32_t GetFullSortInnerLoopTimes(uint32_t elementCount, const uint32_t singleMergeElementCount)
{
    uint32_t loop = 0;
    while (elementCount > singleMergeElementCount) {
        elementCount /= MRG_SORT_ELEMENT_LEN;
        loop++;
    }
    return loop;
}

template <typename T>
__aicore__ inline void DoFullSort(
    const LocalTensor<T>& dst, const LocalTensor<T>& concat, const LocalTensor<uint32_t>& index, LocalTensor<T>& tmp,
    const int32_t repeatTime)
{
    uint32_t elementCount = concat.GetSize();
    uint32_t singleMergeElementCount = singleSortElementCountV200;
    uint32_t loop = GetFullSortInnerLoopTimes(elementCount, singleMergeElementCount);
    uint16_t singleMergeTmpElementCount = singleMergeElementCount;
    uint32_t srcElementCount = repeatTime * singleMergeElementCount;
    uint32_t dstElementCount = srcElementCount * regionProposalDataSize;
    int32_t mergeTmpTotalQueNum = repeatTime;
    int32_t mergeTmpTailQueNum = repeatTime % MRG_SORT_ELEMENT_LEN;
    int32_t mergeTmpQueNum = mergeTmpTotalQueNum - mergeTmpTailQueNum;
    int32_t mergeTmpRepeatTimes = repeatTime / MRG_SORT_ELEMENT_LEN;
    DataCopy(tmp, dst, dstElementCount);
    PipeBarrier<PIPE_V>();
    for (int i = 0; i < loop; i++) {
        uint32_t baseOffset;
        baseOffset = singleMergeTmpElementCount * regionProposalDataSize;
        FullSortInnerLoop(dst, tmp, baseOffset, singleMergeTmpElementCount, mergeTmpRepeatTimes);
        PipeBarrier<PIPE_V>();
        uint16_t elementCountTail = srcElementCount % singleMergeTmpElementCount ?
                                        srcElementCount % singleMergeTmpElementCount :
                                        singleMergeTmpElementCount;
        FullSortInnerLoopTail(
            dst, tmp, baseOffset, singleMergeTmpElementCount, elementCountTail, mergeTmpRepeatTimes,
            mergeTmpTailQueNum);
        PipeBarrier<PIPE_V>();
        DataCopy(tmp, dst, dstElementCount);
        PipeBarrier<PIPE_V>();
        singleMergeTmpElementCount *= MRG_SORT_ELEMENT_LEN;
        mergeTmpTotalQueNum = mergeTmpTotalQueNum % MRG_SORT_ELEMENT_LEN ?
                                  mergeTmpTotalQueNum / MRG_SORT_ELEMENT_LEN + 1 :
                                  mergeTmpTotalQueNum / MRG_SORT_ELEMENT_LEN;
        mergeTmpTailQueNum = mergeTmpTotalQueNum % MRG_SORT_ELEMENT_LEN;
        if (mergeTmpTailQueNum == 0 && elementCountTail != singleMergeTmpElementCount) {
            mergeTmpTailQueNum = MRG_SORT_ELEMENT_LEN;
        }
        mergeTmpQueNum = mergeTmpTotalQueNum - mergeTmpTailQueNum;
        mergeTmpRepeatTimes = mergeTmpQueNum / MRG_SORT_ELEMENT_LEN;
    }
}

__aicore__ inline void GetMrgSortResultImpl(
    uint16_t& mrgSortList1, uint16_t& mrgSortList2, uint16_t& mrgSortList3, uint16_t& mrgSortList4)
{
    int64_t mrgSortResult = get_vms4_sr();
    constexpr uint64_t resMask = 0x1FFF;
    // VMS4_SR[12:0], number of finished region proposals in list0
    mrgSortList1 = static_cast<uint64_t>(mrgSortResult) & resMask;
    constexpr uint64_t sortList2Bit = 13;
    // VMS4_SR[25:13], number of finished region proposals in list1
    mrgSortList2 = (static_cast<uint64_t>(mrgSortResult) >> sortList2Bit) & resMask;
    constexpr uint64_t sortList3Bit = 26;
    // VMS4_SR[38:26], number of finished region proposals in list2
    mrgSortList3 = (static_cast<uint64_t>(mrgSortResult) >> sortList3Bit) & resMask;
    constexpr uint64_t sortList4Bit = 39;
    // VMS4_SR[51:39], number of finished region proposals in list3
    mrgSortList4 = (static_cast<uint64_t>(mrgSortResult) >> sortList4Bit) & resMask;
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_PROPOSAL_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_IMPL_H__
#endif
