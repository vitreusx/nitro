#pragma once
#include <memory>
#include <nitro/lane_at.h>
#include <nitro/lane_const_at.h>

namespace nitro {
template <typename T, typename Alloc, typename Idx> class def_vector {
public:
  def_vector() : def_vector(Alloc()){};

  explicit def_vector(Alloc alloc)
      : data{nullptr}, _size{0}, _capacity{0}, alloc{alloc} {};

  explicit def_vector(Idx n, T const &init = T(), Alloc alloc = Alloc()) {
    this->alloc = alloc;
    data = this->alloc.allocate(n);
    std::uninitialized_fill_n(data, n, init);
    _size = _capacity = n;
  }

  ~def_vector() { destroy(); }

  def_vector(def_vector const &other) {
    alloc = other.alloc;
    data = alloc.allocate(other._size);
    std::uninitialized_copy_n(other.data, other._size, data);
    _size = _capacity = other._size;
  }

  def_vector &operator=(def_vector const &other) {
    if (&*this != &other) {
      destroy();
      alloc = other.alloc;
      data = alloc.allocate(other._size);
      std::uninitialized_copy_n(other.data, other._size, data);
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

  T &operator[](Idx idx) { return data[idx]; }

  T const &operator[](Idx idx) const { return data[idx]; }

  template <size_t N> def_lane_at<T, N> lane_at(Idx idx) {
    return def_lane_at<T, N>(data[N * idx]);
  }

  template <size_t N> lane<T, N> lane_at(Idx idx) const {
    lane<T, N> res;
    res.load(data + N * idx);
    return res;
  }

  template <size_t N> Idx num_lanes() const { return size() / N; }

private:
  void destroy() {
    if (data) {
      std::destroy_n(data, _size);
      alloc.deallocate(data, _capacity);
    }
    _size = _capacity = 0;
  }

  T *data;
  Idx _size, _capacity;
  Alloc alloc;
};

}