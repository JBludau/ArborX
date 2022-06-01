#ifndef PYARBORX_EXECUTION_SPACE_CPP
#define PYARBORX_EXECUTION_SPACE_CPP

#include <ArborX_Point.hpp>

#include "config.hpp"
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(ExecutionSpace, m)
{

  m.attr("__name__") = "pyArborX.ExecutionSpace";
  py::class_<ExecutionSpace>(m, "ExecutionSpace")
      .def(py::init<>([]() { return new ExecutionSpace{}; }));
}

#endif

// TODO: think again about if the interface should copy the values ... this
// would be a deviation of the interface of the original library
