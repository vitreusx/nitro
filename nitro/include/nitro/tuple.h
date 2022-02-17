#pragma once
#include <nitro/expr.h>
#include <nitro/indexed.h>
#include <nitro/type_list.h>
#include <utility>

template <typename... Types> class tuple;

template <> class tuple<> : public ind_expr<tuple<>> {
  static constexpr size_t num_types = 0;
};

template <typename Head, typename... Tail>
class tuple<Head, Tail...> : public ind_expr<tuple<Head, Tail...>> {
public:
  tuple(Head &&head, Tail &&...tail)
      : head{std::forward<Head>(head)}, tail{std::forward<Tail>(tail)...} {}

  template <typename E>
  tuple(ind_expr<E> const &e)
      : tuple(e, std::make_index_sequence<E::num_types>{}) {}

  template <size_t I> decltype(auto) get() {
    if constexpr (I == 0)
      return (head);
    else
      return tail.template get<I - 1>();
  }

  template <size_t I> decltype(auto) get() const {
    if constexpr (I == 0)
      return (head);
    else
      return tail.template get<I - 1>();
  }

  static constexpr size_t num_types = type_list<Head, Tail...>::num_types;

  template <size_t I>
  using ith_type = typename type_list<Head, Tail...>::template ith_type<I>;

private:
  Head head;
  tuple<Tail...> tail;

  template <typename E, size_t... ISeq>
  tuple(ind_expr<E> const &e, std::index_sequence<ISeq...>)
      : tuple{e.template get<ISeq>()...} {}
};

template <typename E, typename... Types> struct expr_impl<E, tuple<Types...>> {
  using type = ind_expr<E>;
};

template <typename E, typename... Types>
struct auto_expr_impl<E, tuple<Types...>> {
  using type = ind_expr<E>;
};

template <typename... Types>
struct is_indexed_impl<tuple<Types...>> : std::true_type {};