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
 * \file kernel_operator_dump_tensor_impl.h
 * \brief
 */
#ifndef ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_DUMP_TENSOR_IMPL_H

#include "../kernel_utils.h"
#include "../../../include/basic_api/kernel_tensor.h"
#include "kernel_operator_common_impl.h"

namespace AscendC {
template <typename T>
__aicore__ void DumpTensorGM2GMImpl(const GlobalTensor<T>& src, uint32_t desc, uint32_t size)
{}

template <typename T>
__aicore__ void DumpTensorLocal2GMImpl(const LocalTensor<T>& src, uint32_t desc, uint32_t size)
{}

__aicore__ inline void DumpShapeImpl(const ShapeInfo& shapeInfo) {}
} // namespace AscendC
#endif
