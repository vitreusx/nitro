#pragma once
#include "../indexed.h"
#include "../lane/lane.h"

namespace nitro {

template <typename T, size_t N, bool Indexed> struct lane_const_at_aux;

template <typename T, size_t N>
struct lane_const_at_expr_impl : lane_const_at_aux<T, N, is_indexed<T>> {};

template <typename T, size_t N>
using lane_const_at_expr = typename lane_const_at_expr_impl<T, N>::type;

template <typename T, size_t N> struct lane_const_at_aux<T, N, false> {
  using type = lane<T, N>;
};
}