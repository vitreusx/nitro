#pragma once
#include "../at.h"
#include "../const_view.h"
#include "../lane_at.h"
#include "../lane_const_at.h"
#include "../view.h"
#include <algorithm>
#include <memory>

namespace nitro {
template <typename T, typename Alloc, typename Idx> class def_vector {
public:
  def_vector() = default;

  explicit def_vector(Alloc alloc) : alloc{alloc} {};

  explicit def_vector(Idx n, T const &init = T(), Alloc alloc = Alloc()) {
    this->alloc = alloc;
    data = this->alloc.allocate(n);
    std::uninitialized_fill_n(data, n, init);
    _size = _capacity = n;
  }

  ~def_vector() { destroy(); }

  def_vector(def_vector const &other) {
    alloc = other.alloc;
    if (other._size > 0) {
      data = alloc.allocate(other._size);
      std::uninitialized_copy_n(other.data, other._size, data);
    }
    _size = _capacity = other._size;
  }

  def_vector &operator=(def_vector const &other) {
    if (&*this != &other) {
      destroy();
      alloc = other.alloc;
      if (other._size > 0) {
        data = alloc.allocate(other._size);
        std::uninitialized_copy_n(other.data, other._size, data);
      }
      _size = _capacity = other._size;
    }
    return *this;
  }

  def_vector(def_vector &&other) noexcept {
    data = other.data;
    _size = other._size;
    _capacity = other._capacity;
    other.data = nullptr;
    other._size = other._capacity = 0;
  }

  def_vector &operator=(def_vector &&other) noexcept {
    if (&*this != &other) {
      destroy();
      data = other.data;
      _size = other._size;
      _capacity = other._capacity;
      other.data = nullptr;
      other._size = other._capacity = 0;
    }
    return *this;
  }

  Idx size() const { return _size; }

  Idx capacity() const { return _size; }

  at_expr<T> operator[](Idx idx) { return view()[idx]; }

  const_at_expr<T> operator[](Idx idx) const { return view()[idx]; }

  at_expr<T> at(Idx idx) { return view().at(idx); }

  const_at_expr<T> at(Idx idx) const { return view().at(idx); }

  template <size_t N> def_lane_at<T, N> lane_at(Idx idx) {
    return view().lane_at(idx);
  }

  template <size_t N> lane<T, N> lane_at(Idx idx) const {
    return view().lane_at(idx);
  }

  template <size_t N> Idx num_lanes() const { return view().num_lanes(); }

  def_view<T, Idx> view() { return def_view<T, Idx>(data, _size); }

  def_const_view<T, Idx> view() const {
    return def_const_view<T, Idx>(data, _size);
  }

  void clear() {
    if (data) {
      std::destroy_n(data, _size);
    }
    _size = 0;
  }

  void reserve(Idx new_capacity) {
    if (new_capacity <= _capacity)
      return;

    auto new_data = alloc.allocate(new_capacity);
    if (data) {
      std::uninitialized_move_n(data, _size, new_data);
      std::destroy_n(data, _size);
      alloc.deallocate(data, _capacity);
    }
    data = new_data;
    _capacity = new_capacity;
  }

  void resize(Idx new_size, T const &init = T()) {
    if (new_size < _size) {
      std::destroy_n(data + new_size, _size - new_size);
    } else if (new_size > _size) {
      reserve(new_size);
      std::uninitialized_fill_n(data + _size, new_size - _size, init);
    }
    _size = new_size;
  }

  void push_back(T const &value) {
    if (_size + 1 > _capacity)
      reserve(2 * _size + 8);

    std::uninitialized_copy_n(&value, 1, data + _size);
    ++_size;
  }

  template <typename... Args> T &emplace_back(Args &&...args) {
    if (_size >= _capacity)
      reserve(2 * (_size + 1) + 8);

    ::new (data + _size) T(std::forward<Args>(args)...);
    return data[_size++];
  }

  iterator<T> begin() { return view().begin(); }

  iterator<T> begin() const { return view().begin(); }

  iterator<T> end() { return view().end(); }

  iterator<T> end() const { return view().end(); }

private:
  void destroy() {
    if (data) {
      std::destroy_n(data, _size);
      alloc.deallocate(data, _capacity);
      data = nullptr;
    }
    _size = _capacity = 0;
  }

  T *data = nullptr;
  Idx _size = 0, _capacity = 0;
  Alloc alloc = Alloc();
};

} // namespace nitro