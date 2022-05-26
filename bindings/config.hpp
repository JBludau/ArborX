#ifndef PYARBORX_CONFIG_H
#define PYARBORX_CONFIG_H

#include <ArborX_Point.hpp>
#include <ArborX_Predicates.hpp>
#include <ArborX_Sphere.hpp>
#include <ArborX_SpaceFillingCurves.hpp>

#include <Kokkos_Core.hpp>


// instantiation of all template arguments by hand, as there is no interface for these ... for now
using ExecutionSpace = Kokkos::Cuda;
using MemorySpace = ExecutionSpace::memory_space;
// using MemorySpace = Kokkos::CudaUVMSpace;

using intView1DType = Kokkos::View<int *,MemorySpace>;
using PrimitivesBaseType = ArborX::Point;
using Primitives = Kokkos::View<PrimitivesBaseType *, MemorySpace>;
using PredicatesBaseType = decltype(ArborX::intersects(ArborX::Sphere{}));
using Predicates = Kokkos::View<PredicatesBaseType *, MemorySpace>;
using SpaceFillingCurve = ArborX::Experimental::Morton64;

#endif
