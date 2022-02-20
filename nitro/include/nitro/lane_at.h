#pragma once
#include <cstddef>
#include <nitro/indexed.h>
#include <nitro/lane.h>

namespace nitro {

template <typename T, size_t N, bool Indexed> struct lane_at_expr_aux;

template <typename T, size_t N>
struct lane_at_expr_impl : lane_at_expr_aux<T, N, is_indexed<T>> {};

template <typename T, size_t N>
using lane_at_expr = typename lane_at_expr_impl<T, N>::type;

template <typename T, size_t N> struct def_lane_at {
public:
  explicit def_lane_at(T &ref) : ref{ref} {}
  def_lane_at(def_lane_at &other) = default;
  def_lane_at(def_lane_at &&other) noexcept = default;

  operator lane<T, N>() {
    lane<T, N> res;
    res.load(&ref);
    return res;
  }

  def_lane_at &operator=(def_lane_at const &other) {
    assign(other);
    return *this;
  }

  template <typename E> def_lane_at &operator=(E const &e) {
    assign(e);
    return *this;
  }

  def_lane_at &operator=(def_lane_at &&other) noexcept {
    assign(other);
    return *this;
  }

  template <typename E> def_lane_at &operator=(E &&e) noexcept {
    assign(e);
    return *this;
  }

private:
  template <typename E> void assign(E const &e) noexcept {
    lane<T, N> x = e;
    x.store(&ref);
  }

  T &ref;
};

template <typename T, size_t N> struct lane_at_expr_aux<T, N, false> {
  using type = def_lane_at<T, N>;
};
}