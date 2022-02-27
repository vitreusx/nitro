#pragma once
#include "../at/at.h"
#include "../const_at/const_at.h"
#include "../const_view/const_view_decl.h"
#include "../lane_at/lane_at.h"
#include "../lane_const_at/lane_const_at.h"

namespace nitro {
template <typename T, typename Idx> class def_view {
public:
  def_view() = default;
   explicit def_view(T *data, Idx n) : data{data}, n{n} {};

   T &operator[](Idx idx) const { return data[idx]; }

   T &at(Idx idx) const { return (*this)[idx]; }

  template <size_t N>  def_lane_at<T, N> lane_at(Idx idx) const {
    return def_lane_at<T, N>(data[N * idx]);
  }

  template <size_t N>  Idx num_lanes() const { return size() / N; }

   Idx size() const { return n; }

  T *data;
  Idx n;
};
} // namespace nitro