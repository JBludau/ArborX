#ifndef PYARBORX_DETAILS_HPP
#define PYARBORX_DETAILS_HPP

#include "config.hpp"
#include <ArborX_MinimumSpanningTree.hpp>
#include <pybind11/pybind11.h>

namespace py = pybind11;

void generateDetailsWrapper(py::module &m);

#endif
