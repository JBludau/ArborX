#ifndef PYARBORX_SFC_CPP
#define PYARBORX_SFC_CPP

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>

#include <ArborX_SpaceFillingCurves.hpp>

#include "../config.hpp"

namespace py = pybind11;

PYBIND11_MODULE(Experimental, m) {

    m.attr("__name__") = "pyArborX.Experimental";
    py::class_<ArborX::Experimental::Morton64>(m,"Morton64")
      .def(py::init<>());
}


#endif
