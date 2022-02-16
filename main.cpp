#include <iostream>
#include <memory>
#include <string_view>

template <typename Impl> struct oexpr {
  template <size_t I> decltype(auto) get() {
    return static_cast<Impl &>(*this).template get<I>();
  }

  template <size_t I> decltype(auto) get() const {
    return static_cast<Impl const &>(*this).template get<I>();
  }
};

template <typename... Types> struct tuple;

template <> struct tuple<> : public oexpr<tuple<>> {};

template <typename Head, typename... Tail>
struct tuple<Head, Tail...> : public oexpr<tuple<Head, Tail...>> {
  tuple() = default;
  tuple(Head &&head, Tail &&...tail)
      : head{std::forward<Head>(head)}, tail{std::forward<Tail>(tail)...} {};

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

  Head head;
  tuple<Tail...> tail;
};

template <size_t I, typename T> struct nth_impl;

template <size_t I, typename T> using nth = typename nth_impl<I, T>::type;

template <size_t I, typename Head, typename... Tail>
struct nth_impl<I, tuple<Head, Tail...>> : nth_impl<I - 1, tuple<Tail...>> {};

template <typename Head, typename... Tail>
struct nth_impl<0, tuple<Head, Tail...>> {
  using type = Head;
};

template <typename... Types> struct serial : tuple<Types...> {
  serial() = delete;
  ~serial() = delete;
};

template <size_t I, typename... Types>
struct nth_impl<I, serial<Types...>> : nth_impl<I, tuple<Types...>> {};

template <typename... Types> struct parallel : tuple<Types...> {
  parallel() = delete;
  ~parallel() = delete;
};

template <size_t I, typename... Types>
struct nth_impl<I, parallel<Types...>> : nth_impl<I, tuple<Types...>> {};

template <typename T> struct def_alloc_impl { using type = std::allocator<T>; };

template <typename T> using def_alloc = typename def_alloc_impl<T>::type;

template <typename... Types> struct def_alloc_impl<serial<Types...>> {
  using type = std::allocator<tuple<Types...>>;
};

template <typename... Types> struct def_alloc_impl<parallel<Types...>> {
  using type = tuple<def_alloc<Types>...>;
};

template <typename T, typename Alloc, typename Idx> class serial_vector {
public:
  serial_vector(Alloc alloc = Alloc())
      : data{nullptr}, _size{0}, _capacity{0}, alloc{alloc} {};

  serial_vector(Idx n, T const &init = T(), Alloc alloc = Alloc()) {
    this->alloc = alloc;
    data = this->alloc.allocate(n);
    std::uninitialized_fill_n(data, n, init);
    _size = _capacity = n;
  }

  ~serial_vector() { destroy(); }

  serial_vector(serial_vector const &other) {
    alloc = other.alloc;
    data = alloc.allocate(other._size);
    std::uninitialized_copy_n(other.data, other._size, data);
    _size = _capacity = other._size;
  }

  serial_vector &operator=(serial_vector const &other) {
    destroy();
    alloc = other.alloc;
    data = alloc.allocate(other._size);
    std::uninitialized_copy_n(other.data, other._size, data);
    _size = _capacity = other._size;
    return *this;
  }

  serial_vector(serial_vector &&other) noexcept {
    data = other.data;
    _size = other._size;
    _capacity = other._capacity;
    other.data = nullptr;
    other._size = other._capacity = 0;
  }

  serial_vector &operator=(serial_vector &&other) noexcept {
    destroy();
    data = other.data;
    _size = other._size;
    _capacity = other._capacity;
    other.data = nullptr;
    other._size = other._capacity = 0;
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

template <typename T, typename Alloc, typename Idx> struct vector_impl {
  using type = serial_vector<T, Alloc, Idx>;
};

template <typename T, typename Alloc = def_alloc<T>, typename Idx = int>
using vector = typename vector_impl<T, Alloc, Idx>::type;

template <typename Alloc, typename Idx, typename... Types>
struct vector_impl<serial<Types...>, Alloc, Idx>
    : vector_impl<tuple<Types...>, Alloc, Idx> {};

template <typename T> struct at_expr_impl { using type = T &; };

template <typename T> using at_expr = typename at_expr_impl<T>::type;

template <typename... Types>
struct at_expr_impl<serial<Types...>> : at_expr_impl<tuple<Types...>> {};

template <typename Types, size_t... ISeq>
class parallel_at_expr : public oexpr<parallel_at_expr<Types, ISeq...>> {
public:
  parallel_at_expr(at_expr<nth<ISeq, Types>> &&...at_exprs)
      : at_exprs{std::forward<at_expr<nth<ISeq, Types>>>(at_exprs)...} {};

  parallel_at_expr &operator=(parallel_at_expr const &other) const {
    (..., assign<ISeq>(other));
    return *this;
  }

  template <typename E>
  parallel_at_expr const &operator=(oexpr<E> const &e) const {
    (..., assign<ISeq, E>(e));
    return *this;
  }

  template <size_t I> decltype(auto) get() const {
    return at_exprs.template get<I>();
  }

private:
  template <size_t I, typename E> void assign(oexpr<E> const &e) const {
    at_exprs.template get<I>() = e.template get<I>();
  }

  tuple<at_expr<nth<ISeq, Types>>...> at_exprs;
};

template <typename... Types> struct at_expr_impl<parallel<Types...>> {
private:
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>)
      -> parallel_at_expr<parallel<Types...>, ISeq...>;

public:
  using type = decltype(aux(std::make_index_sequence<sizeof...(Types)>{}));
};

template <typename T> struct at_const_expr_impl { using type = T const &; };

template <typename T>
using at_const_expr = typename at_const_expr_impl<T>::type;

template <typename... Types>
struct at_const_expr_impl<serial<Types...>>
    : at_const_expr_impl<tuple<Types...>> {};

template <typename Types, size_t... ISeq>
class parallel_at_const_expr
    : public oexpr<parallel_at_const_expr<Types, ISeq...>> {
public:
  parallel_at_const_expr(at_const_expr<nth<ISeq, Types>> &&...at_exprs)
      : at_exprs{std::forward<at_const_expr<nth<ISeq, Types>>>(at_exprs)...} {};

  parallel_at_const_expr &
  operator=(parallel_at_const_expr const &other) = delete;

  template <size_t I> decltype(auto) get() const {
    return at_exprs.template get<I>();
  }

private:
  tuple<at_expr<nth<ISeq, Types>>...> at_exprs;
};

template <typename... Types> struct at_const_expr_impl<parallel<Types...>> {
private:
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>)
      -> parallel_at_const_expr<parallel<Types...>, ISeq...>;

public:
  using type = decltype(aux(std::make_index_sequence<sizeof...(Types)>{}));
};

