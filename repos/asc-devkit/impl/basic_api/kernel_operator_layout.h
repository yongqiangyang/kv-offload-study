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
 * \file kernel_operator_layout.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_layout.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_LAYOUT_H__
#endif
#ifndef TIKCFW_IMPL_KERNEL_OPERATOR_LAYOUT_H
#define TIKCFW_IMPL_KERNEL_OPERATOR_LAYOUT_H

#include "../../include/utils/std/tuple.h"
#include "../../include/utils/std/type_traits.h"
#include "../../include/utils/std/utility.h"

namespace AscendC {
namespace LayoutInternal {
constexpr size_t TWO_DIM_DEPTH = 2;
constexpr size_t FOUR_DIM_DEPTH = 4;
} // namespace LayoutInternal

template <typename T, typename U, typename S>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const U& shape, const S& stride);

template <typename... Shapes>
using Shape = Std::tuple<Shapes...>;

template <typename... Strides>
using Stride = Std::tuple<Strides...>;

template <typename T>
struct nesting_depth {
    static constexpr size_t value = 1;
};

template <>
struct nesting_depth<Std::tuple<>> {
    static constexpr size_t value = 0;
};

template <typename... Args>
struct nesting_depth<Std::tuple<Args...>> {
    static constexpr size_t value = (nesting_depth<Args>::value + ...);
};

template <typename T>
constexpr size_t nesting_depth_v = nesting_depth<T>::value;

template <size_t Dim, typename T, typename U>
struct IsStaticLayout {
private:
    template <typename T1>
    struct include_dynamic_type : Std::true_type {};

    template <size_t v>
    struct include_dynamic_type<Std::Int<v>> : Std::false_type {};

    template <typename... Args>
    struct include_dynamic_type<Std::tuple<Args...>> : Std::bool_constant<(include_dynamic_type<Args>::value || ...)> {
    };

    __aicore__ inline static constexpr auto TestStaticLayout()
    {
        if constexpr (
            nesting_depth_v<T> == Dim && !(include_dynamic_type<T>::value || include_dynamic_type<U>::value)) {
            return true;
        }
        return false;
    }

public:
    static constexpr bool value = TestStaticLayout();
};

template <typename T, typename U>
struct StaticLayoutSize {
private:
    __aicore__ inline static constexpr auto GetFourDimStaticLayoutSize()
    {
        using rowShapeType = typename Std::tuple_element<0, T>::type;
        using colShapeType = typename Std::tuple_element<1, T>::type;
        using rowStrideType = typename Std::tuple_element<0, U>::type;
        using colStrideType = typename Std::tuple_element<1, U>::type;

        using outterRowNumType = typename Std::tuple_element<1, rowShapeType>::type;
        using outterRowStrideType = typename Std::tuple_element<1, rowStrideType>::type;
        using outterColNumType = typename Std::tuple_element<1, colShapeType>::type;
        using outterColStrideType = typename Std::tuple_element<1, colStrideType>::type;

        return (outterRowNumType{} * outterRowStrideType{}) > (outterColNumType{} * outterColStrideType{}) ?
                   (outterRowNumType{} * outterRowStrideType{}) :
                   (outterColNumType{} * outterColStrideType{});
    }

    __aicore__ inline static constexpr auto GetTwoDimStaticLayoutSize()
    {
        using rowNumType = typename Std::tuple_element<0, T>::type;
        using colNumType = typename Std::tuple_element<1, T>::type;
        using rowStrideType = typename Std::tuple_element<0, U>::type;
        using colStrideType = typename Std::tuple_element<1, U>::type;

        return (rowNumType{} * rowStrideType{}) > (colNumType{} * colStrideType{}) ? (rowNumType{} * rowStrideType{}) :
                                                                                     (colNumType{} * colStrideType{});
    }

    __aicore__ inline static constexpr auto GetStaticLayoutSize()
    {
        if constexpr (IsStaticLayout<LayoutInternal::FOUR_DIM_DEPTH, T, U>::value) {
            return GetFourDimStaticLayoutSize();
        } else if constexpr (IsStaticLayout<LayoutInternal::TWO_DIM_DEPTH, T, U>::value) {
            return GetTwoDimStaticLayoutSize();
        } else {
            return Std::Int<0>{};
        }
    }

public:
    static constexpr size_t size = GetStaticLayoutSize();
};

template <typename... Ts>
__aicore__ inline constexpr Shape<Ts...> MakeShape(const Ts&... t)
{
    return {t...};
}

