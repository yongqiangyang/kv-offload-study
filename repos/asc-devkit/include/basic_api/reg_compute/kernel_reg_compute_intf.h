/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file kernel_reg_compute_intf.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "kernel_reg_compute_intf.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_INTF_H__
#endif

#ifndef ASCENDC_MODULE_REG_COMPUTE_INTERFACE_H
#define ASCENDC_MODULE_REG_COMPUTE_INTERFACE_H

#include "../../../impl/basic_api/kernel_macros.h"

#if defined(ASCENDC_CPU_DEBUG) && ASCENDC_CPU_DEBUG == 1
#include "stub_def.h"
#endif

#include "kernel_reg_compute_utils.h"
#include "kernel_reg_compute_maskreg_intf.h"
#include "kernel_reg_compute_common_intf.h"
#include "kernel_reg_compute_membar_intf.h"
#include "kernel_reg_compute_addrreg_intf.h"
#include "kernel_reg_compute_vec_duplicate_intf.h"
#include "kernel_reg_compute_vec_cmpsel_intf.h"
#include "kernel_reg_compute_vec_binary_intf.h"
#include "kernel_reg_compute_vec_binary_scalar_intf.h"
#include "kernel_reg_compute_copy_intf.h"
#include "kernel_reg_compute_datacopy_intf.h"
#include "kernel_reg_compute_gather_mask_intf.h"
#include "kernel_reg_compute_pack_intf.h"
#include "kernel_reg_compute_struct_intf.h"
#include "kernel_reg_compute_vec_arange_intf.h"
#include "kernel_reg_compute_vec_reduce_intf.h"
#include "kernel_reg_compute_vec_ternary_scalar_intf.h"
#include "kernel_reg_compute_vec_unary_intf.h"
#include "kernel_reg_compute_vec_vconv_intf.h"
#include "kernel_reg_compute_vec_fused_intf.h"
#include "kernel_reg_compute_histograms_intf.h"
#endif // ASCENDC_MODULE_REG_COMPUTE_INTERFACE_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_INTF_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_INTF_H__
#endif
