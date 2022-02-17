#pragma once
#include <cstddef>
#include <nitro/expr.h>
#include <nitro/indexed.h>
#include <nitro/tuple.h>
#include <utility>

template <typename T, bool Indexed> struct at_const_expr_impl_aux;

template <typename T>
struct at_const_expr_impl : at_const_expr_impl_aux<T, is_indexed<T>> {};

template <typename T>
using at_const_expr = typename at_const_expr_impl<T>::type;

template <typename T> struct at_const_expr_impl_aux<T, false> {
  using type = T const &;
};

template <typename Types, size_t... ISeq>
class par_at_const_expr
    : public auto_expr<par_at_const_expr<Types, ISeq...>, Types>,
      public tuple_wrapper<
          at_const_expr<typename Types::template ith_type<ISeq>>...> {
public:
  using Base =
      tuple_wrapper<at_const_expr<typename Types::template ith_type<ISeq>>...>;
  using Base::Base;
  using Base::get;

  par_at_const_expr(par_at_const_expr const &other)
      : Base(static_cast<Base const &>(other)) {}

  par_at_const_expr &operator=(par_at_const_expr const &other) = delete;

  par_at_const_expr(par_at_const_expr &&other)
      : Base(static_cast<Base &&>(other)) {}

  par_at_const_expr &operator=(par_at_const_expr &&other) = delete;
};

template <typename T> struct at_const_expr_impl_aux<T, true> {
private:
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>)
      -> par_at_const_expr<T, ISeq...> {}

public:
  using type = decltype(aux(std::make_index_sequence<T::num_types>{}));
};