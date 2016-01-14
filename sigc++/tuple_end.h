#ifndef _SIGC_TUPLE_END_H_
#define _SIGC_TUPLE_END_H_

#include <sigc++/tuple_cdr.h>

namespace sigc
{

namespace {

template<typename T, std::size_t remove_from_start>
struct tuple_type_end_impl
{
  using type = typename tuple_type_end_impl<typename tuple_type_cdr<T>::type, remove_from_start - 1>::type;
};

template<typename T>
struct tuple_type_end_impl<T, 0>
{
  using type = T;
};

} //anonymous namespace


/**
 * Get the type of a tuple with the last @a len types of the original.
 */
template<typename T, std::size_t len>
struct tuple_type_end
: tuple_type_end_impl<T, std::tuple_size<T>::value - len>
{};


namespace {

template<typename T, std::size_t remove_from_start>
struct tuple_end_impl
{
  static
  decltype(auto) //typename tuple_type_end<T, size - remove_from_start>::type 
  tuple_end(const T& t) {
    return tuple_end_impl<typename tuple_type_cdr<T>::type, remove_from_start - 1>::tuple_end(tuple_cdr(t));
  }
};

template<typename T>
struct tuple_end_impl<T, 0>
{
  static
  T
  tuple_end(const T& t) {
    return t;
  }
};

} //anonymous namespace

/**
 * Get the ttuple with the last @a len items of the original.
 */
template<std::size_t len, typename T>
decltype(auto) //typename tuple_type_end<T, len>::type 
tuple_end(const T& t) {
  constexpr auto size = std::tuple_size<T>::value; 
  return tuple_end_impl<T, size - len>::tuple_end(t);
}

} //namespace sigc;

#endif //_SIGC_TUPLE_END_H_
