#pragma once
#include "../at.h"
#include "decl.h"

namespace nitro {
template <typename T> class def_iterator {
public:
  explicit def_iterator(T *ptr) : ptr{ptr} {};

  at_expr<T> operator*() const { return *ptr; }

  def_iterator operator++() const { return def_iterator(ptr++); }

  def_iterator &operator++(int) {
    ++ptr;
    return *this;
  }

  template <typename Idx> def_iterator operator+(Idx const &offset) const {
    return def_iterator(ptr + offset);
  }

  template <typename Idx> def_iterator &operator+=(Idx const &offset) {
    ptr += offset;
    return *this;
  }

  def_iterator operator--() const { return def_iterator(ptr--); }

  def_iterator &operator--(int) {
    --ptr;
    return *this;
  }

  template <typename Idx> def_iterator operator-(Idx const &offset) const {
    return def_iterator(ptr - offset);
  }

  template <typename Idx> def_iterator &operator-=(Idx const &offset) {
    ptr -= offset;
    return *this;
  }

  bool operator<(def_iterator const &other) const { return ptr < other.ptr; }

  bool operator<=(def_iterator const &other) const { return ptr <= other.ptr; }

  bool operator>(def_iterator const &other) const { return ptr > other.ptr; }

  bool operator>=(def_iterator const &other) const { return ptr >= other.ptr; }

  bool operator==(def_iterator const &other) const { return ptr == other.ptr; }

  bool operator!=(def_iterator const &other) const { return ptr != other.ptr; }

private:
  T *ptr;
};
} // namespace nitro