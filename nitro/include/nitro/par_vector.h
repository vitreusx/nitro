#pragma once
#include "par_const_view.h"
#include "par_view.h"
#include <cstddef>
#include <nitro/expr.h>
#include <nitro/par_at.h>
#include <nitro/par_const_at.h>
#include <nitro/par_lane_at.h>
#include <nitro/par_lane_const_at.h>
#include <nitro/vector_decl.h>

namespace nitro {
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
  explicit par_vector(Idx n, ind_expr<E> const &e, Allocs allocs = Allocs())
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

  at_expr<Types> at(Idx idx) { return (*this)[idx]; }

  const_at_expr<Types> at(Idx idx) const { return (*this)[idx]; }

  template <size_t N> lane_at_expr<Types, N> lane_at(Idx idx) {
    return {slices.template get<ISeq>().template lane_at<N>(idx)...};
  }

  template <size_t N> lane_const_at_expr<Types, N> lane_at(Idx idx) const {
    return {slices.template get<ISeq>().template lane_at<N>(idx)...};
  }

  template <size_t N> Idx num_lanes() const { return size() / N; }

  par_view<Types, Idx, ISeq...> view() {
    return par_view<Types, Idx, ISeq...>(slices.template get<ISeq>().view()...);
  }

  par_const_view<Types, Idx, ISeq...> view() const {
    return par_const_view<Types, Idx, ISeq...>(
        slices.template get<ISeq>().view()...);
  }

  void clear() { (..., slices.template get<ISeq>().clear()); }

  void reserve(Idx new_capacity) {
    (..., slices.template get<ISeq>().reserve(new_capacity));
  }

  void resize(Idx new_size) { resize(new_size, Types()); }

  template <typename E> void resize(Idx new_size, ind_expr<E> const &e) {
    (..., slices.template get<ISeq>().resize(new_size, e.template get<ISeq>()));
  }

  template <typename E> void push_back(ind_expr<E> const &e) {
    (..., slices.template get<ISeq>().push_back(e.template get<ISeq>()));
  }

  template <typename... Args> at_expr<Types> emplace_back(Args &&...args) {
    push_back(Types(args...));
    return at(size() - 1);
  }

private:
  tuple<slice<ISeq>...> slices;
};
} // namespace nitro