template <typename Types, typename Allocs, typename Idx, size_t... ISeq>
class parallel_vector {
public:
  template <size_t I> using part_t = vector<nth<I, Types>, nth<I, Allocs>, Idx>;

  parallel_vector(Allocs const &allocs = Allocs())
      : shards{part_t<ISeq>{allocs.template get<ISeq>()}...} {}

  parallel_vector(
      Idx n,
      tuple<nth<ISeq, Types>...> const &init = tuple<nth<ISeq, Types>...>(),
      Allocs allocs = Allocs())
      : shards{part_t<ISeq>{n, init.template get<ISeq>(),
                            allocs.template get<ISeq>()}...} {}

  template <typename E>
  parallel_vector(Idx n, oexpr<E> const &init, Allocs allocs = Allocs())
      : shards{part_t<ISeq>{n, init.template get<ISeq>(),
                            allocs.template get<ISeq>()}...} {}

  Idx size() const { return shards.template get<0>().size(); }
  Idx capacity() const { return shards.template get<0>().capacity(); }

  at_expr<Types> operator[](Idx idx) {
    return {shards.template get<ISeq>()[idx]...};
  }

  at_const_expr<Types> operator[](Idx idx) const {
    return {shards.template get<ISeq>()[idx]...};
  }

private:
  tuple<part_t<ISeq>...> shards;
};

template <typename Allocs, typename Idx, typename... Types>
struct vector_impl<parallel<Types...>, Allocs, Idx> {
private:
  template <size_t... ISeq>
  static auto aux(std::index_sequence<ISeq...>)
      -> parallel_vector<parallel<Types...>, Allocs, Idx, ISeq...>;

public:
  using type = decltype(aux(std::make_index_sequence<sizeof...(Types)>{}));
};

template <typename T> constexpr auto type_name() {
  std::string_view name, prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto type_name() [with T = ";
  suffix = "]";
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  prefix = "auto __cdecl type_name<";
  suffix = ">(void)";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}

template <typename E> std::ostream &print(std::ostream &os, oexpr<E> const &e) {
  os << "(" << e.template get<0>() << ", " << e.template get<1>() << ", "
     << e.template get<2>() << ")";
  return os;
}

int main() {
  vector<parallel<char, int, float>> v(10, {'0', 1, 2.0f});

  for (int i = 0; i < v.size(); ++i)
    print(std::cout, v[i]) << '\n';

  for (int i = 0; i < v.size(); ++i)
    v[i] = tuple<char, int, float>{'0' + i, 10 + i, 20.0f + i};

  for (int i = 0; i < v.size(); ++i)
    print(std::cout, v[i]) << '\n';

  return 0;
}