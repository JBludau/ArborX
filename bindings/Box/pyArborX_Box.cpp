#ifndef PYARBORX_POINT_CPP
#define PYARBORX_POINT_CPP

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>

#include <ArborX_Box.hpp>

#include "pyArborX_Box_Util.hpp" 

namespace py = pybind11;

PYBIND11_MODULE(Box, m) {

    m.attr("__name__") = "pyArborX.Box";
    py::class_<ArborX::Box>(m,"Box")
      .def(py::init<>())
      .def(py::init<ArborX::Point,ArborX::Point>(),py::arg("min_corner"),py::arg("max_corner"))

      .def("minCorner",py::overload_cast<>(&ArborX::Box::minCorner,py::const_),py::return_value_policy::reference)
      .def("minCorner",py::overload_cast<>(&ArborX::Box::minCorner),py::return_value_policy::reference)
      .def("maxCorner",py::overload_cast<>(&ArborX::Box::maxCorner,py::const_),py::return_value_policy::reference)
      .def("maxCorner",py::overload_cast<>(&ArborX::Box::maxCorner),py::return_value_policy::reference)

      .def("__iadd__",py::overload_cast<ArborX::Box const &>(&ArborX::Box::operator+=),py::arg("ArborX::Box"),py::return_value_policy::reference)

      .def("__repr__",py::overload_cast<ArborX::Box const &>(&pyArborXHelper::pyPrint));
}


#endif
