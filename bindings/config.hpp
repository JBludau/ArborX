#ifndef PYARBORX_CONFIG_H
#define PYARBORX_CONFIG_H

#include <ArborX_Point.hpp>
#include <ArborX_Predicates.hpp>
#include <ArborX_Sphere.hpp>
#include <ArborX_SpaceFillingCurves.hpp>

#include <Kokkos_Core.hpp>


// instantiation of all template arguments by hand, as there is no interface for these ... for now
using ExecutionSpace = Kokkos::DefaultExecutionSpace;
using MemorySpace = ExecutionSpace::memory_space;
// using MemorySpace = Kokkos::CudaUVMSpace;

using intView1DType = Kokkos::View<int *,MemorySpace>;
using Primitives = Kokkos::View<ArborX::Point *, MemorySpace>;
using Predicates = Kokkos::View<decltype(ArborX::intersects(ArborX::Sphere{})) *, MemorySpace>;
using SpaceFillingCurve = ArborX::Experimental::Morton64;

#endif
