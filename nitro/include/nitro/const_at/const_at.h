#pragma once
#include "../indexed.h"

namespace nitro {
template <typename T, bool Indexed> struct const_at_aux;

template <typename T>
struct const_at_expr_impl : const_at_aux<T, is_indexed<T>> {};

template <typename T>
using const_at_expr = typename const_at_expr_impl<T>::type;

template <typename T> struct const_at_aux<T, false> { using type = T const &; };

}