#ifndef _SIGC_TUPLE_START_H_
#define _SIGC_TUPLE_START_H_

#include <tuple>
#include <utility>

namespace sigc
{

namespace {

template<typename T, typename Seq>
struct tuple_type_start_impl;

template<typename T, std::size_t... I>
struct tuple_type_start_impl<T, std::index_sequence<I...>>
{
  using type = std::tuple<typename std::tuple_element<I, T>::type...>;
};

} //anonymous namespace

/**
 * Get the type of a tuple with just the first @len items.
 */
template<typename T, std::size_t len>
struct tuple_type_start
: tuple_type_start_impl<T, std::make_index_sequence<len>>
{};


namespace {

template<typename T, typename Seq>
struct tuple_start_impl;

template<typename T, std::size_t... I>
struct tuple_start_impl<T, std::index_sequence<I...>>
{
  static
  decltype(auto)
  tuple_start(const T& t) {
    return std::make_tuple(std::get<I>(t)...);
  }
};

} //anonymous namespace

/**
 * Get the tuple with the last @a len items of the original.
 */
template<std::size_t len, typename T>
decltype(auto) //typename tuple_type_end<T, len>::type 
tuple_start(const T& t) {
  return tuple_start_impl<T, std::make_index_sequence<len>>::tuple_start(t);
}

} //namespace sigc;

#endif //_SIGC_TUPLE_START_H_
