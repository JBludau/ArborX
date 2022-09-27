#ifndef PYARBORX_BVH_CPP
#define PYARBORX_BVH_CPP

#include "pyArborX_BVH.hpp"
using BVH = ArborX::BVH<MemorySpace>;

namespace py = pybind11;

void generateBVHWrapper(py::module &m)
{
  py::class_<TraversalPolicy>(m, "Experimental_TraversalPolicy");

  m.def("ArborX::Details::sortObjects",
        &ArborX::Details::sortObjects<ExecutionSpace, intView1DType,
                                      unsigned int>,
        py::arg("ExecutionSpace"), py::arg("ViewType"));

  py::class_<BVH>(m, "BVH")
      .def(py::init<>())
      .def(py::init<ExecutionSpace, Primitives, SpaceFillingCurve>(),
           py::arg("ExecutionSpace"), py::arg("Primitives"),
           py::arg("SpaceFillingCurve") = SpaceFillingCurve())
      .def(py::init<ExecutionSpace,
                    Kokkos::View<float **, Kokkos::LayoutLeft, MemorySpace>>())

      .def("query",
           &BVH::query<ExecutionSpace const &, Predicates const &,
                       intView1DType &, intView1DType &>,
           py::arg("ExecutionSpace"), py::arg("Predicates"), py::arg("Indices"),
           py::arg("Offsets"))

      .def("query",
           &BVH::query<ExecutionSpace, PrimitivesWithRadius,
                       FDBSCANCallback const &>,
           py::arg("space"), py::arg("Predicates"), py::arg("Callback"),
           py::arg("Policy") = TraversalPolicy());
}

#endif
