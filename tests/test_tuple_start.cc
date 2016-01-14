// -*- c++ -*-
/* Copyright 2016, The libsigc++ Development Team
 *  Assigned to public domain.  Use as you wish without restriction.
 */


#include <sigc++/tuple_start.h>
#include <cstdlib>
#include <cassert>

void test_tuple_type_start()
{
  {
    using type_tuple = std::tuple<int, short, double>;
    using type_tuple_start = sigc::tuple_type_start<type_tuple, 1>::type;
    using type_tuple_expected = std::tuple<int>;

    static_assert(std::is_same<type_tuple_start, type_tuple_expected>::value,
        "unexpected type_tuple_start type");
  }

  {
    using type_tuple = std::tuple<int, short, double>;
    using type_tuple_start = sigc::tuple_type_start<type_tuple, 2>::type;
    using type_tuple_expected = std::tuple<int, short>;

    static_assert(std::is_same<type_tuple_start, type_tuple_expected>::value,
        "unexpected type_tuple_start type");
  }

  {
    using type_tuple = std::tuple<int, short, double>;
    using type_tuple_start = sigc::tuple_type_start<type_tuple, 3>::type;
    using type_tuple_expected = std::tuple<int, short, double>;

    static_assert(std::is_same<type_tuple_start, type_tuple_expected>::value,
        "unexpected type_tuple_start type");
  }
}

void test_tuple_start()
{
  {
    auto t_original = std::make_tuple(nullptr, std::string("hello"),
      std::string("world"));
    auto t_prefix = sigc::tuple_start<3>(t_original);

    static_assert(std::tuple_size<decltype(t_prefix)>::value == 3,
      "unexpected tuple_start()ed tuple size.");

    assert(std::get<0>(t_prefix) == nullptr);
    assert(std::get<1>(t_prefix) == "hello");
    assert(std::get<2>(t_prefix) == "world");

    static_assert(std::is_same<decltype(t_prefix), decltype(t_original)>::value,
      "unexpected start()ed tuple type");
  }

  {
    auto t_original = std::make_tuple(nullptr, std::string("hello"),
      std::string("world"));
    auto t_prefix = sigc::tuple_start<2>(t_original);

    static_assert(std::tuple_size<decltype(t_prefix)>::value == 2,
      "unexpected tuple_start()ed tuple size.");
      
    assert(std::get<0>(t_prefix) == nullptr);
    assert(std::get<1>(t_prefix) == "hello");

    using type_tuple_prefix = std::tuple<std::nullptr_t, std::string>;
    static_assert(std::is_same<decltype(t_prefix), type_tuple_prefix>::value,
      "unexpected start()ed tuple type");
  }

  {
    auto t_original = std::make_tuple(nullptr, std::string("hello"),
      std::string("world"));
    auto t_prefix = sigc::tuple_start<1>(t_original);

    static_assert(std::tuple_size<decltype(t_prefix)>::value == 1,
      "unexpected tuple_start()ed tuple size.");

    assert(std::get<0>(t_prefix) == nullptr);

    using type_tuple_prefix = std::tuple<std::nullptr_t>;
    static_assert(std::is_same<decltype(t_prefix), type_tuple_prefix>::value,
      "unexpected start()ed tuple type");
  }
}

int main()
{  
  test_tuple_type_start();
  test_tuple_start();
}
