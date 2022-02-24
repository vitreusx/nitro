#pragma once
#include "def_view.h"
#include "par_view.h"

namespace nitro {
template <typename T, typename Idx, bool Indexed> struct view_impl_aux;

template <typename T, typename Idx>
struct view_impl : view_impl_aux<T, Idx, is_indexed<T>> {};

template <typename T, typename Idx> struct view_impl_aux<T, Idx, false> {
  using type = def_view<T, Idx>;
};

template <typename T, typename Idx> struct view_impl_aux<T, Idx, true> {
private:
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>) -> par_view<T, Idx, ISeq...> {}

public:
  using type = decltype(aux(std::make_index_sequence<T::num_types>{}));
};
} // namespace nitro