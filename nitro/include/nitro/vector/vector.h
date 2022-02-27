#pragma once
#include "vector_decl.h"
#include "def_vector.h"
#include "par_vector.h"

namespace nitro {
template <typename T, typename Alloc, typename Idx, bool Indexed>
struct vector_impl_aux;

template <typename T, typename Alloc, typename Idx>
struct vector_impl : vector_impl_aux<T, Alloc, Idx, is_indexed<T>> {};

template <typename T, typename Alloc, typename Idx>
struct vector_impl_aux<T, Alloc, Idx, false> {
  using type = def_vector<T, Alloc, Idx>;
};

template <typename T, typename Alloc, typename Idx>
struct vector_impl_aux<T, Alloc, Idx, true> {
private:
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>)
      -> par_vector<T, Alloc, Idx, ISeq...> {}

public:
  using type = decltype(aux(std::make_index_sequence<T::num_types>{}));
};
}
