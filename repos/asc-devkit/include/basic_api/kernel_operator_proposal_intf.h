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
 * \file kernel_operator_proposal_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_operator_proposal_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_INTF_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_PROPOSAL_INTERFACE_H
#define ASCENDC_MODULE_OPERATOR_PROPOSAL_INTERFACE_H

#include "../../impl/basic_api/kernel_macros.h"
#include "kernel_tensor.h"
#include "kernel_struct_proposal.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include <cstdint>
#include "stub_def.h"
#endif

namespace AscendC {
#pragma begin_pipe(V)
/* **************************************** MrgSort4 ****************************************** */
/*
 * @ingroup MrgSort4
 * @brief Arrange and merge up to four arranged potential queues into one queue
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor list
 * @param [in] filter input LocalTensor
 * @param [in] Params.elementLengths length of proposal list
 * @param [in] Params.ifExhaustedSuspension judge whether to stop after a queue is exhausted
 * @param [in] Params.validBit judge value is valid or not
 * @param [in] Params.repeatTimes repeat times
 */
template <typename T>
__aicore__ inline void MrgSort4(const LocalTensor<T>& dst, const MrgSortSrcList<T>& src, const MrgSort4Info& params);

/* **************************************** RpSort16 ****************************************** */
/*
 * @ingroup RpSort16
 * @brief Sort them according to the score field in the Region Proposals
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] repeatTime repeat times
 */
template <typename T>
__aicore__ inline void RpSort16(const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime);

/* **************************************** MrgSort ****************************************** */
/*
 * @ingroup MrgSort
 * @brief Arrange and merge up to four arranged potential queues into one queue
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor list
 * @param [in] filter input LocalTensor
 * @param [in] Params.elementLengths length of proposal list
 * @param [in] Params.ifExhaustedSuspension judge whether to stop after a queue is exhausted
 * @param [in] Params.validBit judge value is valid or not
 * @param [in] Params.repeatTimes repeat times
 */
template <typename T>
__aicore__ inline void MrgSort(const LocalTensor<T>& dst, const MrgSortSrcList<T>& src, const MrgSort4Info& params);

/* **************************************** Sort32 ****************************************** */
/*
 * @ingroup Sort32
 * @brief Sort 32 elements
 * @param [out] dst output LocalTensor
 * @param [in] src0 input LocalTensor
 * @param [in] src1 input LocalTensor
 * @param [in] repeatTime repeat times
 */
template <typename T>
__aicore__ inline void Sort32(
    const LocalTensor<T>& dst, const LocalTensor<T>& src0, const LocalTensor<uint32_t>& src1, const int32_t repeatTime);

/* **************************************** ProposalConcat ****************************************** */
/*
 * @ingroup ProposalConcat
 * @brief Combine continuous elements into corresponding positions in the Region Proposal
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] repeatTime repeat times
 * @param [in] modeNumbe Position parameter
 */
template <typename T>
__aicore__ inline void ProposalConcat(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime, const int32_t modeNumber);

/* **************************************** ProposalExtract ****************************************** */
/*
 * @ingroup ProposalExtract
 * @brief ProposalExtract and rearrange the individual elements in the corresponding position from the Region Proposals
 * @param [out] dst output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] repeatTime repeat times
 * @param [in] modeNumbe Position parameter
 */
template <typename T>
__aicore__ inline void ProposalExtract(
    const LocalTensor<T>& dst, const LocalTensor<T>& src, const int32_t repeatTime, const int32_t modeNumber);

/* **************************************** Concat ****************************************** */
/*
 * @ingroup Concat
 * @brief Combine continuous elements into corresponding positions
 * @param [out] concat output LocalTensor
 * @param [in] src input LocalTensor
 * @param [in] tmp tmp buffer
 * @param [in] repeatTime repeat times
 */
template <typename T>
__aicore__ inline void Concat(
    LocalTensor<T>& concat, const LocalTensor<T>& src, const LocalTensor<T>& tmp, const int32_t repeatTime);

/* **************************************** Extract ****************************************** */
/*
 * @ingroup Extract
 * @brief Extract and rearrange the individual elements in the corresponding position
 * @param [out] dstValue output LocalTensor
 * @param [in] dstIndex output LocalTensor
 * @param [in] sorted input LocalTensor
 * @param [in] repeatTime repeat times
 */
template <typename T>
__aicore__ inline void Extract(
    const LocalTensor<T>& dstValue, const LocalTensor<uint32_t>& dstIndex, const LocalTensor<T>& sorted,
    const int32_t repeatTime);

/* **************************************** MrgSort ****************************************** */
/*
 * @ingroup MrgSort
 * @brief Arrange and merge up to four arranged potential queues into one queue
 * @param [out] dst output LocalTensor
 * @param [in] sortList input LocalTensor list
 * @param [in] elementCountList input LocalTensor list length
 * @param [in] sortedNum output sorted numbers
 * @param [in] validBit input valid bit
 * @param [in] repeatTime repeat times
 */
template <typename T, bool isExhaustedSuspension = false>
__aicore__ inline void MrgSort(
    const LocalTensor<T>& dst, const MrgSortSrcList<T>& sortList, const uint16_t elementCountList[4],
    uint32_t sortedNum[4], uint16_t validBit, const int32_t repeatTime);

/* ***************************************** Sort ****************************************** */
/*
 * @ingroup Sort
 * @brief Sort them according to the value
 * @param [out] dst output LocalTensor
 * @param [in] concat input LocalTensor
 * @param [in] index input LocalTensor
 * @param [in] tmp tmp buffer
 * @param [in] repeatTime repeat times
 */
template <typename T, bool isFullSort>
__aicore__ inline void Sort(
    const LocalTensor<T>& dst, const LocalTensor<T>& concat, const LocalTensor<uint32_t>& index, LocalTensor<T>& tmp,
    const int32_t repeatTime);

/* **************************************** GetSortOffset ****************************************** */
/*
 * @ingroup GetSortOffset
 * @brief get sort offset in the sorted struct
 * @param [in] elemOffset element number offer
 */
template <typename T>
__aicore__ inline uint32_t GetSortOffset(const uint32_t elemOffset);

/* **************************************** GetSortLen ****************************************** */
/*
 * @ingroup GetSortLen
 * @brief get sort length in the sorted struct
 * @param [in] elemOffset element number ocountffer
 */
template <typename T>
__aicore__ inline uint32_t GetSortLen(const uint32_t elemCount);
#pragma end_pipe
__aicore__ inline __inout_pipe__(S) void GetMrgSortResult(
    uint16_t& mrgSortList1, uint16_t& mrgSortList2, uint16_t& mrgSortList3, uint16_t& mrgSortList4);
} // namespace AscendC

#if defined(__NPU_ARCH__)
#include "../../impl/basic_api/kernel_operator_proposal_intf_impl.h"
#endif // ASCENDC_MODULE_OPERATOR_PROPOSAL_INTERFACE_H
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_INTF_H__
#endif
