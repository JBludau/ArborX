#ifndef PYARBORX_POINT_HPP
#define PYARBORX_POINT_HPP

#include <ArborX_Point.hpp>

#include <pybind11/pybind11.h>
#include <string.h>

namespace py = pybind11;

namespace pyArborXHelper
{
std::string pyPrintPoint(ArborX::Point const &p);
}

void generatePointWrapper(py::module &m);

#endif

// TODO: think again about if the interface should copy the values ... this
// would be a deviation of the interface of the original library
