#ifndef PYARBORX_POINT_CPP
#define PYARBORX_POINT_CPP

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>

#include <ArborX_Point.hpp>

#include "../config.hpp"

#include "pyArborX_Point_Util.hpp" 

namespace py = pybind11;

PYBIND11_MODULE(Point, m) {

    m.attr("__name__") = "pyArborX.Point";
    py::class_<ArborX::Point>(m,"Point")
      .def(py::init<>())
      // .def(py::init<ArborX::Abomination>(),py)
      .def(py::init<float,float,float>(),py::arg("x"),py::arg("y"),py::arg("z"))

      .def("__getitem__",py::overload_cast<unsigned int>(&ArborX::Point::operator[],py::const_),py::arg("i"))
      .def("__setitem__",
          [](ArborX::Point& p,int idx, double val){p[idx]=val;})
          
          // py::overload_cast<unsigned int>(&ArborX::Point::operator[]),py::arg("i"),py::return_value_policy::reference)

      .def("__repr__",py::overload_cast<ArborX::Point const &>(&pyArborXHelper::pyPrint));
}


#endif

//TODO: think again about if the interface should copy the values ... this would be a deviation of the interface of the original library
