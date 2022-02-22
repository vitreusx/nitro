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

  T &at(Idx idx) { return (*this)[idx]; }

  T const &at(Idx idx) const { return (*this)[idx]; }

  template <size_t N> def_lane_at<T, N> lane_at(Idx idx) {
    return def_lane_at<T, N>(data[N * idx]);
  }

  template <size_t N> lane<T, N> lane_at(Idx idx) const {
    lane<T, N> res;
    res.load(data + N * idx);
    return res;
  }

  template <size_t N> Idx num_lanes() const { return size() / N; }

  void clear() { destroy(); }

  void reserve(Idx new_capacity) {
    if (new_capacity <= _capacity)
      return;

    auto new_data = alloc.allocate(new_capacity);
    std::uninitialized_move_n(data, _size, new_data);
    std::destroy_n(data, _size);
    alloc.deallocate(data, _capacity);
    data = new_data;
    _capacity = new_capacity;
  }

  void resize(Idx new_size, T const &init = T()) {
    if (new_size < _size) {
      std::destroy_n(data + new_size, _size - new_size);
    } else {
      reserve(new_size);
      std::uninitialized_fill_n(data + _size, new_size - _size, init);
    }
    _size = new_size;
  }

  void push_back(T const &value) {
    if (_size + 1 > _capacity)
      reserve(2 * (_size + 1) + 8);

    std::uninitialized_copy_n(&value, 1, data + _size);
    ++_size;
  }

  template <typename... Args> T& emplace_back(Args &&...args) {
    if (_size + 1 > _capacity)
      reserve(2 * (_size + 1) + 8);

    ::new (data + _size) T(std::forward<Args>(args)...);
    return data[_size++];
  }

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

} // namespace nitro