// -*- c++ -*-
/* Copyright 2016, The libsigc++ Development Team
 *  Assigned to public domain.  Use as you wish without restriction.
 */


#include <sigc++/tuple_end.h>
#include <cstdlib>
#include <cassert>
#include <iostream>

void test_tuple_type_end()
{
  {
    using type_tuple = std::tuple<int, short, double>;
    using type_tuple_end = sigc::tuple_type_end<type_tuple, 1>::type;
    using type_tuple_expected = std::tuple<double>;

    static_assert(std::tuple_size<type_tuple_end>::value == 1,
      "unexpected tuple_end()ed tuple size.");
    static_assert(std::is_same<type_tuple_end, type_tuple_expected>::value,
      "unexpected type_tuple_end type");
  }

  {
    using type_tuple = std::tuple<int, short, double>;
    using type_tuple_end = sigc::tuple_type_end<type_tuple, 2>::type;
    using type_tuple_expected = std::tuple<short, double>;

    static_assert(std::tuple_size<type_tuple_end>::value == 2,
      "unexpected tuple_end()ed tuple size.");
    static_assert(std::is_same<type_tuple_end, type_tuple_expected>::value,
      "unexpected type_tuple_end type");
  }

  {
    using type_tuple = std::tuple<int, short, double>;
    using type_tuple_end = sigc::tuple_type_end<type_tuple, 3>::type;
    using type_tuple_expected = std::tuple<int, short, double>;

    static_assert(std::tuple_size<type_tuple_end>::value == 3,
      "unexpected tuple_end()ed tuple size.");
    static_assert(std::is_same<type_tuple_end, type_tuple_expected>::value,
        "unexpected type_tuple_end type");
  }
}

void test_tuple_end()
{
  {
    auto t_original = std::make_tuple(nullptr, std::string("hello"),
      std::string("world"));
    auto t_suffix = sigc::tuple_end<3>(t_original);

    static_assert(std::tuple_size<decltype(t_suffix)>::value == 3,
      "unexpected tuple_end()ed tuple size.");

    assert(std::get<0>(t_suffix) == nullptr);
    assert(std::get<1>(t_suffix) == "hello");
    assert(std::get<2>(t_suffix) == "world");

    static_assert(std::is_same<decltype(t_suffix), decltype(t_original)>::value,
      "unexpected end()ed tuple type");
  }

  {
    auto t_original = std::make_tuple(nullptr, std::string("hello"),
      std::string("world"));
    auto t_suffix = sigc::tuple_end<2>(t_original);

    static_assert(std::tuple_size<decltype(t_suffix)>::value == 2,
      "unexpected tuple_end()ed tuple size.");
      
    assert(std::get<0>(t_suffix) == "hello");
    assert(std::get<1>(t_suffix) == "world");

    using type_tuple_suffix = std::tuple<std::string, std::string>;
    static_assert(std::is_same<decltype(t_suffix), type_tuple_suffix>::value,
      "unexpected end()ed tuple type");
  }

  {
    auto t_original = std::make_tuple(nullptr, std::string("hello"),
      std::string("world"));
    auto t_suffix = sigc::tuple_end<1>(t_original);

    static_assert(std::tuple_size<decltype(t_suffix)>::value == 1,
      "unexpected tuple_end()ed tuple size.");

    assert(std::get<0>(t_suffix) == "world");

    using type_tuple_suffix = std::tuple<std::string>;
    static_assert(std::is_same<decltype(t_suffix), type_tuple_suffix>::value,
      "unexpected end()ed tuple type");
  }
}

int main()
{
  test_tuple_type_end();
  test_tuple_end();
      
  return EXIT_SUCCESS;
}
