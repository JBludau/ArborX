#ifndef PYARBORX_EXECUTION_SPACE_CPP
#define PYARBORX_EXECUTION_SPACE_CPP

#include "pyArborX_ExecutionSpace.hpp"

namespace py = pybind11;

void generateExecutionSpaceWrapper(py::module &m)
{
  py::class_<ExecutionSpace>(m, "ExecutionSpace")
      .def(py::init<>([]() { return new ExecutionSpace{}; }))

      .def("fence", py::overload_cast<>(&ExecutionSpace::fence, py::const_));
}

#endif
