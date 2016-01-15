#ifndef _SIGC_FUNCTORS_FUNCTOR_TRAIT_H_
#define _SIGC_FUNCTORS_FUNCTOR_TRAIT_H_
#include <sigc++/type_traits.h>
#include <type_traits>

namespace sigc {

//TODO: When we can break ABI, replace nil by something else, such as sigc_nil.
// nil is a keyword in Objective C++. When gcc is used for compiling Objective C++
// programs, nil is defined as a preprocessor macro.
// https://bugzilla.gnome.org/show_bug.cgi?id=695235
#if defined(nil) && defined(SIGC_PRAGMA_PUSH_POP_MACRO)
  #define SIGC_NIL_HAS_BEEN_PUSHED 1
  #pragma push_macro("nil")
  #undef nil
#endif

/** nil struct type.
 * The nil struct type is used as default template argument in the
 * unnumbered sigc::signal and sigc::slot templates.
 *
 * @ingroup signal
 * @ingroup slot
 */
#ifndef DOXYGEN_SHOULD_SKIP_THIS
struct nil;
#else
struct nil {};
#endif

#ifdef SIGC_NIL_HAS_BEEN_PUSHED
  #undef SIGC_NIL_HAS_BEEN_PUSHED
  #pragma pop_macro("nil")
#endif

/** @defgroup sigcfunctors Functors
 * Functors are copyable types that define operator()().
 *
 * Types that define operator()() overloads with different return types are referred to
 * as multi-type functors. Multi-type functors are only partially supported in libsigc++.
 *
 * Closures are functors that store all information needed to invoke a callback from operator()().
 *
 * Adaptors are functors that alter the signature of a functor's operator()().
 *
 * libsigc++ defines numerous functors, closures and adaptors.
 * Since libsigc++ is a callback library, most functors are also closures.
 * The documentation doesn't distinguish between functors and closures.
 *
 * The basic functor types libsigc++ provides are created with ptr_fun() and mem_fun()
 * and can be converted into slots implicitly.
 * The set of adaptors that ships with libsigc++ is documented in the @ref adaptors module.
 *
 * If you want to mix user-defined and third party functors with libsigc++,
 * and you want them to be implicitly convertible into slots, libsigc++ must know
 * the result type of your functors. There are different ways to achieve that.
 *
 * - Use the macro SIGC_FUNCTOR_TRAIT(T_functor,T_return) in namespace sigc.
 *   Multi-type functors are only partly supported.
 *
 * If all these ways to deduce the result type fail, void is assumed.
 *
 * With libsigc++ versions before 2.6, the macro 
 * #SIGC_FUNCTORS_DEDUCE_RESULT_TYPE_WITH_DECLTYPE activated the test with
 * decltype(). That macro is now unneccesary and deprecated.
 */

/** Trait that specifies the return type of any type.
 * Template specializations for functors whose result type can be deduced with decltype(),
 * for function pointers and for class methods are provided.
 *
 * @tparam T_functor Functor type.
 * @tparam I_can_use_decltype Whether the result type of @p T_functor can be deduced
 *                            with decltype().
 *
 * @ingroup sigcfunctors
 */
template <class T_functor>
struct functor_trait
{
  typedef T_functor functor_type;
};


#ifndef DOXYGEN_SHOULD_SKIP_THIS
// detect the the functor version of non-functor types.

template <class T_return, class... T_args>
class pointer_functor;


//functor ptr fun:

template <class T_return, class... T_arg>
struct functor_trait<T_return (*)(T_arg...)>
{
  typedef pointer_functor<T_return, T_arg...> functor_type;
};


//functor mem fun:

template <class T_return, class T_obj, class... T_arg> class mem_functor;
template <class T_return, class T_obj, class... T_arg> class const_mem_functor;

template <class T_return, class T_obj, class... T_arg>
struct functor_trait<T_return (T_obj::*)(T_arg...)>
{
  typedef mem_functor<T_return, T_obj, T_arg...> functor_type;
};

template <class T_return, class T_obj, class... T_arg>
struct functor_trait<T_return (T_obj::*)(T_arg...) const>
{
  typedef const_mem_functor<T_return, T_obj, T_arg...> functor_type;
};

#endif // DOXYGEN_SHOULD_SKIP_THIS

} /* namespace sigc */
#endif /* _SIGC_FUNCTORS_FUNCTOR_TRAIT_H_ */
