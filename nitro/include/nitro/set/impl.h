#pragma once
#include "../vector.h"
#include "node.h"

namespace nitro {
template <typename T, typename Alloc = allocator<set_node<T>>,
          typename Idx = int>
class set : public vector<set_node<T>, Alloc, Idx> {
public:
  using Base = vector<set_node<T>, Alloc, Idx>;

  set() : Base() {}

  explicit set(Alloc const &alloc) : Base(alloc) {}

  explicit set(Idx n, T const &init = T(), Alloc const &alloc = Alloc())
      : Base(n, set_node<T>(init, false), alloc) {}

  template <typename U>
  explicit set(Idx n, U const &init, Alloc const &alloc = Alloc())
      : Base(n, set_node<T>(init, false), alloc) {}

  template <typename U> void insert(U const &value) {
    push_back(set_node<T>(value, false));
  }

  template <typename... Args> at_expr<set_node<T>> emplace(Args &&...args) {
    return emplace_back(T(std::forward<Args>(args)...), false);
  }

  void remove(Idx idx) { (*this)[idx].remove(); }

private:
  using Base::emplace_back;
  using Base::push_back;
  using Base::reserve;
  using Base::resize;
};
} // namespace nitro