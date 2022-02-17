#pragma once
#include <nitro/def_alloc.h>

template <typename T, typename Alloc, typename Idx> struct vector_impl;

template <typename T, typename Alloc = def_alloc<T>, typename Idx = int>
using vector = typename vector_impl<T, Alloc, Idx>::type;