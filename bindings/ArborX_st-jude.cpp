#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

#include <ArborX.hpp>

#include <Kokkos_Core.hpp>

namespace py = pybind11;

// instantiation of all template arguments by hand, as there is no interface for these ... for now
using ExecutionSpace = Kokkos::DefaultExecutionSpace;
using MemorySpace = ExecutionSpace::memory_space;
// using MemorySpace = Kokkos::CudaUVMSpace;

using 1DintViewType = Kokkos::View<int *,MemorySpace>;
using Primitives = Kokkos::View<ArborX::Point *, MemorySpace>;
using Predicates = Kokkos::View<decltype(ArborX::intersects(ArborX::Sphere{})) *, MemorySpace>
using BVH = ArborX::BVH<MemorySpace>;


Kokkos::View<ArborX::Point*, Kokkos::CudaSpace> vec2view(std::vector<ArborX::Point> const &in, std::string const &label = "")
{
  Kokkos::View<ArborX::Point*, Kokkos::CudaSpace> out(
      Kokkos::view_alloc(label, Kokkos::WithoutInitializing), in.size());
  Kokkos::deep_copy(out, Kokkos::View<ArborX::Point const *, Kokkos::HostSpace,
                                      Kokkos::MemoryTraits<Kokkos::Unmanaged>>{
                             in.data(), in.size()});
  return out;
}

PYBIND11_MODULE(PyArborX_st-jude, m) {
    py::class_<ExecutionSpace>(m, "ExecSpace")
        .def(py::init([](){ Kokkos::initialize(); return ExecutionSpace{}; }));

    m.def("ArborX::Details::sortObjects",&ArborX::Details::sortObjects<ExecutionSpace,1DintViewType,unsigned int>,
        py::arg("ExecutionSpace"),
        py::arg("ViewType")
        )

    // Needed to have default policy argument for query
    py::class_<ArborX::Experimental::TraversalPolicy>(m, "TraversalPolicy");

    py::class_<BVH>(m, "BVH")
        .def(py::init<>())
        .def(py::init<ExecutionSpace, Primitives,SpaceFillingCurve>(),
              py::arg("ExecutionSpace"),
              py::arg("Primitives"),
              py::arg("SpaceFillingCurve") = ArborX::Experimental::Morton64()
            )
            
        .def("query", &BVH::query<ExecutionSpace, Predicates,1DintViewType,1DintViewType>,
              py::arg("ExecutionSpace"),
              py::arg("Predicates"),
              py::arg("Indices"),
              py::arg("Offsets")
            )
        
    m.def("vec2view", &vec2view);
}
