#pragma once
#include "vector.h"

namespace nitro {
template <typename T, typename DataAlloc = def_alloc<T>,
          typename MarkAlloc = def_alloc<bool>, typename Idx = int>
class set {
public:
  set() = default;
  explicit set(DataAlloc data_alloc, MarkAlloc mark_alloc)
      : data{data_alloc}, mark{mark_alloc} {};

  explicit set(Idx n, T const &init = T(), DataAlloc data_alloc = DataAlloc(),
               MarkAlloc mark_alloc = MarkAlloc())
      : data{n, init, data_alloc}, mark{n, false, mark_alloc} {};

  template <typename E>
  explicit set(Idx n, ind_expr<E> const &e, DataAlloc data_alloc = DataAlloc(),
               MarkAlloc mark_alloc = MarkAlloc())
      : data{n, e, data_alloc}, mark{n, false, mark_alloc} {};

  Idx size() const { return data.size(); }

  Idx capacity() const { return data.capacity(); }

  decltype(auto) operator[](Idx idx) { return data[idx]; }

  decltype(auto) operator[](Idx idx) const { return data[idx]; }

  decltype(auto) at(Idx idx) { return data.at(idx); }

  decltype(auto) at(Idx idx) const { return data.at(idx); }

  template <size_t N> decltype(auto) lane_at(Idx idx) {
    return data.template lane_at<N>(idx);
  }

  template <size_t N> decltype(auto) lane_at(Idx idx) const {
    return data.template lane_at<N>(idx);
  }

  template <size_t N> Idx num_lanes() const {
    return data.template num_lanes<N>();
  }

  void clear() {
    data.clear();
    mark.clear();
  }

  void resize(Idx new_size, T const &init = T()) {
    data.resize(new_size, init);
    mark.resize(new_size, init);
  }

  template <typename E> void resize(Idx new_size, E const &e) {
    data.resize(new_size, e);
    mark.resize(new_size, false);
  }

  template <typename E> void push_back(E const &e) {
    data.push_back(e);
    mark.push_back(false);
  }

  template<typename... Args> decltype(auto) emplace_back(Args&&... args) {
    mark.push_back(false);
    return data.template emplace_back(std::forward<Args>(args)...);
  }

  void remove(Idx idx) {
    mark[idx] = true;
  }

  bool is_vacant(Idx idx) const {
    return mark[idx];
  }

private:
  vector<T> data;
  vector<bool> mark;
};
} // namespace nitro