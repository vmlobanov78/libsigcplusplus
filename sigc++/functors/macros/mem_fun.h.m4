dnl Copyright 2002, The libsigc++ Development Team
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public
dnl License as published by the Free Software Foundation; either
dnl version 2.1 of the License, or (at your option) any later version.
dnl
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
dnl Lesser General Public License for more details.
dnl
dnl You should have received a copy of the GNU Lesser General Public
dnl License along with this library; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
dnl
divert(-1)

include(template.macros.m4)

define([MEMBER_FUNCTOR],[dnl
/** [$1]mem_functor wraps $2 methods with argument(s).
 * Use the convenience function mem_fun() to create an instance of [$1]mem_functor.
 *
 * The following template arguments are used:
 * - @e T_arg... Argument types used in the definition of operator()().
 * - @e T_return The return type of operator()().
 * - @e T_obj The object type.
 *
 * @ingroup mem_fun
 */
template<class T_return, class T_obj, class... T_arg>
using [$1]mem_functor =
  mem_functor_base<
    T_return (T_obj::*)(T_arg...) $2,
    T_return, T_obj, T_arg...>;
])

define([BOUND_MEMBER_FUNCTOR],[dnl

/** bound_[$1]mem_functor encapsulates a $2 method with arguments and an object instance.
 * Use the convenience function mem_fun() to create an instance of bound_[$1]mem_functor.
 *
 * The following template arguments are used:
 * - @e T_arg... Argument type used in the definition of operator()().
 * - @e T_return The return type of operator()().
 * - @e T_obj The object type.
 *
 * @ingroup mem_fun
 */
template<class T_return, class T_obj, class... T_arg>
using bound_[$1]mem_functor =
  bound_mem_functor_base<
    T_return (T_obj::*)(T_arg...) $2,
    T_return, T_obj, T_arg...>;
])

define([MEM_FUN],[dnl
/** Creates a functor of type sigc::[$1]mem_functor which wraps a $3 method.
 * @param _A_func Pointer to method that should be wrapped.
 * @return Functor that executes _A_func on invokation.
 *
 * @ingroup mem_fun
 */
template <class T_return, class T_obj, class... T_arg>
inline decltype(auto)
mem_fun(T_return (T_obj::*_A_func)(T_arg...) $3)
{ return mem_functor_base<
    T_return (T_obj::*)(T_arg...) $3,
    T_return, T_obj, T_arg...>(_A_func); }

])
define([BOUND_MEM_FUN],[dnl
/** Creates a functor of type sigc::bound_[$1]mem_functor which encapsulates a method and an object instance.
 * @param _A_obj Pointer to object instance the functor should operate on.
 * @param _A_func Pointer to method that should be wrapped.
 * @return Functor that executes @e _A_func on invokation.
 *
 * @ingroup mem_fun
 */
template <class T_return, class T_obj, class T_obj2, class... T_arg>
inline decltype(auto)
mem_fun(/*$2*/ T_obj* _A_obj, T_return (T_obj2::*_A_func)(T_arg...) $3)
{
  return bound_mem_functor_base<
    T_return (T_obj::*)(T_arg...) $3,
    T_return, T_obj, T_arg...>(_A_obj, _A_func);
}

/** Creates a functor of type sigc::bound_[$1]mem_functor which encapsulates a method and an object instance.
 * @param _A_obj Reference to object instance the functor should operate on.
 * @param _A_func Pointer to method that should be wrapped.
 * @return Functor that executes @e _A_func on invokation.
 *
 * @ingroup mem_fun
 */
template <class T_return, class T_obj, class T_obj2, class... T_arg>
inline decltype(auto)
mem_fun(/*$2*/ T_obj& _A_obj, T_return (T_obj2::*_A_func)(T_arg...) $3)
{
  return bound_mem_functor_base<
    T_return (T_obj::*)(T_arg...) $3,
    T_return, T_obj, T_arg...>(_A_obj, _A_func);
}

])

divert(0)

// implementation notes:
//  - we do not use bind here, because it would introduce
//    an extra copy and complicate the header include order if bind is
//    to have automatic conversion for member pointers.
_FIREWALL([FUNCTORS_MEM_FUN])
#include <sigc++/type_traits.h>
#include <sigc++/limit_reference.h>
#include <sigc++/member_method_trait.h>

namespace sigc {

/** @defgroup mem_fun mem_fun()
 * mem_fun() is used to convert a pointer to a method to a functor.
 *
 * Optionally, a reference or pointer to an object can be bound to the functor.
 * Note that only if the object type inherits from sigc::trackable is
 * the slot automatically cleared when the object goes out of scope!
 *
 * If the member function pointer is to an overloaded type, you must specify
 * the types using template arguments starting with the first argument.
 * It is not necessary to supply the return type.
 *
 * @par Example:
 * @code
 * struct foo : public sigc::trackable
 * {
 *   void bar(int) {}
 * };
 * foo my_foo;
 * sigc::slot<void, int> sl = sigc::mem_fun(my_foo, &foo::bar);
 * @endcode
 *
 * For const methods mem_fun() takes a const reference or pointer to an object.
 *
 * @par Example:
 * @code
 * struct foo : public sigc::trackable
 * {
 *   void bar(int) const {}
 * };
 * const foo my_foo;
 * sigc::slot<void, int> sl = sigc::mem_fun(my_foo, &foo::bar);
 * @endcode
 *
 * Use mem_fun#() if there is an ambiguity as to the number of arguments.
 *
 * @par Example:
 * @code
 * struct foo : public sigc::trackable
 * {
 *   void bar(int) {}
 *   void bar(float) {}
 *   void bar(int, int) {}
 * };
 * foo my_foo;
 * sigc::slot<void, int> sl = sigc::mem_fun1<int>(my_foo, &foo::bar);
 * @endcode
 *
 * @ingroup sigcfunctors
 */

template <class T_func, class T_return, class T_obj, class... T_arg>
class mem_functor_base : public functor_base
{
public:
  using function_type = T_func;
  using result_type = T_return;

