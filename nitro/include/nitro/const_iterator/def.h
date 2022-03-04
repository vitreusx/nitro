#pragma once
#include "../const_at.h"
#include "decl.h"

namespace nitro {
template <typename T> class def_const_iterator {
public:
  explicit def_const_iterator(T const *ptr) : ptr{ptr} {};

  const_at_expr<T> operator*() const { return *ptr; }

  def_const_iterator operator++() const { return def_const_iterator(ptr++); }

  def_const_iterator &operator++(int) {
    ++ptr;
    return *this;
  }

  template <typename Idx>
  def_const_iterator operator+(Idx const &offset) const {
    return def_const_iterator(ptr + offset);
  }

  template <typename Idx> def_const_iterator &operator+=(Idx const &offset) {
    ptr += offset;
    return *this;
  }

  def_const_iterator operator--() const { return def_const_iterator(ptr--); }

  def_const_iterator &operator--(int) {
    --ptr;
    return *this;
  }

  template <typename Idx>
  def_const_iterator operator-(Idx const &offset) const {
    return def_const_iterator(ptr - offset);
  }

  template <typename Idx> def_const_iterator &operator-=(Idx const &offset) {
    ptr -= offset;
    return *this;
  }

  bool operator<(def_const_iterator const &other) const {
    return ptr < other.ptr;
  }

  bool operator<=(def_const_iterator const &other) const {
    return ptr <= other.ptr;
  }

  bool operator>(def_const_iterator const &other) const {
    return ptr > other.ptr;
  }

  bool operator>=(def_const_iterator const &other) const {
    return ptr >= other.ptr;
  }

  bool operator==(def_const_iterator const &other) const {
    return ptr == other.ptr;
  }

  bool operator!=(def_const_iterator const &other) const {
    return ptr != other.ptr;
  }

private:
  T *const ptr;
};
} // namespace nitro