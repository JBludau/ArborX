#ifndef PYARBORX_DETAILS_CPP
#define PYARBORX_DETAILS_CPP

#include "pyArborX_Details.hpp"

namespace py = pybind11;

void generateDetailsWrapper(py::module &m)
{

  py::class_<ArborX::Details::MinimumSpanningTree<MemorySpace>>(m,"Details_MinimumSpanningTree")
    .def(py::init<ExecutionSpace const &,Primitives const &,int>(),py::arg("ExecutionSpace"), py::arg("Primitives"),py::arg("k")=1);

}

#endif
