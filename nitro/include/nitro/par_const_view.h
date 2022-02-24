#pragma once
#include "const_at.h"
#include "const_view_decl.h"
#include "lane_const_at.h"
#include "par_view.h"
#include "tuple.h"
#include <cstddef>

namespace nitro {
template <typename Types, typename Idx, size_t... ISeq> class par_const_view {
public:
  template <size_t I>
  using slice = const_view<typename Types::template ith_type<I>, Idx>;

  par_const_view():
    slices{slice<ISeq>()...} {};

  explicit par_const_view(slice<ISeq> const &...slices) : slices{slices...} {};
  par_const_view(par_view<Types, Idx, ISeq...> const &mut_view)
      : slices{mut_view.slices.template get<ISeq>()...} {};

  const_at_expr<Types> operator[](Idx idx) const {
    return {slices.template get<ISeq>()[idx]...};
  }

  const_at_expr<Types> at(Idx idx) const { return (*this)[idx]; }

  template <size_t N> lane_const_at_expr<Types, N> lane_at(Idx idx) const {
    return {slices.template get<ISeq>().template lane_at<N>(idx)...};
  }

  template <size_t N> Idx num_lanes() const { return size() / N; }

  Idx size() const { return slices.template get<0>().size(); }

  tuple<slice<ISeq>...> slices;
};
} // namespace nitro