#pragma once
#include <nitro/indexed.h>

template <typename T, bool Indexed> struct flat_size_aux;

template <typename T>
struct flat_size_impl : flat_size_aux<T, is_indexed<T>> {};

template <typename T> constexpr size_t flat_size = flat_size_impl<T>::value;

template <typename T> struct flat_size_aux<T, false> {
  static constexpr size_t value = 1;
};

template <size_t... ISeq> struct sum_impl;

template <size_t... ISeq> constexpr size_t sum = sum_impl<ISeq...>::value;

template <> struct sum_impl<> { static constexpr size_t value = 0; };

template <size_t Head, size_t... Tail> struct sum_impl<Head, Tail...> {
  static constexpr size_t value = Head + sum<Tail...>;
};

template <typename T> struct flat_size_aux<T, true> {
private:
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>) -> std::integral_constant<
      size_t, sum<flat_size<typename T::template ith_type<ISeq>>...>> {}

public:
  static constexpr size_t value =
      decltype(aux(std::make_index_sequence<T::num_types>{}))::value;
};

template <size_t I, typename T, bool Indexed> struct flat_get_aux;

template <size_t I, typename T>
struct flat_get_impl : flat_get_aux<I, T, is_indexed<T>> {};

template <size_t I, typename E, typename T = E> decltype(auto) flat_get(E &e) {
  return flat_get_impl<I, T>::template impl<E>(e);
}

template <size_t I, typename E, typename T = E>
decltype(auto) flat_get(E const &e) {
  return flat_get_impl<I, T>::template impl<E>(e);
}

template <size_t I, typename T> struct flat_get_aux<I, T, false> {
  template <typename E> static decltype(auto) impl(E &e) {
    if constexpr (I == 0)
      return e;
  }

  template <typename E> static decltype(auto) impl(E const &e) {
    if constexpr (I == 0)
      return e;
  }
};

template <size_t I, size_t Cur, typename T> struct indexed_flat_get {
  using CurT = typename T::template ith_type<Cur>;

  template <typename E> static decltype(auto) impl(E &x) {
    if constexpr (I < flat_size<CurT>)
      return flat_get<I, CurT>(x.template get<Cur>());
    else
      return indexed_flat_get<I - flat_size<CurT>, Cur + 1,
                              T>::template impl<E>(x);
  }

  template <typename E> static decltype(auto) impl(E const &x) {
    if constexpr (I < flat_size<CurT>)
      return flat_get<I, CurT>(x.template get<Cur>());
    else
      return indexed_flat_get<I - flat_size<CurT>, Cur + 1,
                              T>::template impl<E>(x);
  }
};

template <typename T, bool Indexed> struct repr_impl_aux;

template <typename T> struct repr_impl_aux<T, false> {
  using type = type_list<T>;
};

template <typename T> struct repr_impl_aux<T, true> {
private:
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>)
      -> type_list<typename T::template ith_type<ISeq>...> {}

public:
  using type = decltype(aux(std::make_index_sequence<T::num_types>{}));
};

template <typename T> struct repr_impl : repr_impl_aux<T, is_indexed<T>> {};

template <typename T> using repr = typename repr_impl<T>::type;

template <size_t I, typename T>
struct flat_get_aux<I, T, true> : indexed_flat_get<I, 0, T> {};

template <typename T, bool Indexed> struct flat_repr_aux;

template <typename T>
struct flat_repr_impl : flat_repr_aux<T, is_indexed<T>> {};

template <typename T> using flat_repr = typename flat_repr_impl<T>::type;

template <typename T> struct flat_repr_aux<T, false> { using type = repr<T>; };

template <typename T> struct flat_repr_aux<T, true> {
  using type = concat<map<to_functor<flat_repr>, repr<T>>>;
};