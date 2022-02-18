#pragma once
#include <cstddef>
#include <nitro/const_at.h>
#include <nitro/expr.h>
#include <nitro/tuple.h>
#include <utility>

template <typename Types, size_t... ISeq>
class par_const_at_expr
    : public auto_expr<par_const_at_expr<Types, ISeq...>, Types>,
      public tuple_wrapper<
          const_at_expr<typename Types::template ith_type<ISeq>>...> {
public:
  using Base =
      tuple_wrapper<const_at_expr<typename Types::template ith_type<ISeq>>...>;
  using Base::Base;
  using Base::get;

  par_const_at_expr(par_const_at_expr const &other)
      : Base(static_cast<Base const &>(other)) {}

  par_const_at_expr(par_const_at_expr &&other)
      : Base(static_cast<Base &&>(other)) {}

  par_const_at_expr &operator=(par_const_at_expr const &other) = delete;
  par_const_at_expr &operator=(par_const_at_expr &&other) = delete;
};

template <typename T> struct const_at_aux<T, true> {
private:
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>)
      -> par_const_at_expr<T, ISeq...> {}

public:
  using type = decltype(aux(std::make_index_sequence<T::num_types>{}));
};