#pragma once
#include "def.h"
#include "par.h"

namespace nitro {

template <typename T, bool Indexed> struct at_expr_aux;

template <typename T> struct at_expr_impl : at_expr_aux<T, is_indexed<T>> {};

template <typename T> struct at_expr_aux<T, false> { using type = def_at<T>; };

template <typename T> struct at_expr_aux<T, true> {
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>) -> par_at_expr<T, ISeq...> {}

  using type = decltype(aux(std::make_index_sequence<T::num_types>{}));
};
} // namespace nitro