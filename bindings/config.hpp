#ifndef PYARBORX_CONFIG_H
#define PYARBORX_CONFIG_H

#include <ArborX.hpp>

#include <Kokkos_Core.hpp>


// instantiation of all template arguments by hand, as there is no interface for these ... for now
using ExecutionSpace = Kokkos::DefaultExecutionSpace;
using MemorySpace = typename ExecutionSpace::memory_space;
// using MemorySpace = Kokkos::CudaUVMSpace;

using 1DintViewType = Kokkos::View<int *,MemorySpace>;
using Primitives = Kokkos::View<ArborX::Point *, MemorySpace>;
using Predicates = Kokkos::View<decltype(ArborX::intersects(ArborX::Sphere{})) *, MemorySpace>
using BVH = ArborX::BVH<MemorySpace>;

#endif
