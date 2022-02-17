#pragma once
#include <nitro/indexed.h>

template <typename... Types> struct type_list;

template <size_t I, typename Head, typename... Tail>
struct ith_type_impl : ith_type_impl<I - 1, Tail...> {};

template <typename Head, typename... Tail>
struct ith_type_impl<0, Head, Tail...> {
  using type = Head;
};

template <typename... Types> struct type_list {
  static constexpr size_t num_types = sizeof...(Types);

  template <size_t I>
  using ith_type = typename ith_type_impl<I, Types...>::type;
};
