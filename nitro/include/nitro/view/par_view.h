#pragma once
#include "../at/at.h"
#include "../const_at/const_at.h"
#include "../lane_at/lane_at.h"
#include "../lane_const_at/lane_const_at.h"
#include "../tuple.h"
#include "view_decl.h"
#include <cstddef>

namespace nitro {
template <typename Types, typename Idx, size_t... ISeq> class par_view {
public:
  template <size_t I>
  using slice = view<typename Types::template ith_type<I>, Idx>;

   par_view() : slices{slice<ISeq>()...} {};
   explicit par_view(slice<ISeq> const &...slices)
      : slices{slices...} {};

   at_expr<Types> operator[](Idx idx) const {
    return {slices.template get<ISeq>()[idx]...};
  }

   at_expr<Types> at(Idx idx) const { return (*this)[idx]; }

  template <size_t N>
   lane_at_expr<Types, N> lane_at(Idx idx) const {
    return {slices.template get<ISeq>().template lane_at<N>(idx)...};
  }

  template <size_t N>  Idx num_lanes() const { return size() / N; }

   Idx size() const { return slices.template get<0>().size(); }

  tuple<slice<ISeq>...> slices;
};
} // namespace nitro