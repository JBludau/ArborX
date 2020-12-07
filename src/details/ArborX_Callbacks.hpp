/****************************************************************************
 * Copyright (c) 2012-2020 by the ArborX authors                            *
 * All rights reserved.                                                     *
 *                                                                          *
 * This file is part of the ArborX library. ArborX is                       *
 * distributed under a BSD 3-clause license. For the licensing terms see    *
 * the LICENSE file in the top-level directory.                             *
 *                                                                          *
 * SPDX-License-Identifier: BSD-3-Clause                                    *
 ****************************************************************************/
#ifndef ARBORX_CALLBACKS_HPP
#define ARBORX_CALLBACKS_HPP

#include <ArborX_AccessTraits.hpp>
#include <ArborX_DetailsConcepts.hpp>

#include <Kokkos_Macros.hpp>

#include <utility> // declval

namespace ArborX
{

enum class CallbackTreeTraversalControl
{
  early_exit,
  normal_continuation
};

namespace Details
{

struct InlineCallbackTag
{
};

struct PostCallbackTag
{
};

struct CallbackDefaultSpatialPredicate
{
  using tag = InlineCallbackTag;
  template <typename Query, typename OutputFunctor>
  KOKKOS_FUNCTION void operator()(Query const &, int index,
                                  OutputFunctor const &output) const
  {
    output(index);
  }
};

struct CallbackDefaultNearestPredicate
{
  using tag = InlineCallbackTag;
  template <typename Query, typename OutputFunctor>
  KOKKOS_FUNCTION void operator()(Query const &, int index, float,
                                  OutputFunctor const &output) const
  {
    output(index);
  }
};

struct CallbackDefaultNearestPredicateWithDistance
{
  using tag = InlineCallbackTag;
  template <typename Query, typename OutputFunctor>
  KOKKOS_FUNCTION void operator()(Query const &, int index, float distance,
                                  OutputFunctor const &output) const
  {
    output({index, distance});
  }
};

// archetypal expression for user callbacks
template <typename Callback, typename Predicate, typename Out>
using NearestPredicateInlineCallbackArchetypeExpression =
    decltype(std::declval<Callback const &>()(
        std::declval<Predicate const &>(), 0, 0., std::declval<Out const &>()));

template <typename Callback, typename Predicate, typename Out>
using SpatialPredicateInlineCallbackArchetypeExpression =
    decltype(std::declval<Callback const &>()(std::declval<Predicate const &>(),
                                              0, std::declval<Out const &>()));

// archetypal alias for a 'tag' type member in user callbacks
template <typename Callback>
using CallbackTagArchetypeAlias = typename Callback::tag;

template <typename Callback>
struct is_tagged_post_callback
    : std::is_same<detected_t<CallbackTagArchetypeAlias, Callback>,
                   PostCallbackTag>::type
{
};

// output functor to pass to the callback during detection
template <typename T>
struct Sink
{
  void operator()(T const &) const {}
};

template <typename OutputView>
using OutputFunctorHelper = Sink<typename OutputView::value_type>;

template <class Callback>
void check_generic_lambda_support(Callback const &)
{
#ifdef __NVCC__
  // Without it would get a segmentation fault and no diagnostic whatsoever
  static_assert(
      !__nv_is_extended_host_device_lambda_closure_type(Callback),
      "__host__ __device__ extended lambdas cannot be generic lambdas");
#endif
}

template <typename Callback, typename Predicates, typename OutputView>
void check_valid_callback(Callback const &callback, Predicates const &,
                          OutputView const &)
{
  check_generic_lambda_support(callback);

  using Access = AccessTraits<Predicates, PredicatesTag>;
  using PredicateTag = typename AccessTraitsHelper<Access>::tag;
  using Predicate = typename AccessTraitsHelper<Access>::type;

  static_assert(
      (std::is_same<PredicateTag, SpatialPredicateTag>{} &&
       is_detected<SpatialPredicateInlineCallbackArchetypeExpression, Callback,
                   Predicate, OutputFunctorHelper<OutputView>>{}) ||
          (std::is_same<PredicateTag, NearestPredicateTag>{} &&
           is_detected<NearestPredicateInlineCallbackArchetypeExpression,
                       Callback, Predicate, OutputFunctorHelper<OutputView>>{}),
      "Callback 'operator()' does not have the correct signature");

  static_assert(
      (std::is_same<PredicateTag, SpatialPredicateTag>{} &&
       std::is_void<detected_t<
           SpatialPredicateInlineCallbackArchetypeExpression, Callback,
           Predicate, OutputFunctorHelper<OutputView>>>{}) ||
          (std::is_same<PredicateTag, NearestPredicateTag>{} &&
           std::is_void<detected_t<
               NearestPredicateInlineCallbackArchetypeExpression, Callback,
               Predicate, OutputFunctorHelper<OutputView>>>{}),
      "Callback 'operator()' return type must be void");
}

// EXPERIMENTAL archetypal expression for user callbacks
template <typename Callback, typename Predicate, typename Primitive>
using Experimental_NearestPredicateCallbackArchetypeExpression = decltype(
    std::declval<Callback const &>()(std::declval<Predicate const &>(),
                                     std::declval<Primitive const &>(), 0.));

template <typename Callback, typename Predicate, typename Primitive>
using Experimental_SpatialPredicateCallbackArchetypeExpression =
    decltype(std::declval<Callback const &>()(
        std::declval<Predicate const &>(), std::declval<Primitive const &>()));

// Determine whether the callback returns a hint to exit the tree traversal
// early.
template <typename Callback, typename Predicate, typename Primitive>
struct invoke_callback_and_check_early_exit_helper
    : std::is_same<
          CallbackTreeTraversalControl,
          detected_t<Experimental_SpatialPredicateCallbackArchetypeExpression,
                     Callback, Predicate, Primitive>>::type
{
};

// Invoke a callback that may return a hint to interrupt the tree traversal and
// return true for early exit, or false for normal continuation.
template <typename Callback, typename Predicate, typename Primitive>
KOKKOS_INLINE_FUNCTION
    std::enable_if_t<invoke_callback_and_check_early_exit_helper<
                         std::decay_t<Callback>, std::decay_t<Predicate>,
                         std::decay_t<Primitive>>::value,
                     bool>
    invoke_callback_and_check_early_exit(Callback &&callback,
                                         Predicate &&predicate,
                                         Primitive &&primitive)
{
  return ((Callback &&) callback)((Predicate &&) predicate,
                                  (Primitive &&) primitive) ==
         CallbackTreeTraversalControl::early_exit;
}

// Invoke a callback that does not return a hint.  Always return false to
// signify that the tree traversal should continue normally.
template <typename Callback, typename Predicate, typename Primitive>
KOKKOS_INLINE_FUNCTION
    std::enable_if_t<!invoke_callback_and_check_early_exit_helper<
                         std::decay_t<Callback>, std::decay_t<Predicate>,
                         std::decay_t<Primitive>>::value,
                     bool>
    invoke_callback_and_check_early_exit(Callback &&callback,
                                         Predicate &&predicate,
                                         Primitive &&primitive)
{
  ((Callback &&) callback)((Predicate &&) predicate, (Primitive &&) primitive);
  return false;
}

template <typename Callback, typename Predicates>
void check_valid_callback(Callback const &callback, Predicates const &)
{
  check_generic_lambda_support(callback);

  using Access = AccessTraits<Predicates, PredicatesTag>;
  using PredicateTag = typename AccessTraitsHelper<Access>::tag;
  using Predicate = typename AccessTraitsHelper<Access>::type;

  static_assert(
      (std::is_same<PredicateTag, SpatialPredicateTag>{} &&
       is_detected<Experimental_SpatialPredicateCallbackArchetypeExpression,
                   Callback, Predicate, int>{}) ||
          (std::is_same<PredicateTag, NearestPredicateTag>{} &&
           is_detected<Experimental_NearestPredicateCallbackArchetypeExpression,
                       Callback, Predicate, int>{}),
      "Callback 'operator()' does not have the correct signature");

  static_assert(
      (std::is_same<PredicateTag, SpatialPredicateTag>{} &&
       (std::is_same<
            CallbackTreeTraversalControl,
            detected_t<Experimental_SpatialPredicateCallbackArchetypeExpression,
                       Callback, Predicate, int>>{} ||
        std::is_void<
            detected_t<Experimental_SpatialPredicateCallbackArchetypeExpression,
                       Callback, Predicate, int>>{})) ||
          std::is_same<PredicateTag, NearestPredicateTag>{},
      "Callback 'operator()' return type must be void or "
      "ArborX::CallbackTreeTraversalControl");

  static_assert(
      std::is_same<PredicateTag, SpatialPredicateTag>{} ||
          (std::is_same<PredicateTag, NearestPredicateTag>{} &&
           std::is_void<detected_t<
               Experimental_NearestPredicateCallbackArchetypeExpression,
               Callback, Predicate, int>>{}),
      "Callback 'operator()' return type must be void");
}

} // namespace Details
} // namespace ArborX

#endif
