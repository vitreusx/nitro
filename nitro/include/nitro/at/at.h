#pragma once
#include "../indexed.h"
namespace nitro {

template <typename T, bool Indexed> struct at_expr_aux;

template <typename T> struct at_expr_impl : at_expr_aux<T, is_indexed<T>> {};

template <typename T> using at_expr = typename at_expr_impl<T>::type;

template <typename T> struct at_expr_aux<T, false> { using type = T &; };
}