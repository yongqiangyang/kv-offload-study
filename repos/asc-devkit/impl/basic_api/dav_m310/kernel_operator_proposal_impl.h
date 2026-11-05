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
    "impl/basic_api/dav_m310/kernel_operator_proposal_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_IMPL_H__
#endif
#ifndef ASCENDC_MODULE_OPERATOR_PROPOSAL_IMPL_H
#define ASCENDC_MODULE_OPERATOR_PROPOSAL_IMPL_H

namespace AscendC {
template <typename T>
__aicore__ inline void Vmrgsort4Cal(__ubuf__ T* dstLocal, __ubuf__ T* addrArray[MRG_SORT_ELEMENT_LEN], uint64_t config)
{
    ASSERT(false && "unsupported Vbitsort on current device");
}

template <typename T>
__aicore__ inline void VbitsortCal(__ubuf__ T* dstLocal, __ubuf__ T* srcLocal, const ProposalIntriParams& intriParams)
{
    ASSERT(false && "unsupported Vbitsort on current device");
}

template <typename T>
__aicore__ inline void VbitsortCal(
    __ubuf__ T* dstLocal, __ubuf__ T* src0Local, __ubuf__ uint32_t* src1Local, const ProposalIntriParams& intriParams)
{
    ASSERT(false && "unsupported Vbitsort on current device");
}

template <typename T>
__aicore__ inline void Vmrgsort4Cal(
    __ubuf__ T* dstLocal, __ubuf__ T* addrArray[MRG_SORT_ELEMENT_LEN], uint64_t src1, uint64_t config)
{
    ASSERT(false && "unsupported Vmrgsort on current device");
}

template <typename T>
__aicore__ inline void VconcatCal(__ubuf__ T* dstLocal, __ubuf__ T* srcLocal, const ProposalIntriParams& intriParams)
{
    ASSERT(false && "unsupported VCONCAT on current device");
}

template <typename T>
__aicore__ inline void VextractCal(__ubuf__ T* dstLocal, __ubuf__ T* srcLocal, const ProposalIntriParams& intriParams)
{
    ASSERT(false && "unsupported VEXTRACT on current device");
}

__aicore__ inline void GetMrgSortResultImpl(
    uint16_t& mrgSortList1, uint16_t& mrgSortList2, uint16_t& mrgSortList3, uint16_t& mrgSortList4)
{
    ASCENDC_ASSERT((false), "GetMrgSortResult is not supported on current device");
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_PROPOSAL_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_PROPOSAL_IMPL_H__
#endif
