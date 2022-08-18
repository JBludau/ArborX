#ifndef PYARBORX_EXECUTION_SPACE_HPP
#define PYARBORX_EXECUTION_SPACE_HPP

#include "config.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

void generateExecutionSpaceWrapper(py::module &m);

#endif
