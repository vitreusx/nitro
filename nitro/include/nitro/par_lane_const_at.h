#pragma once
#include <nitro/expr.h>
#include <nitro/lane_const_at.h>
#include <nitro/tuple.h>

namespace nitro {
template <typename T, size_t N, size_t... ISeq>
class par_lane_const_at_expr
    : public auto_expr<par_lane_const_at_expr<T, N, ISeq...>, T>,
      public tuple_wrapper<
          lane_const_at_expr<typename T::template ith_type<ISeq>, N>...> {
public:
  using Base = tuple_wrapper<
      lane_const_at_expr<typename T::template ith_type<ISeq>, N>...>;
  using Base::Base;
  using Base::get;

  par_lane_const_at_expr(par_lane_const_at_expr const &other)
      : Base(static_cast<Base const &>(other)) {}

  par_lane_const_at_expr(par_lane_const_at_expr &&other) noexcept
      : Base(static_cast<Base &&>(other)) {}

  par_lane_const_at_expr &
  operator=(par_lane_const_at_expr const &other) = delete;
  par_lane_const_at_expr &operator=(par_lane_const_at_expr &&other) = delete;
};

template <typename T, size_t N> struct lane_const_at_aux<T, N, true> {
private:
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>)
      -> par_lane_const_at_expr<T, N, ISeq...> {}

public:
  using type = decltype(aux(std::make_index_sequence<T::num_types>{}));
};
}