template <typename... Ts>
__aicore__ inline constexpr Stride<Ts...> MakeStride(const Ts&... t)
{
    return {t...};
}

template <typename T, typename U>
struct Layout : private Std::tuple<T, U> {
    static constexpr auto size = StaticLayoutSize<T, U>::size;

    __aicore__ inline constexpr Layout(const T& shape = {}, const U& stride = {}) : Std::tuple<T, U>(shape, stride)
    {
        static_assert(Std::is_tuple_v<T> && Std::is_tuple_v<U>, "Shape or Stride is not tuple!");
    }

    __aicore__ inline constexpr decltype(auto) GetSize() const { return GetLayoutSize(); }

    __aicore__ inline constexpr decltype(auto) layout() { return *this; }

    __aicore__ inline constexpr decltype(auto) layout() const { return *this; }

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) GetShape()
    {
        return GetValue<0, I...>(static_cast<Std::tuple<T, U>&>(*this));
    }

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) GetShape() const
    {
        return GetValue<0, I...>(static_cast<const Std::tuple<T, U>&>(*this));
    }

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) GetStride()
    {
        return GetValue<1, I...>(static_cast<Std::tuple<T, U>&>(*this));
    }

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) GetStride() const
    {
        return GetValue<1, I...>(static_cast<const Std::tuple<T, U>&>(*this));
    }

    template <typename S>
    __aicore__ inline constexpr auto operator()(const S& coord) const
    {
        return Crd2Idx(coord, GetShape(), GetStride());
    }

private:
    template <size_t index, size_t I, size_t... Is, typename Tuple>
    __aicore__ inline constexpr decltype(auto) GetValue(const Tuple& t)
    {
        auto tupleEle = Std::get<index>(t);
        return Std::make_tuple(Std::get<I>(tupleEle), Std::get<Is>(tupleEle)...);
    }

    template <size_t index, size_t I, size_t... Is, typename Tuple>
    __aicore__ inline constexpr decltype(auto) GetValue(const Tuple& t) const
    {
        auto tupleEle = Std::get<index>(t);
        return Std::make_tuple(Std::get<I>(tupleEle), Std::get<Is>(tupleEle)...);
    }

    template <size_t index, typename Tuple>
    __aicore__ inline constexpr decltype(auto) GetValue(const Tuple& t)
    {
        return Std::get<index>(t);
    }

    template <size_t index, typename Tuple>
    __aicore__ inline constexpr decltype(auto) GetValue(const Tuple& t) const
    {
        return Std::get<index>(t);
    }

    __aicore__ inline constexpr decltype(auto) GetLayoutSize() const
    {
        uint32_t ret = -1;
        auto t = static_cast<const Std::tuple<T, U>&>(*this);
        if constexpr (nesting_depth_v<T> == LayoutInternal::FOUR_DIM_DEPTH) {
            auto rowShape = Std::get<0>(Std::get<0>(t));
            auto StrideInRow = Std::get<0>(Std::get<1>(t));
            auto colShape = Std::get<1>(Std::get<0>(t));
            auto StrideInCol = Std::get<1>(Std::get<1>(t));

            auto rowNum = Std::get<1>(rowShape);
            auto rowStride = Std::get<1>(StrideInRow);
            auto colNum = Std::get<1>(colShape);
            auto colStride = Std::get<1>(StrideInCol);

            auto size1 = rowNum * rowStride;
            auto size2 = colNum * colStride;
            ret = size1 > size2 ? size1 : size2;
        } else if constexpr (nesting_depth_v<T> == LayoutInternal::TWO_DIM_DEPTH) {
            auto shape = Std::get<0>(t);
            auto stride = Std::get<1>(t);

            auto rowNum = Std::get<0>(shape);
            auto rowStride = Std::get<0>(stride);
            auto colNum = Std::get<1>(shape);
            auto colStride = Std::get<1>(stride);

            auto size1 = rowNum * rowStride;
            auto size2 = colNum * colStride;
            ret = size1 > size2 ? size1 : size2;
        }
        return ret;
    }
};

template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride)
{
    static_assert(Std::is_tuple_v<T> && Std::is_tuple_v<U>, "Shape or Stride is not tuple!");
    return Layout<T, U>(shape, stride);
}

template <typename T>
struct is_layout : Std::false_type {};

template <typename T, typename U>
struct is_layout<Layout<T, U>> : Std::true_type {};

template <typename T>
constexpr bool is_layout_v = is_layout<T>::value;

} // namespace AscendC
#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_LAYOUT_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_LAYOUT_H__
#endif