  using obj_type_with_modifier = typename std::conditional_t<
    member_method_is_const<T_func>::value, const T_obj, T_obj>;

  /// Constructs an invalid functor.
  mem_functor_base() : func_ptr_(nullptr) {}

  /** Constructs a mem_functor object that wraps the passed method.
   * @param _A_func Pointer to method will be invoked from operator()().
   */
  explicit mem_functor_base(function_type _A_func) : func_ptr_(_A_func) {}

  /** Execute the wrapped method operating on the passed instance.
   * @param _A_obj Pointer to instance the method should operate on.
   * @param _A_a... Argument to be passed on to the method.
   * @return The return value of the method invocation.
   */
  decltype(auto)
  operator()(obj_type_with_modifier* _A_obj, type_trait_take_t<T_arg>... _A_a) const
    { return (_A_obj->*(this->func_ptr_))(_A_a...); }

  /** Execute the wrapped method operating on the passed instance.
   * @param _A_obj Reference to instance the method should operate on.
   * @param _A_a... Argument to be passed on to the method.
   * @return The return value of the method invocation.
   */
  decltype(auto)
  operator()(obj_type_with_modifier& _A_obj, type_trait_take_t<T_arg>... _A_a) const
    { return (_A_obj.*func_ptr_)(_A_a...); }

protected:
  function_type func_ptr_;
};

//mem_functor and const_mem_functor are just convenience aliases used in the
//definition of functor_trait<> specializations. TODO: Remove them?

MEMBER_FUNCTOR([],[])
MEMBER_FUNCTOR([const_],[const])

template <class T_func,
  class T_return, class T_obj, class... T_arg>
class bound_mem_functor_base
: mem_functor_base<T_func, T_return, T_obj, T_arg...>
{
  using base_type = mem_functor_base<T_func, T_return, T_obj, T_arg...>;
public:
  using function_type = typename base_type::function_type;
  using result_type = typename base_type::result_type;

  using obj_type_with_modifier = typename std::conditional_t<
    member_method_is_const<T_func>::value, const T_obj, T_obj>;
  using T_limit_reference = typename std::conditional_t<
    member_method_is_const<T_func>::value,
      limit_reference<const T_obj>, limit_reference<T_obj>>;

  /** Constructs a bound_mem_functor_base object that wraps the passed method.
   * @param _A_obj Pointer to instance the method will operate on.
   * @param _A_func Pointer to method will be invoked from operator()().
   */
  bound_mem_functor_base(obj_type_with_modifier* _A_obj, function_type _A_func)
    : base_type(_A_func),
      obj_(*_A_obj)
    {}

  /** Constructs a bound_mem_functor_base object that wraps the passed method.
   * @param _A_obj Reference to instance the method will operate on.
   * @param _A_func Pointer to method will be invoked from operator()().
   */
  bound_mem_functor_base(obj_type_with_modifier& _A_obj, function_type _A_func)
    : base_type(_A_func),
      obj_(_A_obj)
    {}

  /** Execute the wrapped method operating on the stored instance.
   * @param _A_a... Argument to be passed on to the method.
   * @return The return value of the method invocation.
   */
  decltype(auto)
  operator()(type_trait_take_t<T_arg>... _A_a) const
    { return (obj_.invoke().*(this->func_ptr_))(_A_a...); }

//protected: TODO?
  // Reference to stored object instance.
  // This is the handler object, such as TheObject in void TheObject::signal_handler().
  T_limit_reference obj_;
};


#ifndef DOXYGEN_SHOULD_SKIP_THIS
//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::bound_[$1]mem_functor performs a functor
 * on the object instance stored in the sigc::bound_[$1]mem_functor object.
 *
 * @ingroup mem_fun
 */
template <class T_func, class T_return, class T_obj, class... T_arg>
struct visitor<bound_mem_functor_base<T_func, T_return, T_obj, T_arg...> >
{
  template <class T_action>
  static void do_visit_each(const T_action& _A_action,
                            const bound_mem_functor_base<T_func, T_return, T_obj, T_arg...>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.obj_);
  }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS


//bound_const_mem_functor is just a convenience aliases used in the
//definition of make_slot(). TODO: Remove it?

BOUND_MEMBER_FUNCTOR([const_],[const])


MEM_FUN([],[],[])
MEM_FUN([const_],[const],[const])
MEM_FUN([volatile_],[],[volatile])
MEM_FUN([const_volatile_],[const],[const volatile])

BOUND_MEM_FUN([],[],[])
BOUND_MEM_FUN([const_],[const],[const])
BOUND_MEM_FUN([volatile_],[],[volatile])
BOUND_MEM_FUN([const_volatile_],[const],[const volatile])

} /* namespace sigc */
