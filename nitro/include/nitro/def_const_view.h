#pragma once
#include "const_at.h"
#include "def_view.h"
#include "lane_const_at.h"

namespace nitro {
template <typename T, typename Idx> class def_const_view {
public:
  def_const_view() = default;
  explicit def_const_view(T *const data, Idx n) : data{data}, n{n} {};
  def_const_view(def_view<T, Idx> const &mut_view)
      : data{mut_view.data}, n{mut_view.n} {};

  T const &operator[](Idx idx) const { return data[idx]; }

  T const &at(Idx idx) const { return (*this)[idx]; }

  template <size_t N> lane<T, N> lane_at(Idx idx) const {
    lane<T, N> res;
    res.load(data + N * idx);
    return res;
  }

  template <size_t N> Idx num_lanes() const { return size() / N; }

  Idx size() const { return n; }

  T const *data;
  Idx n;
};
} // namespace nitro