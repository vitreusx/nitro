#pragma once
#include <memory>

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