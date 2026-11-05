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
 * \file kernel_operator_coord.h
 * \brief
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/kernel_operator_coord.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_operator_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_COORD_H__
#endif
#ifndef TIKCFW_IMPL_KERNEL_OPERATOR_COORD_H
#define TIKCFW_IMPL_KERNEL_OPERATOR_COORD_H

#include "kernel_operator_layout.h"

namespace AscendC {

namespace CoordImpl {
using IntZero = Std::Int<0>;
}

template <typename... Coords>
using Coord = Std::tuple<Coords...>;

template <typename... Ts>
__aicore__ inline constexpr Coord<Ts...> MakeCoord(Ts const&... t)
{
    return {t...};
}

template <typename TupleType>
using tuple_sequence = Std::make_index_sequence<Std::tuple_size_v<Std::remove_cvref_t<TupleType>>>;

template <typename T, typename F, typename G, size_t... I>
__aicore__ inline constexpr auto TupleApply(T&& t, F&& f, G&& g, Std::index_sequence<I...>)
{
    return g(f(Std::get<I>(static_cast<T&&>(t)))...);
}

template <typename T, typename F, typename G>
__aicore__ inline constexpr auto TransformApply(T&& t, F&& f, G&& g)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<T>>) {
        return TupleApply(static_cast<T&&>(t), f, g, tuple_sequence<T>{});
    } else {
        return g(f(static_cast<T&&>(t)));
    }
}

struct MultipliesUnaryLeftFold {
    template <typename... T>
    __aicore__ inline constexpr auto operator()(T&&... t) const
    {
        return (... * t);
    }
};

struct Product {
    template <typename T>
    __aicore__ inline constexpr auto operator()(const T& intT) const
    {
        if constexpr (Std::is_tuple_v<T>) {
            if constexpr (Std::tuple_size_v<T> == 0) {
                return Std::Int<1>{};
            } else {
                return TransformApply(intT, Product{}, MultipliesUnaryLeftFold{});
            }
        } else if constexpr (Std::is_integral<T>::value) {
            return intT;
        } else {
            static_assert(sizeof(T) == 0, "Invalid Product parameters");
        }
    }
};

template <typename T, typename U, typename S>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const U& shape, const S& stride);

template <typename T, typename U, typename S, size_t... Is>
__aicore__ inline constexpr auto Crd2IdxTTT(const T& coord, const U& shape, const S& stride, Std::index_sequence<Is...>)
{
    return (... + Crd2Idx(Std::get<Is>(coord), Std::get<Is>(shape), Std::get<Is>(stride)));
}

template <typename T, typename U, typename S, size_t I0, size_t... Is>
__aicore__ inline constexpr auto Crd2IdxITT(
    const T& coord, const U& shape, const S& stride, Std::index_sequence<I0, Is...>)
{
    if constexpr (sizeof...(Is) == 0) { // Avoid recursion and mod on single/last iter
        return Crd2Idx(coord, Std::get<I0>(shape), Std::get<I0>(stride));
    } else if constexpr (Std::is_constant<0, T>::value) {
        return Crd2Idx(CoordImpl::IntZero{}, Std::get<I0>(shape), Std::get<I0>(stride)) +
               (CoordImpl::IntZero{} + ... + Crd2Idx(CoordImpl::IntZero{}, Std::get<Is>(shape), Std::get<Is>(stride)));
    } else { // General case
        auto prod = Product{}(Std::get<I0>(shape));
        auto div = coord / prod;
        auto mod = coord % prod;
        return Crd2Idx(mod, Std::get<I0>(shape), Std::get<I0>(stride)) +
               Crd2IdxITT(div, shape, stride, Std::index_sequence<Is...>{});
    }
}

template <typename T, typename U, typename S>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const U& shape, const S& stride)
{
    if constexpr (Std::is_tuple_v<T>) {
        if constexpr (Std::is_tuple_v<U>) { // tuple tuple tuple
            static_assert(Std::tuple_size_v<T> == Std::tuple_size_v<U>, "Shape and Coord Mismatched Ranks");
            static_assert(Std::tuple_size_v<T> == Std::tuple_size_v<S>, "Stride and Coord Mismatched Ranks");
            return Crd2IdxTTT(coord, shape, stride, tuple_sequence<T>{});
        } else { // tuple "int" "int"
            static_assert(sizeof(T) == 0, "Invalid parameters, U is not tuple!");
        }
    } else {
        if constexpr (Std::is_tuple_v<U>) { // "int" tuple tuple
            static_assert(Std::tuple_size_v<U> == Std::tuple_size_v<S>, "Shape and Stride Mismatched Ranks");
            return Crd2IdxITT(coord, shape, stride, tuple_sequence<U>{});
        } else { // "int" "int" "int"
            return coord * stride;
        }
    }
}

template <typename T, typename U, typename S>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const Layout<U, S>& layout)
{
    return Crd2Idx(coord, layout.GetShape(), layout.GetStride());
}

} // namespace AscendC
#endif
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_COORD_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_COORD_H__
#endif
