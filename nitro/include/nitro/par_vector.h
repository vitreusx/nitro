#pragma once
#include <cstddef>
#include <nitro/expr.h>
#include <nitro/par_at.h>
#include <nitro/par_const_at.h>
#include <nitro/par_lane_at.h>
#include <nitro/par_lane_const_at.h>
#include <nitro/vector_decl.h>

template <typename Types, typename Allocs, typename Idx, size_t... ISeq>
class par_vector {
public:
  template <size_t I>
  using slice = vector<typename Types::template ith_type<I>,
                       typename Allocs::template ith_type<I>, Idx>;

  par_vector() : par_vector(Allocs()){};

  explicit par_vector(Allocs const &allocs)
      : slices{slice<ISeq>(allocs.template get<ISeq>())...} {};

  explicit par_vector(Idx n, Types const &init = Types(),
                      Allocs allocs = Allocs())
      : slices{slice<ISeq>(n, init.template get<ISeq>(),
                           allocs.template get<ISeq>())...} {};

  template <typename E>
  explicit par_vector(Idx n, expr<E, Types> const &e, Allocs allocs = Allocs())
      : slices{slice<ISeq>(n, e.template get<ISeq>(),
                           allocs.template get<ISeq>())...} {};

  Idx size() const { return slices.template get<0>().size(); }
  Idx capacity() const { return slices.template get<0>().capacity(); }

  at_expr<Types> operator[](Idx idx) {
    return {slices.template get<ISeq>()[idx]...};
  }

  const_at_expr<Types> operator[](Idx idx) const {
    return {slices.template get<ISeq>()[idx]...};
  }

  template <size_t N> lane_at_expr<Types, N> lane_at(Idx idx) {
    return {slices.template get<ISeq>().template lane_at<N>(idx)...};
  }

  template <size_t N> lane_const_at_expr<Types, N> lane_at(Idx idx) const {
    return {slices.template get<ISeq>().template lane_at<N>(idx)...};
  }

  template <size_t N> Idx num_lanes() const { return size() / N; }

private:
  tuple<slice<ISeq>...> slices;
};