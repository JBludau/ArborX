#ifndef PYARBORX_BVH_CPP
#define PYARBORX_BVH_CPP

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>


namespace py = pybind11;

Kokkos::View<ArborX::Point*, Kokkos::CudaSpace> vec2view(std::vector<ArborX::Point> const &in, std::string const &label = "")
{
  Kokkos::View<ArborX::Point*, Kokkos::CudaSpace> out(
      Kokkos::view_alloc(label, Kokkos::WithoutInitializing), in.size());
  Kokkos::deep_copy(out, Kokkos::View<ArborX::Point const *, Kokkos::HostSpace,
                                      Kokkos::MemoryTraits<Kokkos::Unmanaged>>{
                             in.data(), in.size()});
  return out;
}


PYBIND11_MODULE(BVH, m) {
    m.attr("__name__") = "pyArborX.BVH";

    m.def("ArborX::Details::sortObjects",&ArborX::Details::sortObjects<ExecutionSpace,1DintViewType,unsigned int>,
        py::arg("ExecutionSpace"),
        py::arg("ViewType")
        );

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
            );
        
    m.def("vec2view", &vec2view);

#endif
