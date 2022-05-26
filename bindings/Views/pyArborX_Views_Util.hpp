#ifndef PYARBORX_POINTVIEW_UTIL_HPP
#define PYARBORX_POINTVIEW_UTIL_HPP

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>

#include <ArborX_Point.hpp>

#include "config.hpp"

// using PointView = Kokkos::View<ArborX::Point *,MemorySpace>;


// template<typename ViewType>
// ViewType allocWithoutInitialize (std::string name ,unsigned long int size)
// {
   // ViewType* view = ViewType(Kokkos::view_alloc(
                                    // ExecutionSpace{},
                                    // Kokkos::WithoutInitializing,
                                    // name)
                 // ,size);
   // return *view;
// o}
//

// auto query_pts_view = query_pts_vec[f];
// unsigned int const n_queries = query_pts_view.extent(0);
// Kokkos::View<decltype(ArborX::intersects(ArborX::Sphere{})) *, MemorySpace>
        // within_queries("within_queries", n_queries);
    // Kokkos::parallel_for(
        // "Setup queries",
        // Kokkos::RangePolicy<ExecutionSpace>(execution_space, 0, n_queries),
        // KOKKOS_LAMBDA(int i) {
          // within_queries(i) =
              // ArborX::intersects(ArborX::Sphere{query_pts_view(i), radius});
        // });

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

