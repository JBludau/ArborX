#include <string.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>

#include <Kokkos_Core.hpp>
#include <ArborX.hpp>

namespace py = pybind11;

namespace pyArborXHelper {
std::string pyPrint (ArborX::Point const & p)
{
  return "<pyArborX::Point with " + std::to_string(p[0]) +" "+ std::to_string(p[1]) +" "+ std::to_string(p[2]) +">";
}

std::string pyPrint (ArborX::Box const & b)
{
  return "<pyArborX::Box with min Corner \n" 
            + pyPrint(b.minCorner()) +
            "and max Corner \n"
            + pyPrint(b.maxCorner()) +
            ">";
}
} // end of namespace pyArborXHelper

PYBIND11_MODULE(pyArborX, m) {
    py::class_<Kokkos::Cuda>(m, "Cuda")
        .def(py::init([](){ Kokkos::initialize(); return Kokkos::Cuda{}; }));

    py::class_<ArborX::Point>(m,"Point")
      .def(py::init<>())
      // .def(py::init<ArborX::Abomination>(),py)
      .def(py::init<float,float,float>(),py::arg("x"),py::arg("y"),py::arg("z"))

      .def("__getitem__",py::overload_cast<unsigned int>(&ArborX::Point::operator[],py::const_),py::arg("i"))
      .def("__setitem__",py::overload_cast<unsigned int>(&ArborX::Point::operator[]),py::arg("i"))

      .def("__repr__",py::overload_cast<ArborX::Point const &>(&pyArborXHelper::pyPrint));

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
