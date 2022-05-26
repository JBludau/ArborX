#ifndef PYARBORX_POINTVIEW_UTIL_HPP
#define PYARBORX_POINTVIEW_UTIL_HPP

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>

#include <ArborX_Point.hpp>

#include "config.hpp"

using PointView = Kokkos::View<ArborX::Point *,MemorySpace>;


// template<typename ViewType>
// ViewType allocWithoutInitialize (std::string name ,unsigned long int size)
// {
   // ViewType* view = ViewType(Kokkos::view_alloc(
                                    // ExecutionSpace{},
                                    // Kokkos::WithoutInitializing,
                                    // name)
                 // ,size);
   // return *view;
// }

template<typename ViewType>
auto create_mirror_view(ViewType const & source)
{
  return Kokkos::create_mirror_view(source);
}

template<typename ViewDest,typename ViewSrc>
auto deep_copy(ViewDest const & dest, ViewSrc const & src)
{
  return Kokkos::deep_copy(dest,src);
}

#endif

