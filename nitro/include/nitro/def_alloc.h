#pragma once
#include <memory>
#include <nitro/indexed.h>
#include <nitro/tuple.h>

template <typename T> struct def_alloc_impl;

template <typename T> using def_alloc = typename def_alloc_impl<T>::type;

template <typename Types, size_t... ISeq>
class par_alloc
    : public tuple<def_alloc<typename Types::template ith_type<ISeq>>...> {
public:
  template <size_t I>
  using slice = def_alloc<typename Types::template ith_type<I>>;

  using Base = tuple<slice<ISeq>...>;
  using Base::Base;

  par_alloc() : Base{slice<ISeq>()...} {};
};

template <typename T, bool Indexed> struct def_alloc_impl_aux;

template <typename T>
struct def_alloc_impl : def_alloc_impl_aux<T, is_indexed<T>> {};

template <typename T> struct def_alloc_impl_aux<T, true> {
private:
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>) -> par_alloc<T, ISeq...> {}

public:
  using type = decltype(aux(std::make_index_sequence<T::num_types>{}));
};

template <typename T> struct def_alloc_impl_aux<T, false> {
  using type = std::allocator<T>;
};