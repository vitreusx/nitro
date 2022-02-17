#pragma once
#include <nitro/expr.h>
#include <nitro/indexed.h>
#include <nitro/tuple.h>
#include <ostream>

template <typename Scalar> class vec3;

template <typename E> struct vec3_expr : public ind_expr<E> {
  decltype(auto) x() { return static_cast<E &>(*this).x(); }
  decltype(auto) x() const { return static_cast<E const &>(*this).x(); }
  decltype(auto) y() { return static_cast<E &>(*this).y(); }
  decltype(auto) y() const { return static_cast<E const &>(*this).y(); }
  decltype(auto) z() { return static_cast<E &>(*this).z(); }
  decltype(auto) z() const { return static_cast<E const &>(*this).z(); }

  template <size_t I> decltype(auto) get() {
    if constexpr (I == 0)
      return x();
    else if constexpr (I == 1)
      return y();
    else if constexpr (I == 2)
      return z();
  }

  template <size_t I> decltype(auto) get() const {
    if constexpr (I == 0)
      return x();
    else if constexpr (I == 1)
      return y();
    else if constexpr (I == 2)
      return z();
  }
};

template <typename E, typename Scalar> struct expr_impl<E, vec3<Scalar>> {
  using type = vec3_expr<E>;
};

template <typename E> struct vec3_auto_expr : public vec3_expr<E> {
  decltype(auto) x() { return static_cast<E &>(*this).template get<0>(); }
  decltype(auto) x() const {
    return static_cast<E const &>(*this).template get<0>();
  }
  decltype(auto) y() { return static_cast<E &>(*this).template get<1>(); }
  decltype(auto) y() const {
    return static_cast<E const &>(*this).template get<1>();
  }
  decltype(auto) z() { return static_cast<E &>(*this).template get<2>(); }
  decltype(auto) z() const {
    return static_cast<E const &>(*this).template get<2>();
  }

  template <size_t I> decltype(auto) get() {
    return static_cast<E &>(*this).template get<I>();
  }

  template <size_t I> decltype(auto) get() const {
    return static_cast<E const &>(*this).template get<I>();
  }
};

template <typename E, typename Scalar> struct auto_expr_impl<E, vec3<Scalar>> {
  using type = vec3_auto_expr<E>;
};

template <typename Scalar>
class vec3 : public vec3_auto_expr<vec3<Scalar>>,
             public tuple_wrapper<Scalar, Scalar, Scalar> {
public:
  using Base = tuple_wrapper<Scalar, Scalar, Scalar>;
  using Base::get;

  vec3() : Base((Scalar)0, (Scalar)0, (Scalar)0) {}

  vec3(Scalar &&x, Scalar &&y, Scalar &&z)
      : Base(std::forward<Scalar>(x), std::forward<Scalar>(y),
             std::forward<Scalar>(z)) {}

  template <typename E>
  vec3(vec3_expr<E> const &e) : vec3(e.x(), e.y(), e.z()) {}
};

template <typename Scalar>
struct is_indexed_impl<vec3<Scalar>> : std::true_type {};

using vec3f = vec3<float>;

template <typename E1, typename E2>
class cross_expr : public vec3_expr<cross_expr<E1, E2>> {
public:
  cross_expr(E1 const &e1, E2 const &e2) : e1{e1}, e2{e2} {};

  decltype(auto) x() const { return e1.y() * e2.z() - e1.z() * e2.y(); }

  decltype(auto) y() const { return e1.z() * e2.x() - e1.x() * e2.z(); }

  decltype(auto) z() const { return e1.x() * e2.y() - e1.y() * e2.x(); }

private:
  E1 e1;
  E2 e2;
};

template <typename E1, typename E2>
decltype(auto) cross(vec3_expr<E1> const &e1, vec3_expr<E2> const &e2) {
  return cross_expr<E1, E2>(static_cast<E1 const &>(e1),
                            static_cast<E2 const &>(e2));
}

template <typename E>
std::ostream &operator<<(std::ostream &os, vec3_expr<E> const &e) {
  os << "(" << e.x() << ", " << e.y() << ", " << e.z() << ")";
  return os;
}